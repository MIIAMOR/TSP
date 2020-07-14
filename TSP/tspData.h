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

//���ַ���Ϊ��������
int trans(string num);

//�������Լ�����
class point
{
public:
	//�������
	int n;
	//�������
	int pos[2];

	//���캯��
	point(string data);
};
class tspData
{
public:
	int total;//����ܸ���
	vector<point> points;//��
	int** table;//�ڽӾ���
	int bestWayLength;//�������Ž��·�߳���
	int minWayLength;//·�߳���
	vector<int> minWay;//·�߷���

	//���캯�� ������������Ϣ �Լ��ڽӾ���
	tspData(string infile, string outfile);
	~tspData();//��������
	int getindex(int n);//����������±�
	void best();//���������Ž�
	bool ifIn(int index);//�ж�������Ƿ���� �����ǵ�����
	//��� ��� Ϊindex�ĵ��ܹ���������С���� ����ֵ����һ��Ҫ�����õ���±��Ӧ�ĵ����
	int minLength(int index);
	/*------------TSP�������-----------------*/

	//����1--̰����� �ο�����https://wenku.baidu.com/view/6db3d333be23482fb4da4cd1.html
	//ÿ�α���ֻ�����ֲ����Ž� ������Ľ��ֻ���ǽ��ƽ⣬δ�������Ž�
	//�������ļ���ַ �Լ� ��ʼλ��
	void TSP1(string file, int begin);
};

