
#include <iostream>
#include <cstring>

using namespace std;


const unsigned scale = 225;


// * * * replace with your own personal modulus * * * //
// const unsigned M = 3779;

const unsigned M = 3373;    // originally 3387 and my num is 3373



// Desc:  Dynamic Set of strings using a Hash Table (generic version)
//  Pre:  class T must have an attribute key of type string
template <class T>
class Set {
    private:
        T * A[M];



        // * * * Add attributes and helper methods here if you wish. * * * //

        // Desc:  Contains hash function that converts string key 
        //        to index for hash map position
        int stringConvert(string s) const;
        

        // unsigned size; // * * * remove this * * * //
        


    public:
        // Desc:  Default constructor.  Initialize table to NULL.
        Set() { 
            memset(A, 0, sizeof(T *)*M); 
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

// Desc:  Contains hash function that converts string key 
//        to index for hash map position
template<class T>
int Set<T>::stringConvert(string s) const{

    static constexpr char mapping[] = {
        '0','1','2','3','4','5','6','7','8','9',
        'a','b','c','d','e','f','g','h','i','j',
        'k','l','m','n','o','p','q','r','s','t',
        'u','v','w','x','y','z','A','B','C','D',
        'E','F','G','H','I','J','K','L','M','N',
        'O','P','Q','R','S','T','U','V','W','X',
        'Y','Z',' '
    };

    // implements Horner's Method 
    int ret = 0;
    int index = 0;
    int nextIndex = 0;
    if (s.size() > 1){
        for (int i = 0; i < s.size()-1; i++){
            if (ret < 1){
                for (int j = 0; j < 63; j++){
                    if (s[i] == mapping[j]){
                        index = j;
                        break;
                    }
                }
            }   
            else{
                index = ret;
            }
            for (int k = 0; k < 63; k++){
                if (s[i+1] == mapping[k]){
                    nextIndex = k;
                    break;
                }
            }
            ret = (index*64+nextIndex)%M;
        }
    }
    else if (s.size() == 1){
        for (int i = 0; i < 63; i++){
            if (s[0] == mapping[i]){
                index = i;
                break;
            }
        }
        ret = index%M;
    }
    return (ret*225)%M;
}

// Desc:  Insert x into the Set, according to its ->key.
//        If ->key is already present, then replace it by x.
//        Collisions are resolved via quadratic probing sequence.
// Post:  returns the table index where x was inserted
//        returns -1 if x was not inserted
template <class T>
int Set<T>::insert(T * x) {

    int increment = -1;

    // convert string x->key to an int
    int index = stringConvert(x->key);
    int origIndex = index;

    // quadratic probing stops when it gets to the starting index

    // performs first probing 
    if (A[index] != nullptr && A[index]->key != x->key){
        increment = increment+2;
        index = (index + increment)%M;
    }
    else{
        A[index] = x;
        return index;
    }

    // then after first probing, we keep probing until it gets to the starting index
    while (origIndex != index){
        if (A[index] != nullptr && A[index]->key != x->key){
            increment = increment+2;
            index = (index + increment)%M;
        }
        else{
            A[index] = x;
            return index;
        }
    }
    return -1;
}


// Desc:  Returns T * x such that x->key == key, if and only if 
//        key is in the Set.
//        Returns NULL if and only if key is not in the Set.
// Post:  Set is unchanged
template <class T>
T * Set<T>::search(string key) const{

    int index = stringConvert(key);
    int increment = -1;

    int origIndex = index;

    if (A[index] == nullptr){
        return nullptr;
    }
    else if (A[index]->key == key){
        return A[index];
    }
    else{
        increment = increment+2;
        index = (index + increment)%M;
    }

    // quadratic probing stops when it gets to the starting index
    while (index != origIndex){
        if (A[index] == nullptr){
            return nullptr;
        }
        else if (A[index]->key == key){
            return A[index];
        }
        else{
            increment = increment+2;
            index = (index + increment)%M;
        }
    }
    return NULL;


}


