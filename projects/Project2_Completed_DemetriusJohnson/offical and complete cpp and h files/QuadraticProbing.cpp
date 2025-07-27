#include "QuadraticProbing.h"
#include <iostream>
using namespace std;

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
int nextPrime( int n )
{
    if( n <= 0 )
        n = 3;

    if( n % 2 == 0 )
        n++;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

/**
 * A hash routine for string objects.
 */
int hash1( const string & key )
{
    int hashVal = 0;

    for(unsigned int i = 0; i < key.length( ); i++ )    //I wonder why use unsigned int here??? int and unsigned int are the same size, with unsigned reserving all bits for positive values
        hashVal = 37 * hashVal + key[ i ];              //I notice right here that this is a polynomial hash function similar to the one shown in the notes
                                                        //runtime is O(N)

    return hashVal;
}

/**
 * A hash routine for ints.
 */
int hash1( int key ) //use this function and the original provided simple main for testing before I introduce string hash table
{
    return key; //runtime is O(1)
}
