#pragma once

/*
Dijkstra算法是由E.W.Dijkstra于1959年提出，
又叫迪杰斯特拉算法，它应用了贪心算法模式，
是目前公认的最好的求解最短路径的方法。算法
解决的是有向图中单个源点到其他顶点的最短路径
问题，其主要特点是每次迭代时选择的下一个顶点
是标记点之外距离源点最近的顶点。但由于dijkstra
算法主要计算从源点到其他所有点的最短路径，所以
算法的效率较低。*/
#ifndef dijkstra
#define dijkstra
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Edge;
class Node {
public:
	int id;
	vector<Edge*> outEdges;
	int distance;
	Node();
	Node(int id);
};
/*
class Pair{
public:
	Node* nodes;
	int distance;
	Pair(Node* newNode,int dis);
};
Pair::Pair(Node* newNode, int dis) {
	this->distance = dis;
	this->nodes = newNode;
}*/
class Edge {
public:
	int id;
	int len;
	int start, end;
	Edge();
	Edge(int id, int start, int end, int len);
};

Node::Node(int id) {
	this->id = id;
}

Edge::Edge(int id, int start, int end, int len) {
	this->end = end;
	this->id = id;
	this->start = start;
	this->len = len;
}

class Dijkstra {
public:
	vector<Node*> nodes;
	vector<Edge*> edges;
	void initGraph();
	void getKruskal(int start);
};

void Dijkstra::getKruskal(int start) {
	vector<int> distance;
	vector<bool> marked;
	int i, j;
	for (i = 0;i < nodes.size();i++) {
		distance.push_back(99999);
		marked.push_back(false);
	}
	marked[start] = true;
	nodes[start]->distance = 0;
	distance[start] = 0;
	
	while (1) {
		int dis = 99999;
		int pos = 0;
		bool state = false;
		int mindistance=99999;
		for (j = 0;j < marked.size();j++) {
			if (marked[j]) {
				for (i = 0;i < nodes[j]->outEdges.size();i++) {
					if (nodes[j]->distance + nodes[j]->outEdges[i]->len<mindistance&&!marked[nodes[j]->outEdges[i]->end-1]) {
						mindistance = nodes[j]->distance + nodes[j]->outEdges[i]->len;
						pos = nodes[j]->outEdges[i]->end-1;
						state = true;
					}
				}
			}
			
		}
		if (state == false) {
			break;
		}
		else {
			distance[pos] = mindistance;
			nodes[pos]->distance = mindistance;
			marked[pos] = true;
		}
	}
	for (i = 0;i < distance.size();i++) {
		cout << distance[i] << " ";
	}
	cout << endl;
}
void Dijkstra::initGraph() {
	int i, j;
	nodes.push_back(new Node(1));
	nodes.push_back(new Node(2));
	nodes.push_back(new Node(3));
	nodes.push_back(new Node(4));
	nodes.push_back(new Node(5));
	nodes.push_back(new Node(6));
	nodes.push_back(new Node(7));
	edges.push_back(new Edge(1, 1, 2, 7));
	edges.push_back(new Edge(2, 1, 3, 3));
	edges.push_back(new Edge(3, 2, 3, 3));
	edges.push_back(new Edge(4, 2, 4, 5));
	edges.push_back(new Edge(5, 3, 4, 3));
	edges.push_back(new Edge(6, 3, 5, 4));
	edges.push_back(new Edge(7, 4, 5, 2));
	edges.push_back(new Edge(8, 4, 6, 3));
	edges.push_back(new Edge(9, 5, 6, 5));
	edges.push_back(new Edge(10, 5, 7, 4));
	edges.push_back(new Edge(11, 6, 7, 3));

	nodes[0]->outEdges.push_back(edges[0]);
	nodes[0]->outEdges.push_back(edges[1]);
	nodes[1]->outEdges.push_back(edges[2]);
	nodes[1]->outEdges.push_back(edges[3]);
	nodes[2]->outEdges.push_back(edges[4]);
	nodes[2]->outEdges.push_back(edges[5]);
	nodes[3]->outEdges.push_back(edges[6]);
	nodes[3]->outEdges.push_back(edges[7]);
	nodes[4]->outEdges.push_back(edges[8]);
	nodes[4]->outEdges.push_back(edges[9]);
	nodes[5]->outEdges.push_back(edges[10]);
}

#endif