#include "TSPsolve.h"

TSPsolve::TSPsolve()
{
	dirName.clear();
	//文件夹名字存放在数组中
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
	cout << "贪心思想最近邻优先算法已经求解完所有被选中数据" << endl;
	cout << "解的序列存放在 data/实例名/resultGreedy.txt 中" << endl;
	cout << "说明文件是 data/实例名/readme.txt" << endl;
	system("pause");
	system("cls");
	cout << "遗传算法消耗的时间很多，请耐心等待" << endl;
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
		//添加说明文件
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------贪心算法--------------------------------------------------*/
		/*-------程序计时--------*/
		clock_t start, end;//定义clock_t变量
		start = clock();//开始时间
		//方法1：贪心求解
		tsp.TSPGreedy(TSPGreedy, 1);
		ofs << "\n/*---------------------------贪心算法思想下解的说明------------------------------*/\n" << endl;
		ofs << "算法思想：以当前所在的点为参考选择离自己最近的点作为下一站，走过的点不在经过\n\n" << endl;
		ofs << "贪心思想只能求得每一步只能求得局部最短路径，因此最终结果是近似最优解" << endl;
		ofs << "\n贪心算法求解的最短路程的长度为：" << tsp.minWayGreedy << endl;
		ofs << "实际的最短路径为：" << tsp.bestWayLength << endl;
		ofs << "此算法和最优解的近似度为:" << tsp.minWayGreedy / double(tsp.bestWayLength) << endl;
		end = clock();//结束时间
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
		/*-------程序计时--------*/
		ofs << "/*---------------------------贪心算法思想下解的说明------------------------------*/\n\n" << endl;
		/*--------------------------------------------贪心算法--------------------------------------------------*/
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
		//添加说明文件
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------遗传算法1--------------------------------------------------*/
		/*-------程序计时--------*/
		clock_t start, end;//定义clock_t变量
		start = clock();//开始时间
		//方法2：遗传求解
		tsp.TSPGenetic1(gene, lifeCounnt, crossRate, mutationRate, TSPGenetic);
		ofs << "\n/*---------------------------遗传算法1思想下解的说明------------------------------*/\n" << endl;
		ofs << "算法思想：随机生成一个路线序列，路线路径长度短的会更可能遗传下去，所以经过多次代际迭代，最终的结果会趋近于最优解\n\n" << endl;
		ofs << "代际更迭次数越多，越接近最优解。这种算法最终结果是近似最优解" << endl;
		ofs << "\n遗传算法求解的最短路程的长度为：" << tsp.minWayGenetic.back() << endl;
		ofs << "实际的最短路径为：" << tsp.bestWayLength << endl;
		ofs << "此算法和最优解的近似度为:" << tsp.minWayGA / double(tsp.bestWayLength) << endl;
		end = clock();//结束时间
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
		ofs << "这个算法中共计迭代 " << gene << "次" << endl;
		ofs << "其中每一代个体数为 " << lifeCounnt << "   交叉遗传的概率为 " << crossRate << "   变异的概率为 " << mutationRate << endl << endl;
		ofs << "迭代过程中 交叉遗传次数为" << tsp.crossCount << "  " << "其中变异次数为" << tsp.mutationCount << endl << endl;
		ofs << "以下是迭代过程中每一代中最好的个体所对应的路先长度变化（每10代输出一次）" << endl << endl;
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
		/*-------程序计时--------*/
		ofs << "/*---------------------------遗传算法1思想下解的说明------------------------------*/\n\n" << endl;
		/*--------------------------------------------遗传算法1--------------------------------------------------*/
		ofs.close();
		cout << "对应 " << dirName[i] << "中的数据" << endl;
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
		//添加说明文件
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------遗传算法2--------------------------------------------------*/
		/*-------程序计时--------*/
		clock_t start, end;//定义clock_t变量
		start = clock();//开始时间
		//方法2：遗传求解
		tsp.TSPGenetic2(gene, lifeCounnt, crossRate, mutationRate, TSPGenetic);
		ofs << "\n/*---------------------------遗传算法2思想下解的说明------------------------------*/\n" << endl;
		ofs << "算法思想：随机生成一个路线序列，路线路径长度短的会更可能遗传下去，所以经过多次代际迭代，最终的结果会趋近于最优解\n\n" << endl;
		ofs << "代际更迭次数越多，越接近最优解。这种算法最终结果是近似最优解" << endl;
		ofs << "\n遗传算法求解的最短路程的长度为：" << tsp.minWayGenetic.back() << endl;
		ofs << "实际的最短路径为：" << tsp.bestWayLength << endl;
		ofs << "此算法和最优解的近似度为:" << tsp.minWayGA / double(tsp.bestWayLength) << endl;
		end = clock();//结束时间
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
		ofs << "这个算法迭代终止条件是前后 " << gene << " 代最优个体一样" << endl;
		ofs << "这个算法中共计迭代 " << tsp.minWayGenetic.size() << "次" << endl;
		ofs << "其中每一代个体数为 " << lifeCounnt << "   交叉遗传的概率为 " << crossRate << "   变异的概率为 " << mutationRate << endl << endl;
		ofs << "迭代过程中 交叉遗传次数为" << tsp.crossCount << "  " << "其中变异次数为" << tsp.mutationCount << endl << endl;
		ofs << "以下是迭代过程中每一代中最好的个体所对应的路先长度变化（每20代输出一次）" << endl << endl;
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
		/*-------程序计时--------*/
		ofs << "/*---------------------------遗传算法2思想下解的说明------------------------------*/\n\n" << endl;
		/*--------------------------------------------遗传算法2--------------------------------------------------*/
		ofs.close();
		ofs1.close();
		cout << "对应 " << dirName[i] << "中的数据" << endl;
		system("pause");
		system("cls");
	}
}