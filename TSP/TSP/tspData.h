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
double trans(string s);

//点的序号以及坐标
class point
{
public:
	//点的排序
	int n;
	//点的坐标
	double pos[2];

	//构造函数
	point(string data);
};
class tspData
{
public:
	int total;//点的总个数
	vector<point> points;//点
	double** table;//邻接矩阵
	double bestWayLength;//真正最优解的路线长度

	//构造函数 构建点的相关信息 以及邻接矩阵
	tspData(string infile1, string infile2, string outfile);
	~tspData();//析构函数
	int getindex(int n);//根据序号求下标
	void best(string infile);//真正的最优解
	/*------------TSP问题求解-----------------*/

	/*-----------------------------贪心算法求解--------------------------------*/
	//方法1--贪心求解 参考资料https://wenku.baidu.com/view/6db3d333be23482fb4da4cd1.html
	double minWayGreedy;//贪心算法最优路线
	vector<int> wayGreedy;//路线方案
	//每次遍历只能求解局部最优解 因此最后的结果只能是近似解，未必是最优解
	bool ifIn(int index);//判断这个点是否遍历 参数是点的序号
	//求解 序号 为index的点能够遍历得最小距离 返回值是下一个要遍历得点的下标对应的点序号
	int minLength(int index);
	//参数是文件地址 以及 起始位置
	void TSPGreedy(string file, int begin);
	void TSPGreedy1(int begin);
	/*-----------------------------贪心算法求解--------------------------------*/

	/*-----------------------------遗传算法求解--------------------------------*/
	//参考资料：https://blog.csdn.net/v_JULY_v/article/details/6132775
	int crossCount; int mutationCount;//交叉遗传次数和变异次数
	int lifeCount;//一代个体的数量
	double minWayGA;//记录全局最优
	vector<int> wayGA;//记录全局最优方案
	vector<vector<int>> life;//每一代的个体（一个序列）,一代个体有lifeCount个
	vector<int> child1, child2;//临时存放的两个子代
	vector<int> bestLife;//记录最后一次迭代时中最好的个体
	vector<double> minWayGenetic;//遗传算法下的长度
	vector<double> score;//每一代的个体的评估值
	vector<double> judgeTable;//构建一代的评估轮盘

	void initPopulation();//随机初始化种群
	void initPopulation1();//随机初始化种群
	void getScore1();//针对个体求其适配度
	void getScore2();//优化适应度函数  增大差异性 强化择优能力
	vector<int>& getBestandSecond();//选择出一代个体中最好的一个次优
	vector<int>& getOne1();//随机选择出一代个体中的一个  轮盘赌选择算子
	vector<int>& getOne2();//选择出一代个体中的一个  锦标赛选择算子
	void cross1(vector<int> p1, vector<int> p2);//交叉遗传  两点顺序交叉
	void cross2(vector<int> p1, vector<int> p2);//交叉遗传  循环交叉
	void cross3(vector<int> p1, vector<int> p2);//交叉遗传  交替位置交叉法
	void cross4(vector<int> p1, vector<int> p2);//交叉遗传  多随机位置顺序交叉法
	vector<int> mutation1(vector<int> aLife);//变异  双点随机位置交换
	vector<int> mutation2(vector<int> aLife);//变异  随机点位置重排列
	void newChild1(double crossRate, double mutationRate);//从父代产生新的个体
	void newChild2(double crossRate, double mutationRate);//从父代产生新的个体
	//遗传算法1： 参数分别是 迭代次数  一代个体数  交叉遗传概率 个体变异概率  自己控制迭代次数
	void TSPGenetic1(int ge, int lifeCount, double crossRate, double mutationRate, string file);
	//遗传算法2： 参数分别是 收敛性终止条件  一代个体数  交叉遗传概率 个体变异概率  前后ge代种群最优个体相同，终止循环
	void TSPGenetic2(int ge, int lifeCount, double crossRate, double mutationRate, string file);
	/*-----------------------------遗传算法求解--------------------------------*/
};