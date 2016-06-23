#pragma once
//传统凝聚层次聚类算法
#ifndef  diana
#define diana

#include <iostream>
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

class Diana {
public:
	vector<Node*> dataset;
	vector<vector<Node*>*> cluster;
	vector<double> distances; 
	vector<Pair*> pairs;
	Diana() {}
	double getDistXY(Node* t1, Node* t2);
	void InitDataSet(string NODE_FILE_PATH);
	void GetDiana(string OUTPUT_FILE_PATH, int k);
	void print(string OUTPUT_FILE_PATH);
	double getDiameter(vector<Node*> *nodes, int &i1, int &i2);
	Node* getMeans(vector<Node*> *cluster);
	bool isNearToNew(vector<Node*> *nodes1,vector<Node*> *nodes2,Node* node);

};
//获得当前簇的均值（质心）  
Node* Diana::getMeans(vector<Node*> *cluster) {
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
double Diana::getDistXY(Node* t1, Node* t2) {
	double sum = 0;
	sum += (t1->GetLatitude() - t2->GetLatitude()) * (t1->GetLatitude() - t2->GetLatitude());
	sum += (t1->GetLongitude() - t2->GetLongitude()) * (t1->GetLongitude() - t2->GetLongitude());
	return sqrt(sum);
}

bool Diana::isNearToNew(vector<Node*> *nodes1, vector<Node*> *nodes2, Node* node) {
	Node *node1 = getMeans(nodes1);
	Node *node2 = getMeans(nodes2);
	double dis1 = getDistXY(node1,node);
	double dis2 = getDistXY(node2,node);
	return dis2 <= dis1;
}
double Diana::getDiameter(vector<Node*> *nodes,int &i1,int &i2) {
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
void Diana::print(string OUTPUT_FILE_PATH)
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
void Diana::GetDiana(string OUTPUT_FILE_PATH, int kk) {
	int i;
	cluster.push_back(new vector<Node*>());
	distances.push_back(0);
	pairs.push_back(new Pair(-1,-1));
	for (i = 0;i < dataset.size();i++) {
		cluster[0]->push_back(dataset[i]);
	}
	int num = 1;
	int pos = 0;
	while (num<kk) {
		cout << "第" << num << "次迭代" << endl;
		cout << "\t" << "目前已经有" << ++num << "个簇" << endl;
		double maxdis = 0;
		int a, b;
		int &aa = a, &bb = b;
		int cid=-1;
		for (i = 0;i < cluster.size();i++) {
			if (distances[i] == 0) {
				distances[i] = getDiameter(cluster[i],aa,bb);
				pairs[i]->i = aa;
				pairs[i]->j = bb;
				if (maxdis < distances[i]) {
					maxdis = distances[i];
					cid = i;
				}
			}
		}
		cluster.push_back(new vector<Node*>());
		distances.push_back(0);
		pairs.push_back(new Pair(-1, -1));
		cluster[cluster.size() - 1]->push_back(cluster[cid]->at(pairs[cid]->i));
		cluster[cluster.size() - 1]->push_back(cluster[cid]->at(pairs[cid]->j));
		if (pairs[cid]->j > pairs[cid]->i) {
			cluster[cid]->erase(cluster[cid]->begin() + pairs[cid]->j);
			cluster[cid]->erase(cluster[cid]->begin() + pairs[cid]->i);
		}else {
			cluster[cid]->erase(cluster[cid]->begin() + pairs[cid]->i);
			cluster[cid]->erase(cluster[cid]->begin() + pairs[cid]->j);
		}
		
		for (i = 0;i < cluster[cid]->size();i++) {
			if (isNearToNew(cluster[cid], cluster[cluster.size() - 1], cluster[cid]->at(i))) {
				cluster[cluster.size() - 1]->push_back(cluster[cid]->at(i));
				cluster[cid]->erase(cluster[cid]->begin()+i);
				i--;
			}
		}
		distances[cid] = 0;
		pairs[cid]->i = -1;
		pairs[cid]->j = -1;
	}
	print(OUTPUT_FILE_PATH);
}

void Diana::InitDataSet(string NODE_FILE_PATH) {
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