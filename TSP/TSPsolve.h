#pragma once
#include<iostream>
#include<string>
#include<time.h>
#include"tspData.h"
#include"config.h"
using namespace std;

class TSPsolve
{
public:
	vector<string> dirName;//文件夹名称
	TSPsolve();//构造函数  获取文件夹数据
	void getSolution();//TSP问题求解
	void Greedy();//贪心法求解
};

