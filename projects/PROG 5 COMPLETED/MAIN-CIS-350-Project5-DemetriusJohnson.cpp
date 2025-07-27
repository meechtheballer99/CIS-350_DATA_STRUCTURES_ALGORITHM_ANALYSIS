// This file contains the 'main' function. Program execution begins and ends there.
//Author: Demetrius E Johnson
//Purpose: create a program that uses the backtracking algorithm to perform a word search on a character grid
//Date Created: 8/12/21
//Date Modified: 8/19/21



#include "Table.h"
#include <iostream>
#include <sstream>
#include<string>
#include <fstream>
using namespace std;


void searchSequence(string search, Table& grid, ofstream& of);

int main()
{
	string userInputFile;
	string userOutputFile;
	string testInputFile = "testGrid.dat";

	cout << "Welcome to the Backtracking Word Search Test Program\n";
	cout << "Enter output file name: ";
	cin >> userOutputFile;
	ofstream outputFile(userOutputFile);

	//output file not opened sucessfully case:
	if (!outputFile.good()) {
		cout << "file " << userOutputFile << " cannot be opened – program terminated...\n";
		system("pause");
		return 1;
	}

	outputFile << "Welcome to the Backtracking Word Search Test Program\n";
	outputFile << "Output file: " << userOutputFile << endl;

	//now build 4x4 test table using input test file "testGrid.dat" and test backtracking program functionality
	Table cGrid;
	char** testTable = cGrid.getTableReference();
	ifstream testInput(testInputFile);

	//ensure test files open:
	if (!testInput.is_open()) {
	
		cout << "ERROR: The test input file" << testInputFile << " could not be opened or found in proper system files location successfully...Program Terminated...\n";
		outputFile << "ERROR: The test input file" << testInputFile << " could not be opened or found in proper system files location successfully...Program Terminated...\n";
		system("pause");
		return 1;
	}

	//if return is true, then table was built successfully; otherwise return is false; exit program --> return 1 for program terminated with errors
	if (cGrid.buildTable(testInput, outputFile)) {
	
		cout << "Test character table loaded successfully...Test table:\n\n";
		outputFile << "Test character table loaded successfully...Test table:\n\n";
	}
	else { 
		system("pause");
		return 1;
	}

	//now call functions that test print functionality and wordsearch functionality:
	cGrid.printTable(outputFile);

	//Call test phrases to search using backtrack algorithm

	cout << "\nTesting Default Scenario...searching for words/phrases:\n'test' and 'TEST', 'test test', 'Test TEST', and 'test negative'.\n";
	outputFile << "\nTesting Default Scenario...searching for words/phrases:\n'test' and 'TEST', 'test test', 'Test TEST', and 'test negative'.\n";
	//run tests:
	searchSequence("test", cGrid, outputFile);
	searchSequence("TEST", cGrid, outputFile);
	searchSequence("test test", cGrid, outputFile);
	searchSequence("Test TEST", cGrid, outputFile);
	searchSequence("test negative", cGrid, outputFile);

	cout << "Default Test Scenario completed: success...\n\n";
	outputFile << "Default Test Scenario completed: success...\n\n";

	//now get user input file:

	cout << "Enter an input file name for character table: ";
	cin >> userInputFile;
	outputFile << "File name for user input character table: " << userInputFile << endl;
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

	//now build the character table/grid (2D array) from user input file:

	cGrid.destroyTable();								//clear out the test table
	cout << "Input file openend successfully...attempting to build table...\n";
	outputFile << "Input file openend successfully...attempting to build table...\n";

	//if return is true, then table was built successfully; otherwise return is false; exit program --> return 1 for program terminated with errors
	if (cGrid.buildTable(inputFile, outputFile)) {

		cout << "User input file " << cGrid.getNumRow() << "x" << cGrid.getNumCol() << " table loaded successfully...Program Launch Success - ready to search words.\nUser table:\n\n";
		outputFile << "User input file " << cGrid.getNumRow() << "x" << cGrid.getNumCol() << " table loaded successfully...Program Launch Success - ready to search words.\nUser table:\n\n";
		cGrid.printTable(outputFile);
		cout << endl;
		outputFile << endl;
	}
	else { 
		system("pause");
		return 1; 
	}
	char** inputCharTable = cGrid.getTableReference();	//get reference to the new table
	int numberofRows = cGrid.getNumRow();				//store row dimension from table
	int numberofCols = cGrid.getNumCol();				//store column dimension from table

	string userSearchInput = " ";

	//ensure cin is clear so that we can use getline function with no issues
	cin.seekg(0, ios::end);
	cin.clear();

	//do the following loop until user inputs a single character that is nonalphabetical:
	do {
		//start search prompts:
		cout << "Enter a word or phrase to search, or enter a single nonalphabetical character to quit:\n";
		cout << "Search word or phrase: ";
		getline(cin, userSearchInput);										//cannot simply use cin; need to use getline so that input doesn't stop at ws char and the entire input from user is acquired
		outputFile << "Search word or phrase: " << userSearchInput << endl; //output searched word/phrase to output file so we know the word being searched for
		cGrid.inputSearchVal(userSearchInput);								//add word to grid search value
		userSearchInput = cGrid.getSearchVal();								//now get the search key that is fully corrected for addtional ws char and capital letters changed to lowercase
		if (userSearchInput.size() == 1 && !isalpha(userSearchInput[0]) || userSearchInput.size() == 0) {	//check case where user wants to quit program (if size == 0, user has entered only ws character(s))
			break; 
		}
		while (cGrid.is_validWord() == false) {								//while search value is invalid, loop and ask for valid input
		
			cout << "ERROR: Invalid word or phrase: '" << userSearchInput << "'.\nRe-eneter word/phrase: ";
			outputFile << "ERROR: Invalid word or phrase: '" << userSearchInput << "'.\nRe-eneter word/phrase: ";
			getline(cin, userSearchInput);
			cGrid.inputSearchVal(userSearchInput);										//add word to grid
			userSearchInput = cGrid.getSearchVal();										//now get search key that is fully corrected for additional ws char and capital letters changed to lowercase
			if (userSearchInput.size() == 1 && !isalpha(userSearchInput[0] || userSearchInput.size() == 0)) { break; } //make sure user can still enter a single nonalpha char (or ws only) to exit program
		}
		cout << "Search word/phrase is valid.\nNumber of words in phrase is: " << cGrid.getNumWordsInSearchPhrase() << endl;
		outputFile << "Search word/phrase is valid. Number of words in phrase is: " << cGrid.getNumWordsInSearchPhrase() << endl;

		//now search for valid word/phrase and see if it is in the table:
		if (cGrid.wordSearch(outputFile)) {

			//if word is found, the two below functions need to execute to output coordinates of each word and the capitalized words of phrase table
			cGrid.printPhraseCoord(outputFile);
			cGrid.printPhraseFoundTable(outputFile);
		}
		//re-print table for next word to search:
		cout << endl;
		outputFile << endl;
		cGrid.printTable(outputFile);
		cout << endl;
		outputFile << endl;



	} while (userSearchInput.size() != 1 || isalpha(userSearchInput[0])); //both need to be false in order for loop to break; false OR false == false
	
	//while loop above exited; user has entered a single non alphabetical character:
	cout << "\n\nA single nonalphabetical character '" << userSearchInput << "' has been entered: quitting program...\n";
	outputFile << "\n\nA single nonalphabetical character " << userSearchInput << "has been entered: quitting program...\n";

	cout << "\n~Thank you for running the Word Search Backtracking Program written by Demetrius Johnson!\n";
	outputFile << "\n~Thank you for running the Word Search Backtracking Program written by Demetrius Johnson!\n";

	//close files
	testInput.close();
	outputFile.close();
	inputFile.close();

	system("pause");
	return 0;
}


//Author: Demetrius E Johnson
//Purpose: condense the initiation of a search, and if necessary print out of coordinates and table of a found phrase/word
//Date Created: 8/19/21
//Date Modified: 8/19/21
void searchSequence(string search, Table& grid, ofstream& of) {

	grid.inputSearchVal(search);
	if (grid.wordSearch(of)) {

		grid.printPhraseCoord(of);
		grid.printPhraseFoundTable(of);
	}
}