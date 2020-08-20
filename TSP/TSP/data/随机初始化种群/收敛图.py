from os import path
import numpy as np
import matplotlib.pyplot as plt
infileData = "testData.txt"
dirName = ["bayg29", "bays29", "att48", "eil51", "berlin52", "st70", "eil76",
           "pr76", "rat99", "kroa100", "krob100", "kroc100", "krod100", "kroe100",
           "eil101", "lin105", "pr107", "pr124", "pr136", "pr144", "bier127",
           "kroa150", "krob150", "pr152", "rat195", "krob200", "ts225", "tsp225", "a280"]
picname = "收敛曲线图"
infile = "rateGenetic.txt"
for i in range(len(dirName)):
    file = open(dirName[i]+"/"+infile)  # 读入文件
    lines = file.readlines()
    count = len(lines)
    '''处理数据'''
    rate = np.zeros(count)
    num = np.zeros(count)
    count = 0
    for line in lines:
        rate[count] = float(line)
        num[count] = count+1
        count += 1
    print(rate)
    file.close()
    plt.figure(figsize=(12, 12), dpi=100)
    plt.plot(num, rate, color='r')
    plt.savefig(dirName[i]+"/"+dirName[i]+picname)
    plt.close()
