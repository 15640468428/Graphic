#pragma once
#ifndef  dbscan
#define dbscan

#include <iostream>
#include <vector>
#include <queue>
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

class DBScan {
public:
	vector<Node*> dataset;
	vector<vector<Node*>*> cluster;
	vector<double> distances;
	vector<bool> marked;
	vector<bool> iscluster;
	vector<Pair*> pairs;
	vector<bool> visited;
	DBScan() {}
	double getDistXY(Node* t1, Node* t2);
	void InitDataSet(string NODE_FILE_PATH);
	void GetDBScan(string OUTPUT_FILE_PATH,double radius, int objectNum);
	void print(string OUTPUT_FILE_PATH);
	double getDiameter(vector<Node*> *nodes, int &i1, int &i2);
	Node* getMeans(vector<Node*> *cluster);
	bool isNearToNew(vector<Node*> *nodes1, vector<Node*> *nodes2, Node* node);
	vector<Node*>* getNodes(Node* node, double radius, int num, int i);

};
//获得当前簇的均值（质心）  
Node* DBScan::getMeans(vector<Node*> *cluster) {
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
double DBScan::getDistXY(Node* t1, Node* t2) {
	double sum = 0;
	sum += (t1->GetLatitude() - t2->GetLatitude()) * (t1->GetLatitude() - t2->GetLatitude());
	sum += (t1->GetLongitude() - t2->GetLongitude()) * (t1->GetLongitude() - t2->GetLongitude());
	return sqrt(sum);
}

bool DBScan::isNearToNew(vector<Node*> *nodes1, vector<Node*> *nodes2, Node* node) {
	Node *node1 = getMeans(nodes1);
	Node *node2 = getMeans(nodes2);
	double dis1 = getDistXY(node1, node);
	double dis2 = getDistXY(node2, node);
	return dis2 <= dis1;
}
double DBScan::getDiameter(vector<Node*> *nodes, int &i1, int &i2) {
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
void DBScan::print(string OUTPUT_FILE_PATH)
{
	FILE *fp = fopen(OUTPUT_FILE_PATH.c_str(), "w");
	int pos = 1;
	for (int lable = 0; lable<cluster.size(); lable++)
	{
		if (cluster[lable] != NULL&&cluster[lable]->size()!=0&&iscluster[lable]) {
			vector<Node*> *t = cluster[lable];
			fprintf(fp, "第%d个簇(%d个)：\n", pos++, t->size());
			for (int i = 0; i<t->size(); i++){
				fprintf(fp, "%d(", t->at(i)->GetId());
				fprintf(fp, "%lf,%lf", t->at(i)->GetLatitude(), t->at(i)->GetLongitude());
				fprintf(fp, ")\t");
			}
			fprintf(fp, "\n");
		}
	}
	fclose(fp);
}
vector<Node*>* DBScan::getNodes(Node* node,double radius,int num,int ii) {
	vector<Node*> *result=new vector<Node*>();
	int i;
	for (i = 0;i < dataset.size();i++) {
		if (getDistXY(node, dataset[i]) <= radius) {
			result->push_back(dataset[i]);
		}
	}
	if (result->size()>=num) {
		marked[dataset[ii]->GetId()] = true;
	}
	return result;
}
void DBScan::GetDBScan(string OUTPUT_FILE_PATH,double radius, int objectNum) {
	int i,j,k,h;
	for (i = 0;i < dataset.size();i++) {
		vector<Node*>* tmp = getNodes(dataset[i], radius, objectNum,i);
		if (tmp != NULL) {
			cluster.push_back(tmp);
		}
	}
	queue<Node*> myqueue;
	int pos = 1;
	for (i = 0;i < visited.size();i++) {
		if (visited[dataset[i]->GetId()] == false && marked[i] == true) {
			myqueue.push(dataset[i]);
			iscluster[i] = true;
			while (!myqueue.empty()) {
				Node* tmp = myqueue.front();
				visited[tmp->GetId()] = true;
				myqueue.pop();
				bool state = false;
				for (k = 0;k < cluster[i]->size();k++) {
					if (tmp->GetId() == cluster[i]->at(k)->GetId()) {
						state = true;
						break;
					}
				}
				if (state == false) {
					cluster[i]->push_back(tmp);
					
				}
				
				cout << "第" << pos++ << "次迭代" << endl;
				for (j = 0;j < cluster[tmp->GetId()]->size();j++) {
					if (!visited[cluster[tmp->GetId()]->at(j)->GetId()]) {
						visited[cluster[tmp->GetId()]->at(j)->GetId()] = true;
						myqueue.push(cluster[tmp->GetId()]->at(j));
					}
				}
			}
		}
	}
	print(OUTPUT_FILE_PATH);
}

void DBScan::InitDataSet(string NODE_FILE_PATH) {
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
		visited.push_back(false);
		iscluster.push_back(false);
		if (dataset.size() >= 1000) {
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