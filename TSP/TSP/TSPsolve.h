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
	vector<string> dirName;//�ļ�������
	TSPsolve();//���캯��  ��ȡ�ļ�������
	void getSolution();//TSP�������
	void Greedy();//̰�ķ����
	void Genetic1();//�Ŵ��㷨��� ���Ƶ�������
	void Genetic2();//�Ŵ��㷨��� ���������̶�
};

