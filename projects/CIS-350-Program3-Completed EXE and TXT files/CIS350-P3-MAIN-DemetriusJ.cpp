// This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius E Johnson
//Purpose: create a program that uses a priority queue (using a heap) to implement Prim's aglorithm and effectively execute Minimum Spanning Tree Protocol
//Date Created: 7/14/21
//Date Modified: 7/15/21



#include <iostream>
#include <sstream>
#include<string>
#include <fstream>
#include "graph.h"
using namespace std;



int main()
{
	string userInputFile;
	string userOutputFile;

	cout << "Welcome to the MST Test Program\n";
	cout << "Enter output file name: ";
	cin >> userOutputFile;
	ofstream outputFile(userOutputFile);

	//output file not opened sucessfully case:
	if (!outputFile.good()) {
		cout << "file " << userOutputFile << " cannot be opened – program terminated...\n";
		return 1;
	}
	
	outputFile << "Welcome to the MST Test Program\n";
	outputFile << "Output file: " << userOutputFile << endl;

		//Create an empty graph and test functionality – No MST
	cout << "Testing Default Scenario...\n";
	outputFile << "Testing Default Scenario...\n";
	graph emptyGraphTest;
	emptyGraphTest.primMST(outputFile);
	emptyGraphTest.printMST(outputFile);

	cout << "Enter file name for graph data: ";
	cin >> userInputFile;
	outputFile << "File name for graph data: " << userInputFile << endl;
	ifstream inputFile(userInputFile);

	//input file not open successfully case:
	if (!inputFile.good()) {
		cout << "file " << userInputFile << " cannot be opened or does not exist - program terminated...\n";
		outputFile << "file " << userInputFile << " cannot be opened or does not exist - program terminated...\n";
		system("pause");
		return 1;
	}
	//empty input file case:
	while (inputFile.peek() == ' ' || inputFile.peek() == '\n') { inputFile.ignore(); } //ignore leading white spaces and newlines until we reach a char or EOF
	if (inputFile.peek() == EOF) {
		
		cout << "file " << userInputFile << " contains no data - program terminated...\n";
		outputFile << "file " << userInputFile << " contains no data - program terminated...\n";
		system("pause");
		return 1;
	}

	//loop until end of file; each loop instance is one graph:

	stringstream ss;		//use this to input an integer stored in a string into an int
	string lineParse;		//use this to parse each line from the input file
	int numVertices;		//store current numVertices for a graph in the input file
	int numEdges;			//store current numEdges for a graph in the input file
	graph* fileInputGraph;	//this is necessary so that each iteration of while loop we can create the proper graph instance

	while (inputFile.peek() != EOF) {

		cout << endl << endl;
		outputFile << endl << endl;

		while (inputFile.peek() == ' ' || inputFile.peek() == '\n') { inputFile.ignore(); } //if necessary: ignore leading white spaces / lines before next graph
		

		getline(inputFile, lineParse);	//store current line from input file into lineParse
		ss << lineParse;				//output to stream
		ss >> numVertices >> numEdges;	//convert char/string to integer values //side note: istream::operator>> only extracts characters; it does not also discard them; use cin.ignore() function to clear buffer if necessary
		ss.clear();						//clear stream in case of any bad bits set

		

		//next set of if statements will check number of vertices from the passed in graph:
		if (numVertices < 0) {

			cout << "ERROR: number of vertices: " << numVertices << " is less than zero\n"
				<< "Empty Graph Will Be Created\n";
			outputFile << "ERROR: number of vertices: " << numVertices << " is less than zero\n"
				<< "Empty Graph Will Be Created\n";
			fileInputGraph = new graph(0);
		}
		else if (numVertices == 0) {

			cout << "Number of vertices: " << numVertices << " is equal to zero\n"
				<< "Empty Graph Will Be Created\n";
			outputFile << "Number of vertices: " << numVertices << " is equal to zero\n"
				<< "Empty Graph Will Be Created\n";
			fileInputGraph = new graph(0);

		}
		else {

			cout << "Number of vertices: " << numVertices << " is valid\n";
			outputFile << "Number of vertices: " << numVertices << " is valid\n";

			//next set of if statements will check number of edges from the passed in graph:

			if (numEdges < (numVertices - 1) || numEdges < 0) {

				cout << "ERROR: number of edges: " << numEdges << " is invalid to create connected graph\n"
					<< "Empty Graph Will Be Created\n";
				outputFile << "ERROR: number of edges: " << numEdges << " is invalid to create connected graph\n"
					<< "Empty Graph Will Be Created\n";
				fileInputGraph = new graph(0);


			}
			else {

				cout << "Graph with " << numVertices << " vertices and " << numEdges << " edges will be created\n";
				outputFile << "Graph with " << numVertices << " vertices and " << numEdges << " edges will be created\n";
				fileInputGraph = new graph(numVertices);
				cout << "Number of input edges to process is: " << numEdges << endl;
				outputFile << "Number of input edges to process is: " << numEdges << endl;
			}
		}

		

		//second loop (process all edges given from the file):

		for (int i = 0; i < numEdges; i++) {

			int fileSource, fileDestination, fileWeight;		//use these to store the current edge
			getline(inputFile, lineParse);						//get current line (edge) which is a string
			ss << lineParse;									//output string into stringstream for integer conversion on next line
			ss >> fileSource >> fileDestination >> fileWeight;	//input the values into the appropriate integers
			ss.clear();											//clear stream in case of bad bit set; helps with felxibility of input file format
			fileInputGraph->addEdge(fileSource, fileDestination, fileWeight, outputFile); //add edge

		}

		//now call appropriate functions of the graph to create MST and output all results:
		fileInputGraph->printGraph(outputFile);
		fileInputGraph->primMST(outputFile);
		fileInputGraph->printMST(outputFile);

		delete fileInputGraph; // end of loop: delete graph; MST and other algorithms already ran; will create new graph at start of loop for next graph if necessary

	}//end of while loop

	cout << "Thank you for running the MST Test Program written by Demetrius Johnson!\n";
	outputFile << "Thank you for running the MST Test Program written by Demetrius Johnson!\n";

	//close files
	outputFile.close();
	inputFile.close();

	system("pause");
}



