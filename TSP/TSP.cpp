#include<iostream>
#include<string>
#include<time.h>
#include"tspData.h"
#include"config.h"
using namespace std;

int main()
{
	//对应的文件夹
	vector<string> dirName;
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
		string infile, outfile, TSPGreedy, best, readme;
		infile = dirName[i] + inFileName;
		best = dirName[i] + bestfile;
		outfile = dirName[i] + outFileName;
		TSPGreedy = dirName[i] + tspGreedyfile;
		readme = dirName[i] + readmefile;
		tspData tsp(infile, best, outfile);
		//添加说明文件
		ofstream ofs;
		ofs.open(readme, ios::out);
		/*--------------------------------------------贪心算法--------------------------------------------------*/
		/*-------程序计时--------*/
		clock_t start, end;//定义clock_t变量
		start = clock();//开始时间
		//方法1：贪心求解
		tsp.TSPGreedy(TSPGreedy, 1);
		ofs << "\n/*---------------------------贪心算法思想下解的说明------------------------------*/\n" << endl;
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
	return 0;
}