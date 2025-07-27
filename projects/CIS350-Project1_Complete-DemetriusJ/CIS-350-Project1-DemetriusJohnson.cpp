// CIS-350-Project1-DemetriusJohnson.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/** program1Test.cpp
 *
 * Author: Demetrius Johnson
 * Date: 5-25-21
 * Modification Date: 5-27-21
 * Description: A Test Program for Program 1; this program implements the use of a sorted array using dynamically allocated memory
 */

#include <iostream>
#include <string>
#include "SortedArray.h"

using namespace std;


const int N = 1787;
const int M = 2357;

int main()
{
	cout << "-----WELCOME TO THE SORTED ARRAY PROGRAM -- BY DEMETRIUS JOHNSON - CIS-350 WITH DR. JINHUA GUO---\n\n\n";

	SortedArray<int> q;
	SortedArray<string> animals;

	string str1[] = { "zebra", "tiger", "lion", "dog", "monkey", "snake", "turkey",
		"cat", "duck", "alligator" };

	int minVal1, maxVal1;
	int i, j;

	int index;

	for (i = N; i != 0; i = (i + N) % M)
		q.insert(i);

	cout << "Completed first round of insertions" << endl;

	for (j = 1; !q.empty(); j++)
	{
		minVal1 = q.findMin();
		q.deleteMin();
		if (minVal1 != j)
			cout << "OOPS!!!!!" << minVal1 << " " << j << endl;

		maxVal1 = q.findMax();
		q.deleteMax();
		if (maxVal1 != M - j)
			cout << "OOPS!!!!!" << maxVal1 << " " << (M - j) << endl;
	}

	cout << "Completed first round of deletions" << endl;

	for (i = N; i != 0; i = (i + N) % M)
		q.insert(i);

	cout << "Completed second round of insertions" << endl;

	for (i = 0; i < M - 5; i += 2)
	{
		q.deleteMin(); q.deleteMax();
	}

	while (!q.empty()) {
		cout << q.findMin() << endl;
		cout << q.findMax() << endl;
		q.deleteMin();
		q.deleteMax();
	}

	cout << "Completed the Integer Array test" << endl << endl;

	for (i = 0; i < 10; i++) {
		animals.insert(str1[i]);
	}

	cout << "Animal list: " << endl;
	animals.print(cout, '\n');
	cout << endl << endl;

	index = animals.binarySearch("lion");
	if (index >= 0) {
		cout << animals[index] << " is found in the list." << endl;
	}
	else {
		cout << "OOPS!!!!!" << "Lion should be in the list." << endl;
	}

	index = animals.binarySearch("bear");
	if (index >= 0) {
		cout << "OOPS!!!!!" << "Bear should not be in the list." << endl;
	}
	else {
		cout << "Bear is not found in the list." << endl;
	}

	SortedArray<string> animalsCopy(animals);

	if (animalsCopy.equals(animals)) {
		cout << "Correct! " << "The two animal lists are the same." << endl;
	}
	else {
		cout << "OOPS!!!!!" << "They should be equal" << endl;
	}

	animalsCopy.deleteMin();
	animalsCopy.deleteMax();

	cout << endl << "Animal list after the deletion: " << endl;
	animalsCopy.print(cout, '\n');
	cout << endl << endl;

	cout << "Completed the String Array test" << endl << endl;

	///////////////////////////////////////////////////////////////
	//ADDITIONAL TESTS THAT I DEVELOPED TO TEST ALL OF MY FUNCTIONS:
	//////////////////////////////////////////////////////////////

	cout << "Additional Tests: " << endl << endl;


	cout << animals[3] << endl; //should output position 3 (the 4th element == duck) from animals 
	cout << "Size of animal array: " << animals.size() << endl;
	cout << "Capacity of animal array: " << animals.capacity() << endl;
	cout << "Binary search for 'duck': " << animals.binarySearch("duck") << endl << endl;//should output 3, since 'duck' is at element position 3

	//should do nothing since capacity is greater than 3....size should still be maintained
	animals.reserve(3); 
	cout << "Size of animal array: " << animals.size() << endl;
	cout << "Capacity of animal array: " << animals.capacity() << endl << endl;

	//should increase capacity since capacity < reserve request...while still maintaining size
	animals.reserve(44);
	cout << "Size of animal array: " << animals.size() << endl;
	cout << "Capacity of animal array: " << animals.capacity() << endl << endl;


	//test if insert function will properly resize the array if capacity needs to be increased to insert an element
		//size is 16; after adding 74 elements, capacity should be 89 based on insertion/reserve algorithm
		//algorithm is set so that when capacity == size, capacity is doubled (and +1 added); in doing this 
		//new memory (and re-input of all elements) won't have to keep being allocated every time new elements need to be inserted 
	string teststring;
	for (int w = 48; w < 122; w++) { //start at asii char set; decimal 48 == '0'
		teststring = w;
		animals.insert(teststring);
	}
	cout << "Size of animal array: " << animals.size() << endl;
	cout << "Capacity of animal array : " << animals.capacity() << endl << endl;

	//all elements should be removed (size = 0) ....capacity should be maintained 
	animals.clear(); 
	cout << "Size of animal array: " << animals.size() << endl;
	cout << "Capacity of animal array: " << animals.capacity() << endl;

	


	cout << "\n\n~The program has finished execution with all tests ran successfully. Exiting...\n\n";
	cout << system("pause") << endl;

	return 0;
}

