
#include <iostream>
#include <cstring>

using namespace std;



const unsigned M = 4000;



// Desc:  Dynamic Set of strings using a Hash Table (generic version)
//  Pre:  class T must have an attribute key of type string
template <class T>
class Set {
    private:
        T * A[M];
        unsigned size; 


    public:
        // Desc:  Default constructor.  Initialize table to NULL.
        Set() { 
            memset(A, 0, sizeof(T *)*M); 
            size = 0;
        }


        // Desc:  Insert x into the Set, according to its ->key.
        //        If ->key is already present, then replace it by x.
        //        Collisions are resolved via quadratic probing sequence.
        // Post:  returns the table index where x was inserted
        //        returns -1 if x was not inserted
        int insert(T * x);


        // Desc:  Returns T * x such that x->key == key, if and only if 
        //        key is in the Set.
        //        Returns NULL if and only if key is not in the Set.
        // Post:  Set is unchanged
        T * search(string key) const;

};


// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it by x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T * x) {
    // * * * remove all the code here, and write your own * * * //
    if (size == M) return -1;
    for (int i = size-1; i >= 0; i--) {
        if (A[i]->key == x->key) {
            A[i] = x;
            return i;
        }
    }
    A[size] = x;
    size++;
    return size-1;
}


// Desc:  Returns T * x such that x->key == key, if and only if 
//        key is in the Set.
//        Returns NULL if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T * Set<T>::search(string key) const {
    for (int i = size-1; i >= 0; i--) {
        if (A[i]->key == key) return A[i];
    }
    return NULL;
}



