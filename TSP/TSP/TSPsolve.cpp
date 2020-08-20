#include "TSPsolve.h"

TSPsolve::TSPsolve()
{
	dirName.clear();
	//�ļ������ִ����������
	dirName.push_back("data/bayg29/");
	dirName.push_back("data/bays29/");
	dirName.push_back("data/att48/");
	dirName.push_back("data/eil51/");
	dirName.push_back("data/berlin52/");
	dirName.push_back("data/st70/");
	dirName.push_back("data/eil76/");
	dirName.push_back("data/pr76/");
	dirName.push_back("data/rat99/");
	dirName.push_back("data/kroa100/");
	dirName.push_back("data/krob100/");
	dirName.push_back("data/kroc100/");
	dirName.push_back("data/krod100/");
	dirName.push_back("data/kroe100/");
	dirName.push_back("data/eil101/");
	dirName.push_back("data/lin105/");
	dirName.push_back("data/pr107/");
	dirName.push_back("data/pr124/");
	dirName.push_back("data/bier127/");
	dirName.push_back("data/pr136/");
	dirName.push_back("data/pr144/");
	dirName.push_back("data/kroa150/");
	dirName.push_back("data/krob150/");
	dirName.push_back("data/pr152/");
	dirName.push_back("data/rat195/");
	dirName.push_back("data/krob200/");
	dirName.push_back("data/tsp225/");
	dirName.push_back("data/ts225/");
	dirName.push_back("data/a280/");
	for (unsigned int i = 0; i < dirName.size(); i++)
	{
		string readme = dirName[i] + readmefile;
		ofstream ofs;
		ofs.open(readme, ios::trunc);
		ofs.close();
	}
}

void TSPsolve::getSolution()
{
	Greedy();
	cout << "̰��˼������������㷨�Ѿ���������б�ѡ������" << endl;
	cout << "������д���� data/ʵ����/resultGreedy.txt ��" << endl;
	cout << "˵���ļ��� data/ʵ����/readme.txt" << endl;
	system("pause");
	system("cls");
	cout << "�Ŵ��㷨���ĵ�ʱ��ܶ࣬�����ĵȴ�" << endl;
	//Genetic1();
	Genetic2();
}

void TSPsolve::Greedy()
{
	for (unsigned int i = 0; i < dirName.size(); i++)
	{
		string infile, outfile, TSPGreedy, best, readme;
		infile = dirName[i] + inFileName;
		best = dirName[i] + bestfile;
		outfile = dirName[i] + outFileName;
		TSPGreedy = dirName[i] + tspGreedyfile;
		readme = dirName[i] + readmefile;
		tspData tsp(infile, best, outfile);
		//���˵���ļ�
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------̰���㷨--------------------------------------------------*/
		/*-------�����ʱ--------*/
		clock_t start, end;//����clock_t����
		start = clock();//��ʼʱ��
		//����1��̰�����
		tsp.TSPGreedy(TSPGreedy, 1);
		ofs << "\n/*---------------------------̰���㷨˼���½��˵��------------------------------*/\n" << endl;
		ofs << "�㷨˼�룺�Ե�ǰ���ڵĵ�Ϊ�ο�ѡ�����Լ�����ĵ���Ϊ��һվ���߹��ĵ㲻�ھ���\n\n" << endl;
		ofs << "̰��˼��ֻ�����ÿһ��ֻ����þֲ����·����������ս���ǽ������Ž�" << endl;
		ofs << "\n̰���㷨�������·�̵ĳ���Ϊ��" << tsp.minWayGreedy << endl;
		ofs << "ʵ�ʵ����·��Ϊ��" << tsp.bestWayLength << endl;
		ofs << "���㷨�����Ž�Ľ��ƶ�Ϊ:" << tsp.minWayGreedy / double(tsp.bestWayLength) << endl;
		end = clock();//����ʱ��
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //���ʱ�䣨��λ����
		/*-------�����ʱ--------*/
		ofs << "/*---------------------------̰���㷨˼���½��˵��------------------------------*/\n\n" << endl;
		/*--------------------------------------------̰���㷨--------------------------------------------------*/
		ofs.close();
	}
}

void TSPsolve::Genetic1()
{
	for (unsigned int i = 1; i < 2; i++)
	//for (unsigned int i = 1; i < dirName.size(); i++)
	{
		int gene = 2000, lifeCounnt = 20;
		double crossRate = 0.8, mutationRate = 0.1;
		string infile, outfile, TSPGenetic, best, readme, rateFile;
		infile = dirName[i] + inFileName;
		best = dirName[i] + bestfile;
		outfile = dirName[i] + outFileName;
		TSPGenetic = dirName[i] + tspGeneticfile;
		readme = dirName[i] + readmefile;
		rateFile = dirName[i] + tspGeneticRate;
		tspData tsp(infile, best, outfile);
		//lifeCounnt = tsp.total / 3;
		//���˵���ļ�
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------�Ŵ��㷨1--------------------------------------------------*/
		/*-------�����ʱ--------*/
		clock_t start, end;//����clock_t����
		start = clock();//��ʼʱ��
		//����2���Ŵ����
		tsp.TSPGenetic1(gene, lifeCounnt, crossRate, mutationRate, TSPGenetic);
		ofs << "\n/*---------------------------�Ŵ��㷨1˼���½��˵��------------------------------*/\n" << endl;
		ofs << "�㷨˼�룺�������һ��·�����У�·��·�����ȶ̵Ļ�������Ŵ���ȥ�����Ծ�����δ��ʵ��������յĽ�������������Ž�\n\n" << endl;
		ofs << "���ʸ�������Խ�࣬Խ�ӽ����Ž⡣�����㷨���ս���ǽ������Ž�" << endl;
		ofs << "\n�Ŵ��㷨�������·�̵ĳ���Ϊ��" << tsp.minWayGenetic.back() << endl;
		ofs << "ʵ�ʵ����·��Ϊ��" << tsp.bestWayLength << endl;
		ofs << "���㷨�����Ž�Ľ��ƶ�Ϊ:" << tsp.minWayGA / double(tsp.bestWayLength) << endl;
		end = clock();//����ʱ��
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //���ʱ�䣨��λ����
		ofs << "����㷨�й��Ƶ��� " << gene << "��" << endl;
		ofs << "����ÿһ��������Ϊ " << lifeCounnt << "   �����Ŵ��ĸ���Ϊ " << crossRate << "   ����ĸ���Ϊ " << mutationRate << endl << endl;
		ofs << "���������� �����Ŵ�����Ϊ" << tsp.crossCount << "  " << "���б������Ϊ" << tsp.mutationCount << endl << endl;
		ofs << "�����ǵ���������ÿһ������õĸ�������Ӧ��·�ȳ��ȱ仯��ÿ10�����һ�Σ�" << endl << endl;
		ofstream ofs1;
		ofs1.open(rateFile, ios::out);
		for (unsigned int i1 = 0; i1 < tsp.minWayGenetic.size(); i1++)
		{
			ofs1 << tsp.minWayGenetic[i1] / (double)tsp.bestWayLength << endl;
			if ((i1 + 1) % 10 == 0)
			{
				ofs << tsp.minWayGenetic[i1] << " ";
			}
			if ((i1 + 1) % 200 == 0)
			{
				ofs << endl;
			}
		}
		ofs << endl;
		/*-------�����ʱ--------*/
		ofs << "/*---------------------------�Ŵ��㷨1˼���½��˵��------------------------------*/\n\n" << endl;
		/*--------------------------------------------�Ŵ��㷨1--------------------------------------------------*/
		ofs.close();
		cout << "��Ӧ " << dirName[i] << "�е�����" << endl;
		system("pause");
		system("cls");
	}
}

void TSPsolve::Genetic2()
{
	//for (unsigned int i = 1; i < 3; i++)
	for (unsigned int i = 0; i < dirName.size(); i++)
	{
		int gene = 700, lifeCounnt = 40;
		double crossRate = 0.8, mutationRate = 0.12;
		string infile, outfile, TSPGenetic, best, readme, rateFile;
		infile = dirName[i] + inFileName;
		best = dirName[i] + bestfile;
		outfile = dirName[i] + outFileName;
		TSPGenetic = dirName[i] + tspGeneticfile;
		readme = dirName[i] + readmefile;
		rateFile = dirName[i] + tspGeneticRate;
		tspData tsp(infile, best, outfile);
		if (tsp.total >= 60)
		{
			lifeCounnt = 3 * tsp.total / 4;
			gene = tsp.total * 7 + 300;
		}
		//���˵���ļ�
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------�Ŵ��㷨2--------------------------------------------------*/
		/*-------�����ʱ--------*/
		clock_t start, end;//����clock_t����
		start = clock();//��ʼʱ��
		//����2���Ŵ����
		tsp.TSPGenetic2(gene, lifeCounnt, crossRate, mutationRate, TSPGenetic);
		ofs << "\n/*---------------------------�Ŵ��㷨2˼���½��˵��------------------------------*/\n" << endl;
		ofs << "�㷨˼�룺�������һ��·�����У�·��·�����ȶ̵Ļ�������Ŵ���ȥ�����Ծ�����δ��ʵ��������յĽ�������������Ž�\n\n" << endl;
		ofs << "���ʸ�������Խ�࣬Խ�ӽ����Ž⡣�����㷨���ս���ǽ������Ž�" << endl;
		ofs << "\n�Ŵ��㷨�������·�̵ĳ���Ϊ��" << tsp.minWayGenetic.back() << endl;
		ofs << "ʵ�ʵ����·��Ϊ��" << tsp.bestWayLength << endl;
		ofs << "���㷨�����Ž�Ľ��ƶ�Ϊ:" << tsp.minWayGA / double(tsp.bestWayLength) << endl;
		end = clock();//����ʱ��
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //���ʱ�䣨��λ����
		ofs << "����㷨������ֹ������ǰ�� " << gene << " �����Ÿ���һ��" << endl;
		ofs << "����㷨�й��Ƶ��� " << tsp.minWayGenetic.size() << "��" << endl;
		ofs << "����ÿһ��������Ϊ " << lifeCounnt << "   �����Ŵ��ĸ���Ϊ " << crossRate << "   ����ĸ���Ϊ " << mutationRate << endl << endl;
		ofs << "���������� �����Ŵ�����Ϊ" << tsp.crossCount << "  " << "���б������Ϊ" << tsp.mutationCount << endl << endl;
		ofs << "�����ǵ���������ÿһ������õĸ�������Ӧ��·�ȳ��ȱ仯��ÿ20�����һ�Σ�" << endl << endl;
		ofstream ofs1;
		ofs1.open(rateFile, ios::out);
		for (unsigned int i1 = 0; i1 < tsp.minWayGenetic.size(); i1++)
		{
			ofs1 << tsp.minWayGenetic[i1] / (double)tsp.bestWayLength << endl;
			if ((i1 + 1) % 20 == 0)
			{
				ofs << tsp.minWayGenetic[i1] << " ";
			}
			if ((i1 + 1) % 400 == 0)
			{
				ofs << endl;
			}
		}
		ofs << endl;
		/*-------�����ʱ--------*/
		ofs << "/*---------------------------�Ŵ��㷨2˼���½��˵��------------------------------*/\n\n" << endl;
		/*--------------------------------------------�Ŵ��㷨2--------------------------------------------------*/
		ofs.close();
		ofs1.close();
		cout << "��Ӧ " << dirName[i] << "�е�����" << endl;
		system("pause");
		system("cls");
	}
}