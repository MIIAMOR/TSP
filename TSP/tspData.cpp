#include"tspData.h"

/*-----------------------------数据构造--------------------------------*/
//文件读取 字符串变为整型数
int trans(string num)
{
	int result = 0;
	for (int i = num.size() - 1; i >= 0; i--)
	{
		int temp = 0;
		temp = num[i] - '0';
		temp = temp * pow(10, num.size() - i - 1);
		result += temp;
	}
	return result;
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
		else
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
tspData::tspData(string infile, string outfile)
{
	points.clear();
	minWay.clear();
	minWayLength = 0;
	total = 0;
	bestWayLength = 0;
	table = NULL;
	ifstream ifs;
	ifs.open(infile, ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		return;
	}
	//把文件读入，并构建点的数组
	string temp;
	while (getline(ifs, temp))
	{
		if (temp[0] > '9' || temp[0] < '0')
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
	best();
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
	minWay.clear();
	if (table != NULL)
	{
		for (int i = 0; i < total; i++)
		{
			delete[] table[i];
		}
		delete[] table;
	}
	total = 0;
	minWayLength = 0;
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
void tspData::best()
{
	ifstream ifs;
	string file = bestfile;
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
		if (temp[0] > '9' || temp[0] < '0')
		{
			continue;
		}
		record.push_back(trans(temp));
	}
	ifs.close();
	record.push_back(1); //int m = 0;
	for (unsigned int i = 0; i < record.size() - 1; i++)
	{
		bestWayLength += table[getindex(record[i])][getindex(record[i + 1])];
		//m++;
	}
	//cout << m << endl;
}

bool tspData::ifIn(int index)
{
	for (unsigned int i = 0; i < minWay.size(); i++)
	{
		//如果点在记录的集合中，返回真
		if (index == minWay[i])
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
/*-----------------------------数据构造--------------------------------*/

/*-----------------------------贪心算法求解--------------------------------*/
void tspData::TSP1(string file, int begin)
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
	cout << "<----以下是贪心算法思想求解TSP问题---->" << endl;
	cout << "贪心思想只能求得每一步的局部最短路径，因此最终结果是近似最优解" << endl;
	cout << "结果保存在 data/testResult1.txt 文件中" << endl;
	//起点记录
	minWay.push_back(begin);
	//循环条件：没有完全遍历
	while (minWay.size() < total)
	{
		int x = 0, y = 0;
		//测试 数组长度
		//cout << minWay.size() << " ";
		//把目前最短距离对应的点记录
		x = minWay[minWay.size() - 1];
		minWay.push_back(minLength(x));
		//记录路径长度
		x = minWay[minWay.size() - 2];
		y = minWay[minWay.size() - 1];
		minWayLength += table[getindex(x)][getindex(y)];
	}
	//文件记录
	ofs << "贪心算法思想下的最优路线为：" << endl;
	ofs << "贪心思想只能求得每一步只能求得局部最短路径，因此最终结果是近似最优解" << endl;
	minWayLength += table[minWay.back() - 1][begin - 1];
	ofs << "\n最短路程的长度为：" << minWayLength << endl;
	ofs << "真正的最短路径为：" << bestWayLength << endl;
	ofs << "此算法和最优解的近似度为:" << minWayLength / double(bestWayLength) << endl;
	for (unsigned int i = 0; i < minWay.size(); i++)
	{
		ofs << minWay[i] << endl;
	}
	//遍历结束，回到起点
	ofs << begin << endl;
	ofs.close();
	//成功写入文件中后 清空数组
	minWay.clear();
	minWayLength = 0;
}
/*-----------------------------贪心算法求解--------------------------------*/