#ifndef EDGE
#define EDGE
class edge
{

private:
	int sourceVertex;
	int destinationVertex;
	int edgeWeight;
	edge* nextEdge;
	friend class graph;  //make graph a friend of edge so that graph can access the private members of this class
public:
	edge(); //default constructor
	edge(int source, int destination, int weight);


};

#endif