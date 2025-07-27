/*
	SortedArray.cpp skeleton
*/
/** 
 *
 * Author: Demetrius Johnson
 * Date: 5-25-21
 * Modification Date: 5-27-21
 * Description: A Test Program for Program 1; this program implements the use of a sorted array using dynamically allocated memory
 */

#ifndef SORTEDARRAY_CPP
#define SORTEDARRAY_CPP

#include "SortedArray.h"


// Default constructor
template <typename Object>
SortedArray<Object>::SortedArray()
{
	//default capacity is 16

	objects = new Object[SPARE_CAPACITY];
	theSize = 0;
	theCapacity = SPARE_CAPACITY;
	return; //I know this return isn't necessary, it's just my style so I know I completed the function
}

// Copy constructor
template <typename Object>
SortedArray<Object>::SortedArray(const SortedArray<Object> &from)
{
	this->objects = new Object[from.theCapacity];
	this->theSize = from.theSize;
	this->theCapacity = from.theCapacity;

	//deep copy:
	for (int i = 0; i < theSize; i++) {

		this->objects[i] = from.objects[i];

	}

	return;
}

// destructor
template <typename Object>
SortedArray<Object>::~SortedArray()
{

	delete[] objects;
	cout << "~destructor called\n";
	return;

}

// overloaded assignment operator
template <typename Object>
const SortedArray<Object> & SortedArray<Object>::operator= (const SortedArray<Object> &from)
{
	delete[] this->objects; //delete old array
	this->objects = new Object[from.theCapacity]; //allocate new array of the object-to-copy size
	this->theSize = from.theSize;
	this->theCapacity = from.theCapacity;

	//deep copy:
	for (int i = 0; i < theSize; i++) {

		this->objects[i] = from.objects[i];

	}

	return *this; //return a constant reference to this object
}

// Accessor Operator []
template <typename Object>
const Object & SortedArray<Object>::operator[](int idx) const
{
	return this->objects[idx]; //return a constant reference (another alias/implicit pointer) to the idx'th element of the array
}

// check if two sorted arrays are the same
template <typename Object>
bool SortedArray<Object>::equals(const SortedArray <Object> &rhs)
{
	if (this->theSize != rhs.theSize) { return false; } //first, check to make sure sizes are the same (same # of elements)
	for (int i = 0; i < this->theSize; i++) {
	
		if (this->objects[i] != rhs.objects[i]) { return false; }
	
	}

	return true; //reach this statement if the above for loop fully iterates and does not hit a false during a comparison; 
				 //thus all elements are equal and the two sorted arrays are the same
}

//check if the array is empty //return true if array is empty
template <typename Object>
bool SortedArray<Object>::empty() const
{
	if (theSize == 0) { return true; } //array is empty
	else { return false; } //array is not empty
}

//get the size
template <typename Object>
int SortedArray<Object>::size() const
{
	return theSize;
}

//get the capacity 
template <typename Object>
int SortedArray<Object>::capacity() const
{
	return theCapacity;
}

// reserve
template<typename Object>
void SortedArray<Object>::reserve(int newCapacity)
{
	//do nothing if the exisiting capacity is larger than the requested capacity

	if (theCapacity > newCapacity) { return; }				

	//if theCapctiy < newCapacity, set capacity to the requested size; we will have to store the elements in a temporary array so that we can resize the array

	Object* tempObjArray = new Object[theSize];			//use this to store only the elements of the sortedArray so that we can potentially save space 
														//otherwise, we would create another sorted array that would have 
														//the same capacity with possibly unused memory as the current SortedArray

	for (int i = 0; i < theSize; i++) {
	
		tempObjArray[i] = objects[i];
	
	}

	delete[] objects;							//delete the old allocation (capacity)
	objects = new Object[newCapacity];			//allocate new memory (set the new capacity)
	theCapacity = newCapacity;					//set theCapacity equal to its new capacity
	for (int j = 0; j < theSize; j++) {
	
		objects[j] = tempObjArray[j];     //copy the original elements into the array with its new size (capacity)
	}

	delete[] tempObjArray; //deallocate dynamic array
	return;

}

//print elements of the array
template <typename Object>
void SortedArray<Object>::print(ostream &out, char delimiter) const
{
	//note to self: delimiter is the selected character that will be used to separate elements in the list as they are printed

	for (int i = 0; i < theSize; i++) {
	
		out << ' ' << objects[i] << delimiter; //put the extra space in case something like a ',' char is used
	}
}


// clear array of elements
template <typename Object>
void SortedArray<Object>::clear ()
{
	delete[] objects;						//clear will keep capacity but delete all elements; easiest thing to do is clear the memory
	objects = new Object[theCapacity];		// now, reallocate the memory with the same capacity
	theSize = 0;							//now all elements were deleted; set size to 0
}

// insert element into sorted array
template <typename Object>
void SortedArray<Object>::insert(const Object &obj)
{
	//check capacity first and ensure its large enough to allow the insertion of a new item
	//when capacity == the size, we will allocate a new capacity of 2*size + 1 so that we do not have to frequently resize the array when inserting new items

	if (theCapacity == theSize) { reserve(2 * theSize + 1); }


	//sorted list; take advantage and use a binary search algorithm to find where to insert the item

	int lowerBound = 0;			 //maintain the lower bound of the the binary search range
	int upperBound = theSize;     //maintain the upper bound of the binary search range
	int midpoint = theSize / 2;  //initial midpoint of the binary search
	int insertionPoint = -1;
	
	
	//we can use the below if statement if theSize = 0 (array is empty) to simply insert the item

	if (theSize == 0) {
		
		objects[0] = obj;
		theSize++;
		return; 
	}

	//We also need to check the case where an item will simply be added onto the end or at the front of the array

	if (obj < objects[0]) {  //case: insert item at the front
	
		for (int i = theSize - 1; i >= 0; i--) {

			objects[i + 1] = objects[i];	//slide element down to the right in the array
		}
		objects[0] = obj;					//insertion point position has now been cleared, we can now add in the new element
		theSize++;							//new item added; size increased by 1
		return;
	}

	if (obj > objects[theSize - 1]) {  //case: insert item at the end
	
		objects[theSize] = obj;				//append a new element to the end of the array
		theSize++;							//new item added; size increased by 1
		return;
	}

	//---other wise execute the following code below:

	//use the below while loop to first find the insertion point for the new item
	//insertion point will be such that all elements will be shifted to the right to leave a space for the new item

	while (insertionPoint == -1) {
		if (obj  > objects[midpoint]) {

			if (obj < objects[midpoint + 1]) { insertionPoint = midpoint + 1;}   //insertion point found: it is between [midpoint] and [midpoint + 1]
			else { 
															//case: obj > than current midpoint
				lowerBound = midpoint;						//new lower boundary of the search range is now the current midpoint 
				midpoint = (midpoint + upperBound) / 2;		//set new midpoint between current midpoint and upper bounndary of the search range
			}					

		}
		else if (obj < objects[midpoint]) {

			if (obj > objects[midpoint - 1]) { insertionPoint = midpoint; }   //insertion point found: it is between [midpoint - 1] and [midpoint]
			else {
															//case: obj < than current midpoint
				upperBound = midpoint;						//new upper boundary of the search range is now the current midpoint 
				midpoint = (midpoint + lowerBound) / 2;		//set new midpoint between current midpoint and lower bounndary of the search range
				
			}                        
			
		}
		else if (obj == objects[midpoint]) { return; } //return if item is already in the list
	}
	
	for (int k = theSize - 1; k >= insertionPoint; k--) {

		objects[k + 1] = objects[k];			//slide element down to the right in the array
	}

	objects[insertionPoint] = obj;		//after insertion point position has now been cleared from the above for loop, we can now add in the new element
	theSize++;							//new item added; size increased by 1
	return;

}

//remove the first element
template <typename Object>
void SortedArray<Object>::deleteMin()
{
	//delete minimum item from sorted, non-decreasing order array means to delete the front (element 0)

	for (int i = 1; i < theSize; i++) {
	
		objects[i - 1] = objects[i]; //overwrite the element to the left of current element [i - 1], with the current element [i]
	}

	theSize--; //size reduced by 1
	return;
}

//delete the maximum element from the sorted array
template <typename Object>
void SortedArray<Object>::deleteMax()
{
	//simply delete an element from the end of the sorted array (the max value) by reducing the size by 1

	theSize--;
	return;
}

//find the minimum element in the sorted array
template <typename Object>
const Object & SortedArray<Object>::findMin() const
{
	return objects[0];
}

//find the max element in the sorted array
template <typename Object>
const Object & SortedArray<Object>::findMax() const
{
	return objects[theSize - 1];
}

//binary search the array
template <typename Object>
int SortedArray<Object>::binarySearch(const Object &obj)
{
	int lowerBound = 0;			 //maintain the lower bound of the the binary search range
	int upperBound = theSize;     //maintain the upper bound of the binary search range
	int midpoint = theSize / 2;  //initial midpoint of the binary search

	while (midpoint != 0) {
		if (obj > objects[midpoint]){

			//case: obj > than current midpoint

			lowerBound = midpoint;						//new lower boundary of the search range is now the current midpoint 
			midpoint = (midpoint + upperBound) / 2;		//set new midpoint between current midpoint and upper bounndary of the search range

			}
		else if (obj < objects[midpoint]) {

			//case: obj < than current midpoint

			upperBound = midpoint;						//new upper boundary of the search range is now the current midpoint 
			midpoint = (midpoint + lowerBound) / 2;		//set new midpoint between current midpoint and lower bounndary of the search range

		}
		else if (obj == objects[midpoint]) { return midpoint; } //return element position if item is in the list
	}

	//if we get to these next lines of code, then midpoint has been set to 0 

	if (obj == objects[midpoint]) { return 0; } //check to see if first element is the searched value

	else { return -1; } //reach this return if item is not in the array

}


#endif