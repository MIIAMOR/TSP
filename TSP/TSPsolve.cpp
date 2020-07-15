#include "TSPsolve.h"

TSPsolve::TSPsolve()
{
	dirName.clear();
	//�ļ������ִ����������
	dirName.push_back("data/att48/");
	dirName.push_back("data/a280/");
	dirName.push_back("data/eil76/");
	dirName.push_back("data/bayg29/");
	dirName.push_back("data/berlin52/");
	dirName.push_back("data/eil101/");
	dirName.push_back("data/eil51/");
	dirName.push_back("data/krod100/");
	dirName.push_back("data/lin105/");
	dirName.push_back("data/st70/");
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