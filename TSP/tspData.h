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
int trans(string s);

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

	//���캯�� ������������Ϣ �Լ��ڽӾ���
	tspData(string infile1, string infile2, string outfile);
	~tspData();//��������
	int getindex(int n);//����������±�
	void best(string infile);//���������Ž�
	/*------------TSP�������-----------------*/

	/*-----------------------------̰���㷨���--------------------------------*/
	//����1--̰����� �ο�����https://wenku.baidu.com/view/6db3d333be23482fb4da4cd1.html
	int minWayGreedy;//̰���㷨����·��
	vector<int> wayGreedy;//·�߷���
	//ÿ�α���ֻ�����ֲ����Ž� ������Ľ��ֻ���ǽ��ƽ⣬δ�������Ž�
	bool ifIn(int index);//�ж�������Ƿ���� �����ǵ�����
	//��� ��� Ϊindex�ĵ��ܹ���������С���� ����ֵ����һ��Ҫ�����õ���±��Ӧ�ĵ����
	int minLength(int index);
	//�������ļ���ַ �Լ� ��ʼλ��
	void TSPGreedy(string file, int begin);
	/*-----------------------------̰���㷨���--------------------------------*/

	/*-----------------------------�Ŵ��㷨���--------------------------------*/
	//�ο����ϣ�https://blog.csdn.net/v_JULY_v/article/details/6132775
	int crossCount; int mutationCount;//�����Ŵ������ͱ������
	int lifeCount;//һ�����������
	vector<vector<int>> life;//ÿһ���ĸ��壨һ�����У�,һ��������lifeCount��
	vector<int> bestLife;//��¼���һ�ε���ʱ����õĸ���
	vector<int> minWayGenetic;//�Ŵ��㷨�µĳ���
	vector<double> score;//ÿһ���ĸ��������ֵ
	vector<double> judgeTable;//����һ������������

	void initPopulation();//�����ʼ����Ⱥ
	void getScore();//��Ը������������
	vector<int>& getOne();//���ѡ���һ�������е�һ��
	vector<int>& getBest();//ѡ���һ����������õ�һ��
	vector<int> cross(vector<int> p1, vector<int> p2);//�����Ŵ�
	vector<int> mutation(vector<int> aLife);//����
	vector<int> newChild(double crossRate, double mutationRate);//�Ӹ��������µĸ���
	//�Ŵ��㷨 �����ֱ��� ��������  һ��������  �����Ŵ����� ����������
	void TSPGenetic(int ge, int lifeCount, double crossRate, double mutationRate, string file);
	/*-----------------------------�Ŵ��㷨���--------------------------------*/
};