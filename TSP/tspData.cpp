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