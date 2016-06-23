#pragma once
#ifndef kruskal
#define kruskal

#include <iostream>
#include <vector>
using namespace std;

class Node {
public:
	int id;
	Node();
	Node(int id);
};
class Edge {
public:
	int id;
	int len;
	int start, end;
	Edge();
	Edge(int id,int start,int end,int len);
};

Node::Node(int id) {
	this->id = id;
}

Edge::Edge(int id,int start,int end,int len) {
	this->end = end;
	this->id = id;
	this->start = start;
	this->len = len;
}
class Kruskal {
public:
	vector<Node*> nodes;
	vector<Edge*> edges;
	vector<vector<Node*>*> clusters;
	void initGraph();
	void getKruskal();
	void rangeEdge();
	bool isApart(Edge *edge);
	int getClusters(Node* node);
};

int Kruskal::getClusters(Node* node) {
	for (int i = 0;i < clusters.size();i++) {
		for (int j = 0;j < clusters[i]->size();j++) {
			if (clusters[i]->at(j)->id == node->id) {
				return i;
			}
		}
	}
	return -1;
}

bool Kruskal::isApart(Edge *edge) {
	int i, j;
	Node* node1 = nodes[edge->start-1];
	Node* node2 = nodes[edge->end-1];
	for (i = 0;i < clusters.size();i++) {
		bool state1 = false,state2=false;
		for (j = 0;j < clusters[i]->size();j++) {
			if (clusters[i]->at(j)->id == node1->id) {
				state1 = true;
			}
			if (clusters[i]->at(j)->id == node2->id) {
				state2 = true;
			}
		}
		if (state1&&state2) {
			return true;
		}
	}
	return false;
}
void Kruskal::rangeEdge() {
	int i, j;
	Edge* tmp;
	for (i = 0;i < edges.size();i++) {
		for (j = 0;j < edges.size() - i - 1;j++) {
			if (edges[j + 1]->len < edges[j]->len) {
				tmp = edges[j + 1];
				edges[j + 1] = edges[j];
				edges[j] = tmp;
			}
		}
	}
}
void Kruskal::getKruskal() {
	rangeEdge();
	int i, j;
	vector<Edge*> result;
	for (i = 0;i < edges.size();i++){
		int cluster1 = getClusters(nodes[edges[i]->start-1]);
		int cluster2 = getClusters(nodes[edges[i]->end - 1]);
		if (clusters[cluster1]->at(0)->id == clusters[cluster2]->at(0)->id) {
			continue;
		}
		else {
			result.push_back(edges[i]);
			for (j = 0;j < clusters[cluster2]->size();j++) {
				clusters[cluster1]->push_back(clusters[cluster2]->at(j));
			}
			clusters[cluster2]->clear();
		}
	}
	for (i = 0;i < result.size();i++) {
		cout << result[i]->start<<","<< result[i]->end << endl;
	}
}

void Kruskal::initGraph(){
	nodes.push_back(new Node(1)); clusters.push_back(new vector<Node*>()); clusters[0]->push_back(nodes[0]);
	nodes.push_back(new Node(2)); clusters.push_back(new vector<Node*>()); clusters[1]->push_back(nodes[1]);
	nodes.push_back(new Node(3)); clusters.push_back(new vector<Node*>()); clusters[2]->push_back(nodes[2]);
	nodes.push_back(new Node(4)); clusters.push_back(new vector<Node*>()); clusters[3]->push_back(nodes[3]);
	nodes.push_back(new Node(5)); clusters.push_back(new vector<Node*>()); clusters[4]->push_back(nodes[4]);
	nodes.push_back(new Node(6)); clusters.push_back(new vector<Node*>()); clusters[5]->push_back(nodes[5]);
	edges.push_back(new Edge(1,1,2,6));
	edges.push_back(new Edge(2, 1, 4, 5));
	edges.push_back(new Edge(3, 2, 3, 5));
	edges.push_back(new Edge(4, 3, 4, 5));
	edges.push_back(new Edge(5, 1, 3, 1));
	edges.push_back(new Edge(6, 2, 5, 3));
	edges.push_back(new Edge(7, 3, 5, 6));
	edges.push_back(new Edge(8, 3, 6, 4));
	edges.push_back(new Edge(9, 5, 6, 6));
	edges.push_back(new Edge(10, 4, 6, 2));
}

#endif