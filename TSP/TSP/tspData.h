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
double trans(string s);

//�������Լ�����
class point
{
public:
	//�������
	int n;
	//�������
	double pos[2];

	//���캯��
	point(string data);
};
class tspData
{
public:
	int total;//����ܸ���
	vector<point> points;//��
	double** table;//�ڽӾ���
	double bestWayLength;//�������Ž��·�߳���

	//���캯�� ������������Ϣ �Լ��ڽӾ���
	tspData(string infile1, string infile2, string outfile);
	~tspData();//��������
	int getindex(int n);//����������±�
	void best(string infile);//���������Ž�
	/*------------TSP�������-----------------*/

	/*-----------------------------̰���㷨���--------------------------------*/
	//����1--̰����� �ο�����https://wenku.baidu.com/view/6db3d333be23482fb4da4cd1.html
	double minWayGreedy;//̰���㷨����·��
	vector<int> wayGreedy;//·�߷���
	//ÿ�α���ֻ�����ֲ����Ž� ������Ľ��ֻ���ǽ��ƽ⣬δ�������Ž�
	bool ifIn(int index);//�ж�������Ƿ���� �����ǵ�����
	//��� ��� Ϊindex�ĵ��ܹ���������С���� ����ֵ����һ��Ҫ�����õ���±��Ӧ�ĵ����
	int minLength(int index);
	//�������ļ���ַ �Լ� ��ʼλ��
	void TSPGreedy(string file, int begin);
	void TSPGreedy1(int begin);
	/*-----------------------------̰���㷨���--------------------------------*/

	/*-----------------------------�Ŵ��㷨���--------------------------------*/
	//�ο����ϣ�https://blog.csdn.net/v_JULY_v/article/details/6132775
	int crossCount; int mutationCount;//�����Ŵ������ͱ������
	int lifeCount;//һ�����������
	double minWayGA;//��¼ȫ������
	vector<int> wayGA;//��¼ȫ�����ŷ���
	vector<vector<int>> life;//ÿһ���ĸ��壨һ�����У�,һ��������lifeCount��
	vector<int> child1, child2;//��ʱ��ŵ������Ӵ�
	vector<int> bestLife;//��¼���һ�ε���ʱ����õĸ���
	vector<double> minWayGenetic;//�Ŵ��㷨�µĳ���
	vector<double> score;//ÿһ���ĸ��������ֵ
	vector<double> judgeTable;//����һ������������

	void initPopulation();//�����ʼ����Ⱥ
	void initPopulation1();//�����ʼ����Ⱥ
	void getScore1();//��Ը������������
	void getScore2();//�Ż���Ӧ�Ⱥ���  ��������� ǿ����������
	vector<int>& getBestandSecond();//ѡ���һ����������õ�һ������
	vector<int>& getOne1();//���ѡ���һ�������е�һ��  ���̶�ѡ������
	vector<int>& getOne2();//ѡ���һ�������е�һ��  ������ѡ������
	void cross1(vector<int> p1, vector<int> p2);//�����Ŵ�  ����˳�򽻲�
	void cross2(vector<int> p1, vector<int> p2);//�����Ŵ�  ѭ������
	void cross3(vector<int> p1, vector<int> p2);//�����Ŵ�  ����λ�ý��淨
	void cross4(vector<int> p1, vector<int> p2);//�����Ŵ�  �����λ��˳�򽻲淨
	vector<int> mutation1(vector<int> aLife);//����  ˫�����λ�ý���
	vector<int> mutation2(vector<int> aLife);//����  �����λ��������
	void newChild1(double crossRate, double mutationRate);//�Ӹ��������µĸ���
	void newChild2(double crossRate, double mutationRate);//�Ӹ��������µĸ���
	//�Ŵ��㷨1�� �����ֱ��� ��������  һ��������  �����Ŵ����� ����������  �Լ����Ƶ�������
	void TSPGenetic1(int ge, int lifeCount, double crossRate, double mutationRate, string file);
	//�Ŵ��㷨2�� �����ֱ��� ��������ֹ����  һ��������  �����Ŵ����� ����������  ǰ��ge����Ⱥ���Ÿ�����ͬ����ֹѭ��
	void TSPGenetic2(int ge, int lifeCount, double crossRate, double mutationRate, string file);
	/*-----------------------------�Ŵ��㷨���--------------------------------*/
};