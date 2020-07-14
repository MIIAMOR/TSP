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
int trans(string num);

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
	int minWayLength;//路线长度
	vector<int> minWay;//路线方案

	//构造函数 构建点的相关信息 以及邻接矩阵
	tspData(string infile, string outfile);
	~tspData();//析构函数
	int getindex(int n);//根据序号求下标
	void best();//真正的最优解
	bool ifIn(int index);//判断这个点是否遍历 参数是点的序号
	//求解 序号 为index的点能够遍历得最小距离 返回值是下一个要遍历得点的下标对应的点序号
	int minLength(int index);
	/*------------TSP问题求解-----------------*/

	//方法1--贪心求解 参考资料https://wenku.baidu.com/view/6db3d333be23482fb4da4cd1.html
	//每次遍历只能求解局部最优解 因此最后的结果只能是近似解，未必是最优解
	//参数是文件地址 以及 起始位置
	void TSP1(string file, int begin);
};

