#include<iostream>
#include<string>
#include<time.h>
#include"tspData.h"
#include"TSPsolve.h"
#include"config.h"
using namespace std;

int main()
{
	srand((unsigned)time(NULL));
	TSPsolve TSP;
	TSP.getSolution();
	return 0;
}