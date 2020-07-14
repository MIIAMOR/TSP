from os import path
import numpy as np
import matplotlib.pyplot as plt
infileData = "testData.txt"
infile = ["bestData.txt", "resultGreedy.txt"]
picname = ["最优解", "贪婪算法"]

file1 = open(infileData)  # 读入文件
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
        elif (len(temp) != 0 and temp[0] != '0'):
            # print(temp)
            city[count][j] = int(temp)
            j += 1
            temp = ""
        else:
            temp = ""
    count += 1
    # print(line)
    # print(len(line))
file1.close()
print(city)
'''读取解文件'''
for j in range(len(infile)):
    file2 = open(infile[j])
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
    plt.scatter(city[int(way[0])][1], city[int(way[0])]
                [2], color='b', marker='x')
    plt.plot([city[int(way[0])][1], city[int(way[1])][1]],
             [city[int(way[0])][2], city[int(way[1])][2]], color='r')
    for i in range(1, len(way)-1):
        index1 = int(way[i])
        index2 = int(way[i+1])
        plt.scatter(city[index1][1], city[index1][2], color='b')
        plt.plot([city[index1][1], city[index2][1]],
                 [city[index1][2], city[index2][2]], color='r')
    file2.close()
    plt.savefig(picname[j]+".png")
