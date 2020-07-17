#include"tspData.h"

/*-----------------------------数据构造--------------------------------*/
//文件读取 字符串变为整型数
int trans(string s)
{
	int i = 0, i1 = 0, i2 = 0, m = 0;
	double result = 0, x = 0, y = 0;
	for (i = 0; i < s.size(); i++)
	{
		if (s[i] == '.')
		{
			m = i; break;
		}
		if (i == s.size() - 1)
		{
			m = s.size(); break;
		}
	}
	for (i1 = m - 1; i1 >= 0; i1--)
	{
		double temp;
		temp = (double)s[i1] - (double)'0';
		temp *= pow(10, m - 1 - i1);
		x += temp;
	}
	if (m != s.size())//还有小数部分
	{
		for (i2 = m + 1; i2 < s.size(); i2++)
		{
			double temp;
			temp = (double)s[i2] - (double)'0';
			temp *= pow(10, m - i2);
			y += temp;
		}
	}
	result = x + y;
	return int(result);
}

//构建点 点的构造函数
point::point(string data)
{
	n = 0; pos[0] = 0; pos[1] = 0;
	int num[3] = { 0 }, j = 0;
	if (data.empty())return;
	string temp = "";
	for (unsigned int i = 0; i <= data.size(); i++)
	{
		if (data[i] != ' ' && data[i] != '\0')
		{
			temp += data[i];
		}
		else if(!temp.empty())
		{
			//cout << temp;
			num[j] = trans(temp);
			j++;
			temp = "";
		}
	}
	n = num[0]; pos[0] = num[1]; pos[1] = num[2];
}

//图 tsp数据的构造函数
tspData::tspData(string infile1,string infile2, string outfile)
{
	minWayGreedy = 0;
	points.clear();
	total = 0;
	bestWayLength = 0;
	table = NULL;
	ifstream ifs;
	ifs.open(infile1, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}
	//把文件读入，并构建点的数组
	string temp;
	while (getline(ifs, temp))
	{
		if ((temp[0] > '9' || temp[0] < '0') && temp[0] != ' ')
		{
			continue;
		}
		points.push_back(point(temp));
		total++;
	}
	ifs.close();
	//测试代码，是否成功读入数据
	/*cout << total;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		cout << points[i].n << " " << points[i].pos[0] << " " << points[i].pos[1] << " " << endl;
	}*/
	ofstream ofs;
	ofs.open(outfile, ios::out);
	table = new int* [total];
	for (int i = 0; i < total; i++)
	{
		table[i] = new int[total];
	}
	//构件好点的数组后计算得到邻接矩阵
	for (int i = 0; i < total; i++)
	{
		for (int j = 0; j < total; j++)
		{
			if (i == j)
			{
				table[i][j] = INT_MAX; ofs << "MAX ";
			}
			else
			{
				int x = 0, y = 0;
				x = points[i].pos[0] - points[j].pos[0];
				y = points[i].pos[1] - points[j].pos[1];
				table[i][j] = sqrt(x * x + y * y);
				ofs << table[i][j] << " ";
			}
		}
		ofs << endl;
	}
	ofs.close();
	best(infile2);
	//测试代码，是否成功读取文件
	/*for (int i = 0; i < total; i++)
	{
		for (int j = 0; j < total; j++)
		{
			cout << table[i][j] << " ";
		}
		cout << endl;
	}*/
}

//析构函数 释放堆区空间以及清零操作
tspData::~tspData()
{
	points.clear();
	wayGreedy.clear();
	if (table != NULL)
	{
		for (int i = 0; i < total; i++)
		{
			delete[] table[i];
		}
		delete[] table;
	}
	total = 0;
	table = NULL;
}

//二分查找求下标
int tspData::getindex(int n)
{
	int left = 0, right = points.size();
	int mid = points.size() / 2;
	while (points[mid].n != n)
	{
		if (points[mid].n > n)
		{
			right = mid - 1;
			mid = (left + right) / 2;
			continue;
		}
		if (points[mid].n < n)
		{
			left = mid + 1;
			mid = (left + right) / 2;
			continue;
		}
	}
	return mid;
}

//最优解
void tspData::best(string infile)
{
	ifstream ifs;
	string file = infile;
	ifs.open(file, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}
	//从文件中读最优解
	string temp;
	vector<int> record;
	while (getline(ifs, temp))
	{
		if ((temp[0] > '9' || temp[0] < '0') && temp[0] != ' ')
		{
			continue;
		}
		record.push_back(trans(temp));
	}
	ifs.close();
	//int m = 0;
	for (int i = 0; i < record.size() - 1; i++)
	{
		bestWayLength += table[getindex(record[i])][getindex(record[i + 1])];
		//m++;
	}
	//cout << m << endl;
}
/*-----------------------------数据构造--------------------------------*/


/*-----------------------------贪心算法求解--------------------------------*/
bool tspData::ifIn(int index)
{
	for (unsigned int i = 0; i < wayGreedy.size(); i++)
	{
		//如果点在记录的集合中，返回真
		if (index == wayGreedy[i])
		{
			return true;
		}
	}
	return false;
}

int tspData::minLength(int index)
{
	int temp = INT_MAX;
	int index_find = 0;
	for (int i = 0; i < total; i++)
	{
		//点没有遍历 且 比已经记录的最小值小 重新记录最小值
		if (!ifIn(points[i].n) && table[getindex(index)][i] < temp)
		{
			temp = table[getindex(index)][i];
			index_find = i;
		}
	}
	return points[index_find].n;
}

void tspData::TSPGreedy(string file, int begin)
{
	//超出点集合，退出
	if (begin > total || begin <= 0)
	{
		cout << "不存在的起始位置" << endl;
		return;
	}
	//打开对应的文件
	ofstream ofs;
	ofs.open(file, ios::out);
	if (!ofs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}
	//起点记录
	wayGreedy.push_back(begin);
	//循环条件：没有完全遍历
	while (wayGreedy.size() < total)
	{
		int x = 0, y = 0;
		//测试 数组长度
		//cout << minWay.size() << " ";
		//把目前最短距离对应的点记录
		x = wayGreedy[wayGreedy.size() - 1];
		wayGreedy.push_back(minLength(x));
		//记录路径长度
		x = wayGreedy[wayGreedy.size() - 2];
		y = wayGreedy[wayGreedy.size() - 1];
		minWayGreedy += table[getindex(x)][getindex(y)];
	}
	//文件记录
	minWayGreedy += table[wayGreedy.back() - 1][begin - 1];
	for (unsigned int i = 0; i < wayGreedy.size(); i++)
	{
		ofs << wayGreedy[i] << endl;
	}
	//遍历结束，回到起点
	ofs << begin << endl;
	ofs.close();
	//成功写入文件中后 清空数组
}
/*-----------------------------贪心算法求解--------------------------------*/


/*-----------------------------遗传算法求解--------------------------------*/
void tspData::initPopulation()
{
	//cout << lifeCount;
	life.clear();//先清空种群
	//扫描每一个个体
	vector<int> lifeTemp;
	for (int i = 0; i < lifeCount; i++)
	{
		for (int i1 = 0; i1 < total; i1++)
		{
			lifeTemp.push_back(i1 + 1);
		}
		//测试输出
		/*for (int i1 = 0; i1 < total; i1++)
		{
			cout << lifeTemp[i1] << " ";
		}
		cout << endl;*/
		random_shuffle(lifeTemp.begin(), lifeTemp.end());
		life.push_back(lifeTemp);
		lifeTemp.clear();
	}
}

void tspData::getScore()
{
	double totalScore = 0;
	score.clear(); judgeTable.clear();
	for (int i = 0; i < lifeCount; i++)
	{
		int tempLen = 0; double s = 0;
		for (int i1 = 0; i1 < total - 2; i1++)
		{
			tempLen += table[getindex(life[i][i1])][getindex(life[i][i1 + 1])];
		}
		tempLen += table[getindex(life[i][total - 1])][getindex(life[i].front())];
		//cout << tempLen << " ";
		s = 1000000 / double(tempLen);
		totalScore += s;
		//cout << s << " ";
		score.push_back(s);
	}
	//cout << totalScore << " " << endl;
	for (int i = 0; i < lifeCount; i++)
	{
		if (i == 0)
		{
			judgeTable.push_back(score[i] / totalScore);
		}
		else
		{
			judgeTable.push_back(score[i] / totalScore + judgeTable[i - 1]);
		}
		//cout << judgeTable[i] << " ";
	}
	//cout << endl;
}

vector<int>& tspData::getOne()
{
	double rate = (rand() % 1000) / double(1000);
	//cout << rate << endl;
	for (int i = lifeCount - 2; i >= 0; i--)
	{
		if (rate > judgeTable[i])
		{
			return life[i + 1];
		}
	}
	return life[0];
	cout << "不存在的个体，出现错误" << endl;
	exit(0);
}

vector<int>& tspData::getBest()
{
	int index = 0; double rate = 0;
	for (int i = 0; i < lifeCount; i++)
	{
		if (score[i] > rate)
		{
			rate = score[i]; index = i;
		}
		//cout << score[i] << " ";
	}
	return life[index];
}

vector<int> tspData::cross(vector<int> p1, vector<int> p2)
{
	int index1, index2 = 0;
	index1 = rand() % total; 
	index2 = rand() % (total - index1) + index1; //cout << index1 << " " << index2 << endl;
	//把p2中index1到index2的片段交叉放到p1中，形成新的序列
	vector<int> temp, newLife;
	for (int i = index1; i <= index2; i++)
	{
		temp.push_back(p2[i]);
	}
	for (unsigned int i = 0; i < p1.size(); i++)
	{
		if (i == index1)
		{
			for (unsigned int i1 = 0; i1 < temp.size(); i1++)
			{
				newLife.push_back(temp[i1]);
			}
		}
		int sign = -1;
		for (unsigned int i1 = 0; i1 < temp.size(); i1++)
		{
			//临时序列中出现了数字，记录
			if (p1[i] == temp[i1])
			{
				sign = i1;
				break;
			}
		}
		if (sign == -1)
		{
			newLife.push_back(p1[i]);
		}
	}
	//cout << newLife.size() << endl;
	/*for (unsigned int i = 0; i < total; i++)
	{
		cout << newLife[i] << " ";
	}*/
	return newLife;
}

vector<int> tspData::mutation(vector<int> aLife)
{
	vector<int> newOne;//建立新个体
	newOne = aLife;
	int index1, index2, temp;
	index1 = rand() % total; index2 = rand() % total;
	temp = newOne[index1];
	newOne[index1]= newOne[index2];
	newOne[index2] = temp;
	return newOne;
}

vector<int> tspData::newChild(double crossRate, double mutationRate)
{
	vector<int> newOne;
	//从现有种群中选出两个父代
	vector<int> parent1 = getOne();
	vector<int> parent2 = getOne();
	//cout << parent1.size() << " " << parent2.size() << endl;
	double rate;
	//1.直接遗传还是交叉遗传
	rate = (rand() % 1000) / double(1000);
	if (rate < crossRate)
	{
		//cout<<cross(parent1, parent2).size();
		newOne = cross(parent1, parent2);
		//cout << newOne.size() << endl;
		crossCount++;
	}
	else
	{
		newOne = parent1;
	}
	//1.是否出现变异
	rate = (rand() % 1000) / double(1000);
	if (rate < mutationRate)
	{
		newOne = mutation(newOne);
		mutationCount++;
	}
	//cout << newOne.size() << endl;
	return newOne;
}

void tspData::TSPGenetic(int ge, int lifeCount, double crossRate, double mutationRate, string file)
{
	this->lifeCount = lifeCount;
	initPopulation();//初始化种群
	//代际交替
	for (int i = 0; i < ge; i++)
	{
		int minWay = 0;
		getScore();//计算每一代中每个个体的适应度
		vector<vector<int>> newLives;//记录下一代
		//这一代最优个体存活
		bestLife = getBest();
		newLives.push_back(bestLife);
		bestLife.push_back(bestLife.front());
		for (int i1 = 0; i1 < total; i1++)
		{
			minWay += table[getindex(bestLife[i1])][getindex(bestLife[i1 + 1])];
		}
		minWayGenetic.push_back(minWay);
		//cout << minWay << endl;
		//当新一代个体数不够时
		while (newLives.size() < lifeCount)
		{
			//产生的新子代进入种群
			vector<int> temp;
			temp = newChild(crossRate, mutationRate);
			//cout << temp.size();
			newLives.push_back(temp);
		}
		//代际更替
		life = newLives;
	}
	ofstream ofs;
	ofs.open(file, ios::out);
	bestLife = getBest();
	bestLife.push_back(bestLife.front());
	for (int i = 0; i < total; i++)
	{
		//minWay += table[getindex(bestLife[i])][getindex(bestLife[i + 1])];
		ofs << bestLife[i] << endl;
	}
	ofs << bestLife.back();
	ofs.close();
}
/*-----------------------------遗传算法求解--------------------------------*/