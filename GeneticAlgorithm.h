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

	//ϴ���㷨�����������
	Gene(int n) {
		for (int i = 0; i < n; i++) {
			data.push_back(i);
		}
		std::mt19937 gen(std::random_device{}());
		shuffle(data.begin(), data.end(), gen);
	}

	//������Ӧ��
	double getFitnessValue(vector<double> &d) {
		double res = 0;
		int n = data.size();
		for (int i = 1; i < n; i++) {
			res += d[data[i - 1] * n + data[i]];
		}
		res += d[data[n - 1] * n + data[0]];
		return value = res;
	}

	//�ض���ȽϺ���
	bool operator < (const Gene &x) const {
		return value < x.value;
	}
};

class GA {
public:
	//����
	int populationSize = 2000; //��Ⱥ��С
	double num_child = 1.0f; //�����Ӵ�/��������
	double p_crossover = 0.6f; //�������
	double p_mutation = 0.25f; //�������
	int elite = 1800; //��Ӣ����
	int times_iterate = 9000; //��������

	int n = 0;
	//double ans = inf;
	vector<Gene> population;
	vector<double> distance;

	//����0-1֮��������
	double randBetween01() {
		return (double)(rand() % 10000) / 10000.0;
	}

	//��ȡ�ļ���������о���
	void readFile(string filename) {
		FILE *fp = fopen(filename.c_str(), "r");
		if (fp == NULL) {
			printf("��ȡ�ļ�ʧ��\n");
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

	//��Ⱥ��ʼ��
	void initPopulation() {
		srand(time(0));
		for (int i = 0; i < populationSize; i++) {
			Gene temp = Gene(n);
			temp.getFitnessValue(distance);
			population.push_back(temp);
		}
	}

	//����
	void crossover(const Gene &p, const Gene &m, Gene &child1, Gene &child2) {
		int mid = rand() % n;


		child1 = p; child2 = m;

		//����
		for (int i = 0; i < n - mid; i++) {
			child1.data[i] = m.data[i + mid];
			child2.data[i] = p.data[i + mid];
		}

		//����Ԫ�ص����
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

	//����
	void mutation(Gene &child) {
		int l = rand() % n;
		int r = rand() % n;
		if (l == r)	return;

		swap(child.data[l], child.data[r]);
	}

	//���ε���
	void iterate() {
		//���̶�ѡ�񽻲�ظ���		
		vector<Gene> parent; //����ظ���
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

		//����ͱ���
		for (int i = 1; i < parent.size(); i += 2) {
			Gene child1 = parent[i - 1], child2 = parent[i];
			//����
			double random = randBetween01();
			if (random < p_crossover) {			
				crossover(parent[i - 1], parent[i], child1, child2);
			}
			//����
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

		//������Ӣ����
		sort(population.begin(), population.end());
		vector<Gene> child_population;
		for (int i = 0; i < elite; i++) {
			child_population.push_back(population[i]);
		}

		//ѡ��ʣ�����
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

		//��������
		swap(population, child_population);
	}

	//�Ŵ��㷨���
	double solve(string filename) {
		readFile(filename);
		initPopulation();

		

		for (int i = 0; i < times_iterate; i++) {
			printf("���� %d\n", i);
			printf("%lf\n", population[0].value);
			iterate();
		}

		Gene ans = population[0];
		for (int i = 0; i < n; i++) {
			printf("%d%c", ans.data[i], i == n - 1 ? '\n' : ' ');
		}
		printf("value = %lf\n", ans.value);

		//������
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
