#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<math.h>
#include<fstream>
#include<limits.h>
#include"config.h"
using namespace std;

//把字符变为整型数字
int trans(string s);

//点的序号以及坐标
class point
{
public:
	//点的排序
	int n;
	//点的坐标
	int pos[2];

	//构造函数
	point(string data);
};
class tspData
{
public:
	int total;//点的总个数
	vector<point> points;//点
	int** table;//邻接矩阵
	int bestWayLength;//真正最优解的路线长度

	//构造函数 构建点的相关信息 以及邻接矩阵
	tspData(string infile1, string infile2, string outfile);
	~tspData();//析构函数
	int getindex(int n);//根据序号求下标
	void best(string infile);//真正的最优解
	/*------------TSP问题求解-----------------*/

	/*-----------------------------贪心算法求解--------------------------------*/
	//方法1--贪心求解 参考资料https://wenku.baidu.com/view/6db3d333be23482fb4da4cd1.html
	int minWayGreedy;//贪心算法最优路线
	vector<int> wayGreedy;//路线方案
	//每次遍历只能求解局部最优解 因此最后的结果只能是近似解，未必是最优解
	bool ifIn(int index);//判断这个点是否遍历 参数是点的序号
	//求解 序号 为index的点能够遍历得最小距离 返回值是下一个要遍历得点的下标对应的点序号
	int minLength(int index);
	//参数是文件地址 以及 起始位置
	void TSPGreedy(string file, int begin);
	/*-----------------------------贪心算法求解--------------------------------*/

	/*-----------------------------遗传算法求解--------------------------------*/
	//参考资料：https://blog.csdn.net/v_JULY_v/article/details/6132775
	int crossCount; int mutationCount;//交叉遗传次数和变异次数
	int lifeCount;//一代个体的数量
	vector<vector<int>> life;//每一代的个体（一个序列）,一代个体有lifeCount个
	vector<int> bestLife;//记录最后一次迭代时中最好的个体
	vector<int> minWayGenetic;//遗传算法下的长度
	vector<double> score;//每一代的个体的评估值
	vector<double> judgeTable;//构建一代的评估轮盘

	void initPopulation();//随机初始化种群
	void getScore();//针对个体求其适配度
	vector<int>& getOne();//随机选择出一代个体中的一个
	vector<int>& getBest();//选择出一代个体中最好的一个
	vector<int> cross(vector<int> p1, vector<int> p2);//交叉遗传
	vector<int> mutation(vector<int> aLife);//变异
	vector<int> newChild(double crossRate, double mutationRate);//从父代产生新的个体
	//遗传算法 参数分别是 迭代次数  一代个体数  交叉遗传概率 个体变异概率
	void TSPGenetic(int ge, int lifeCount, double crossRate, double mutationRate, string file);
	/*-----------------------------遗传算法求解--------------------------------*/
};