

#include "graph.h"

graph::graph() {
	
	numberOfVertices = 0;
	adjacencyListGraph = new edge[numberOfVertices];
	adjacencyListMST = new edge[numberOfVertices];
	std::cout << "Default - Empty Graph Created\n";

}
graph::graph(int vertices) {

	numberOfVertices = vertices;
	adjacencyListGraph = new edge[numberOfVertices];
	adjacencyListMST = new edge[numberOfVertices];
}

//Description: adds an edge to the graph
//Pre-condition: need source, dest, and weight, and also output stream so we can output text to the user output file
//Post-condition: edge will be added to the adjacency array and proper outputs will occur to the screen and user file
void graph::addEdge(int source, int destination, int weight, std::ofstream& outFile) {

	//3 if-statements to check if request is valid:

	if (numberOfVertices == 0) {
	
		std::cout << "Empty Graph - Cannot Add Edge: " << source << "," << destination << "," << weight << std::endl;
		outFile << "Empty Graph - Cannot Add Edge: " << source << "," << destination << "," << weight << std::endl;
		return;
	}
	if (source < 0 || destination < 0 || source > (numberOfVertices - 1) || destination > (numberOfVertices - 1)) {
	
		std::cout << "Invalid Source or Destination Vertex - Cannot Add Edge: " << source << "," << destination << "," << weight 
					<< " - Edge request ignored" << std::endl;
		outFile << "Invalid Source or Destination Vertex - Cannot Add Edge: " << source << "," << destination << "," << weight
			<< " - Edge request ignored" << std::endl;
		return;
	}
	
	if (weight <= 0) {
	
		std::cout << "Invalid Weight - Cannot Add Edge: " << source << "," << destination << "," << weight 
					<< " - Edge request ignored" << std::endl;
		outFile << "Invalid Weight - Cannot Add Edge: " << source << "," << destination << "," << weight
			<< " - Edge request ignored" << std::endl;
		return;
	}


	//otherwise add edges (keep in mind that if we reach here, num of vertices != 0 and all other valid checks have been passed):

	edge* edgePtr = &adjacencyListGraph[source]; //point to start of list for the given source vertex

	while(true) {

		if (edgePtr->sourceVertex == -1) {

			edgePtr->sourceVertex = source;
			edgePtr->destinationVertex = destination;
			edgePtr->edgeWeight = weight;
			break;								//edge added; break from loop
		}
		if (edgePtr->nextEdge == nullptr) { 
			edgePtr->nextEdge = new edge(source, destination, weight);
			break;								//edge added; break from loop
		}
		edgePtr = edgePtr->nextEdge;			//edge already occupied; move to next edge and repeat loop
	}
		
	std::cout << "Edge Added: " << source << ", " << destination << ", " << weight << std::endl;
	outFile << "Edge Added: " << source << ", " << destination << ", " << weight << std::endl;

	//now add the additional edge since graph is undirected (destination, source, same weight):
	edgePtr = &adjacencyListGraph[destination]; //point to start of list for the given source vertex

	while (true) {

		if (edgePtr->sourceVertex == -1) {

			edgePtr->sourceVertex = destination; //notice how destination and source are reversed
			edgePtr->destinationVertex = source;
			edgePtr->edgeWeight = weight;
			break;								//edge added; break from loop
		}
		if (edgePtr->nextEdge == nullptr) {
			edgePtr->nextEdge = new edge(destination, source, weight);
			break;								//edge added; break from loop
		}
		edgePtr = edgePtr->nextEdge;			//edge already occupied; move to next edge and repeat loop
	}

	std::cout << "Edge Added: " << destination << ", " << source << ", " << weight << std::endl;
	outFile << "Edge Added: " << destination << ", " << source << ", " << weight << std::endl;

}

//Description: will print the adjacency list for the graph
//Pre-condition: graph must be a valid graph
//Post-condition: output adjacency list for the undirected graph
void graph::printGraph(std::ofstream& outFile) {

	std::cout << "Full Graph - Adjacency List: \n";
	outFile << "Full Graph - Adjacency List: \n";

	edge* edgePtr;	//use this pointer to navigate through linked list
	for (int i = 0; i < numberOfVertices; i++) {
		
		edgePtr = &adjacencyListGraph[i];		//set edge ptr to the current corresponding vertex
		std::cout << "Adj[" << i << "]-> ";		//print adjacency messages for the current vertex
		outFile << "Adj[" << i << "]-> ";

		while (edgePtr != nullptr && edgePtr->sourceVertex != -1) {
		
			std::cout << "(" << edgePtr->destinationVertex << "," << edgePtr->edgeWeight << ")";
			outFile << "(" << edgePtr->destinationVertex << "," << edgePtr->edgeWeight << ")";
			edgePtr = edgePtr->nextEdge;	//move to next edge in the adjacency list for the given vertex
		}
		std::cout << std::endl;
		outFile << std::endl;
	}



}

//Description: primes the MST (builds MST) using a priority queue implemented as a heap
//Pre-condition: must have a valid graph
//Post-condition: MST will be built and stored in the proper adjacency array
void graph::primMST(std::ofstream& outFile) {

	pqData extractedPQData, intoPQData;
	bool* mst;
	mst = new bool[numberOfVertices] {false};
	resultSetClass* resultSet;
	resultSet = new resultSetClass[numberOfVertices];
	int* weights;
	weights = new int[numberOfVertices] {INT_MAX};
}

//Description: print adjacency list of MST and other information to screen and the output file 
//Pre-condition: must have a valid graph
//Post-condition: MST adjacency list and MST value will be printed to user screen and output file
void graph::printMST(std::ofstream& outFile) {}

graph::~graph() {

	//deallocate dynamically allocated memory from constructor
	delete[] adjacencyListGraph;
	delete[] adjacencyListMST;
}