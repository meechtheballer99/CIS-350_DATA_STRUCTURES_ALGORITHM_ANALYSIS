// CIS-350-Project2-DemetriusJohnson.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/** program2Test
 *
 * Author: Demetrius Johnson
 * Date: 6-8-21
 * Modification Date: 6-14-21
 * Description: A Test Program for Program 2; this program implements the use of a hash table to help with the functionality of a spell-checker
 */


#include <iostream>
#include "QuadraticProbing.h"
#include <fstream>
#include <sstream>
using namespace std;

//below I make notes to myself to remember how parametized main works:
/*

//note: the main function always has its first command line argument as the programs name --> //this is because the operating system calls many MAIN() functions, 
    //and the specific main function (the main function is overloaded with numerous function versions!) called depends on the name provided in the first argument parameter.
    //This is also why MAIN is always the entry point of any program; it is because the Operating System calls the MAIN function (and often expects an int return val when program completes execution)
    //Also note: int main(int argc, char *argv[]) is the same thing as --> int main(int argc, char** argv); 
    //argc (argument counter) = number of arguments provided (in the pointer array argv);  //argc will always be = 1 for file name
    //argv (argument "vector") = a double pointer --> points to an array of char pointers (c-style strings) //argv[0] will always be the file name char array
    //remember std::cout overloads the << operator so that cout << char pointer will output the char or the array of char (string) associated to the address
    //so that: char* c = 'h'; cout << c --> output will be 'h' and not the address that the pointer points to like normal pointers will behave.
    //Now I can use the argv array and reference it in my program to open or close input or output files, or to use atoi() function to parse a number out of a char array.
 */


//FUNCTION DECLARATIONS:
void clear_nonAlpha(string& word);
void spellChecker(const string& word, const int& lineNumber, const HashTable<string>& dictionary);


//MAIN FUNCTION:

//COMMAND LINE ARGUMENTS TO USE: ( NUMBER OF WORDS IN THE DICTIONARY, DICTIONARY FILE NAME (USED FOR INPUT), TEXT FILE NAME (USED FOR INPUT, SPELL-CHECKS FILES CONTENTS) )
int main(int argc, char **argv ) 
{
 
   
    
    //by default, argv[0] == program file name
    int numWords = atoi(argv[1]);       //get the string with numerical characters and convert to int for number of words in dictionary
    ifstream ifs_Dictionary(argv[2]);   //use this input file stream to open and input from dictionary
    ifstream ifs_spellCheck(argv[3]);   //use this input file stream to open and input from file to spell check
    string currentWord;                 //get the current line from the dictionary (which contains one word per line)
    HashTable<string> dictionaryHashTable(2 * numWords);  //store dictionary here; starting table size at 2 * length of dictionary words

    if (!ifs_Dictionary.is_open() || !ifs_spellCheck.is_open()) {
    
        cout << "error: ~one or more files not opened successfully.";
        return 1;
    }

            //read dictionary into dictionary hash table:
    for (int i = 0; i < numWords; i++) {

        getline(ifs_Dictionary, currentWord);       //get the current line (word) from dictionary file and store it in currentWord string
        dictionaryHashTable.insert(currentWord);    //insert new word into hash table
        currentWord.clear();                        //clear the string for new entry

    }

    //output statistics:

    cerr << "Number of words " << dictionaryHashTable.numActiveBuckets()
        << ", Table size " << dictionaryHashTable.hashTableSize()
        << ", Load factor " << dictionaryHashTable.getLoadFactor()
        << "\nCollisions " << dictionaryHashTable.total_InsertionCollisions()
        << ", Average chain length " << dictionaryHashTable.avg_InsertionCollisions()
        << ", Longest chain length " << dictionaryHashTable.longest_collision_chain() << endl << endl;


    int lineNum = 1;            //track line number inside of spellcheck file
    string currentLine;         //store each line so that they can be parsed word by word from here
    stringstream parsingStream; //use this stream to parse the current line

        //spell check all words in text file:
    while(ifs_spellCheck.good()) {

        getline(ifs_spellCheck, currentLine);           //get the current line from spell check file and store it in currentLine string

        parsingStream << currentLine;                   //place current line into the stream for parsing//note: leading space char discarded until valid char, stops at next space

        while (parsingStream.good()) {

            parsingStream >> currentWord;                            //output each word in stream into currentWord to spell check it agaisnt dictionary
            clear_nonAlpha(currentWord);                             //delete any non alphabetical chararacters; this will get rid of punctation (, . " ')
            spellChecker(currentWord, lineNum, dictionaryHashTable); //spell check and output suggestions for corrections if necessary
            currentWord.clear();                                     //clear string for a new entry
        }
        parsingStream.clear();                          //bad bit set; need to reset the stream so it can be used again in the next loop
        lineNum++;                                      //next loop will spell check next line; thus we increment lineNum
        currentLine.clear();                            //clear the string for new entry
    }


        //close any opened files
    ifs_Dictionary.close();
    ifs_spellCheck.close(); 
    cout << "\n\n~program has finished execution...\n";
    return 0;
}


//FUNCTION DEFINITIONS:

void clear_nonAlpha(string& word) {

    //erase all non char values
    for (int i = 0; i < word.size(); i++) {
        if (!isalpha(word.at(i))) {
        
            word.erase(i, 1);
            i--; //after removal, all element positions will be shifted down; thus the next element that needs to be checked will be at the current i; so we do i-- to negate the i++
        }
    }
}

void spellChecker(const string& word, const int& lineNumber, const HashTable<string>& dictionary) {

    if (dictionary.contains(word) || word.size() == 0) { return; }  //if dictionary does contain the word or if word is NULL, then return, there is nothing else to do
    
        //now check the word when all letters are lowercase

    static string lowercaseOnly;                                  //use static since this function will be repeatedly used very often; no need to keep deallocating and reallocating
    lowercaseOnly = word;

    for (int i = 0; i < lowercaseOnly.size(); i++){
        lowercaseOnly.at(i) = tolower(lowercaseOnly.at(i)); }

    if (dictionary.contains(lowercaseOnly)) { return; } //now check dictionary again: if dictionary does contain the word, then return, there is nothing else to do
    lowercaseOnly.clear();                              //clear this for the next function use

       //////////////////////////////////////////////////////////////////////////////
      //now the following statements will execute to generate suggestion corrections:
     //////////////////////////////////////////////////////////////////////////////

    static string All_Suggestions;                  //store all suggestions generated in this string
    static string suggestion;                       //store the current suggestion
    static string charToString;                     //use this in order to take advantage of the string.insert() function

    //*1) Construct every string that can be made by deleting one letter from the word. (n possibilities, where n is the length of the word):
    for (int i = 0; i < word.size(); i++) {
       
        suggestion = word;
        suggestion.erase(i, 1);
        if (dictionary.contains(suggestion)) {
           
            All_Suggestions.append(suggestion); //add the new suggestion to the All_Suggestions string
            All_Suggestions.append(1,' ');      //add a space so that next suggestion can have a space of separation
        }
    }
    

    //*2) Construct every string that can be made by inserting any letter of the alphabet at any position in the string. (26 * (n + 1) possibilities):
    for (int i = 0; i < word.size(); i++) {
    
        suggestion = word; //initial suggestion to be modified
       

        //ASCII character set: lower case char begins at decimal 97 and ends after decimal 122 (a - z == 97 - 122)
        for (int j = 97; j < 123; j++) {

            charToString = j;
            suggestion.insert(i, charToString);                  //assign char at position i for the current loop j

            if (dictionary.contains(suggestion)) {

                All_Suggestions.append(suggestion);  //add the new suggestion to the All_Suggestions string
                All_Suggestions.append(1, ' ');      //add a space so that next suggestion can have a space of separation
            }

            suggestion = word; //reset suggestion back to original word since a char was inserted

        }
    
    }
        //now handle case where we need to insert a char at the end of the word:
    suggestion = word;                          //ensure suggestion is reset 
    for (int j = 97; j < 123; j++) {

        suggestion.push_back(j);               //insert at the end

        if (dictionary.contains(suggestion)) {

            All_Suggestions.append(suggestion);  //add the new suggestion to the All_Suggestions string
            All_Suggestions.append(1, ' ');      //add a space so that next suggestion can have a space of separation
        }
        suggestion.pop_back();                   //reset word by deleting appended char
    }

    //*3) Construct every string that can be made by swapping two neighboring characters in the string. (n - 1 possibilities)
    for (int i = 0; i < word.size() - 1; i++) {
       
        suggestion = word;             //reinitialize suggestion every iteration with original word so that adjustments can be made to it for a new suggestion
        char nextChar;
        nextChar = word.at(i + 1);     //let char store the next element

            //perform swap:
        suggestion.at(i + 1) = suggestion.at(i);    //next element = current element
        suggestion.at(i) = nextChar;                //current element = next element (which was saved in nextChar)
        if (dictionary.contains(suggestion)) {

            All_Suggestions.append(suggestion);  //add the new suggestion to the All_Suggestions string
            All_Suggestions.append(1, ' ');      //add a space so that next suggestion can have a space of separation
        }
    }

    //*4) Construct every string that can be made by replacing each letter in the word with some letter of the alphabet. 
    //(26 * n possibilities(including the original word n times, which is probably easier than avoiding it)):
    for (int i = 0; i < word.size(); i++) {

        suggestion = word;

        //ASCII character set: lower case char begins at decimal 97 and ends after decimal 122 (a - z == 97 - 122)
        for (int j = 97; j < 123; j++) {

            suggestion.at(i) = j;                    //assign char at position i for the current loop j

            if (dictionary.contains(suggestion)) {

                All_Suggestions.append(suggestion);  //add the new suggestion to the All_Suggestions string
                All_Suggestions.append(1, ' ');      //add a space so that next suggestion can have a space of separation
            }

        }
    }

        //FINALLY: output suggestions if there are any:
    cout << word << "(" << lineNumber << "): " << All_Suggestions << endl;
    
    charToString.clear();       //clear this string for next function use
    suggestion.clear();         //clear this string for next function use
    All_Suggestions.clear();    //clear this string for next function use
}


//BELOW IS A SIMPLY MAIN USED FOR TESTING USING AN INT HASH TABLE

/*

   HashTable<int> H;

    const int NUMS = 4000;
    const int GAP  =   37;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
        H.insert( i );
    for( i = 1; i < NUMS; i += 2 )
        H.remove( i );

    for( i = 2; i < NUMS; i +=2 )
        if( !H.contains( i ) )
            cout << "Contains fails " << i << endl;

    for( i = 1; i < NUMS; i += 2 )
    {
        if( H.contains( i ) )
            cout << "OOPS!!! " <<  i << endl;
    }

    cerr << "Number of words: " << H.numActiveBuckets() << "\nTable size " << H.hashTableSize() << "\nLoad factor: " << H.getLoadFactor()
         << "\nCollisions: " << H.total_InsertionCollisions() << "\nAverage chain length: " << H.avg_InsertionCollisions() << "\nLongest chain length: " << H.longest_collision_chain();

    return 0;

*/