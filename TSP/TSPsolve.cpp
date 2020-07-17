#include "TSPsolve.h"

TSPsolve::TSPsolve()
{
	dirName.clear();
	//�ļ������ִ����������
	dirName.push_back("data/att48/");
	dirName.push_back("data/eil76/");
	dirName.push_back("data/a280/");
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
	cout << "�Ŵ��㷨���ĵ�ʱ��ܶ࣬�����ĵȴ�" << endl;
	Genetic();
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

void TSPsolve::Genetic()
{
	for (unsigned int i = 0; i < 2; i++)
	{
		int gene = 5000, lifeCounnt = 10;
		double crossRate = 0.8, mutationRate = 0.5;
		string infile, outfile, TSPGenetic, best, readme;
		infile = dirName[i] + inFileName;
		best = dirName[i] + bestfile;
		outfile = dirName[i] + outFileName;
		TSPGenetic = dirName[i] + tspGeneticfile;
		readme = dirName[i] + readmefile;
		tspData tsp(infile, best, outfile);
		//���˵���ļ�
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------�Ŵ��㷨--------------------------------------------------*/
		/*-------�����ʱ--------*/
		clock_t start, end;//����clock_t����
		start = clock();//��ʼʱ��
		//����2���Ŵ����
		tsp.TSPGenetic(gene, lifeCounnt, crossRate, mutationRate, TSPGenetic);
		ofs << "\n/*---------------------------�Ŵ��㷨˼���½��˵��------------------------------*/\n" << endl;
		ofs << "�㷨˼�룺�������һ��·�����У�·��·�����ȶ̵Ļ�������Ŵ���ȥ�����Ծ�����δ��ʵ��������յĽ�������������Ž�\n\n" << endl;
		ofs << "���ʸ�������Խ�࣬Խ�ӽ����Ž⡣�����㷨���ս���ǽ������Ž�" << endl;
		ofs << "\n�Ŵ��㷨�������·�̵ĳ���Ϊ��" << tsp.minWayGenetic.back() << endl;
		ofs << "ʵ�ʵ����·��Ϊ��" << tsp.bestWayLength << endl;
		ofs << "���㷨�����Ž�Ľ��ƶ�Ϊ:" << tsp.minWayGenetic.back() / double(tsp.bestWayLength) << endl;
		end = clock();//����ʱ��
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //���ʱ�䣨��λ����
		ofs << "����㷨�й��Ƶ��� " << gene << "��" << endl;
		ofs << "����ÿһ��������Ϊ " << lifeCounnt << "   �����Ŵ��ĸ���Ϊ " << crossRate << "   ����ĸ���Ϊ " << mutationRate << endl << endl;
		ofs << "���������� �����Ŵ�����Ϊ" << tsp.crossCount << "  " << "���б������Ϊ" << tsp.mutationCount << endl << endl;
		ofs << "�����ǵ���������ÿһ������õĸ�������Ӧ��·�ȳ��ȱ仯" << endl << endl;
		for (unsigned int i1 = 0; i1 < tsp.minWayGenetic.size(); i1++)
		{
			if ((i1 + 1) % 40 == 0)
			{
				ofs << tsp.minWayGenetic[i1] << " ";
			}
			if ((i1 + 1) % 400 == 0)
			{
				ofs << endl;
			}
		}
		/*-------�����ʱ--------*/
		ofs << "/*---------------------------�Ŵ���˼���½��˵��------------------------------*/\n\n" << endl;
		/*--------------------------------------------�Ŵ��㷨--------------------------------------------------*/
		ofs.close();
	}
}