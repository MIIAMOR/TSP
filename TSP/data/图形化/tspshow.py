import math
from os import path
import numpy as np
import matplotlib.pyplot as plt
infile1 = "testData.txt"
infile2 = "resultbest.txt"
infile3 = "result1.txt"

file1 = open(infile1)  # 读入文件
lines = file1.readlines()
count = len(lines)  # 记录文件长度
'''对文件中的数据处理'''
city = np.zeros((count + 1, 3))  # 建立城市个数的数组
count = 1
for line in lines:
    temp = ""  # 空字符串接收数字
    j = 0  # 索引
    for i in range(0, len(line)):
        if (line[i] <= '9' and line[i] >= '0'):
            temp += line[i]
        else:
            # print(temp)
            city[count][j] = int(temp)
            j += 1
            temp = ""
    count += 1
    # print(line)
    # print(len(line))
file1.close()
print(city)
'''读取路径文件'''
file2 = open(infile2)
lines = file2.readlines()
count = len(lines)
way = np.zeros(count)
i = 0
for line in lines:
    way[i] = int(line)
    i += 1
    # print(line)
print(way)
plt.figure(figsize=(12, 12), dpi=100)
for i in range(0, len(way)-1):
    index1 = int(way[i])
    index2 = int(way[i+1])
    plt.scatter(city[index1][1], city[index1][2], color='b')
    plt.plot([city[index1][1], city[index2][1]],
             [city[index1][2], city[index2][2]], color='r')
file2.close()
plt.savefig("最优解.png")
'''读取最优文件'''
file3 = open(infile3)
lines = file3.readlines()
count = len(lines)
waybest = np.zeros(count)
i = 0
for line in lines:
    waybest[i] = int(line)
    i += 1
    # print(line)
print(waybest)
plt.figure(figsize=(12, 12), dpi=100)
for i in range(0, len(waybest)-1):
    index1 = int(waybest[i])
    index2 = int(waybest[i+1])
    plt.scatter(city[index1][1], city[index1][2], color='b')
    plt.plot([city[index1][1], city[index2][1]],
             [city[index1][2], city[index2][2]], color='r')
file3.close()
plt.savefig("贪心.png")
