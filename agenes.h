#pragma once
//传统凝聚层次聚类算法
#ifndef  agenes
#define agenes

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

class Trip {
public :
	int i, j;
	double distance;
	Trip() {
	}
	Trip(double distance,int i,int j) {
		this->distance = distance;
		this->i = i;
		this->j = j;
	}
};

class Agenes {
public:
	vector<Node*> dataset;
	vector<Trip*> trips;
	vector<vector<Node*>*> cluster;
	vector<bool> marked;
	Agenes() {}
	double getDistXY(Node* t1, Node* t2);
	void InitDataSet(string NODE_FILE_PATH);
	void GetAgenes(string OUTPUT_FILE_PATH,int k);
	void print(string OUTPUT_FILE_PATH);

};
double Agenes::getDistXY(Node* t1, Node* t2) {
	double sum = 0;
	sum += (t1->GetLatitude() - t2->GetLatitude()) * (t1->GetLatitude() - t2->GetLatitude());
	sum += (t1->GetLongitude() - t2->GetLongitude()) * (t1->GetLongitude() - t2->GetLongitude());
	return sqrt(sum);
}
void Agenes::print(string OUTPUT_FILE_PATH)
{
	FILE *fp = fopen(OUTPUT_FILE_PATH.c_str(), "w");
	int pos = 1;
	for (int lable = 0; lable<cluster.size(); lable++)
	{
		if (marked[lable]) {
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
	}
	fclose(fp);
}
void Agenes::GetAgenes(string OUTPUT_FILE_PATH, int kk) {
	/*
		int i,j,k,l;
		for (i = 0;i < dataset.size();i++) {
			cluster[i]->push_back(dataset[i]);
		}
		int pos = cluster.size();
		int key=1;
		while (pos!=kk) {
			cout << "第" << key++ << "次迭代" << endl;
			int mini, minj;
			double mindis = 1000000;
			for (i = 0;i < cluster.size();i++) {
				for (j = i + 1;j < cluster.size();j++) {
					for (k = 0;k < cluster[i]->size();k++) {
						for (l = 0;l < cluster[j]->size();l++) {
							double tmp = getDistXY(cluster[i]->at(k),cluster[j]->at(l));
							if (tmp < mindis) {
								mindis = tmp;
								mini = i;
								minj = j;
							}
						}
					}
				}
			}
			for (j = 0;j < cluster[minj]->size();j++) {
				cluster[mini]->push_back(cluster[minj]->at(j));
			}
			cluster.erase(cluster.begin()+minj);
			cout << "\t剩余" << cluster.size() <<endl;
			pos--;
		}
		print(OUTPUT_FILE_PATH);
	*/
	int i, j, k, l;
	for (i = 0;i < dataset.size();i++) {
		cluster[i]->push_back(dataset[i]);
	}
	for (i = 0;i < cluster.size();i++) {
		for (j = i + 1;j < cluster.size();j++) {
			for (k = 0;k < cluster[i]->size();k++) {
				for (l = 0;l < cluster[j]->size();l++) {
					double tmp = getDistXY(cluster[i]->at(k), cluster[j]->at(l));
					trips.push_back(new Trip(tmp,
						i,j));
				}
			}
		}
	}
	//sort(trips.begin(), trips.end(),SortByM1);
	Trip triptmp;
	for (i = 0;i < trips.size();i++) {
		//cout << i << endl;
		for (j = i + 1;j < trips.size();j++) {
			if (trips[j]->distance < trips[i]->distance) {
				triptmp = *trips[j];
				*trips[j] = *trips[i];
				*trips[i] = triptmp;
			}
		}
	}


	//for (i = 0;i < trips.size();i++) {
		//cout << trips[i]->distance << endl;
	//}
	//getchar();
	int pos = 0;
	int key = 1;
	int last=marked.size();
	while (last != kk) {
		cout << "第" << key++ << "次迭代" << endl;
		while (!marked[trips[pos]->i] || !marked[trips[pos]->j]) {
			pos++;
		}
		for (j = 0;j < cluster[trips[pos]->j]->size();j++) {
			cluster[trips[pos]->i]->push_back(cluster[trips[pos]->j]->at(j));
		}
		marked[trips[pos]->j] = false;
		last--;
		cout << "\t剩余" << last << endl;
	}
	print(OUTPUT_FILE_PATH);
}

void Agenes::InitDataSet(string NODE_FILE_PATH) {
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
		cluster.push_back(new vector<Node*>());
		marked.push_back(true);

		if (dataset.size() >= 100) {
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