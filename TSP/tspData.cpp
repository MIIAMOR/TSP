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
	minWayGreedy = 0;
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


/*-----------------------------�Ŵ��㷨���--------------------------------*/
void tspData::initPopulation()
{
	//cout << lifeCount;
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
	cout << "�����ڵĸ��壬���ִ���" << endl;
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
	//��p2��index1��index2��Ƭ�ν���ŵ�p1�У��γ��µ�����
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
			//��ʱ�����г��������֣���¼
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
	vector<int> newOne;//�����¸���
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
	//��������Ⱥ��ѡ����������
	vector<int> parent1 = getOne();
	vector<int> parent2 = getOne();
	//cout << parent1.size() << " " << parent2.size() << endl;
	double rate;
	//1.ֱ���Ŵ����ǽ����Ŵ�
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
	//1.�Ƿ���ֱ���
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
	initPopulation();//��ʼ����Ⱥ
	//���ʽ���
	for (int i = 0; i < ge; i++)
	{
		int minWay = 0;
		getScore();//����ÿһ����ÿ���������Ӧ��
		vector<vector<int>> newLives;//��¼��һ��
		//��һ�����Ÿ�����
		bestLife = getBest();
		newLives.push_back(bestLife);
		bestLife.push_back(bestLife.front());
		for (int i1 = 0; i1 < total; i1++)
		{
			minWay += table[getindex(bestLife[i1])][getindex(bestLife[i1 + 1])];
		}
		minWayGenetic.push_back(minWay);
		//cout << minWay << endl;
		//����һ������������ʱ
		while (newLives.size() < lifeCount)
		{
			//���������Ӵ�������Ⱥ
			vector<int> temp;
			temp = newChild(crossRate, mutationRate);
			//cout << temp.size();
			newLives.push_back(temp);
		}
		//���ʸ���
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
/*-----------------------------�Ŵ��㷨���--------------------------------*/