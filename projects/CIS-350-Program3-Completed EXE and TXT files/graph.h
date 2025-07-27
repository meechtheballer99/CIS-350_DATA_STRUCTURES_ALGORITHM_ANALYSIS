#ifndef GRAPH
#define GRAPH
#include <iostream>
#include <fstream>
#include "pqData.h"
#include "resultSetClass.h"
#include "edge.h"

class graph
{
private:
	int numberOfVertices;
	edge* adjacencyListGraph;
	edge* adjacencyListMST;
public:
	graph();
	graph(int vertices);
	void addEdge(int source, int destination, int weight, std::ofstream& outFile);
	void printGraph(std::ofstream& outFile);
	void primMST(std::ofstream& outFile);
	void printMST(std::ofstream& outFile);;
	~graph();
};

#endif