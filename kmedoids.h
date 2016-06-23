#pragma once
//传统凝聚层次聚类算法
#ifndef  k_medoids
#define k_medoids

#include <iostream>
#include<cstdlib>
#include <time.h>
#include <vector>
using namespace std;

class Node {
public:
	Node();
	Node(int id, double longitude, double latitude);
	void SetId(int id);
	int GetId();
	void SetEdgeNumber(int edgeNumber);
	int GetEdgeNumber();
	void SetLongitude(double longitude);
	double GetLongitude();
	void SetLatitude(double latitude);
	double GetLatitude();
	void Display();
	~Node();
private:
	int id;
	double longitude;
	double latitude;
	int edgeNumber;
};
class Pair {
public:
	int i, j;
	Pair() {

	}
	Pair(int i, int j) {
		this->i = i;
		this->j = j;
	}
};

class KMedoids {
public:
	vector<Node*> dataset;
	vector<vector<Node*>*> cluster;
	vector<Node*> kcenter;
	vector<bool> marked;
	vector<double> curvalue;
	KMedoids() {}
	double getDistXY(Node* t1, Node* t2);
	void InitDataSet(string NODE_FILE_PATH);
	void GetKMedoids(string OUTPUT_FILE_PATH, int k);
	void print(string OUTPUT_FILE_PATH);
	double getDiameter(vector<Node*> *nodes, int &i1, int &i2);
	Node* getMeans(vector<Node*> *cluster);
	bool isNearToNew(vector<Node*> *nodes1, vector<Node*> *nodes2, Node* node);
	int clusterOfTuple(vector<Node*> means, Node *tuple);
	double abs(double data);
	double getDist(vector<Node*> *nodes, Node *node);
};
double KMedoids::abs(double a) {
	return a >= 0 ? a : -a;
}
double KMedoids::getDist(vector<Node*> *nodes, Node *node) {
	double result = 0;
	int i;
	for (i = 0;i < nodes->size();i++) {
		result+= abs(getDistXY(nodes->at(i), node));
	}
	return result;
}
//获得当前簇的均值（质心）  
Node* KMedoids::getMeans(vector<Node*> *cluster) {
	int num = cluster->size();
	Node *t = new Node();
	t->SetLatitude(0);
	t->SetLongitude(0);
	for (int i = 0; i < num; i++)
	{
		t->SetLatitude(t->GetLatitude() + cluster->at(i)->GetLatitude());
		t->SetLongitude(t->GetLongitude() + cluster->at(i)->GetLongitude());
	}
	t->SetLatitude(t->GetLatitude() / num);
	t->SetLongitude(t->GetLongitude() / num);
	return t;
	//cout<<"sum:"<<sum<<endl;  
}
double KMedoids::getDistXY(Node* t1, Node* t2) {
	double sum = 0;
	sum += (t1->GetLatitude() - t2->GetLatitude()) * (t1->GetLatitude() - t2->GetLatitude());
	sum += (t1->GetLongitude() - t2->GetLongitude()) * (t1->GetLongitude() - t2->GetLongitude());
	return sqrt(sum);
}

bool KMedoids::isNearToNew(vector<Node*> *nodes1, vector<Node*> *nodes2, Node* node) {
	Node *node1 = getMeans(nodes1);
	Node *node2 = getMeans(nodes2);
	double dis1 = getDistXY(node1, node);
	double dis2 = getDistXY(node2, node);
	return dis2 <= dis1;
}
double KMedoids::getDiameter(vector<Node*> *nodes, int &i1, int &i2) {
	double maxdis = 0;
	int i, j;
	for (i = 0;i < nodes->size();i++) {
		for (j = i + 1;j < nodes->size();j++) {
			double tp = getDistXY(nodes->at(i), nodes->at(j));
			i1 = i;
			i2 = j;
			if (tp > maxdis) {
				maxdis = tp;
			}
		}
	}
	return maxdis;
}
void KMedoids::print(string OUTPUT_FILE_PATH)
{
	FILE *fp = fopen(OUTPUT_FILE_PATH.c_str(), "w");
	int pos = 1;
	for (int lable = 0; lable<cluster.size(); lable++)
	{
		fprintf(fp, "第%d个簇：\n", pos++);
		vector<Node*> *t = cluster[lable];
		for (int i = 0; i<t->size(); i++)
		{
			fprintf(fp, "%d(", i + 1);
			fprintf(fp, "%lf,%lf", t->at(i)->GetLatitude(), t->at(i)->GetLongitude());
			fprintf(fp, ")\t");
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}
//根据质心，决定当前元组属于哪个簇  
int KMedoids::clusterOfTuple(vector<Node*> means, Node *tuple) {
	double dist = getDistXY(means[0], tuple);
	double tmp;
	int label = 0;//标示属于哪一个簇  
	for (int i = 1;i<kcenter.size();i++) {
		tmp = getDistXY(means[i], tuple);
		if (tmp<dist) {
			dist = tmp;
			label = i;
		}
	}
	return label;
}
void KMedoids::GetKMedoids(string OUTPUT_FILE_PATH, int k) {
	int i,lable;
	for (i = 0;i < k;) {
		int iToSelect = rand() %dataset.size();
		if (!marked[iToSelect]) {
			kcenter.push_back(dataset[iToSelect]);
			cluster.push_back(new vector<Node*>());
			marked[iToSelect] = true;
			i++;
		}
	}
	for (i = 0;i < dataset.size();++i) {
		lable = clusterOfTuple(kcenter, dataset[i]);
		cluster[lable]->push_back(dataset[i]);
	}
	for (i = 0;i < kcenter.size();i++) {
		curvalue.push_back(getDist(cluster[i],kcenter[i]));
	}
	int pos = 1;
	while (1) {
		bool changed = false;
		for (i = 0;i < kcenter.size();i++) {
			if(cluster[i]->size()>0){
				int iToSelect = rand() % cluster[i]->size();
				double tmp = getDist(cluster[i], cluster[i]->at(iToSelect));
				if (tmp<curvalue[i]) {
					curvalue[i] = tmp;
					kcenter[i] = cluster[i]->at(iToSelect);
					changed = true;
				}
			}
		}
		for (i = 0;i < k;i++) {
			cluster[i]->clear();
		}
		for (i = 0;i < dataset.size();++i) {
			lable = clusterOfTuple(kcenter, dataset[i]);
			cluster[lable]->push_back(dataset[i]);
		}
		cout << "第" << pos++ << "次迭代" << endl;
		if (!changed) {
			break;
		}
	}
	print(OUTPUT_FILE_PATH);
}

void KMedoids::InitDataSet(string NODE_FILE_PATH) {
	FILE *nodes, *edges, *edgeGeometry;
	nodes = fopen(NODE_FILE_PATH.c_str(), "r");
	if (!nodes) {
		cout << "打开失败" << endl;
		return;
	}
	int id = 0;
	double longitude = 0, latitude = 0;
	char ch;
	while ((ch = fgetc(nodes)) != EOF) {
		id *= 10; id += (ch - '0');
		while ((ch = fgetc(nodes)) != ' ') {
			id *= 10; id += (ch - '0');
		}
		while ((ch = fgetc(nodes)) != '.') {
			latitude *= 10; latitude += (ch - '0');
		}
		double tmp = 0.1;
		while ((ch = fgetc(nodes)) != ' ') {
			latitude += tmp*(ch - '0');
			tmp *= 0.1;
		}
		while ((ch = fgetc(nodes)) != '.') {
			longitude *= 10; longitude += (ch - '0');
		}
		tmp = 0.1;
		while ((ch = fgetc(nodes)) != '\n'&&ch != '\r') {
			longitude += tmp*(ch - '0');
			tmp *= 0.1;
		}
		//fgetc(nodes);
		Node *node = new Node(id, longitude, latitude);
		dataset.push_back(node);
		marked.push_back(false);
		if (dataset.size() >= 1500) {
			break;
		}
		id = 0;
		latitude = longitude = 0;
	}
	cout << "Node inition is OK!" << endl;
}

Node::Node() {
	this->id = -1;
	this->longitude = 0;
	this->latitude = 0;
	this->edgeNumber = 0;
}
Node::Node(int id, double longitude, double latitude) {
	this->id = id;
	this->longitude = longitude;
	this->latitude = latitude;
	this->edgeNumber = 0;
}
void Node::SetId(int id) {
	this->id = id;
}
int Node::GetId() {
	return this->id;
}
void Node::SetEdgeNumber(int edgeNumber) {
	this->edgeNumber = edgeNumber;
}
int Node::GetEdgeNumber() {
	return this->edgeNumber;
}
void Node::SetLongitude(double longitude) {
	this->longitude = longitude;
}
double Node::GetLongitude() {
	return this->longitude;
}
void Node::SetLatitude(double latitude) {
	this->latitude = latitude;
}
double Node::GetLatitude() {
	return this->latitude;
}

void Node::Display() {
	cout << "Node ID:" << this->GetId() << endl;
	cout << "(" << this->GetLatitude() << "," << this->GetLongitude() << ")" << endl;
}
Node::~Node() {
	this->id = -1;
	this->edgeNumber = 0;
}

#endif // ! agenes