#include"tspData.h"

/*-----------------------------���ݹ���--------------------------------*/
//�ļ���ȡ �ַ�����Ϊ������
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
	if (m != s.size())//����С������
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

//ͼ tsp���ݵĹ��캯��
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
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	//���ļ����룬�������������
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
	best(infile2);
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
void tspData::best(string infile)
{
	ifstream ifs;
	string file = infile;
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
/*-----------------------------���ݹ���--------------------------------*/


/*-----------------------------̰���㷨���--------------------------------*/
bool tspData::ifIn(int index)
{
	for (unsigned int i = 0; i < wayGreedy.size(); i++)
	{
		//������ڼ�¼�ļ����У�������
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
		//��û�б��� �� ���Ѿ���¼����СֵС ���¼�¼��Сֵ
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
	//����¼
	wayGreedy.push_back(begin);
	//ѭ��������û����ȫ����
	while (wayGreedy.size() < total)
	{
		int x = 0, y = 0;
		//���� ���鳤��
		//cout << minWay.size() << " ";
		//��Ŀǰ��̾����Ӧ�ĵ��¼
		x = wayGreedy[wayGreedy.size() - 1];
		wayGreedy.push_back(minLength(x));
		//��¼·������
		x = wayGreedy[wayGreedy.size() - 2];
		y = wayGreedy[wayGreedy.size() - 1];
		minWayGreedy += table[getindex(x)][getindex(y)];
	}
	//�ļ���¼
	minWayGreedy += table[wayGreedy.back() - 1][begin - 1];
	for (unsigned int i = 0; i < wayGreedy.size(); i++)
	{
		ofs << wayGreedy[i] << endl;
	}
	//�����������ص����
	ofs << begin << endl;
	ofs.close();
	//�ɹ�д���ļ��к� �������
}
/*-----------------------------̰���㷨���--------------------------------*/