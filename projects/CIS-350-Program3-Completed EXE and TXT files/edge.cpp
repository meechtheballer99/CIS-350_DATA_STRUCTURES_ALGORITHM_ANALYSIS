

#include "edge.h"


edge::edge() {

	sourceVertex = -1;
	destinationVertex = -1;
	edgeWeight = -1;
	nextEdge = nullptr;

} 
edge::edge(int source, int destination, int weight) {

	sourceVertex = source;
	destinationVertex = destination;
	edgeWeight = weight;
	nextEdge = nullptr;


}