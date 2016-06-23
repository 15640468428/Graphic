#ifndef graphic_h
#define graphic_h

#include <iostream>
#include <vector>
#include <string>
#include "tool.h"
using namespace std;

class Node;
class Edge {
public:
	Edge();
	Edge(int id, Node* startNode, Node* endNode);
	void SetLength(double length);
	void SetLength();
	double GetLength();
	void SetId(int id);
	int GetId();
	void SetStartNode(Node *startNode);
	Node* GetStartNode();
	void SetEndNode(Node *endNode);
	Node* GetEndNode();
	void SetEdgeGeometry(vector<Node*> edgeGeometry);
	vector<Node*> GetEdgeGeometry();
	void Display();
	int GetGeometrySize();
	void SetGeometrySize(int geometrySize);
	void AppendGeometry(Node* point);
	~Edge();
	int id;
	Node *startNode, *endNode;
	vector<Node*> edgeGeometry;
	int geometrySize;
	double length;
};

class Node {
public:
	Node();
	Node(int id, double longitude, double latitude);
	void AppendOutEdge(Edge* e);
	void SetId(int id);
	int GetId();
	void SetEdgeNumber(int edgeNumber);
	int GetEdgeNumber();
	void SetLongitude(double longitude);
	double GetLongitude();
	void SetLatitude(double latitude);
	double GetLatitude();
	void SetOutEdges(vector<Edge*> outEdges);
	vector<Edge*> GetOutEdges();
	Edge *GetOutEdgeById(int id);
	void Display();
	~Node();
private:
	int id;
	double longitude;
	double latitude;
	int edgeNumber;
	vector<Edge*> outEdges;
};

class Graphic {
public:
	Graphic();
	void SetEdgeNumber(int edgeNumber);
	vector<Node*> GetNodesList();
	int GetEdgeNumber();
	void SetNodeNumber(int nodeNumber);
	int GetNodeNumber();
	void AppendNode(Node* newNode);
	void AppendEdge(Edge* newEdge);
	Node *GetNodeById(int id);
	Edge *GetEdgeById(int id);
	void InitGraphic(string NODE_FILE_PATH, string EDGE_FILE_PATH, string EDGEGEOMETRY_FILE_PATH);
	void Display();
	~Graphic();
private:
	int edgeNumber;
	vector<Edge*> edgeList;
	int nodeNumber;
	vector<Node*> nodeList;
};
Edge::Edge() {
	this->id = -1;
	this->startNode = NULL;
	this->endNode = NULL;
	this->edgeGeometry.clear();
}
Edge::Edge(int id, Node* startNode, Node* endNode) {
	this->id = id;
	this->startNode = startNode;
	this->endNode = endNode;
	this->edgeGeometry.clear();
}
void Edge::SetLength() {
	this->SetLength(0);
	for (int i = 1; i < this->GetGeometrySize(); ++i) {
		this->length += calDistance(this->edgeGeometry[i - 1]->GetLatitude(),
			this->edgeGeometry[i - 1]->GetLongitude(), this->edgeGeometry[i]->GetLatitude(),
			this->edgeGeometry[i]->GetLongitude());
	}
}
int Edge::GetGeometrySize() {
	return this->edgeGeometry.size();
}
void Edge::SetGeometrySize(int geometrySize) {
	this->geometrySize = geometrySize;
}
void Edge::AppendGeometry(Node* point) {
	this->geometrySize++;
	this->edgeGeometry.push_back(point);
}
void Edge::SetLength(double length) {
	this->length = length;
}
double Edge::GetLength() {
	return this->length;
}
void Edge::SetId(int id) {
	this->id = id;
}
int Edge::GetId() {
	return this->id;
}
void Edge::SetStartNode(Node *startNode) {
	this->startNode = startNode;
}
Node* Edge::GetStartNode() {
	return this->startNode;
}
void Edge::SetEndNode(Node *endNode) {
	this->endNode = endNode;
}
Node* Edge::GetEndNode() {
	return this->endNode;
}
void Edge::SetEdgeGeometry(vector<Node*> edgeGeometry) {
	this->edgeGeometry = edgeGeometry;
}
vector<Node*> Edge::GetEdgeGeometry() {
	return this->edgeGeometry;
}
void Edge::Display() {
	cout << "Edge ID:" << this->GetId() << endl;
	cout << "\tstart Node:"<<this->GetStartNode()->GetId()<<endl;
	//this->GetStartNode()->Display();
	//cout << endl;
	//cout << "\tend Node:";
	cout<<"\tend Node:"<<this->GetEndNode()->GetId()<<endl;
	//this->GetEndNode()->Display();
	//cout << endl;
}
Edge::~Edge() {
	this->startNode = NULL;
	this->endNode = NULL;
	this->edgeGeometry.clear();
	vector<Node*>().swap(this->edgeGeometry);
	this->length = 0;
	this->id = -1;
}

Node::Node() {
	this->id = -1;
	this->longitude = 0;
	this->latitude = 0;
	this->outEdges.clear();
	this->edgeNumber = 0;
}
Node::Node(int id, double longitude, double latitude) {
	this->id = id;
	this->longitude = longitude;
	this->latitude = latitude;
	this->outEdges.clear();
	this->edgeNumber = 0;
}
void Node::AppendOutEdge(Edge* e) {
	++this->edgeNumber;
	this->outEdges.push_back(e);
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
Edge *Node::GetOutEdgeById(int id) {
	Edge *tmp = this->outEdges.at(id);
	return tmp;
}
void Node::SetOutEdges(vector<Edge*> outEdges) {
	this->outEdges = outEdges;
}
vector<Edge*> Node::GetOutEdges() {
	return this->outEdges;
}

void Node::Display() {
	cout << "Node ID:" << this->GetId() << endl;
	cout << "(" << this->GetLatitude() << "," << this->GetLongitude() << ")" << endl;
	for(int i=0;i<this->GetEdgeNumber();i++){
        this->GetOutEdgeById(i)->Display();
	}
}
Node::~Node() {
	this->id = -1;
	this->edgeNumber = 0;
	this->outEdges.clear();
	vector<Edge*>().swap(this->outEdges);
}

Graphic::Graphic() {
	this->edgeNumber = 0;
	this->nodeNumber = 0;
	this->edgeList.clear();
	this->nodeList.clear();
}
void Graphic::SetEdgeNumber(int edgeNumber) {
	this->edgeNumber = edgeNumber;
}
int Graphic::GetEdgeNumber() {
	return this->edgeNumber;
}
void Graphic::SetNodeNumber(int nodeNumber) {
	this->nodeNumber = nodeNumber;
}
int Graphic::GetNodeNumber() {
	return this->nodeNumber;
}
void Graphic::AppendNode(Node* newNode) {
	this->nodeList.push_back(newNode);
	this->nodeNumber++;
}
void Graphic::AppendEdge(Edge* newEdge) {
	this->edgeList.push_back(newEdge);
	this->edgeNumber++;
}
Node* Graphic::GetNodeById(int id) {
	if (this->nodeList.at(id)->GetId() == id) {
		return this->nodeList.at(id);
	}
	else {
		for (int i = 0; i < this->nodeNumber; ++i) {
			if (this->nodeList.at(id)->GetId() == id) {
				return this->nodeList.at(id);
			}
		}
	}
	return NULL;
}
Edge* Graphic::GetEdgeById(int id) {
	return this->edgeList.at(id);
}
void Graphic::Display() {
	cout << "Edge Number:" << this->GetEdgeNumber() << endl;
	cout << "Node Number:" << this->GetNodeNumber() << endl;
}
Graphic::~Graphic() {
	this->nodeNumber = 0;
	this->edgeNumber = 0;
	this->nodeList.clear();
	this->edgeList.clear();
	vector<Edge*>().swap(this->edgeList);
	vector<Node*>().swap(this->nodeList);
}
void Graphic::InitGraphic(string NODE_FILE_PATH, string EDGE_FILE_PATH, string EDGEGEOMETRY_FILE_PATH) {
	FILE *nodes, *edges, *edgeGeometry;
	nodes = fopen(NODE_FILE_PATH.c_str(), "r");
	edges = fopen(EDGE_FILE_PATH.c_str(), "r");
	edgeGeometry = fopen(EDGEGEOMETRY_FILE_PATH.c_str(), "r");
    if(!nodes){
		cout << NODE_FILE_PATH << "打开失败" << endl;
		return;
	}
    if(!edges){
		cout << EDGE_FILE_PATH << "打开失败" << endl;
		return;
	}
    if(!edgeGeometry){
		cout << EDGEGEOMETRY_FILE_PATH << "打开失败" << endl;
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
		//cout<<latitude<<endl;
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
		this->AppendNode(node);
		id = 0;
		latitude = longitude = 0;
	}
	cout << "Node inition is OK!" << endl;
	id = 0;
	int start = 0, end = 0;
	while ((ch = fgetc(edges)) != EOF) {
		id *= 10; id += (ch - '0');
		while ((ch = fgetc(edges)) != ' ') {
			id *= 10; id += (ch - '0');
		}
		while ((ch = fgetc(edges)) != ' ') {
			start *= 10; start += (ch - '0');
		}
		while ((ch = fgetc(edges)) != '\n'&&ch != '\r') {
			end *= 10; end += (ch - '0');
		}
		//fgetc(edges);
		Edge *edge = new Edge(id, this->GetNodeById(start), this->GetNodeById(end));
		this->AppendEdge(edge);
		this->GetNodeById(start)->AppendOutEdge(edge);
		id = 0;
		start = end = 0;
	}
	cout << "Edge inition is OK!" << endl;
	int tmpId = 0;
	double tmpLat = 0, tmpLng = 0;
	while ((ch = fgetc(edgeGeometry)) != EOF) {
		tmpId *= 10; tmpId += (ch - '0');
		while ((ch = fgetc(edgeGeometry)) != '^') {
			tmpId *= 10; tmpId += (ch - '0');
		}
		Edge* tmpEdge = this->GetEdgeById(tmpId);

		ch = fgetc(edgeGeometry);
		while ((ch = fgetc(edgeGeometry)) != '^') {}
		ch = fgetc(edgeGeometry);
		ch = fgetc(edgeGeometry);
		while ((ch = fgetc(edgeGeometry)) != '\n'&&ch != '\r') {
			tmpLat *= 10; tmpLat += (ch - '0');
			while ((ch = fgetc(edgeGeometry)) != '.') {
				tmpLat *= 10; tmpLat += (ch - '0');
			}
			double tmp = 0.1;
			while ((ch = fgetc(edgeGeometry)) != '^') {
				tmpLat += (ch - '0')*tmp;
				tmp *= 0.1;
			}
			while ((ch = fgetc(edgeGeometry)) != '.') {
				tmpLng *= 10; tmpLng += (ch - '0');
			}
			tmp = 0.1;
			while ((ch = fgetc(edgeGeometry)) != '^'&&ch != '\n'&&ch != '\r') {
				tmpLng += (ch - '0')*tmp;
				tmp *= 0.1;
			}
			tmpEdge->AppendGeometry(new Node(-1, tmpLng, tmpLat));
			tmpLat = tmpLng = 0;
			if (ch == '\n' || ch == '\r') {
                //fgetc(edgeGeometry);
                break;
			}
		}
		tmpEdge->SetLength();
		tmpId = 0;
		tmpLat = tmpLng = 0;
	}
	cout << "edgeGeometry inition is OK!" << endl;
	fclose(nodes);
	fclose(edges);
	fclose(edgeGeometry);
}
vector<Node*> Graphic::GetNodesList() {
	return this->nodeList;
}

#endif

