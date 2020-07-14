#include<iostream>
#include<string>
#include<time.h>
#include"tspData.h"
#include"config.h"
using namespace std;

int main()
{
	string infile = inFileName;
	string outfile = outFileName;
	string TSP1 = tsp1file;
	tspData tsp(infile, outfile);
	/*-------程序计时--------*/
	clock_t start, end;//定义clock_t变量
	start = clock();//开始时间
	//方法1：贪心求解
	tsp.TSP1(TSP1, 1);

	end = clock();//结束时间
	cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
	/*-------程序计时--------*/
	return 0;
}