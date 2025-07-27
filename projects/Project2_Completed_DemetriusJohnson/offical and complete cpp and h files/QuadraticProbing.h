#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <string>
using namespace std;

//3 necessary declarations needed before defining these functions and functions associated with these functions:
int nextPrime( int n );
int hash1( const string & key );
int hash1( int key );

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hash( string str ) --> Global method to hash strings

template <typename HashedObj>
class HashTable
{
public:
    explicit HashTable(int size = 101) : array(nextPrime(size))
    {
        makeEmpty();
    }

    bool contains(const HashedObj& x) const
    {
        return isActive(findPos(x));
    }

    void makeEmpty()
    {
       currentSize = 0;
       tot_numCollisions = 0;
        for (int i = 0; i < array.size(); i++) {
            array[i].info = EMPTY;              //reset bucket to empty
            array[i].collision_chainLength = 1; //reset chain length
        }
    }

    bool insert(const HashedObj& x)
    {
        // Insert x as active
        int currentPos = findPos_insert(x);       //use insert version of the findPos function to track collision statistics
        if (isActive(currentPos))
            return false;

        array[currentPos] = HashEntry(x, ACTIVE);

        // Rehash; see Section 5.5
        if (++currentSize > array.size() / 2)
            rehash();

        return true;
    }

    bool remove(const HashedObj& x)
    {
        int currentPos = findPos(x);
        if (!isActive(currentPos))
            return false;

        array[currentPos].info = DELETED;
   
        return true;
    }

    //statistic functions
    float getLoadFactor(void) { return (float)currentSize / (float)array.size(); } //load factor ==  number of ACTIVE elements/table size -->had to cast the two values to float
    int numActiveBuckets(void){return currentSize;}                                //use this to return number of ACTIVE hash entries (i.e.: number of words in a string hash table)
    int hashTableSize(void){ return array.size(); }
    int total_InsertionCollisions(void){ return tot_numCollisions; }
    float avg_InsertionCollisions(void) { return ((float)tot_numCollisions + float(currentSize))/ (float)currentSize; } // bucket defaults to 1 collision
    int longest_collision_chain(void) { return longest_collisionChain; }

    enum EntryType { ACTIVE, EMPTY, DELETED };                                      //ACTIVE == 0, EMPTY == 1, DELETED == 2

  private:

    struct HashEntry
    {
        HashedObj element;
        EntryType info;
        int collision_chainLength = 1; //track "chain" length of every bucket --> simply number of times a bucket is involved in a collision

        HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
          : element( e ), info( i ) { } //constructor for the HashEntry struct; element initialized with contents of constant ref e, info initialized with i == EMPTY == 0
    };
    
    vector<HashEntry> array;         //store all hash entries in this array //array.size() will tell you size of the hash table (total number of buckets)
    int currentSize = 0;             //keep track of number of ACTIVE buckets in the hash table; I will update remove function so this value does not include buckets marked DELETED
    int tot_numCollisions = 0;       //track the total number of collisions that occur
    int longest_collisionChain = 0;  //track longest collision chain length

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x ) const
    {
        int offset = 1;
        int currentPos = myhash( x );

          // Assuming table is half-empty, and table length is prime,
          // this loop terminates
        while( array[ currentPos ].info != EMPTY &&
                array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        return currentPos;
    }

    //use the below findPos_insert function only for the insert function so that I can acquire collision statistics...
    int findPos_insert(const HashedObj& x)
    {
        int offset = 1;
        int currentPos = myhash(x);

        // Assuming table is half-empty, and table length is prime,
        // this loop terminates
        while (array[currentPos].info !=EMPTY &&       //!= EMPTY means the cell could be DELETED or ACTIVE
            array[currentPos].element != x)
        {
            tot_numCollisions++;                                                    //total num collisions increased every time a position is reached for entry
            array[currentPos].collision_chainLength++;                              //increase collision chain length at current position
            if (array[currentPos].collision_chainLength > longest_collisionChain)   //set new longest chain length if necessary
                longest_collisionChain = array[currentPos].collision_chainLength;

            currentPos += offset;    // Compute ith probe
            offset += 2;             //quadratic probing; simplified version of f(i^2); derived from f(i) - f(i-1), 
                                     //which shows offset is increased by 2 + previous-offset value to get quadratic probe i^2 value
                                     //cpu can process arithmetic much faster and efficiently than multiplication operations
            
            if (currentPos >= array.size())
                currentPos -= array.size();
        }

        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array; //store elements of the original array that need to be rehashed

            // Create new double-sized, empty table //I notice we do not simply use the use the vector.resize() function which will automatically copy the old elements to the new array...
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( int j = 0; j < array.size( ); j++ )
            array[ j ].info = EMPTY;

            // Copy table over
        longest_collisionChain = 0;     //reset longest chain for new hash table
        tot_numCollisions = 0;          //need to also reset total insertion collisions since we are making a new hash table
        currentSize = 0;                //reset size since insert function will increment currentSize each time an item is added
        for( int i = 0; i < oldArray.size( ); i++ )
            if( oldArray[ i ].info == ACTIVE )
                insert( oldArray[ i ].element );
    }

    int myhash( const HashedObj & x ) const
    {
        int hashVal = hash1( x );

        hashVal %= array.size( );
        if( hashVal < 0 )
            hashVal += array.size( );

        return hashVal;
    }
};

#endif
