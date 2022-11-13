# 遗传算法求解旅行商问题
&emsp;&emsp;程序首先从文件中读取城市二维坐标点并初始化种群。基因序列定义为长度为城市数量的数组，初始生成一定数量的随机基因序列，形成原始种群。  
&emsp;&emsp;每一次迭代时，首先使用轮盘赌的方法选取父代进入交叉池，父代按照一定的概率进行交叉，然后再按照一定的概率变异形成子基因。之后计算每个基因的适应度（道路总长度），选取总长度最短的一定数量的精英个体保留，之后再按照轮盘赌的方法选取个体，最终形成子代种群。  
&emsp;&emsp;交叉时，随机选择两个个体，产生一个随机数，确定交叉的起始位置，对起始位置及之后的位置进行交换，并放在首位，然后再从各自的路径中按顺序填充未出现过的数字。这是为了避免交叉后产生冲突，导致一个城市被访问两次。  
&emsp;&emsp;变异时，生成两个随机数，交换基因中这两个随机数位置的基因。  
&emsp;&emsp;多次迭代后，取适应度最好的基因，保存基因序列。  
## 实验结果展示及分析
&emsp;&emsp;对二十个城市的旅行商问题求解，然后使用python绘图展示结果如下：
![image](https://user-images.githubusercontent.com/86156654/201520079-74ffcbbd-75cb-42c4-af4e-b8203e187b25.png)
