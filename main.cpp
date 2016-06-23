#include <iostream>
//#include "kmeans.h"
//#include "agenes.h"
//#include "diana.h"
//#include "kmedoids.h"
//#include "dbscan.h"
//#include "kruskal.h"
//#include "dijkstra.h"
#include "acm.h"
#include <string>
using namespace std;

int main() {
	/*
		Graphic *graph = new Graphic();
		graph->InitGraphic(
			"D://yangkai/plt/dataset/singapore/roadnetwork/WA_Nodes.txt",
			"D://yangkai/plt/dataset/singapore/roadnetwork/WA_Edges.txt",
			"D://yangkai/plt/dataset/singapore/roadnetwork/WA_EdgeGeometry.txt");
	*/
	/*
		K_Means *kmeans = new K_Means();
		kmeans->Init_K_Means("D://yangkai/plt/dataset/singapore/roadnetwork/WA_Nodes.txt");
		kmeans->Get_K_Means("D://实验数据/KMeans/singapore_nodes_k3.txt",3);
	*/
	/*
		Agenes *myagenes = new Agenes();
		myagenes->InitDataSet("D://yangkai/plt/dataset/singapore/roadnetwork/WA_Nodes.txt");
		myagenes->GetAgenes("D://实验数据/Agenes/singapore_nodes_k3.txt", 3);
	*/

	/*
		Diana *mydiana = new Diana();
		mydiana->InitDataSet("D://yangkai/plt/dataset/singapore/roadnetwork/WA_Nodes.txt");
		mydiana->GetDiana("D://实验数据/Diana/singapore_nodes_1500_k3.txt", 3);
	*/
	/*
		KMedoids *mydiana = new KMedoids();
		mydiana->InitDataSet("D://yangkai/plt/dataset/singapore/roadnetwork/WA_Nodes.txt");
		mydiana->GetKMedoids("D://实验数据/KMedoids/singapore_nodes_1500_k3.txt", 3);
	*/
	/*
		DBScan *mydiana = new DBScan();
		mydiana->InitDataSet("D://yangkai/plt/dataset/singapore/roadnetwork/WA_Nodes.txt");
		mydiana->GetDBScan("D://实验数据/DBScan/singapore_nodes_1000_k3.txt", 0.01,5);
	*/
	/*
		Kruskal *mykruskal = new Kruskal();
		mykruskal->initGraph();
		mykruskal->getKruskal();
	*/
	/*
		Dijkstra* dij = new Dijkstra();
		dij->initGraph();
		dij->getKruskal(0);
	*/
	
	Solution *solution = new Solution();
	/*vector<string> input;
	input.push_back("-2");
	input.push_back("1");
	input.push_back("+");
	input.push_back("3");
	input.push_back("*");
	cout << solution->evalRPN(input) << endl;;*/

	/*
	
	vector<int> number;
	int target;

	number.push_back(2);
	number.push_back(7);
	number.push_back(11);
	number.push_back(15);

	target = 9;
	vector<int> result= solution->twoSum(number, target);
	for (int i = 0;i < result.size();i++) {
		cout << result[i] << " ";
	}
	cout << endl;
	*/
	
	cout << solution->isMatch("bbbba",".*a*a") << endl;

	getchar();
	return 0;
}