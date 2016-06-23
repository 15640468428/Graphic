#pragma once

#ifndef  k_means
#define k_means

#include <iostream>
#include <vector>
#include <string>
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

class K_Means {
public:
	vector<Node*> dataset;
	vector<Node*> kcenter;
	vector<vector<Node*>*> clusters;//k����  
	vector<bool> marked;
	K_Means() {}
	void Init_K_Means(string NODE_FILE_PATH);
	void Get_K_Means(string OUTPUT_FILE_PATH,int k);
	double getDistXY(Node* t1, Node* t2);
	int clusterOfTuple(vector<Node*> means, Node *tuple);
	double getVar(vector<vector<Node*>*> clusters, vector<Node*> means);
	Node* getMeans(vector<Node*> *cluster);
	void print(string OUTPUT_FILE_PATH);
};

double K_Means::getDistXY(Node* t1, Node* t2){
	double sum = 0;
	sum += (t1->GetLatitude() - t2->GetLatitude()) * (t1->GetLatitude() - t2->GetLatitude());
	sum += (t1->GetLongitude() - t2->GetLongitude()) * (t1->GetLongitude() - t2->GetLongitude());
	return sqrt(sum);
}
//�������ģ�������ǰԪ�������ĸ���  
int K_Means::clusterOfTuple(vector<Node*> means, Node *tuple) {
	double dist = getDistXY(means[0], tuple);
	double tmp;
	int label = 0;//��ʾ������һ����  
	for (int i = 1;i<kcenter.size();i++) {
		tmp = getDistXY(means[i], tuple);
		if (tmp<dist) {
			dist = tmp;
			label = i;
		}
	}
	return label;
}
//��ø����ؼ���ƽ�����  
double K_Means::getVar(vector<vector<Node*>*> clusters, vector<Node*> means) {
	double var = 0;
	for (int i = 0; i < means.size(); i++)
	{
		vector<Node*> *t = clusters[i];
		for (int j = 0; j< t->size(); j++){
			var += getDistXY(t->at(j), means[i]);
		}
	}
	return var;
}

//��õ�ǰ�صľ�ֵ�����ģ�  
Node* K_Means::getMeans(vector<Node*> *cluster) {
	int num = cluster->size();
	Node *t = new Node();
	t->SetLatitude(0);
	t->SetLongitude(0);
	for (int i = 0; i < num; i++)
	{
		t->SetLatitude(t->GetLatitude()+cluster->at(i)->GetLatitude());
		t->SetLongitude(t->GetLongitude()+cluster->at(i)->GetLongitude());
	}
	t->SetLatitude(t->GetLatitude()/num);
	t->SetLongitude(t->GetLongitude() / num);
	return t;
	//cout<<"sum:"<<sum<<endl;  
}
void K_Means::print(string OUTPUT_FILE_PATH)
{
	FILE *fp = fopen(OUTPUT_FILE_PATH.c_str(),"w");
	for (int lable = 0; lable<kcenter.size(); lable++)
	{
		fprintf(fp,"��%d���أ�\n", lable + 1);
		vector<Node*> *t = clusters[lable];
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

void K_Means::Get_K_Means(string OUTPUT_FILE_PATH,int k) {
	int i;
	for (i = 0;i < k;) {
		int iToSelect = rand() % dataset.size();
		if (!marked[iToSelect]) {
			kcenter.push_back(dataset[iToSelect]);
			clusters.push_back(new vector<Node*>());
			marked[iToSelect] = true;
			i++;
		}
	}
	int lable = 0;
	for (i = 0;i < dataset.size();++i) {
		lable = clusterOfTuple(kcenter, dataset[i]);
		clusters[lable]->push_back(dataset[i]);
	}
	double oldVar = -1;
	double newVar = getVar(clusters, kcenter);
	cout << "��ʼ�ĵ��������ƽ����Ϊ��" << newVar << endl;
	int t = 0;

	while (abs(newVar - oldVar) >= 1) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ  
	{
		cout << "�� " << ++t << " �ε�����ʼ��" << endl;
		for (i = 0; i < k; i++) //����ÿ���ص����ĵ�  
		{
			kcenter[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, kcenter); //�����µ�׼����ֵ  
		for (i = 0; i < k; i++) //���ÿ����  
		{
			clusters[i]->clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != dataset.size(); ++i) {
			lable = clusterOfTuple(kcenter, dataset[i]);
			clusters[lable]->push_back(dataset[i]);
		}
		cout << "�˴ε���֮����������ƽ����Ϊ��" << newVar << endl;
	}
	print(OUTPUT_FILE_PATH);
}

void K_Means::Init_K_Means(string NODE_FILE_PATH) {
	FILE *nodes, *edges, *edgeGeometry;
	nodes = fopen(NODE_FILE_PATH.c_str(), "r");
	if (!nodes) {
		cout << NODE_FILE_PATH << "��ʧ��" << endl;
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

#endif // ! k_means