//
// Created by Joao on 11/12/20.
//

#ifndef HASHTABLE_PASSSERVER_HASHTABLE_HPP
#define HASHTABLE_PASSSERVER_HASHTABLE_HPP

#include "hashtable.h"

using namespace Joao;

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n){
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
        std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector <unsigned long> v (n+1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
            return n;
        --n;
    }

    return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n)
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes){
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for( i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i ; j < n; j += i)
                vprimes[j] = 0;
    }
}

template<typename K, typename V>
HashTable<K, V>::HashTable(size_t size) {

    //creates a list with the size calling prime_below to get
    //the closest prime number
    theList.resize(prime_below(size));
    currentSize = 0;
    cout << theList.size() << endl;

}

template<typename K, typename V>
HashTable<K, V>::~HashTable() {

    clear();

}

template<typename K, typename V>
bool HashTable<K, V>::contains(const K &k) const{

    //hashes to where k would be
    auto & whichList = theList[myhash(k)];

    auto itr = whichList.begin();

    //will go trhough hashed list to compare k with first of pair
    for(auto & thisList: whichList) {

        //will delete the first one, in theory there should only be one anyways
        if(thisList.first == k){

            return true;

        }

        //next iteratotr, also checks for bounds
        itr++;
        if(itr == end(whichList)){
            return false;
        }

    }

    return false;

}

template<typename K, typename V>
bool HashTable<K, V>::match(const pair<K, V> &kv) const{

    //Will hash to where V would go, and then it goes through
    //the list to find V to match the pair. Will return false
    //if does not find
    auto & whichList = theList[myhash(kv.first)];
    return find(begin(whichList), end(whichList), kv) != end(whichList);

}

template<typename K, typename V>
bool HashTable<K, V>::insert(const pair<K, V> &kv) {

    // add  the key-value pair kv into the hash table. Don't add if kv is already in the hash table.
    //If the key is the hash table but with a different value, the value should be updated to the
    //new one with kv. Return true if kv is inserted or the value is updated; return false otherwise
    //(i.e., if kv is in the hash table).

    if(match(kv))
        return false;

    //Hashes value V, will only insert if it doesn't find
    auto & whichList = theList[myhash(kv.first)];
    /*if(find(begin(whichList), end(whichList), kv) != end(whichList))
        return false;*/

    //Will run through the list to see if any updating is needed
    for(auto & thisList: whichList) {

        if(kv.first == thisList.first && kv.second != thisList.second) {
            thisList.second = kv.second;
            return true;
        }

    }

    whichList.push_back(kv);

    //Has to rehash if the size becomes bigger than the vector
    if(++currentSize > theList.size())
        rehash();

    return true;

}

template<typename K, typename V>
bool HashTable<K, V>::insert(pair<K, V> &&kv) {

    if(match(kv))
        return false;

    //Hashes value V, will only insert if it doesn't find
    auto & whichList = theList[myhash(kv.first)];

    //Will run through the list to see if any updating is needed
    for(auto & thisList: whichList) {

        if(kv.first == thisList.first && kv.second != thisList.second) {
            thisList.second = kv.second;
            std::cout<<"updated"<<std::endl;
            return true;
        }

    }

    whichList.push_back(std::move(kv));

    //Has to rehash if the size becomes bigger than the vector
    if(++currentSize > theList.size())
        rehash();

    return true;

}

template<typename K, typename V>
bool HashTable<K, V>::remove(const K &k) {

    //hashes to where k would be
    auto & whichList = theList[myhash(k)];

    auto itr = whichList.begin();

    //will go trhough hashed list to compare k with first of pair
    for(auto & thisList: whichList) {

        //will delete the first one, in theory there should only be one anyways
        if(thisList.first == k){

            whichList.erase(itr);
            --currentSize;
            return true;

        }

        //next iteratotr, also checks for bounds
        itr++;
        if(itr == end(whichList)){
            return false;
        }

    }

    return false;

}

template<typename K, typename V>
void HashTable<K, V>::clear() {

    //calls private helper function to clear vector
    makeEmpty();

}

template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {

    //Creates ifstream object
    ifstream f;

    //Opens and checks if file opens
    f.open(filename);

    if (!f.is_open()) {
        cout << "File " << filename << " missing!\n\n";
        return false;
    }

    //Until the end of object f, will keep getting username and passwords and adding to file
    while(!f.eof()){

        string username, password;

        f >> username >> password;

        pair<string, string> p2(username, password);

        insert(p2);
    }

    f.close();
}

template<typename K, typename V>
void HashTable<K, V>::dump(){

    cout << endl;

    //Will use iterator to go through linked list at vector[i]
    typename std::list<pair<K, V>>::iterator start, end;

    //Goes through entire vector
    for(int i = 0; i < theList.size(); i++){

        std::cout << "v[" << i << "]";

        start = theList[i].begin();
        end = theList[i].end();

        //If there is something in the linked list it will print everything
        //by dereferencing the pointers and printing the pair
        while(start != end){
            cout << " : " << (*start).first << " " << (*start).second;
            start++;
        }

        cout << endl;

    }




}

tcsetattr(fileno (stdin), TCSANOW, &t_off);

tcsetattr(fileno (stdin), TCSANOW, &t_on);


template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) {

    /*
     * TODO: Add check if file opens, return false if it doesn't
     */

    //creates file oject
    ofstream file;

    //opens/creates file
    file.open(filename);

    //Will use iterator to go through linked list at vector[i]
    typename std::list<pair<K, V>>::iterator start, end;

    //Goes through entire vector
    for(int i = 0; i < theList.size(); i++){

        start = theList[i].begin();
        end = theList[i].end();

        //If there is something in the linked list it will print everything
        //by dereferencing the pointers and printing the pair
        while(start != end){
            file << (*start).first << " " << (*start).second << std::endl;
            start++;
        }

    }

    //closes the file
    file.close();

    return true;

}

template<typename K, typename V>
size_t HashTable<K, V>::size() const {

    return currentSize;

}

template<typename K, typename V>
void HashTable<K, V>::makeEmpty() {

    //Will make hashtable empty
    for(auto & thisList : theList) {
        theList.clear();
    }

}

template<typename K, typename V>
void HashTable<K, V>::rehash() {

    //copieslist to a temp spot
    std::vector<std::list<std::pair<K, V>>> oldList = theList;

    //resizes the new list and clears everything
    theList.resize(prime_below(2*  theList.size()));
    for(auto & thisList : theList)
        thisList.clear();

    //size back to zero
    currentSize = 0;

    //will rehash and reinsert everything
    for(auto & thisList : oldList)
        for(auto & x : thisList)
            insert(std::move(x));

}

template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const{

    //Creates hash functor to hash k
    static hash<K> hf1;
    return hf1(k) % theList.size();

}


#endif //HASHTABLE_PASSSERVER_HASHTABLE_HPP
