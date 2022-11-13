#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<string>
#include<random>
#include<ctime>

using namespace std;
const double inf = 1000000000.0f;

class Node {
public:
	double x, y;

	double distance(Node b) {
		x = x - b.x;
		y = y - b.y;
		return sqrt(x * x + y * y);
	}
};

class Gene {
public:
	vector<int> data;
	double value = 0;

	Gene(){}

	//洗牌算法生成随机序列
	Gene(int n) {
		for (int i = 0; i < n; i++) {
			data.push_back(i);
		}
		std::mt19937 gen(std::random_device{}());
		shuffle(data.begin(), data.end(), gen);
	}

	//计算适应度
	double getFitnessValue(vector<double> &d) {
		double res = 0;
		int n = data.size();
		for (int i = 1; i < n; i++) {
			res += d[data[i - 1] * n + data[i]];
		}
		res += d[data[n - 1] * n + data[0]];
		return value = res;
	}

	//重定义比较函数
	bool operator < (const Gene &x) const {
		return value < x.value;
	}
};

class GA {
public:
	//参数
	int populationSize = 2000; //种群大小
	double num_child = 1.0f; //生成子代/父代比例
	double p_crossover = 0.6f; //交叉概率
	double p_mutation = 0.25f; //变异概率
	int elite = 1800; //精英个体
	int times_iterate = 9000; //迭代次数

	int n = 0;
	//double ans = inf;
	vector<Gene> population;
	vector<double> distance;

	//生成0-1之间的随机数
	double randBetween01() {
		return (double)(rand() % 10000) / 10000.0;
	}

	//读取文件并计算城市距离
	void readFile(string filename) {
		FILE *fp = fopen(filename.c_str(), "r");
		if (fp == NULL) {
			printf("读取文件失败\n");
			return;
		}


		fscanf(fp, "%d", &n);
		vector<Node> points(n);
		for (int i = 0; i < n; i++) {
			int id;
			fscanf(fp, "%d%lf%lf", &id, &points[i].x, &points[i].y);
		}
		fclose(fp);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				distance.push_back(points[i].distance(points[j]));
			}
		}

		/*
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				printf("%lf ", distance[i * n + j]);
			}
		}
		*/
	}

	//种群初始化
	void initPopulation() {
		srand(time(0));
		for (int i = 0; i < populationSize; i++) {
			Gene temp = Gene(n);
			temp.getFitnessValue(distance);
			population.push_back(temp);
		}
	}

	//交叉
	void crossover(const Gene &p, const Gene &m, Gene &child1, Gene &child2) {
		int mid = rand() % n;


		child1 = p; child2 = m;

		//交叉
		for (int i = 0; i < n - mid; i++) {
			child1.data[i] = m.data[i + mid];
			child2.data[i] = p.data[i + mid];
		}

		//后续元素的填充
		int pos = 0;
		for (int i = n - mid; i < n; i++) {		
			int flag = 0;
			do {
				flag = 0;
				for (int j = 0; j < n - mid; j++) {
					if (p.data[pos] == child1.data[j]) {
						flag = 1;
						pos++;
						break;
					}
				}
			} while (flag == 1);

			child1.data[i] = p.data[pos++];
		}
		pos = 0;
		for (int i = n - mid; i < n; i++) {
			int flag = 0;
			do {
				flag = 0;
				for (int j = 0; j < n - mid; j++) {
					if (m.data[pos] == child2.data[j]) {
						flag = 1;
						pos++;
						break;
					}
				}
			} while (flag == 1);

			child2.data[i] = m.data[pos++];
		}
	}

	//变异
	void mutation(Gene &child) {
		int l = rand() % n;
		int r = rand() % n;
		if (l == r)	return;

		swap(child.data[l], child.data[r]);
	}

	//单次迭代
	void iterate() {
		//轮盘赌选择交叉池个体		
		vector<Gene> parent; //交叉池父代
		{
			double valueSum = 0;
			for (int i = 0; i < population.size(); i++) {
				valueSum += population[i].value;
			}
			vector<double> chance(population.size());
			for (int i = 0; i < population.size(); i++) {
				chance[i] = population[i].value / valueSum;
				if (i > 0)	chance[i] += chance[i - 1];
			}
			
			for (int i = 0; i < n * num_child; i++) {
				double random = randBetween01();
				for (int j = 0; j < population.size(); j++) {
					if (random < chance[j]) {
						parent.push_back(population[j]);
						break;
					}
				}
			}
		}

		//交叉和变异
		for (int i = 1; i < parent.size(); i += 2) {
			Gene child1 = parent[i - 1], child2 = parent[i];
			//交叉
			double random = randBetween01();
			if (random < p_crossover) {			
				crossover(parent[i - 1], parent[i], child1, child2);
			}
			//变异
			random = randBetween01();
			if (random < p_mutation) {
				mutation(child1);
			}
			random = randBetween01();
			if (random < p_mutation) {
				mutation(child2);
			}

			child1.getFitnessValue(distance);
			child2.getFitnessValue(distance);

			population.push_back(child1);
			population.push_back(child2);
		}

		//保留精英个体
		sort(population.begin(), population.end());
		vector<Gene> child_population;
		for (int i = 0; i < elite; i++) {
			child_population.push_back(population[i]);
		}

		//选择剩余个体
		{
			double valueSum = 0;
			for (int i = 0; i < population.size(); i++) {
				valueSum += population[i].value;
			}
			vector<double> chance(population.size());
			for (int i = 0; i < population.size(); i++) {
				chance[i] = population[i].value / valueSum;
				if (i > 0)	chance[i] += chance[i - 1];
			}

			for (int i = 0; i < populationSize - elite; i++) {
				double random = randBetween01();
				for (int j = 0; j < population.size(); j++) {
					if (random < chance[j]) {
						child_population.push_back(population[j]);
						break;
					}
				}
			}
		}

		//交换数据
		swap(population, child_population);
	}

	//遗传算法求解
	double solve(string filename) {
		readFile(filename);
		initPopulation();

		

		for (int i = 0; i < times_iterate; i++) {
			printf("迭代 %d\n", i);
			printf("%lf\n", population[0].value);
			iterate();
		}

		Gene ans = population[0];
		for (int i = 0; i < n; i++) {
			printf("%d%c", ans.data[i], i == n - 1 ? '\n' : ' ');
		}
		printf("value = %lf\n", ans.value);

		//保存结果
		FILE *fp = fopen("result.txt", "w");
		for (int i = 0; i < n; i++) {
			fprintf(fp, "%d%c", ans.data[i], i == n - 1 ? '\n' : ' ');
		}
		fprintf(fp, "%lf\n", ans.value);
		fclose(fp);

		return ans.value;
	}
};

#pragma once
