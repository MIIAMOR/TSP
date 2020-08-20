#include"tspData.h"

/*-----------------------------数据构造--------------------------------*/
//文件读取 字符串变为整型数
double trans(string s)
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
	return result;
}

//构建点 点的构造函数
point::point(string data)
{
	n = 0; pos[0] = 0; pos[1] = 0;
	double num[3] = { 0 };
	int j = 0;
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
	lifeCount = 0; crossCount = 0; mutationCount = 0;
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
	table = new double* [total];
	for (int i = 0; i < total; i++)
	{
		table[i] = new double[total];
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
	life.clear();
	child1.clear(); child2.clear();
	bestLife.clear(); minWayGenetic.clear();
	score.clear(); judgeTable.clear();
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
	int count = 0;
	while (getline(ifs, temp))
	{
		if ((temp[0] > '9' || temp[0] < '0') && temp[0] != ' ')
		{
			continue;
		}
		if (trans(temp) > 100 && count == 0)
		{
			bestWayLength = trans(temp);
			return;
		}
		record.push_back(trans(temp));
		count++;
	}
	ifs.close();
	//int m = 0;
	double tempLen = 0;
	for (int i = 0; i < record.size() - 1; i++)
	{
		tempLen += table[getindex(record[i])][getindex(record[i + 1])];
		//m++;
	}
	bestWayLength = tempLen;
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
	double tempLen = 0;
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
		tempLen += table[getindex(x)][getindex(y)];
	}
	//文件记录
	tempLen += table[wayGreedy.back() - 1][begin - 1];
	minWayGreedy = tempLen;
	for (unsigned int i = 0; i < wayGreedy.size(); i++)
	{
		ofs << wayGreedy[i] << endl;
	}
	//遍历结束，回到起点
	ofs << begin << endl;
	ofs.close();
	//成功写入文件中后 清空数组
}

void tspData::TSPGreedy1(int begin)
{
	//超出点集合，退出
	if (begin > total || begin <= 0)
	{
		cout << "不存在的起始位置" << endl;
		return;
	}
	//起点记录
	wayGreedy.push_back(begin);
	double tempLen = 0;
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
		tempLen += table[getindex(x)][getindex(y)];
	}
	tempLen += table[wayGreedy.back() - 1][begin - 1];
	minWayGreedy = tempLen;
}
/*-----------------------------贪心算法求解--------------------------------*/


/*-----------------------------遗传算法求解--------------------------------*/
//初始化种群
void tspData::initPopulation()
{
	//lifeCount = 5;
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

//初始化种群1
void tspData::initPopulation1()
{
	//lifeCount = 5;
	//用贪心算法的的解来初始化种群
	life.clear();//先清空种群
	int n = rand() % total + 1;
	TSPGreedy1(n);
	life.push_back(wayGreedy);
	for (int i = 1; i < lifeCount; i++)
	{
		int index = rand() % life.size();
		life.push_back(mutation2(life[index]));
	}
}

//普通适应度函数
void tspData::getScore1()
{
	double totalScore = 0;
	score.clear(); judgeTable.clear();
	for (int i = 0; i < lifeCount; i++)
	{
		int tempLen = 0; double s = 0;
		for (int i1 = 0; i1 < total - 1; i1++)
		{
			tempLen += table[getindex(life[i][i1])][getindex(life[i][i1 + 1])];
		}
		tempLen += table[getindex(life[i][total - 1])][getindex(life[i].front())];
		//cout << tempLen << " ";
		s = 100 * bestWayLength / double(tempLen);
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
}

//差别扩大适应度函数
void tspData::getScore2()
{
	double totalScore = 0;
	score.clear(); judgeTable.clear();
	for (int i = 0; i < lifeCount; i++)
	{
		int tempLen = 0; double s = 0;
		for (int i1 = 0; i1 < total - 1; i1++)
		{
			tempLen += table[getindex(life[i][i1])][getindex(life[i][i1 + 1])];
		}
		tempLen += table[getindex(life[i][total - 1])][getindex(life[i].front())];
		//cout << tempLen << " ";
		s = 100 * bestWayLength / double(tempLen);
		score.push_back(s);
	}
	double tempScore = 100000;
	//求出最小适应度
	for (int i = 0; i < lifeCount; i++)
	{
		if (tempScore > score[i])
		{
			tempScore = score[i];
		}
	}
	//重新计算适应度  为了增加种群多样性，还会在加一个常数  让最差的个体也有繁殖的可能性
	for (int i = 0; i < lifeCount; i++)
	{
		score[i] = score[i] - tempScore * 0.9;
		//cout << score[i] << " ";
		totalScore += score[i];
	}
	//cout << endl;
	//cout << totalScore << " " << endl;
	for (int i = 0; i < lifeCount; i++)
	{
		if (i == 0)
		{
			judgeTable.push_back(score[i] / totalScore);
		}
		else
		{
			int i1 = i - 1;
			judgeTable.push_back(score[i] / totalScore + judgeTable[i1]);
		}
		//cout << judgeTable[i] << " ";
	}
	//cout << endl;
}

//轮盘赌选择
vector<int>& tspData::getOne1()
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

//锦标赛选择
vector<int>& tspData::getOne2()
{
	vector<int> index;//记录第一步被选择的个体下标
	int tempIndex = 0; double tempScore = 0;
	int num = lifeCount / 5;//第一步随机选择个体，数量取种群数的20%
	//开始选择个体
	for (int i = 0; i < num; i++)
	{
		index.push_back(rand() % lifeCount);//个体下标已经存放
	}
	//在已经选择的个体中寻找适应度最高的
	for (int i = 0; i < num; i++)
	{
		if (score[index[i]] > tempScore)
		{
			tempIndex = index[i];
			tempScore = score[i];
		}
	}
	return life[tempIndex];
}

//求一代最优和次优
vector<int>& tspData::getBestandSecond()
{
	bestLife.clear();
	int index = 0; double rate = 0;
	for (int i = 0; i < lifeCount; i++)
	{
		if (score[i] > rate)
		{
			rate = score[i]; index = i;
		}
		//cout << score[i] << " ";
	}
	bestLife = life[index];
	rate = 0; int index2 = 0;
	for (int i = 0; i < lifeCount; i++)
	{
		if (score[i] > rate && score[i] < score[index])
		{
			rate = score[i]; index2 = i;
		}
		//cout << score[i] << " ";
	}
	return life[index2];
}

//两点顺序交叉
void tspData::cross1(vector<int> p1, vector<int> p2)
{
	int index1, index2 = 0;
	index1 = rand() % total;
	index2 = rand() % (total - index1) + index1; //cout << index1 << " " << index2 << endl;
	//把p2中index1到index2的片段交叉放到p1中，形成新的序列
	vector<int> temp1, temp2, newLife1, newLife2;
	for (int i = index1; i <= index2; i++)
	{
		temp1.push_back(p2[i]);
		temp2.push_back(p1[i]);
	}
	for (int i = 0; i < total; i++)
	{
		if (i == index1)
		{
			for (unsigned int i1 = 0; i1 < temp1.size(); i1++)
			{
				newLife1.push_back(temp1[i1]);
				newLife2.push_back(temp2[i1]);
			}
		}
		int sign1 = -1, sign2 = -1;
		for (unsigned int i1 = 0; i1 < temp1.size(); i1++)
		{
			//临时序列中出现了数字，记录
			if (p1[i] == temp1[i1])
			{
				sign1 = i1;
				break;
			}
		}
		for (unsigned int i1 = 0; i1 < temp2.size(); i1++)
		{
			//临时序列中出现了数字，记录
			if (p2[i] == temp2[i1])
			{
				sign2 = i1;
				break;
			}
		}
		if (sign1 == -1)
		{
			newLife1.push_back(p1[i]);
		}
		if (sign2 == -1)
		{
			newLife2.push_back(p2[i]);
		}
	}
	//cout << newLife1.size() << endl;
	/*for (unsigned int i = 0; i < total; i++)
	{
		cout << newLife1[i] << " ";
	}*/
	child1 = newLife1;
	child2 = newLife2;
}

//循环交叉
void tspData::cross2(vector<int> p1, vector<int> p2)
{
	int index, indexTemp;
	//两个新生命 先用-1填充空间，分配内存
	vector<int> newLife1, newLife2;
	for (int i = 0; i < total; i++)
	{
		newLife1.push_back(-1);
		newLife2.push_back(-1);
	}
	//循环交叉 child1
	//第一步：从p1循环继承基因  newLife2的求法相同
	index = rand() % total;
	indexTemp = index;
	newLife1[index] = p1[index];
	while (p2[index] != p1[indexTemp])
	{
		for (int i = 0; i < total; i++)
		{
			if (p2[index] == p1[i])
			{
				index = i; break;
			}
		}
		newLife1[index] = p1[index];
	}
	index = rand() % total;
	indexTemp = index;
	newLife2[index] = p2[index];
	while (p1[index] != p2[indexTemp])
	{
		for (int i = 0; i < total; i++)
		{
			if (p1[index] == p2[i])
			{
				index = i; break;
			}
		}
		newLife2[index] = p2[index];
	}
	//第二步：从p2中的数据填补空位  newLife2的求法相同
	for (int i = 0; i < total; i++)
	{
		if (newLife1[i] == -1)
		{
			newLife1[i] = p2[i];
		}
		if (newLife2[i] == -1)
		{
			newLife2[i] = p1[i];
		}
	}
	child1 = newLife1;
	child2 = newLife2;
}

//交替位置交叉法
void tspData::cross3(vector<int> p1, vector<int> p2)
{
	vector<int> newLife1, newLife2;
	int i1, i2;
	//交替位置交叉，一个父本取一个基因
	i1 = 0; i2 = 0;
	newLife1.push_back(p1[i1]); i1++;
	newLife1.push_back(p2[i2]); i2++;
	while (newLife1.size() < total)
	{
		int sign1 = -1, sign2 = -1;
		for (unsigned int i = 0; i < newLife1.size(); i++)
		{
			if (newLife1[i] == p1[i1])sign1 = i;
		}
		if (sign1 == -1)newLife1.push_back(p1[i1]);
		for (unsigned int i = 0; i < newLife1.size(); i++)
		{
			if (newLife1[i] == p2[i2])sign2 = i;
		}
		if (sign2 == -1)newLife1.push_back(p2[i2]);
		i1++; i2++;
	}
	i1 = 0; i2 = 0;
	newLife2.push_back(p2[i1]); i1++;
	newLife2.push_back(p1[i2]); i2++;
	while (newLife2.size() < total)
	{
		int sign1 = -1, sign2 = -1;
		for (unsigned int i = 0; i < newLife2.size(); i++)
		{
			if (newLife2[i] == p2[i1])sign1 = i;
		}
		if (sign1 == -1)newLife2.push_back(p2[i1]);
		for (unsigned int i = 0; i < newLife2.size(); i++)
		{
			if (newLife2[i] == p1[i2])sign2 = i;
		}
		if (sign2 == -1)newLife2.push_back(p1[i2]);
		i1++; i2++;
	}
	/*for (int i = 0; i < total; i++)
	{
		cout << newLife1[i] << " ";
	}
	for (int i = 0; i < total; i++)
	{
		cout << newLife2[i] << " ";
	}*/
	child1 = newLife1;
	child2 = newLife2;
}

//多随机位置顺序交叉法
void tspData::cross4(vector<int> p1, vector<int> p2)
{
	int m = 40;
	vector<int> newOne1, newOne2;
	//用-1填充站位
	for (int i = 0; i < total; i++)
	{
		newOne1.push_back(-1); newOne2.push_back(-1);
	}
	//以20为分界线，分批次交叉
	int times = total / m;
	//截取片段放到子代
	for (int i = 0; i <= times; i++)
	{
		int index1 = 0, index2 = 0;
		//小于times  考虑m的整数倍
		if (i < times)
		{
			/*----------------把截取到的片段赋值给子代----------------*/
			//随机生成区间大小小于m的坐标点
			index1 = rand() % m + i * m;
			index2 = rand() % (i * m + m - index1) + index1;
			//cout << index1 << " " << index2 << endl;
			for (int i1 = index1; i1 <= index2; i1++)
			{
				newOne1[i1] = p1[i1];
			}
			//随机生成区间大小小于20的坐标点
			index1 = rand() % m + i * m;
			index2 = rand() % (i * m + m - index1) + index1;
			//cout << index1 << " " << index2 << endl;
			for (int i1 = index1; i1 <= index2; i1++)
			{
				newOne2[i1] = p2[i1];
			}
		}
		//尾巴处理
		else if (total - i * m != 0)
		{
			index1 = rand() % (total - i * m) + i * m;
			index2 = rand() % (total - index1) + index1;
			//cout << index1 << " " << index2 << endl;
			for (int i1 = index1; i1 <= index2; i1++)
			{
				newOne1[i1] = p1[i1];
			}
			index1 = rand() % (total - i * m) + i * m;
			index2 = rand() % (total - index1) + index1;
			//cout << index1 << " " << index2 << endl;
			for (int i1 = index1; i1 <= index2; i1++)
			{
				newOne2[i1] = p2[i1];
			}
		}
	}
	//填充子代剩下的片段
	int index1 = 0, index2 = 0;
	for (int i = 0; i < total; i++)
	{
		int sign1 = -1, sign2 = -1;
		for (int i1 = 0; i1 < total; i1++)
		{
			//newOne填充 判断
			if (p2[i] == newOne1[i1])sign1 = i1;
			if (p1[i] == newOne2[i1])sign2 = i1;
			//退出循环判断
			if (sign1 != -1 && sign2 != -1)break;
		}
		if (index1 < total)
		{
			while (newOne1[index1] != -1 && index1 < total - 1)index1++;
		}
		if (index2 < total)
		{
			while (newOne2[index2] != -1 && index2 < total - 1)index2++;
		}
		if (sign1 == -1 && index1 < total)newOne1[index1] = p2[i];
		if (sign2 == -1 && index2 < total)newOne2[index2] = p1[i];
	}
	/*sort(newOne1.begin(), newOne1.end());
	sort(newOne2.begin(), newOne2.end());
	for (int i = 0; i < total; i++)
	{
		cout << newOne1[i] << " " << newOne2[i] << " ";
	}*/
	//cout << newOne1.size() << " " << newOne2.size() << endl;
	//child1 = newOne1; child2 = newOne2;
	cross1(newOne1, newOne2);
}

//两点换位变异
vector<int> tspData::mutation1(vector<int> aLife)
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

//多随机点位置重排列
vector<int> tspData::mutation2(vector<int> aLife)
{
	vector<int> newOne = aLife;
	int num = total / 4;//限定最大范围
	num = rand() % num;//随机生成发生变异的点数
	vector<int> point, order;//记录被选择的点的下标 以及一个 随机位置顺序
	for (int i = 0; i < num; i++)
	{
		int tempPoint = rand() % total;
		int sign = -1;
		for (int i1 = 0; i1 < point.size(); i1++)
		{
			if (tempPoint == point[i1])
			{
				sign = i1; break;
			}
		}
		if (sign == -1)
		{
			point.push_back(tempPoint);
		}
		else
		{
			i--;
		}
	}
	for (int i = 0; i < num; i++)
	{
		order.push_back(i);
	}
	random_shuffle(order.begin(), order.end());
	//按照随机生成的提示进行变异
	for (int i = 0; i < num; i++)
	{
		newOne[point[i]] = aLife[point[order[i]]];
	}
	/*sort(newOne.begin(), newOne.end());
	for (int i = 0; i < total; i++)
	{
		cout << newOne[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < total; i++)
	{
		cout << aLife[i] << " ";
	}*/
	return newOne;
}

//产生新个体
void tspData::newChild1(double crossRate, double mutationRate)
{
	child1.clear(); child2.clear();
	//从现有种群中选出两个父代
	//轮盘赌选择算子
	vector<int> parent1 = getOne1();
	vector<int> parent2 = getOne1();
	////锦标赛选择算子
	//vector<int> parent1 = getOne2();
	//vector<int> parent2 = getOne2();
	//选择不同的父代交叉遗传
	for (int i = 0; i < 5; i++)
	{
		if (parent1 != parent2)
		{
			break;
		}
		parent1 = getOne1();
		if (i == 4)
		{
			parent1 = mutation2(parent1);
		}
		//parent1 = getOne2();
	}
	double tempLen1 = 0, tempLen2 = 0;
	double tempLen, tempLenMax, tempLenMin;
	for (int i1 = 0; i1 < total - 1; i1++)
	{
		tempLen1 += table[getindex(parent1[i1])][getindex(parent1[i1 + 1])];
		tempLen2 += table[getindex(parent2[i1])][getindex(parent2[i1 + 1])];
	}
	tempLen1 += table[getindex(parent1[total - 1])][getindex(parent1.front())];
	tempLen2 += table[getindex(parent2[total - 1])][getindex(parent2.front())];
	tempLenMin = tempLen1 < tempLen2 ? tempLen1 : tempLen2;
	tempLenMax = tempLen1 > tempLen2 ? tempLen1 : tempLen2;
	tempLen = (tempLen1 + tempLen2) / 2;
	double rate;
	//1.直接遗传还是交叉遗传
	rate = (rand() % 10000) / double(10000);
	if (rate < crossRate)
	{
		vector<int> tchild1 = parent1, tchild2 = parent2;//临时存储最优子代
		double child1Len = tempLen1, child2Len = tempLen2;
		int count = 0;
		while (true)
		{
			count++;
			////循环交叉
			//if (parent1 == parent2)cross2(parent1, mutation2(parent2));
			//else cross2(parent1, parent2);
			//两点交叉算子
			if (parent1 == parent2)parent2 = mutation2(parent2);
			if (total <= 60)cross1(parent1, parent2);
			else
			{
				//多随机点混合顺序交叉
				cross4(parent1, parent2);
			}
			//cross4(parent1, parent2);
			//增加功能，新子代 必须优于父代
			double cLen1 = 0, cLen2 = 0;
			double cLen = 0, cLenMax = 0, cLenMin = 0;
			for (int i1 = 0; i1 < total - 1; i1++)
			{
				cLen1 += table[getindex(child1[i1])][getindex(child1[i1 + 1])];
				cLen2 += table[getindex(child2[i1])][getindex(child2[i1 + 1])];
			}
			cLen1 += table[getindex(child1[total - 1])][getindex(child1.front())];
			cLen2 += table[getindex(child2[total - 1])][getindex(child2.front())];
			//增加临时变量，记录过程中的最优子代
			cLen = (cLen1 + cLen2) / 2;
			cLenMin = cLen1 < cLen2 ? cLen1 : cLen2;
			cLenMax = cLen1 > cLen2 ? cLen1 : cLen2;
			if (child1Len == child2Len)
			{
				child2Len = cLen1;
				tchild2 = child1;
			}
			if (child1Len > cLen1 || child1Len > cLen2)
			{
				if (cLen1 < cLen2)
				{
					child1Len = cLen1;
					tchild1 = child1;
				}
				else
				{
					child1Len = cLen2;
					tchild1 = child2;
				}
			}
			if (child2Len > cLen1 || child2Len > cLen2)
			{
				if (cLen1 < cLen2 && cLen1 != child1Len)
				{
					child2Len = cLen1;
					tchild2 = child1;
				}
				else if(cLen1 > cLen2 && cLen2 != child1Len)
				{
					child2Len = cLen2;
					tchild2 = child2;
				}
			}
			//cout << cLenMin << " " << tempLenMin << " "<<count<<endl;
			int crossNum = lifeCount;//交叉次数
			double rate = tempLenMax / bestWayLength;//求出两个父本中最差结果的近似度 然后调整交叉次数
			//大于等于2的情况  次数取种群大小
			if (rate >= 2)crossNum = lifeCount / 3;
			//1.5到2的情况   次数取种群大小的2/3
			else if (rate >= 1.5 && rate < 2)crossNum = 3 * lifeCount / 8;
			//1.2到1.5的情况  次数取种群大小的1/3
			else if (rate >= 1.2 && rate < 1.5)crossNum = lifeCount / 6;
			//小于1.2的情况  次数取种群大小的1/6
			else if (rate < 1.2)crossNum = lifeCount / 8;
			if (crossNum > 8)crossNum = 8;
			if (cLenMin < tempLenMin || count > crossNum)
			{	
				if (count == crossNum + 1)
				{
					child1 = tchild1; child2 = tchild2;
					//cout << tempLen1 << " " << tempLen2 << " " << child1Len << " " << child2Len << " " << count << endl;
				}
				else
				{
					//cout << tempLen1 << " " << tempLen2 << " " << cLen1 << " " << cLen2 << " " << count << endl;
				}
				count = 0;
				break;
			}
		}
		crossCount++;
	}
	else
	{
		child1 = parent1;
		child2 = parent2;
	}
	//1.是否出现变异
	//根据收敛程度适当调整变异率
	double degree = tempLenMin / bestWayLength;
	if (degree < 2 && degree >= 1.5)mutationRate *= 1.2;
	else if (degree < 1.5 && degree >= 1.2)mutationRate *= 1.7;
	else if (degree < 1.2)mutationRate *= 2;
	rate = (rand() % 10000) / double(10000);
	if (rate < mutationRate)
	{
		////两点交换变异
		//child1 = mutation1(child1);
		//child2 = mutation1(child2);
		//随机点位置重排列
		child1 = mutation2(child1);
		child2 = mutation2(child2);
		mutationCount++;
	}
}

//产生新个体
void tspData::newChild2(double crossRate, double mutationRate)
{
	child1.clear(); child2.clear();
	//从现有种群中选出两个父代
	////轮盘赌选择算子
	vector<int> parent1 = getOne1();
	vector<int> parent2 = getOne1();
	double rate;
	//1.直接遗传还是交叉遗传
	rate = (rand() % 10000) / double(10000);
	if (rate < crossRate)
	{
		cross4(parent1, parent2);
		crossCount++;
	}
	else
	{
		child1 = parent1;
		child2 = parent2;
	}
	//1.是否出现变异
	//根据收敛程度适当调整变异率
	rate = (rand() % 10000) / double(10000);
	double degree = (double)minWayGA / bestWayLength;
	if (degree <= 1.5 && degree > 1.3)mutationRate *= 1.5;
	else if (degree <= 1.3 && degree > 1.1)mutationRate *= 2;
	else if (degree <= 1.1 && degree > 1)mutationRate *= 2.5;
	if (rate < mutationRate)
	{
		////两点交换变异
		//child1 = mutation1(child1);
		//child2 = mutation1(child2);
		//随机点位置重排列
		child1 = mutation2(child1);
		child2 = mutation2(child2);
		mutationCount++;
	}
}

void tspData::TSPGenetic1(int ge, int lifeCount, double crossRate, double mutationRate, string file)
{
	minWayGA = INT_MAX;
	this->lifeCount = lifeCount;
	initPopulation();//初始化种群
	//代际交替
	for (int i = 0; i < ge; i++)
	{
		int minWay = 0;
		vector<int> tempOne;
		getScore2();//计算每一代中每个个体的适应度
		vector<vector<int>> newLives;//记录下一代
		//这一代最优个体存活
		tempOne = getBestandSecond();
		newLives.push_back(bestLife);
		newLives.push_back(tempOne);
		//每代最好个体发生一次变异  方便跳出局部最优
		newLives.push_back(mutation1(bestLife));
		//newLives.push_back(mutation2(bestLife));
		bestLife.push_back(bestLife.front());
		for (int i1 = 0; i1 < total; i1++)
		{
			minWay += table[getindex(bestLife[i1])][getindex(bestLife[i1 + 1])];
		}
		minWayGenetic.push_back(minWay);
		cout << "第" << minWayGenetic.size() << " 代最优个体对应的路程长度为: " << minWay;
		cout << "   对应的近似度为 " << (double)minWay / bestWayLength << endl;
		if (minWayGA > minWayGenetic.back())
		{
			minWayGA = minWayGenetic.back();
			wayGA = bestLife;
		}
		minWayGA = minWayGenetic.back() < minWayGA ? minWayGenetic.back() : minWayGA;
		//cout << minWay << endl;
		//当新一代个体数不够时
		while (newLives.size() < lifeCount)
		{
			//产生的新子代进入种群
			newChild1(crossRate, mutationRate);
			newLives.push_back(child1);
			newLives.push_back(child2);
		}
		//代际更替
		life = newLives;
	}
	ofstream ofs;
	ofs.open(file, ios::out);
	for (int i = 0; i < wayGA.size() ; i++)
	{
		ofs << wayGA[i] << endl;
	}
	ofs.close();
}

void tspData::TSPGenetic2(int ge, int lifeCount, double crossRate, double mutationRate, string file)
{
	minWayGA = INT_MAX;
	this->lifeCount = lifeCount;
	initPopulation();//初始化种群
	//initPopulation1();//初始化种群
	int i = 0;
	//代际交替
	while ((i >= ge && minWayGenetic[i - 1] != minWayGenetic[i - ge]) || i < ge)
	{
		double minWay = 0;
		vector<int> tempOne;
		getScore2();//计算每一代中每个个体的适应度
		vector<vector<int>> newLives;//记录下一代
		//这一代最优个体和次优个体存活
		tempOne = getBestandSecond();
		newLives.push_back(bestLife);
		newLives.push_back(tempOne);
		//每代最好个体发生一次变异  方便跳出局部最优
		//newLives.push_back(mutation1(bestLife));
		newLives.push_back(mutation2(bestLife));
		newLives.push_back(mutation2(tempOne));
		bestLife.push_back(bestLife.front());
		for (int i1 = 0; i1 < total; i1++)
		{
			minWay += table[getindex(bestLife[i1])][getindex(bestLife[i1 + 1])];
		}
		minWayGenetic.push_back(minWay);
		cout << "第" << minWayGenetic.size() << " 代最优个体对应的路程长度为: " << minWay;
		cout << "   对应的近似度为 " << (double)minWay / bestWayLength << endl;
		if (minWayGA > minWayGenetic.back())
		{
			minWayGA = minWayGenetic.back();
			wayGA = bestLife;
		}
		//cout << minWay << endl;
		//当新一代个体数不够时
		while (newLives.size() < lifeCount)
		{
			//产生的新子代进入种群
			if (total >= 150)
			{
				newChild2(crossRate, mutationRate);
			}
			else
			{
				newChild1(crossRate, mutationRate);
			}
			newLives.push_back(child1);
			newLives.push_back(child2);
		}
		//代际更替
		life = newLives;
		i++;
	}
	ofstream ofs;
	ofs.open(file, ios::out);
	for (int i = 0; i < wayGA.size(); i++)
	{
		ofs << wayGA[i] << endl;
	}
	ofs.close();
}

/*-----------------------------遗传算法求解--------------------------------*/