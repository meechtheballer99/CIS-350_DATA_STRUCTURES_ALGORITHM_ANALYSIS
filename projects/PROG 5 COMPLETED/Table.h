

#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <sstream>
#include<string>
#include <fstream>
#include <vector>
using namespace std;
class Table
{
private:
	int numRow = 0;
	int numCol = 0;
	int numWordsInPhrase = 0;
	//use this bool table to determine if a given coordinate has already been used in finding another word
	bool** usedCoordinates;
	//use this vector and associated functions to store the start and ending coordinates of all words in a found phrase; each found word will take up four vector elements (xy, xy)
	vector<int> foundCoord;
	void addCoord(int x, int y);
	void deleteLetterCoord(void);
	//I could have used a boolean array for all my bool variables and simply did a mapping but I figured it looks easier this way
	bool validWord = false;
	bool userPhraseFound = false;
	bool lastWordFound_right = false;
	bool lastWordFound_down = false;
	bool lastWordFound_left = false;
	bool lastWordFound_up = false;
	//use the below two bool variables to control when a coordinate should be deleted from coordinate vector
	bool addedFirstLetter = false;
	bool addedLastLetter = false;
	//user the char** arrays to store character tables
	char** arrayTable_2D;
	char** arrayTable_2D_copy;
	//use below strings to store the user search phrase: one with spaces, and one without
	string userSearchVal;
	string userSearchVal_noSpaces;

	//this private function will only be used by the inputSearchVal function to set the number of words found in the search phrase or get a given word from the phrase
	void setNumWordsInSearchPhrase(string searchKey);
	//these private functions used to check and set if all directions for a given element in the table have been searched
	void setAllDir_searched(bool val);
	//these private functions used to check a given direction (right, down, left, up)
	bool search_allDir(int& x, int& y, int& z, int& wordCount, ofstream& of);
	bool search_right(int& x, int& y, int& z, int& wordCount, ofstream& of);
	bool search_down(int& x, int& y, int& z, int& wordCount, ofstream& of);
	bool search_left(int& x, int& y, int& z, int& wordCount, ofstream& of);
	bool search_up(int& x, int& y, int& z, int& wordCount, ofstream& of);
	//use these functions to maintain the last word found:
	void foundRight(void);
	void foundDown(void);
	void foundLeft(void);
	void foundUp(void);

public:

	//primary functions/algorithms/set functions:
	bool buildTable(ifstream& inputFile, ofstream& outputFile); //return true == table build successful; return false == table not built succesfully
	bool wordSearch(ofstream& of);
	void inputSearchVal(string val);
	bool is_validWord(void);
	//print functions:
	void printTable(ofstream& of);
	void printPhraseFoundTable(ofstream& of);
	void printPhraseCoord(ofstream& of);
	//get functions:
	string getSearchVal(void);
	string getSearchVal_noSpaces(void);
	int getNumRow(void);
	int getNumCol(void);
	int getNumWordsInSearchPhrase(void);
	char** getTableReference(void);
	string getWordFromPhrase(int wordNum);
	//reset function:
	void resetPhraseTable(void);
	void resetBoolTable(void);
	//destructors
	void destroyTable(void);
	~Table();

};
#endif
