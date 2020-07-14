#include"tspData.h"

/*-----------------------------���ݹ���--------------------------------*/
//�ļ���ȡ �ַ�����Ϊ������
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

//������ ��Ĺ��캯��
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

//ͼ tsp���ݵĹ��캯��
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
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	//���ļ����룬�������������
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
	//���Դ��룬�Ƿ�ɹ���������
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
	//�����õ����������õ��ڽӾ���
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
	//���Դ��룬�Ƿ�ɹ���ȡ�ļ�
	/*for (int i = 0; i < total; i++)
	{
		for (int j = 0; j < total; j++)
		{
			cout << table[i][j] << " ";
		}
		cout << endl;
	}*/
}

//�������� �ͷŶ����ռ��Լ��������
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

//���ֲ������±�
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

//���Ž�
void tspData::best()
{
	ifstream ifs;
	string file = bestfile;
	ifs.open(file, ios::in);
	if (!ifs.is_open())
	{
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	//���ļ��ж����Ž�
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
		//������ڼ�¼�ļ����У�������
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
		//��û�б��� �� ���Ѿ���¼����СֵС ���¼�¼��Сֵ
		if (!ifIn(points[i].n) && table[getindex(index)][i] < temp)
		{
			temp = table[getindex(index)][i];
			index_find = i;
		}
	}
	return points[index_find].n;
}
/*-----------------------------���ݹ���--------------------------------*/

/*-----------------------------̰���㷨���--------------------------------*/
void tspData::TSP1(string file, int begin)
{
	//�����㼯�ϣ��˳�
	if (begin > total || begin <= 0)
	{
		cout << "�����ڵ���ʼλ��" << endl;
		return;
	}
	//�򿪶�Ӧ���ļ�
	ofstream ofs;
	ofs.open(file, ios::out);
	if (!ofs.is_open())
	{
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	cout << "<----������̰���㷨˼�����TSP����---->" << endl;
	cout << "̰��˼��ֻ�����ÿһ���ľֲ����·����������ս���ǽ������Ž�" << endl;
	cout << "��������� data/testResult1.txt �ļ���" << endl;
	//����¼
	minWay.push_back(begin);
	//ѭ��������û����ȫ����
	while (minWay.size() < total)
	{
		int x = 0, y = 0;
		//���� ���鳤��
		//cout << minWay.size() << " ";
		//��Ŀǰ��̾����Ӧ�ĵ��¼
		x = minWay[minWay.size() - 1];
		minWay.push_back(minLength(x));
		//��¼·������
		x = minWay[minWay.size() - 2];
		y = minWay[minWay.size() - 1];
		minWayLength += table[getindex(x)][getindex(y)];
	}
	//�ļ���¼
	ofs << "̰���㷨˼���µ�����·��Ϊ��" << endl;
	ofs << "̰��˼��ֻ�����ÿһ��ֻ����þֲ����·����������ս���ǽ������Ž�" << endl;
	minWayLength += table[minWay.back() - 1][begin - 1];
	ofs << "\n���·�̵ĳ���Ϊ��" << minWayLength << endl;
	ofs << "���������·��Ϊ��" << bestWayLength << endl;
	ofs << "���㷨�����Ž�Ľ��ƶ�Ϊ:" << minWayLength / double(bestWayLength) << endl;
	for (unsigned int i = 0; i < minWay.size(); i++)
	{
		ofs << minWay[i] << endl;
	}
	//�����������ص����
	ofs << begin << endl;
	ofs.close();
	//�ɹ�д���ļ��к� �������
	minWay.clear();
	minWayLength = 0;
}
/*-----------------------------̰���㷨���--------------------------------*/