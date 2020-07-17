#include "TSPsolve.h"

TSPsolve::TSPsolve()
{
	dirName.clear();
	//文件夹名字存放在数组中
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
	cout << "遗传算法消耗的时间很多，请耐心等待" << endl;
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
		//添加说明文件
		ofstream ofs;
		ofs.open(readme, ios::app);
		/*--------------------------------------------遗传算法--------------------------------------------------*/
		/*-------程序计时--------*/
		clock_t start, end;//定义clock_t变量
		start = clock();//开始时间
		//方法2：遗传求解
		tsp.TSPGenetic(gene, lifeCounnt, crossRate, mutationRate, TSPGenetic);
		ofs << "\n/*---------------------------遗传算法思想下解的说明------------------------------*/\n" << endl;
		ofs << "算法思想：随机生成一个路线序列，路线路径长度短的会更可能遗传下去，所以经过多次代际迭代，最终的结果会趋近于最优解\n\n" << endl;
		ofs << "代际更迭次数越多，越接近最优解。这种算法最终结果是近似最优解" << endl;
		ofs << "\n遗传算法求解的最短路程的长度为：" << tsp.minWayGenetic.back() << endl;
		ofs << "实际的最短路径为：" << tsp.bestWayLength << endl;
		ofs << "此算法和最优解的近似度为:" << tsp.minWayGenetic.back() / double(tsp.bestWayLength) << endl;
		end = clock();//结束时间
		ofs << "time = " << (double(end) - double(start)) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
		ofs << "这个算法中共计迭代 " << gene << "次" << endl;
		ofs << "其中每一代个体数为 " << lifeCounnt << "   交叉遗传的概率为 " << crossRate << "   变异的概率为 " << mutationRate << endl << endl;
		ofs << "迭代过程中 交叉遗传次数为" << tsp.crossCount << "  " << "其中变异次数为" << tsp.mutationCount << endl << endl;
		ofs << "以下是迭代过程中每一代中最好的个体所对应的路先长度变化" << endl << endl;
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
		/*-------程序计时--------*/
		ofs << "/*---------------------------遗传法思想下解的说明------------------------------*/\n\n" << endl;
		/*--------------------------------------------遗传算法--------------------------------------------------*/
		ofs.close();
	}
}