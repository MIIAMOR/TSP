
/*---------------------------贪心算法思想下解的说明------------------------------*/

算法思想：以当前所在的点为参考选择离自己最近的点作为下一站，走过的点不在经过


贪心思想只能求得每一步只能求得局部最短路径，因此最终结果是近似最优解

贪心算法求解的最短路程的长度为：513.61
实际的最短路径为：429.983
此算法和最优解的近似度为:1.19449
time = 0.002s
/*---------------------------贪心算法思想下解的说明------------------------------*/



/*---------------------------遗传算法2思想下解的说明------------------------------*/

算法思想：随机生成一个路线序列，路线路径长度短的会更可能遗传下去，所以经过多次代际迭代，最终的结果会趋近于最优解


代际更迭次数越多，越接近最优解。这种算法最终结果是近似最优解

遗传算法求解的最短路程的长度为：437.663
实际的最短路径为：429.983
此算法和最优解的近似度为:1.01786
time = 6.216s
这个算法迭代终止条件是前后 600 代最优个体一样
这个算法中共计迭代 1181次
其中每一代个体数为 100   交叉遗传的概率为 0.8   变异的概率为 0.1

迭代过程中 交叉遗传次数为46374  其中变异次数为13424

以下是迭代过程中每一代中最好的个体所对应的路先长度变化（每20代输出一次）

956.643 713.467 617.279 535.316 499.228 474.077 452.241 452.241 445.255 441.829 441.829 441.829 441.829 439.539 437.909 437.909 437.909 437.909 437.909 437.909 
437.909 437.909 437.909 437.909 437.909 437.909 437.909 437.909 437.909 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 
437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 437.663 
/*---------------------------遗传算法2思想下解的说明------------------------------*/


