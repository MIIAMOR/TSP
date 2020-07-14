# TSP问题
## 数据集合下载连接

http://elib.zib.de/pub/mp-testdata/tsp/tsplib/tsp/index.html

## 总体情况说明

数据放到data文件夹下，每组单独的数据放置在每个单独的文件夹里面。其中testData.txt存放的坐标数据，bestData.txt文件夹中存放的下载的最优解，testDataSolve.txt存放邻接矩阵（数据过大时会出现乱码，a280存在该问题，可无视）。

不同算法求得的最优解放在该文件夹其他txt文件中，例如贪心算法求得的解放到resultGreedy.txt中。

readme.txt文件中存放不同算法的一些说明情况。

python文件是把坐标数据和最优解规划的路线用图形可视化呈现出来。

## 启发式算法（选择或者补充）

### 模拟退火



### 遗传算法



### 蚁群算法





## 近似算法（选择或者补充）

### 贪心算法

每次只查看当前的点所能达到的最短距离，所求距离有局限性。