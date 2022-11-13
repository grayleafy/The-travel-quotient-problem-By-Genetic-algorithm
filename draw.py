import numpy as np
import matplotlib.pyplot as plt

fp=open('data.txt','r')
ls=[]
for line in fp:
    line=line.strip('\n')   #将\n去掉
    ls.append(line.split(' '))   #将空格作为分隔符将一个字符切割成一个字符数组0
fp.close()
n = int(ls[0][0])
x = np.empty(n, dtype = float)
y = np.empty(n, dtype = float)

ls = ls[1:]
for index in range(len(ls)):
    x[index] = float(ls[index][1])
    y[index] = float(ls[index][2])


fp=open('result.txt','r')
ls=[]
for line in fp:
    line=line.strip('\n')   #将\n去掉
    ls.append(line.split(' '))   #将空格作为分隔符将一个字符切割成一个字符数组0
fp.close()

result = float(ls[1][0])
id = np.empty(n, dtype = int)
ls = ls[0]
for index in range(len(ls)):
    id[index] = int(ls[index])



x2 = np.empty(n + 1, dtype = float)
y2 = np.empty(n + 1, dtype = float)
for index in range(n):
    x2[index] = x[id[index]]
    y2[index] = y[id[index]]

x2[n] = x[id[0]]
y2[n] = y[id[0]]

plt.plot(x2, y2)
plt.scatter(x2,y2)
plt.show()


