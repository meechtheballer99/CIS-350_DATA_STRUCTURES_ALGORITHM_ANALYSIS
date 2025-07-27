
#include "Table.h"

//Author: Demetrius E Johnson
//Purpose: add coordinates to coordiante vector in order to keep track of the first and last letter of each found word
//Date Created: 8/19/21
//Date Modified: 8/19/21
void Table::addCoord(int x, int y) {

	foundCoord.push_back(x);
	foundCoord.push_back(y);
}
//Author: Demetrius E Johnson
//Purpose: delete the coordinate of a word (first and last letter) in the event that it is not longer apart of the solution set for found words in a phrase
//Date Created: 8/19/21
//Date Modified: 8/19/21
void Table::deleteLetterCoord(void) {

	foundCoord.pop_back();
	foundCoord.pop_back();
}
//Author: Demetrius E Johnson
//Purpose: build several 2d arrays that are associated with the table input from the user input file
//Date Created: 8/12/21
//Date Modified: 8/19/21
bool Table::buildTable(ifstream& inputFile, ofstream& outputFile) {

	//first, check to ensure a table is not already present; call destroy function to clear the table if necessary:
	destroyTable();

	//now build the character table/grid (2D array)

	stringstream ss;			//use this to input an integer stored in a string into an int
	string lineParse;			//use this to parse each line from the input file




	cout << endl << endl;
	outputFile << endl << endl;

	while (inputFile.peek() == ' ' || inputFile.peek() == '\n') { inputFile.ignore(); } //if necessary: ignore leading white spaces or newlines to make input file more flexible by this program

	//acquire size of the table; the first 2 characters are numbers in the data file giving dimensions of the table; we wills store dimensions in numRow and numCol:

	getline(inputFile, lineParse);	//store current line from input file into lineParse
	ss << lineParse;				//output to stream
	ss >> numRow >> numCol;			//convert char/string to integer values //side note: istream::operator>> only extracts characters; it does not also discard them; use cin.ignore() function to clear buffer if necessary
	ss.clear();						//clear bad bits set for this stream in case of any bad bits set from outputting from the stream into a variable in the previous line
									//inputFile should now be pointing to the next line of the input file (first row of characters in the table)


	//next set of if statements will check the value of the numRow and numCol aquired from the input table to see if they are valid:
	if (numRow <= 0) { //check rows

		cout << "ERROR: number of rows: " << numRow << " is less than or equal to zero.\n"
			<< "Program terminated.\n";
		outputFile << "ERROR: number of rows: " << numRow << " is less than or equal to zero.\n"
			<< "Program terminated.\n";
		return false;
	}
	else if (numCol <= 0) {//check columns

		cout << "ERROR: number of columns: " << numCol << " is less than or equal to zero.\n"
			<< "Program terminated.\n";
		outputFile << "ERROR: number of columns: " << numCol << " is less than or equal to zero.\n"
			<< "Program terminated.\n";
		return false;
	}
	else { //otherwise: number of rows and columns is valid; we can proceed with attempting to build input character table

		cout << "Number of rows(" << numRow << ") and number of columns(" << numCol << ")" << " is valid.\n";
		outputFile << "Number of rows(" << numRow << ") and number of columns(" << numCol << ")" << " is valid.\n";

	}

	//we reach this point if rows and column values from the user input file are valid; now we must allocate the 2D array of the appropriate size:
	arrayTable_2D = new char* [numRow];												//double char ptr that points to an array of single char ptrs
	for (int i = 0; i < numRow; i++) { arrayTable_2D[i] = new char[numCol]; }		//allocate each char pointer to point to an array of char

	//create a copy of the table for later use in the printPhraseFoundTable function:
	arrayTable_2D_copy = new char* [numRow];
	for (int i = 0; i < numRow; i++) { arrayTable_2D_copy[i] = new char[numCol]; }

	//create bool table so we know which char in the table have been used when searching a phrase:
	usedCoordinates = new bool* [numRow];
	for (int i = 0; i < numRow; i++) { usedCoordinates[i] = new bool[numCol]; }


	//below loop: load all characters from input file character grid into the 2D array:

	for (int i = 0; i < numRow; i++) {
		
		getline(inputFile, lineParse);							//get the current line of characters from the table (getline function stops at the newline char and discards it when it reads it)
		for (int j = 0; j < numCol; j++) {

			arrayTable_2D[i][j] = lineParse[j];					//each line is of length j; there are i rows of length j (columns); so we need to acquire all elements from the string (characters)
			//before leaving loop, check to ensure that the character just added is alphabetical and that it is lowercase:
			if (!isalpha(arrayTable_2D[i][j])) {
			
				cout << "ERROR: Invalid character in table input: '" << arrayTable_2D[i][j] << "' in position [" << i << "][" << j << "]. Program Terminated...\n";
				outputFile << "ERROR: Invalid character in table input: '" << arrayTable_2D[i][j] << "' in position [" << i << "][" << j << "]. Program Terminated...\n";
				return false;
			}
			//if element added is alphabetical, convert it to lowercase if necessary:
			if (isupper(arrayTable_2D[i][j])) {
			
				cout << "WARNING: Uppercase letter in table input: '" << arrayTable_2D[i][j] << "' in position [" << i << "][" << j << "]. Converted to lowercase...\n";
				outputFile << "WARNING: Uppercase letter in table input: '" << arrayTable_2D[i][j] << "' in position [" << i << "][" << j << "]. Converted to lowercase...\n";
				arrayTable_2D[i][j] = tolower(arrayTable_2D[i][j]);
			}
			arrayTable_2D_copy[i][j] = arrayTable_2D[i][j]; //add char to copy table
		}
	}
	//2D array should now be fully loaded


	return true;
}
//Author: Demetrius E Johnson
//Purpose: deallocate all dynamic arrays so as to reset them for use in building another table
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::destroyTable(void) {

	//first check to ensure the current table instance (object) does not already have a table loaded
	if (arrayTable_2D != nullptr) {

		//deallocate dynamic 2d array:
		for (int i = 0; i < numRow; i++) { delete[] arrayTable_2D[i]; } //deallocate char* dynamic arrays
		delete[] arrayTable_2D;											//deallocate char** dynamic array
		
		//repeat above process to deallocate the copy version and bool version:

		//deallocate dynamic 2d array:
		for (int i = 0; i < numRow; i++) { delete[] arrayTable_2D_copy[i]; }	//deallocate char* dynamic arrays
		delete[] arrayTable_2D_copy;											//deallocate char** dynamic array
		
		//deallocate dynamic 2d array:
		for (int i = 0; i < numRow; i++) { delete[] usedCoordinates[i]; }	//deallocate char* dynamic arrays
		delete[] usedCoordinates;

		//reset numRow and numCol and set array to null ptr; this allow the object to effectively be reinitialzed to its start state so that it can be reused for a new table
		numRow = 0;
		numCol = 0;
		arrayTable_2D = nullptr;
		arrayTable_2D_copy = nullptr;
		usedCoordinates = nullptr;
	}
}
//Author: Demetrius E Johnson
//Purpose: receive and properly parse user input for search phrase so that we can apply algorithms that need clean, single spaced input
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::inputSearchVal(string val) {

	//default: word is assumed to be a valid input:
	validWord = true;

	//clear the strings so that whenever the function is called, no old data will affect the new value to input
	userSearchVal.clear();
	userSearchVal_noSpaces.clear();

	//set user search val to original user input, ignoring leading spaces and extra spaces between words:
	for (int i = 0; i < val.size(); i++) {

		//all leading spaces will be skipped until we hit a non ws char:
		if (val[i] != ' ') { 

			userSearchVal.push_back(tolower(val[i]));	//add char to userSearchVal, ensuring it is lowercase if it is alphabetical

			//if a whitespace is the next char (and not the last char) add it to the searchVal; additional spaces will be skipped when for loop starts again:
			if (((i + 1) < (val.size())) && ((i + 1) != (val.size() - 1))) { 
				
				if (val[i + 1] == ' ') { userSearchVal.push_back(val[i + 1]); }
			}
		} 
	}
	if (userSearchVal.size() != 0 && userSearchVal[userSearchVal.size() - 1] == ' ') { userSearchVal.pop_back(); } //there may be a single trailing ws char at the end of the final userSearchVal
	setNumWordsInSearchPhrase(userSearchVal);																	  //now call function to store number of words in the phrase -- this will be useful for the search algorithm

	//now, set no-space version of the user input to be used in backtrack algorithm for the word search, while also checking if all values are alphabetical and lowercase:
	for (int i = 0; i < userSearchVal.size(); i++) {
	
		//check to see if the value is alphabetical:
		if (isalpha(userSearchVal[i])) {

			//add lowercase value to no-space version of the search value
			userSearchVal_noSpaces.push_back(tolower(userSearchVal[i]));

		}
		else if (userSearchVal[i] != ' ') {//otherwise, value is not alphabetical or a space; we can simply return and exit function - no need to continue
		
			validWord = false; //set this to false; now calling the function that returns this value will properly tell if the newly added search phrase/word is valid
			return;
		}
	}
}
//Author: Demetrius E Johnson
//Purpose: use this in order to calculate how many words were entered by the user's phrase - this will be used for various functions
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::setNumWordsInSearchPhrase(string searchKey) {

	numWordsInPhrase = 0;	//reset words in phrase counter
	stringstream ss;
	string currentWord;
	ss << searchKey;		//output search key string into ss to be parsed
	while (ss.good()) {
	
		ss >> currentWord;		//input a word into currentWord -- ss will stop inputting characters into currentWord when a ws is reached)
		numWordsInPhrase++;		//word added; increment counter
	}
}
//Author: Demetrius E Johnson
//Purpose: set all directions at a given location in the table to true or false - this sets all or resets all directions that have been searched at a given position in the table
//Date Created: 8/19/21
//Date Modified: 8/19/21
void Table::setAllDir_searched(bool val) {

	//reset all values to false:
	lastWordFound_right = val;
	lastWordFound_down = val;
	lastWordFound_left = val;
	lastWordFound_up = val;
}
//Author: Demetrius E Johnson
//Purpose: search all directions relative to a given position in the table - uses recursion - this is a major part to the backtracking algorithm functioning of this program
//Date Created: 8/17/21
//Date Modified: 8/19/21
bool Table::search_allDir(int& x, int& y, int& z, int& wordCount, ofstream& of) {

	//use these ints in order to track original values for the current function call
	int x_original = x;
	int y_original = y;
	int z_original = z;
	int wordCount_original = wordCount;
	int vectorSize_original = foundCoord.size(); // track original size of vector so that when we need to backtrack, all added cooridantes are deleted

	//search right:
	bool wordFound = search_right(x, y, z, wordCount, of);

	if (wordFound == true && z == userSearchVal.size()) { //initial base case: last word in phrase was found right
	
		return true;
	}
	if (wordFound == true) { wordFound = search_allDir(x, y, z, wordCount, of); } //recursively restart search since word was found; need to search for next word
	if (wordFound == true && z == userSearchVal.size()) { //base case: last word in phrase was found right

		return true;
	}

	//otherwise search down:
	
	//we need to reset values relative to their original position for when the function was called, and delete the last word coordinates found since it was a dead end
	x = x_original;
	y = y_original;
	z = z_original;
	wordCount = wordCount_original;
	//need to delete the last coordinates added since at this point it means the last added word was not a valid path and we had to backtrack
	if (vectorSize_original == foundCoord.size() - 4) {//word was added - delete both cooridantes
		deleteLetterCoord(); 
		deleteLetterCoord();
	}
	else if (vectorSize_original == foundCoord.size() - 2) { deleteLetterCoord(); } //only the first letter of a word was added - delete only a single coordinate
	vectorSize_original = foundCoord.size(); //set new original size

	wordFound = search_down(x, y, z, wordCount, of);
	
	if (wordFound == true && z == userSearchVal.size()) { //base case: last word in phrase was found down

		return true;
	}
	if (wordFound == true) { wordFound = search_allDir(x, y, z, wordCount, of); } //recursively restart search since word was found; need to search for next word
	if (wordFound == true && z == userSearchVal.size()) { //base case: last word in phrase was found right

		return true;
	}

	//otherwise search left:
		
	//we need to reset values relative to their original position for when the function was called
	x = x_original;
	y = y_original;
	z = z_original;
	wordCount = wordCount_original;
	//need to delete the last coordinates added since at this point it means the last added word was not a valid path and we had to backtrack
	if (vectorSize_original == foundCoord.size() - 4) {//word was added - delete both cooridantes
		deleteLetterCoord();
		deleteLetterCoord();
	}
	else if (vectorSize_original == foundCoord.size() - 2) { deleteLetterCoord(); } //only the first letter of a word was added - delete only a single coordinate
	vectorSize_original = foundCoord.size(); //set new original size

	wordFound = search_left(x, y, z, wordCount, of);

	if (wordFound == true && z == userSearchVal.size()) { //base case: last word in phrase was found left

		return true;
	}
	if (wordFound == true) { wordFound = search_allDir(x, y, z, wordCount, of); } //recursively restart search since word was found; need to search for next word
	if (wordFound == true && z == userSearchVal.size()) { //base case: last word in phrase was found right

		return true;
	}

	//otherwise search up:
	
	//we need to reset values relative to their original position for when the function was called
	x = x_original;
	y = y_original;
	z = z_original;
	wordCount = wordCount_original;
	//need to delete the last coordinates added since at this point it means the last added word was not a valid path and we had to backtrack
	if (vectorSize_original == foundCoord.size() - 4) {//word was added - delete both cooridantes
		deleteLetterCoord();
		deleteLetterCoord();
	}
	else if (vectorSize_original == foundCoord.size() - 2) { deleteLetterCoord(); } //only the first letter of a word was added - delete only a single coordinate
	vectorSize_original = foundCoord.size(); //set new original size

	wordFound = search_up(x, y, z, wordCount, of);

	if (wordFound == true && z == userSearchVal.size()) { //base case: last word in phrase was found up

		return true;
	}
	if (wordFound == true) { wordFound = search_allDir(x, y, z, wordCount, of); } //recursively restart search since word was found; need to search for next word
	if (wordFound == true && z == userSearchVal.size()) { //final base case: last word in phrase was found right

		return true;
	}
		
	//otherwise word or phrase was not found:
//need to delete the last coordinates added since at this point it means the last added word was not a valid path and we had to backtrack
	if (vectorSize_original == foundCoord.size() - 4) {//word was added - delete both cooridantes
		deleteLetterCoord();
		deleteLetterCoord();
	}
	else if (vectorSize_original == foundCoord.size() - 2) { deleteLetterCoord(); } //only the first letter of a word was added - delete only a single coordinate
	vectorSize_original = foundCoord.size(); //set new original size

	cout << "search: all directions searched at (" << x << "," << y << ").\n";
	return false; // base case: dead end; phrase not found from the current (passed in) starting position in the table - return false
}

//Author: Demetrius E Johnson
//Purpose: For the below 4 functions: search a given location relative to the current position we are looking at in the table - this is a major part to the backtracking algorithm functioning of this program
//Date Created: 8/17/21
//Date Modified: 8/19/21
bool Table::search_right(int& x, int& y, int& z, int& wordCount, ofstream& of) {

	string currentWord;							//use to print current word if found
	
	//use the below variables to maintain original positions in the event that word is not found:
	int x_original = x;
	int y_original = y;
	int originalPos = z;

	//reset these variables so we correctly represent that no coordinates of the word have been added to coordinate vector
	addedFirstLetter = false;
	addedLastLetter = false;

	//z refers to position in the user phrase/string we are searching, current word refers to current word in the phrase we are searching
	// x and y are set according to table coordinates relative to the current element we are checking in the table

	cout << "search: currPos<right>: (" << x << "," << y + 1 << ").\n";
	of << "search: currPos<right>: (" << x << "," << y + 1 << ").\n";

	while (z < userSearchVal.size() && x < numRow && y + 1 < numCol && arrayTable_2D[x][y + 1] == userSearchVal[z]) {

		cout << "search: right: '" << userSearchVal[z] << "' found at (" << x  << "," << y + 1 << ").\n";
		of << "search: right: '" << userSearchVal[z] << "' found at (" << x  << "," << y + 1 << ").\n";
		
		//use the below if statements to add coordinate of start letter and last letter of each word:
		//add first letter:
		if (z - 1 >= 0 && userSearchVal[z - 1] == ' ') { 
			
			addCoord(x, y + 1); 
			addedFirstLetter = true;
		}
		//add last letter:
		if (z + 1 < userSearchVal.size() && userSearchVal[z + 1] == ' ') { 
			
			addCoord(x, y + 1); 
			addedLastLetter = true;
		}
		else if (z + 1 == userSearchVal.size()) { 
			addCoord(x, y + 1); 
			addedLastLetter = true;

		} //case: we have reached last letter of the entire phrase
		
		//use below statement to set true in the bool table that a given coordinate is now taken if it is not already:
		//case: coordinate already used; exit function
		if (usedCoordinates[x][y + 1] == true) { 
			
			//maintain original positions since word was not found:
			cout << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			of << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			x = x_original;
			y = y_original;
			z = originalPos; 

			return false; 
		} 
		//otherwise: coordinate now used in the search, set it to true so there are no potential overlap or loops later in search
		else { usedCoordinates[x][y + 1] = true; }


		y++;	//move right in the table (change column, maintain current row) for the next search
		z++;	//move to next char in search phrase to check against

		//case: we have reached the end of a word in the phrase:
		if (z < userSearchVal.size() && userSearchVal[z] == ' ') {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			wordCount++;	//move to next word to output from the phrase
			z++;			//skip the space - move to first letter in next word
			foundRight();	//set flags for which direction the last word was found
			return true;
		}
		//case: we have reached the end of the phrase - phrase has been found in the table:
		if (z == userSearchVal.size()) {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			foundRight(); //set flags for which direction the last word was found
			return true;
		}
	}

	//maintain original positions since word was not found:
	cout << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	of << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	x = x_original;
	y = y_original;
	z = originalPos;

	//

	return false;
}
bool Table::search_down(int& x, int& y, int& z, int& wordCount, ofstream& of) {

	string currentWord;								//use to print current word if found

	//use the below variables to maintain original positions in the event that word is not found:
	int x_original = x;
	int y_original = y;
	int originalPos = z;	

	//reset these variables so we correctly represent that no coordinates of the word have been added to coordinate vector
	addedFirstLetter = false;
	addedLastLetter = false;


	//z refers to position in the user phrase/string we are searching, current word refers to current word in the phrase we are searching
	// x and y are set according to table coordinates relative to the current element we are checking in the table

	cout << "search: currPos<down>: (" << x + 1 << "," << y << ").\n";
	of << "search: currPos<down>: (" << x + 1 << "," << y << ").\n";

	while (z < userSearchVal.size() && x + 1 < numRow && y < numCol && arrayTable_2D[x + 1][y] == userSearchVal[z]) {

		cout << "search: down: '" << userSearchVal[z] << "' found at (" << x + 1 << "," << y << ").\n";
		of << "search: down: '" << userSearchVal[z] << "' found at (" << x + 1 << "," << y << ").\n";

		//use the below if statements to add coordinate of start letter and last letter of each word:
		//add first letter:
		if (z - 1 >= 0 && userSearchVal[z - 1] == ' ') { 

			addCoord(x + 1, y);
			addedFirstLetter = true;
		}
		//add last letter:
		if (z + 1 < userSearchVal.size() && userSearchVal[z + 1] == ' ') { 

			addCoord(x + 1, y);
			addedLastLetter = true;
		}
		else if (z + 1 == userSearchVal.size()) { 
			
			addCoord(x + 1, y); 
			addedLastLetter = true;
		} //case: we have reached last letter of the entire phrase

		//use below statement to set true in the bool table that a given coordinate is now taken if it is not already:
		//case: coordinate already used; exit function
		if (usedCoordinates[x + 1][y] == true) {

			//maintain original positions since word was not found:
			cout << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			of << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			x = x_original;
			y = y_original;
			z = originalPos;

			return false;
		}
		//otherwise: coordinate now used in the search, set it to true so there are no potential overlap or loops later in search
		else { usedCoordinates[x + 1][y] = true; }

		x++;	//move down in the table (change row, maintain current column) for the next search
		z++;	//move to next char in search phrase to check against

		//case: we have reached the end of a word in the phrase:
		if (z < userSearchVal.size() && userSearchVal[z] == ' ') {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			wordCount++; //move to next word to output from the phrase
			z++;		 //skip the space - move to first letter in next word
			foundDown(); //set flags for which direction the last word was found
			return true;
		}
		//case: we have reached the end of the phrase - phrase has been found in the table:
		if (z == userSearchVal.size()) {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			foundDown(); //set flags for which direction the last word was found
			return true;
		}
	}

	//maintain original positions since word was not found:
	cout << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	of << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	x = x_original;
	y = y_original;
	z = originalPos; 

	return false;	
}
bool Table::search_left(int& x, int& y, int& z, int& wordCount, ofstream& of) {

	string currentWord;							//use to print current word if found
	
	//use the below variables to maintain original positions in the event that word is not found:
	int x_original = x;
	int y_original = y;
	int originalPos = z;

	//reset these variables so we correctly represent that no coordinates of the word have been added to coordinate vector
	addedFirstLetter = false;
	addedLastLetter = false;

	//z refers to position in the user phrase/string we are searching, current word refers to current word in the phrase we are searching
	// x and y are set according to table coordinates relative to the current element we are checking in the table

	cout << "search: currPos<left>: (" << x << "," << y - 1 << ").\n";
	of << "search: currPos<left>: (" << x << "," << y - 1 << ").\n";

	while (z < userSearchVal.size() && x < numRow && y - 1 < numCol && y - 1 >= 0 && arrayTable_2D[x][y - 1] == userSearchVal[z]) {

		cout << "search: left: '" << userSearchVal[z] << "' found at (" << x << "," << y - 1 << ").\n";
		of << "search: left: '" << userSearchVal[z] << "' found at (" << x << "," << y - 1 << ").\n";

		//use the below if statements to add coordinate of start letter and last letter of each word:
		//add first letter:
		if (z - 1 >= 0 && userSearchVal[z - 1] == ' ') { 
			
			addCoord(x, y - 1);
			addedFirstLetter = true;
		}
		//add last letter:
		if (z + 1 < userSearchVal.size() && userSearchVal[z + 1] == ' ') { 
			
			addCoord(x, y - 1);
			addedLastLetter = true;
		}
		else if (z + 1 == userSearchVal.size()) { 
			
			addCoord(x, y - 1);
			addedLastLetter = true;
		} //case: we have reached last letter of the entire phrase

		//use below statement to set true in the bool table that a given coordinate is now taken if it is not already:
		//case: coordinate already used; exit function
		if (usedCoordinates[x][y - 1] == true) {

			//maintain original positions since word was not found:
			cout << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			of << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			x = x_original;
			y = y_original;
			z = originalPos;

			return false;
		}
		//otherwise: coordinate now used in the search, set it to true so there are no potential overlap or loops later in search
		else { usedCoordinates[x][y - 1] = true; }

		y--;	//move left in the table (change column, maintain current row)
		z++;	//move to next char in search phrase to check against

		//case: we have reached the end of a word in the phrase:
		if (z < userSearchVal.size() && userSearchVal[z] == ' ') {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			wordCount++; //move to next word to output from the phrase
			z++;		 //skip the space - move to first letter in next word
			foundLeft();//set flags for which direction the last word was found
			return true;
		}
		//case: we have reached the end of the phrase - phrase has been found in the table:
		if (z == userSearchVal.size()) {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			foundLeft();//set flags for which direction the last word was found
			return true;
		}
	}

	//maintain original positions since word was not found:
	cout << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	of << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	x = x_original;
	y = y_original;
	z = originalPos;

	return false;
}
bool Table::search_up(int& x, int& y, int& z, int& wordCount, ofstream& of) {

	string currentWord;									//use to print current word if found
	
		//use the below variables to maintain original positions in the event that word is not found:
	int x_original = x;
	int y_original = y;
	int originalPos = z;

	//reset these variables so we correctly represent that no coordinates of the word have been added to coordinate vector
	addedFirstLetter = false;
	addedLastLetter = false;


	//z refers to position in the user phrase/string we are searching, current word refers to current word in the phrase we are searching
	// x and y are set according to table coordinates relative to the current element we are checking in the table

	cout << "search: currPos<up>: (" << x - 1 << "," << y << ").\n";
	of << "search: currPos<up>: (" << x - 1 << "," << y << ").\n";

	while (z < userSearchVal.size() && x - 1 < numRow && x - 1 >= 0 && y < numCol && arrayTable_2D[x - 1][y] == userSearchVal[z]) {

		cout << "search: up: '" << userSearchVal[z] << "' found at (" << x - 1  << "," << y  << ").\n";
		of << "search: up: '" << userSearchVal[z] << "' found at (" << x - 1  << "," << y << ").\n";

		//use the below if statements to add coordinate of start letter and last letter of each word:
		//add first letter:
		if (z - 1 >= 0 && userSearchVal[z - 1] == ' ') {
			
			addCoord(x - 1, y); 
			addedFirstLetter = true;
		}
		//add last letter:
		if (z + 1 < userSearchVal.size() && userSearchVal[z + 1] == ' ') { 
			
			addCoord(x - 1, y);
			addedLastLetter = true;
		}
		else if (z + 1 == userSearchVal.size()) { 
			
			addCoord(x - 1, y);
			addedLastLetter = true;
		} //case: we have reached last letter of the entire phrase

		//use below statement to set true in the bool table that a given coordinate is now taken if it is not already:
		//case: coordinate already used; exit function
		if (usedCoordinates[x - 1][y] == true) {

			//maintain original positions since word was not found:
			cout << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			of << "search: overlap...backtracking: (" << x_original << "," << y_original << ").\n";
			x = x_original;
			y = y_original;
			z = originalPos;

			return false;
		}
		//otherwise: coordinate now used in the search, set it to true so there are no potential overlap or loops later in search
		else { usedCoordinates[x - 1][y] = true; }

		x--;	//move up in the table (change row, maintain current column)
		z++;	//move to next char in search phrase to check against

		//case: we have reached the end of a word in the phrase:
		if (z < userSearchVal.size() && userSearchVal[z] == ' ') {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			wordCount++; //move to next word to output from the phrase
			z++;		 //skip the space - move to first letter in next word
			foundUp(); //set flags for which direction the last word was found
			return true;
		}
		//case: we have reached the end of the phrase - phrase has been found in the table:
		if (z == userSearchVal.size()) {

			currentWord = getWordFromPhrase(wordCount);
			cout << "search: <" << currentWord << "> found.\n";
			of << "search: <" << currentWord << "> found.\n";
			foundUp(); //set flags for which direction the last word was found
			return true;
		}
	}

	//maintain original positions since word was not found:
	cout << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	of << "search: ...backtracking: (" << x_original << "," << y_original << ").\n";
	x = x_original;
	y = y_original;
	z = originalPos;

	return false;
}

//Author: Demetrius E Johnson
//Purpose: create a program that uses the backtracking algorithm to perform a word search on a character grid
//Date Created: 8/18/21
//Date Modified: 8/18/21
void Table::foundRight(void) {

	lastWordFound_right = true;
	lastWordFound_down = false;
	lastWordFound_left = false;
	lastWordFound_up = false;

}
void Table::foundDown(void) {

	lastWordFound_right = false;
	lastWordFound_down = true;
	lastWordFound_left = false;
	lastWordFound_up = false;
}
void Table::foundLeft(void) {

	lastWordFound_right = false;
	lastWordFound_down = false;
	lastWordFound_left = true;
	lastWordFound_up = false;

}
void Table::foundUp(void) {

	lastWordFound_right = false;
	lastWordFound_down = false;
	lastWordFound_left = false;
	lastWordFound_up = true;
}

//below are simple return functions that return user phrase, numRow and numCol in table, num words in search phrase, and reference to the 2d array table
string Table::getSearchVal(void) { return userSearchVal; }
string Table::getSearchVal_noSpaces(void) { return userSearchVal_noSpaces; }
int Table::getNumRow(void) { return numRow; }
int Table::getNumCol(void) { return numCol; }
int Table::getNumWordsInSearchPhrase(void) { return numWordsInPhrase; }
char** Table::getTableReference(void) { return arrayTable_2D; }

//Author: Demetrius E Johnson
//Purpose: returns a word from the phrase relative to the number passed in: for example, if wordNum == 2, then the second word in the phrase will be returned (if possible)
//Date Created: 8/12/21
//Date Modified: 8/19/21
string Table::getWordFromPhrase(int wordNum) {

	if (wordNum > numWordsInPhrase || wordNum <= 0) { return ""; } //invalid word number; return null string
	else {
		stringstream ss;
		string word;
		ss << userSearchVal;	//output the user phrase to ss to be parsed word by word
		for (int i = 0; i < wordNum; i++) { ss >> word; }
		return word;	//word (number position of a word in the phrase) is valid.
	}
}
bool Table::is_validWord(void) { return validWord; }
//Author: Demetrius E Johnson
//Purpose: prints the table (contents of 2d array storing the table) passed in from the input file and built from the buildTable function
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::printTable(ofstream& of) {

	for (int i = 0; i < numRow; i++) { //move to each row
	
		for (int j = 0; j < numCol; j++) { //print each row
		
			cout << arrayTable_2D[i][j] << " ";
			of << arrayTable_2D[i][j] << " ";
		}

		//move to next row:
		cout << endl;
		of << endl;
	}
}
//Author: Demetrius E Johnson
//Purpose: prints the character table, but capitalizing each word from the found user phrase in the table
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::printPhraseFoundTable(ofstream& of) {

	cout << "Phrase Found Table for: '" << userSearchVal << "':\n\n";
	of << "Phrase Found Table for: '" << userSearchVal << "':\n\n";


	//remember from foundCoord vector that every four positions stores the xy xy start and end letter of a word in the phrase
	//also, x1 and x2 are at i and i+2 while y1 and y2 are at i+1 and i+3
	//remember getWordFromPhrase function takes only values > 0; the integer it takes refers to the first, second, third....etc... word from the user search value phrase
	//also remember that every four positions in the vector, starting from the front, coorespond to the xy xy coordinates of start letter and last letter of each word

	//use the below variables to acquire the start coordinates of each word, and to calculate the differences in their positions
	int x1, y1, x2, y2;
	int xDiff, yDiff;

	//first, capitalize all words at the proper locations in the table, each loop will capitalize a word in the table:
	//remember every 4 elements in the foundCoord vector represnts 2 coordinates (first and last letter of a word)
	for (int i = 0; i < foundCoord.size(); i += 4) {
	
		x1 = foundCoord[i];
		x2 = foundCoord[i + 2];

		y1 = foundCoord[i + 1];
		y2 = foundCoord[i + 3];

		xDiff = x2 - x1;
		yDiff = y2 - y1;

		//case: go from x1 to x2, y (column) stays the same - this capitalizes a word found from up to down
		if (xDiff >= 0 && yDiff == 0) {
		
			for (int j = x1; j <= x2; j++) {

				arrayTable_2D_copy[j][y1] = toupper(arrayTable_2D_copy[j][y1]);

			}
		}
		//case: go from y1 to y2, x (row) stays the same - this capitalizes a word found from left to right
		if (xDiff == 0 && yDiff >= 0) {

			for (int j = y1; j <= y2; j++) {

				arrayTable_2D_copy[x1][j] = toupper(arrayTable_2D_copy[x1][j]);

			}
		}
		//case: go from x2 to x1, y (column) stays the same - this capitalizes a word found from down to up
		if (xDiff <= 0 && yDiff == 0) {

			for (int j = x2; j <= x1; j++) {

				arrayTable_2D_copy[j][y1] = toupper(arrayTable_2D_copy[j][y1]);

			}
		}
		//case: go from y2 to y1, x (row) stays the same - this capitalizes a word found from right to left
		if (xDiff == 0 && yDiff <= 0) {

			for (int j = y2; j <= y1; j++) {

				arrayTable_2D_copy[x1][j] = toupper(arrayTable_2D_copy[x1][j]);

			}
		}
	}

	//now print table:

	for (int i = 0; i < numRow; i++) { //move to each row

		for (int j = 0; j < numCol; j++) { //print each row

			cout << arrayTable_2D_copy[i][j] << " ";
			of << arrayTable_2D_copy[i][j] << " ";
		}

		//move to next row:
		cout << endl;
		of << endl;
	}

	resetPhraseTable(); //now reset table for the next function call
	
	cout << endl;
	of << endl;
}
//Author: Demetrius E Johnson
//Purpose: prints the coordinates of the first and last letter of each word from the user phrase that was found in the table
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::printPhraseCoord(ofstream& of) {

	cout << "Phrase Coordinates for: '" << userSearchVal << "'.\n";
	of << "Phrase Coordinates for: '" << userSearchVal << "'.\n";

	//remember getWordFromPhrase function takes only values > 0; the integer it takes refers to the first, second, third....etc... word from the user search value phrase
	//also remember that every four positions in the vector, starting from the front, coorespond to the xy xy coordinates of start letter and last letter of each word

	int nextCoord = 0;

	for (int i = 0; i < numWordsInPhrase; i++) {
	
		cout << "word coordinate: " << getWordFromPhrase(i + 1) << ": <" << foundCoord[nextCoord] << "," << foundCoord[nextCoord + 1] << ">" << " to <" << foundCoord[nextCoord + 2] << "," << foundCoord[nextCoord + 3] << ">.\n";
		of << "word coordinate: " << getWordFromPhrase(i + 1) << ": <" << foundCoord[nextCoord] << "," << foundCoord[nextCoord + 1] << ">" << " to <" << foundCoord[nextCoord + 2] << "," << foundCoord[nextCoord + 3] << ">.\n";
		nextCoord += 4; //move to start of next coordinates in the phrase vector; every 4 elements represents a set of coordinates
	}
	cout << "End coordinates.\n";
	of << "End coordinates.\n";
}
//Author: Demetrius E Johnson
//Purpose: part of the backtracking algorithm, this function initiates the word search, searching at every location in the table using backtracking when necessary
//Date Created: 8/12/21
//Date Modified: 8/19/21
bool Table::wordSearch(ofstream& of) {
	
	//search order: right, down, left, up.
	
	int currentWord = 1; //track current word in the phrase that we are searching
	int phrasePos = 0;	 //track the position (char) in the phrase we are searching
	int x;				//track current row position in table relative to a word found from the phrase
	int y;				//track current column position in the table relative to a word found from the phrase
	foundCoord.clear();	//make sure coordinate vector is clear before starting search

	cout << "Start search at (0,0).\n";
	of << "Start search at (0,0).\n";

	//use below loops to begin going to every location in the table (remeber boolean && and || are evaluated left to right):
	//start search at every location, and if start location is a match, then proceed with back tracking algorithm search for user phrase
	for (int i = 0; i < numRow; i++) {

		x = i;	//set x initially relative to next table search position

		for (int j = 0; j < numCol; j++) {

			y = j;						//set y initially relative to next table search position
			currentWord = 1;			//reset current word to 1 (the first word to be searched) 
			phrasePos = 0;				//reset position searched from user phrase back to start
			foundCoord.clear();			//make sure coordinate vector is clear before starting search from a new position
			resetBoolTable();			//make sure bool table is reset before starting search at a new position

			//if current position is not in the table we can output a message and move to next position in the table to search; otherwise we begin search for the rest of phrase:
			if (userSearchVal[phrasePos] != arrayTable_2D[x][y]) { 
				
				cout << "search: currPos: (" << x << "," << y << "): '" << userSearchVal[phrasePos] << "' not found.\n";
				of << "search: currPos: (" << x << "," << y << "): '" << userSearchVal[phrasePos] << "' not found.\n";
			}
			else {

				cout << "search: currPos: (" << x << "," << y << "): '" << arrayTable_2D[x][y] << "' found.\n";
				of << "search: currPos: (" << x << "," << y << "): '" << arrayTable_2D[x][y] << "' found.\n";
				addCoord(x, y);					//add coordinate of first letter of potentially found word from the phrase
				usedCoordinates[x][y] = true; //coordinate used in the search, set it to true so there are no potential overlap or loops later in search
				phrasePos++;

				//case: we have reached the end of the phrase - phrase has been found in the table; phrase was just a single character:
				if (userSearchVal.size() == 1) {

					cout << "search: <" << getWordFromPhrase(currentWord) << "> found.\n";
					of << "search: <" << getWordFromPhrase(currentWord) << "> found.\n";
					addCoord(x, y);					//add coordinate of last letter of found word from the phrase
					userPhraseFound = true;
					cout << "search: SUCCESS!: '" << userSearchVal << "' found!\n";
					of << "search: SUCCESS!: '" << userSearchVal << "' found!\n";
					return true;
				}
				//case: current word in the phrase is only a single character; need to skip the space and move to next word to initiate remaining search:
				if (userSearchVal[phrasePos] == ' ') {

					cout << "search: <" << getWordFromPhrase(currentWord) << "> found.\n";
					of << "search: <" << getWordFromPhrase(currentWord) << "> found.\n";
					addCoord(x, y);					//add coordinate of last letter of found word from the phrase
					currentWord++;					//move to next word to output from the phrase
					phrasePos++;					//skip the space - move to first letter in next word
					foundRight();					//set flags for which direction the last word was found
				}

				//now continue to search for the current word and the rest of the phrase from the current position:
				
				userPhraseFound = search_allDir(x, y, phrasePos, currentWord, of);
				if (userPhraseFound == true) {
					
					cout << "search: SUCCESS!: '" << userSearchVal << "' found!\n";
					of << "search: SUCCESS!: '" << userSearchVal << "' found!\n";
					return true; 
				}
			}
		}//otherwise loop iteration will end and j will increment and move to next character in the row to start search again
	}//otherwise loop iteration will end and i will increment and move to the next row in the table to begin starting searches from those elements

	cout << "search: FAILED!: '" << userSearchVal << "' not found!\n";
	of << "search: FAILED!: '" << userSearchVal << "' not found!\n";

	userPhraseFound = false;
	return false; //if we reach here, all possible paths (state space tree, but cut off from efficient backtracking) checked; word/phrase is not in table
}
//Author: Demetrius E Johnson
//Purpose: resets the phrase found table - this table needs to be reset to default all-lowercase table after the table has been printed so that it can be re-capitalized based on the next found user phrase
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::resetPhraseTable(void) {

	for (int i = 0; i < numRow; i++) { //move to each row

		for (int j = 0; j < numCol; j++) { //reset row

			arrayTable_2D_copy[i][j] = arrayTable_2D[i][j];
		}
	}
}
//Author: Demetrius E Johnson
//Purpose: reset the bool table - this will reset the table whenever backtracking occurs so that all positions in the table will be cleared and available for use if search algorithm finds a match during another search position
//Date Created: 8/12/21
//Date Modified: 8/19/21
void Table::resetBoolTable(void) {

	for (int i = 0; i < numRow; i++) { //move to each row

		for (int j = 0; j < numCol; j++) { //reset row

			usedCoordinates[i][j] = false;
		}
	}
}
//Author: Demetrius E Johnson
//Purpose: call the destroy function so that when a Table object is destroyed, all dynamically allocated memory is deallocated if it has not been already
//Date Created: 8/12/21
//Date Modified: 8/19/21
Table::~Table() {

	//call destroy table function, which will delete all dynamically allocated memory from the 2D array:
	destroyTable();

}