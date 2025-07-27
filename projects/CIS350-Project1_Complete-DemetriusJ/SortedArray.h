/** 
 *  File:  SortedArray.h  
 *  
 *  Description: Definition of the class template SortedArray<Object>
 */
 /** program1Test.cpp
  *
  * Author: Demetrius Johnson
  * Date: 5-25-21
  * Modification Date: 5-27-21
  * Description: A Test Program for Program 1; this program implements the use of a sorted array using dynamically allocated memory in non decreasing order
  */
#ifndef SortedArray_H
#define SortedArray_H

//INCLUDES
#include <iostream>
using namespace std;

//----------------------------------
//     SortedArray<Object>
//----------------------------------

template <typename Object>
class SortedArray
{
  public:
// LIFECYCLE

	  /** Default constructor
	 */
    SortedArray(); 

	/** Copy constructor
	 *
	 * @param from The oject to copy to this object
	 */
	SortedArray(const SortedArray <Object> &from);

	/** Destructor
	 */
    ~SortedArray();
    
//OPERATORS

	/** Assignment operator
	 * 
	 * @param from The value to assign to this object
	 * 
	 * @return A reference to this object
	 */
    const SortedArray & operator= (const SortedArray &from);

	/** Accessor Operator []
	 * 
	 * @param idx 
	 * 
	 * @return a constant reference to the object at index idx in the array,
	 *  with no bounds-checking
	 */
	const Object & operator[](int idx) const; 

	/** equals
	 * 
	 * @param rhs 
	 * 
	 * @return true if this array and the rhs array have 
	 * the same elements, in the same order. (The capacity of the containers 
	 * may be different, only the actual number of elements in each 
	 * must be the same for the call to return true.)
	 */
	bool equals(const SortedArray <Object> &rhs); 

	/** empty
	 *  
	 * @return true if the array contains no elments, and false otherwise
	 */
    bool empty() const;

	/** size
	 *  
	 * @return the number of elements in the container
	 */
	int size() const;

	/** capacity 
	 *  
	 * @return the internal capacity of the array
	 */
	int capacity() const;

	/** reserve
	 *  
	 * @param capacity
	 * 
	 * Description: sets the new capacity. MAY do nothing if the existing 
	 * capacity (but not the size) is larger than the requested capacity.
	 */
	void reserve(int newCapacity);
    
//DISPLAY METHODS
	void print(ostream &out, char delimiter = ',') const;
	
//SORTED ARRAY PROTOCOTLS

	/** clear
	 *  
	 * Description: removes all the elements of the sorted array.
	 */
	void clear ();

	/** insert
	 *  
	 * @param obj
	 *
	 * Description: insert new elements into the correct position in the 
	 * sorted array, sliding elements over the position to right, as needed
	 */
	void insert(const Object &obj);

	/**
	 * Description: remove the element in position 0, and slide over all
	 * other item
	 */
    void deleteMin();

	/**
	 * Description: remove the last element in the sorted array
	 */
    void deleteMax();

	/**
	 * Description: find the minimum element in the sorted array
	 */
    const Object & findMin() const;

	/**
	 * Description: find the maximum element in the sorted array
	 */
	const Object & findMax() const;

	/**binarySearch
	 *
	 * @param obj: the item to be searched
	 *
	 * @return index where item is found or -1 if not found.
	 *
	 * Description: performs the standard binary search using two comparisons 
	 * per level. 
	 */
	int binarySearch (const Object &obj);

    enum { SPARE_CAPACITY = 16 };

  private:
    int theSize;
    int theCapacity;
    Object *objects;
};

#include "SortedArray.cpp"

#endif

