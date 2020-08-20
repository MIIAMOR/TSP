#include"tspData.h"

/*-----------------------------���ݹ���--------------------------------*/
//�ļ���ȡ �ַ�����Ϊ������
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
	return result;
}

//������ ��Ĺ��캯��
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

//ͼ tsp���ݵĹ��캯��
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
	table = new double* [total];
	for (int i = 0; i < total; i++)
	{
		table[i] = new double[total];
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
	life.clear();
	child1.clear(); child2.clear();
	bestLife.clear(); minWayGenetic.clear();
	score.clear(); judgeTable.clear();
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
	double tempLen = 0;
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
		tempLen += table[getindex(x)][getindex(y)];
	}
	//�ļ���¼
	tempLen += table[wayGreedy.back() - 1][begin - 1];
	minWayGreedy = tempLen;
	for (unsigned int i = 0; i < wayGreedy.size(); i++)
	{
		ofs << wayGreedy[i] << endl;
	}
	//�����������ص����
	ofs << begin << endl;
	ofs.close();
	//�ɹ�д���ļ��к� �������
}

void tspData::TSPGreedy1(int begin)
{
	//�����㼯�ϣ��˳�
	if (begin > total || begin <= 0)
	{
		cout << "�����ڵ���ʼλ��" << endl;
		return;
	}
	//����¼
	wayGreedy.push_back(begin);
	double tempLen = 0;
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
		tempLen += table[getindex(x)][getindex(y)];
	}
	tempLen += table[wayGreedy.back() - 1][begin - 1];
	minWayGreedy = tempLen;
}
/*-----------------------------̰���㷨���--------------------------------*/


/*-----------------------------�Ŵ��㷨���--------------------------------*/
//��ʼ����Ⱥ
void tspData::initPopulation()
{
	//lifeCount = 5;
	life.clear();//�������Ⱥ
	//ɨ��ÿһ������
	vector<int> lifeTemp;
	for (int i = 0; i < lifeCount; i++)
	{
		for (int i1 = 0; i1 < total; i1++)
		{
			lifeTemp.push_back(i1 + 1);
		}
		//�������
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

//��ʼ����Ⱥ1
void tspData::initPopulation1()
{
	//lifeCount = 5;
	//��̰���㷨�ĵĽ�����ʼ����Ⱥ
	life.clear();//�������Ⱥ
	int n = rand() % total + 1;
	TSPGreedy1(n);
	life.push_back(wayGreedy);
	for (int i = 1; i < lifeCount; i++)
	{
		int index = rand() % life.size();
		life.push_back(mutation2(life[index]));
	}
}

//��ͨ��Ӧ�Ⱥ���
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

//���������Ӧ�Ⱥ���
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
	//�����С��Ӧ��
	for (int i = 0; i < lifeCount; i++)
	{
		if (tempScore > score[i])
		{
			tempScore = score[i];
		}
	}
	//���¼�����Ӧ��  Ϊ��������Ⱥ�����ԣ������ڼ�һ������  �����ĸ���Ҳ�з�ֳ�Ŀ�����
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

//���̶�ѡ��
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
	cout << "�����ڵĸ��壬���ִ���" << endl;
	exit(0);
}

//������ѡ��
vector<int>& tspData::getOne2()
{
	vector<int> index;//��¼��һ����ѡ��ĸ����±�
	int tempIndex = 0; double tempScore = 0;
	int num = lifeCount / 5;//��һ�����ѡ����壬����ȡ��Ⱥ����20%
	//��ʼѡ�����
	for (int i = 0; i < num; i++)
	{
		index.push_back(rand() % lifeCount);//�����±��Ѿ����
	}
	//���Ѿ�ѡ��ĸ�����Ѱ����Ӧ����ߵ�
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

//��һ�����źʹ���
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

//����˳�򽻲�
void tspData::cross1(vector<int> p1, vector<int> p2)
{
	int index1, index2 = 0;
	index1 = rand() % total;
	index2 = rand() % (total - index1) + index1; //cout << index1 << " " << index2 << endl;
	//��p2��index1��index2��Ƭ�ν���ŵ�p1�У��γ��µ�����
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
			//��ʱ�����г��������֣���¼
			if (p1[i] == temp1[i1])
			{
				sign1 = i1;
				break;
			}
		}
		for (unsigned int i1 = 0; i1 < temp2.size(); i1++)
		{
			//��ʱ�����г��������֣���¼
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

//ѭ������
void tspData::cross2(vector<int> p1, vector<int> p2)
{
	int index, indexTemp;
	//���������� ����-1���ռ䣬�����ڴ�
	vector<int> newLife1, newLife2;
	for (int i = 0; i < total; i++)
	{
		newLife1.push_back(-1);
		newLife2.push_back(-1);
	}
	//ѭ������ child1
	//��һ������p1ѭ���̳л���  newLife2������ͬ
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
	//�ڶ�������p2�е��������λ  newLife2������ͬ
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

//����λ�ý��淨
void tspData::cross3(vector<int> p1, vector<int> p2)
{
	vector<int> newLife1, newLife2;
	int i1, i2;
	//����λ�ý��棬һ������ȡһ������
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

//�����λ��˳�򽻲淨
void tspData::cross4(vector<int> p1, vector<int> p2)
{
	int m = 40;
	vector<int> newOne1, newOne2;
	//��-1���վλ
	for (int i = 0; i < total; i++)
	{
		newOne1.push_back(-1); newOne2.push_back(-1);
	}
	//��20Ϊ�ֽ��ߣ������ν���
	int times = total / m;
	//��ȡƬ�ηŵ��Ӵ�
	for (int i = 0; i <= times; i++)
	{
		int index1 = 0, index2 = 0;
		//С��times  ����m��������
		if (i < times)
		{
			/*----------------�ѽ�ȡ����Ƭ�θ�ֵ���Ӵ�----------------*/
			//������������СС��m�������
			index1 = rand() % m + i * m;
			index2 = rand() % (i * m + m - index1) + index1;
			//cout << index1 << " " << index2 << endl;
			for (int i1 = index1; i1 <= index2; i1++)
			{
				newOne1[i1] = p1[i1];
			}
			//������������СС��20�������
			index1 = rand() % m + i * m;
			index2 = rand() % (i * m + m - index1) + index1;
			//cout << index1 << " " << index2 << endl;
			for (int i1 = index1; i1 <= index2; i1++)
			{
				newOne2[i1] = p2[i1];
			}
		}
		//β�ʹ���
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
	//����Ӵ�ʣ�µ�Ƭ��
	int index1 = 0, index2 = 0;
	for (int i = 0; i < total; i++)
	{
		int sign1 = -1, sign2 = -1;
		for (int i1 = 0; i1 < total; i1++)
		{
			//newOne��� �ж�
			if (p2[i] == newOne1[i1])sign1 = i1;
			if (p1[i] == newOne2[i1])sign2 = i1;
			//�˳�ѭ���ж�
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

//���㻻λ����
vector<int> tspData::mutation1(vector<int> aLife)
{
	vector<int> newOne;//�����¸���
	newOne = aLife;
	int index1, index2, temp;
	index1 = rand() % total; index2 = rand() % total;
	temp = newOne[index1];
	newOne[index1]= newOne[index2];
	newOne[index2] = temp;
	return newOne;
}

//�������λ��������
vector<int> tspData::mutation2(vector<int> aLife)
{
	vector<int> newOne = aLife;
	int num = total / 4;//�޶����Χ
	num = rand() % num;//������ɷ�������ĵ���
	vector<int> point, order;//��¼��ѡ��ĵ���±� �Լ�һ�� ���λ��˳��
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
	//����������ɵ���ʾ���б���
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

//�����¸���
void tspData::newChild1(double crossRate, double mutationRate)
{
	child1.clear(); child2.clear();
	//��������Ⱥ��ѡ����������
	//���̶�ѡ������
	vector<int> parent1 = getOne1();
	vector<int> parent2 = getOne1();
	////������ѡ������
	//vector<int> parent1 = getOne2();
	//vector<int> parent2 = getOne2();
	//ѡ��ͬ�ĸ��������Ŵ�
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
	//1.ֱ���Ŵ����ǽ����Ŵ�
	rate = (rand() % 10000) / double(10000);
	if (rate < crossRate)
	{
		vector<int> tchild1 = parent1, tchild2 = parent2;//��ʱ�洢�����Ӵ�
		double child1Len = tempLen1, child2Len = tempLen2;
		int count = 0;
		while (true)
		{
			count++;
			////ѭ������
			//if (parent1 == parent2)cross2(parent1, mutation2(parent2));
			//else cross2(parent1, parent2);
			//���㽻������
			if (parent1 == parent2)parent2 = mutation2(parent2);
			if (total <= 60)cross1(parent1, parent2);
			else
			{
				//���������˳�򽻲�
				cross4(parent1, parent2);
			}
			//cross4(parent1, parent2);
			//���ӹ��ܣ����Ӵ� �������ڸ���
			double cLen1 = 0, cLen2 = 0;
			double cLen = 0, cLenMax = 0, cLenMin = 0;
			for (int i1 = 0; i1 < total - 1; i1++)
			{
				cLen1 += table[getindex(child1[i1])][getindex(child1[i1 + 1])];
				cLen2 += table[getindex(child2[i1])][getindex(child2[i1 + 1])];
			}
			cLen1 += table[getindex(child1[total - 1])][getindex(child1.front())];
			cLen2 += table[getindex(child2[total - 1])][getindex(child2.front())];
			//������ʱ��������¼�����е������Ӵ�
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
			int crossNum = lifeCount;//�������
			double rate = tempLenMax / bestWayLength;//�������������������Ľ��ƶ� Ȼ������������
			//���ڵ���2�����  ����ȡ��Ⱥ��С
			if (rate >= 2)crossNum = lifeCount / 3;
			//1.5��2�����   ����ȡ��Ⱥ��С��2/3
			else if (rate >= 1.5 && rate < 2)crossNum = 3 * lifeCount / 8;
			//1.2��1.5�����  ����ȡ��Ⱥ��С��1/3
			else if (rate >= 1.2 && rate < 1.5)crossNum = lifeCount / 6;
			//С��1.2�����  ����ȡ��Ⱥ��С��1/6
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
	//1.�Ƿ���ֱ���
	//���������̶��ʵ�����������
	double degree = tempLenMin / bestWayLength;
	if (degree < 2 && degree >= 1.5)mutationRate *= 1.2;
	else if (degree < 1.5 && degree >= 1.2)mutationRate *= 1.7;
	else if (degree < 1.2)mutationRate *= 2;
	rate = (rand() % 10000) / double(10000);
	if (rate < mutationRate)
	{
		////���㽻������
		//child1 = mutation1(child1);
		//child2 = mutation1(child2);
		//�����λ��������
		child1 = mutation2(child1);
		child2 = mutation2(child2);
		mutationCount++;
	}
}

//�����¸���
void tspData::newChild2(double crossRate, double mutationRate)
{
	child1.clear(); child2.clear();
	//��������Ⱥ��ѡ����������
	////���̶�ѡ������
	vector<int> parent1 = getOne1();
	vector<int> parent2 = getOne1();
	double rate;
	//1.ֱ���Ŵ����ǽ����Ŵ�
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
	//1.�Ƿ���ֱ���
	//���������̶��ʵ�����������
	rate = (rand() % 10000) / double(10000);
	double degree = (double)minWayGA / bestWayLength;
	if (degree <= 1.5 && degree > 1.3)mutationRate *= 1.5;
	else if (degree <= 1.3 && degree > 1.1)mutationRate *= 2;
	else if (degree <= 1.1 && degree > 1)mutationRate *= 2.5;
	if (rate < mutationRate)
	{
		////���㽻������
		//child1 = mutation1(child1);
		//child2 = mutation1(child2);
		//�����λ��������
		child1 = mutation2(child1);
		child2 = mutation2(child2);
		mutationCount++;
	}
}

void tspData::TSPGenetic1(int ge, int lifeCount, double crossRate, double mutationRate, string file)
{
	minWayGA = INT_MAX;
	this->lifeCount = lifeCount;
	initPopulation();//��ʼ����Ⱥ
	//���ʽ���
	for (int i = 0; i < ge; i++)
	{
		int minWay = 0;
		vector<int> tempOne;
		getScore2();//����ÿһ����ÿ���������Ӧ��
		vector<vector<int>> newLives;//��¼��һ��
		//��һ�����Ÿ�����
		tempOne = getBestandSecond();
		newLives.push_back(bestLife);
		newLives.push_back(tempOne);
		//ÿ����ø��巢��һ�α���  ���������ֲ�����
		newLives.push_back(mutation1(bestLife));
		//newLives.push_back(mutation2(bestLife));
		bestLife.push_back(bestLife.front());
		for (int i1 = 0; i1 < total; i1++)
		{
			minWay += table[getindex(bestLife[i1])][getindex(bestLife[i1 + 1])];
		}
		minWayGenetic.push_back(minWay);
		cout << "��" << minWayGenetic.size() << " �����Ÿ����Ӧ��·�̳���Ϊ: " << minWay;
		cout << "   ��Ӧ�Ľ��ƶ�Ϊ " << (double)minWay / bestWayLength << endl;
		if (minWayGA > minWayGenetic.back())
		{
			minWayGA = minWayGenetic.back();
			wayGA = bestLife;
		}
		minWayGA = minWayGenetic.back() < minWayGA ? minWayGenetic.back() : minWayGA;
		//cout << minWay << endl;
		//����һ������������ʱ
		while (newLives.size() < lifeCount)
		{
			//���������Ӵ�������Ⱥ
			newChild1(crossRate, mutationRate);
			newLives.push_back(child1);
			newLives.push_back(child2);
		}
		//���ʸ���
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
	initPopulation();//��ʼ����Ⱥ
	//initPopulation1();//��ʼ����Ⱥ
	int i = 0;
	//���ʽ���
	while ((i >= ge && minWayGenetic[i - 1] != minWayGenetic[i - ge]) || i < ge)
	{
		double minWay = 0;
		vector<int> tempOne;
		getScore2();//����ÿһ����ÿ���������Ӧ��
		vector<vector<int>> newLives;//��¼��һ��
		//��һ�����Ÿ���ʹ��Ÿ�����
		tempOne = getBestandSecond();
		newLives.push_back(bestLife);
		newLives.push_back(tempOne);
		//ÿ����ø��巢��һ�α���  ���������ֲ�����
		//newLives.push_back(mutation1(bestLife));
		newLives.push_back(mutation2(bestLife));
		newLives.push_back(mutation2(tempOne));
		bestLife.push_back(bestLife.front());
		for (int i1 = 0; i1 < total; i1++)
		{
			minWay += table[getindex(bestLife[i1])][getindex(bestLife[i1 + 1])];
		}
		minWayGenetic.push_back(minWay);
		cout << "��" << minWayGenetic.size() << " �����Ÿ����Ӧ��·�̳���Ϊ: " << minWay;
		cout << "   ��Ӧ�Ľ��ƶ�Ϊ " << (double)minWay / bestWayLength << endl;
		if (minWayGA > minWayGenetic.back())
		{
			minWayGA = minWayGenetic.back();
			wayGA = bestLife;
		}
		//cout << minWay << endl;
		//����һ������������ʱ
		while (newLives.size() < lifeCount)
		{
			//���������Ӵ�������Ⱥ
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
		//���ʸ���
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

/*-----------------------------�Ŵ��㷨���--------------------------------*/