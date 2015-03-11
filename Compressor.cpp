#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    //  Create the HashTAble
    int capacity = s.size() / 2 + 512;
    if (capacity > 16384)
        capacity = 16384;
    HashTable<string, unsigned short> wordsHash(2 * capacity, capacity);
    
    //  Initialize all the permanent ones
    int nextFreeID;
    for (nextFreeID = 0; nextFreeID < 256; nextFreeID++)
        //  using the fill string constructor to convert char to string
        wordsHash.set(string(1, static_cast<char>(nextFreeID)), nextFreeID, true);
    
    string runSoFar = "";
    string expandedRun = "";
    string dummyKey = "";
    numbers.clear();
    unsigned short dummyValue;
    unsigned short runSoFarValue;
    unsigned short charValue;
    unsigned short discardedID;
    
    for (int i = 0; i < s.size(); i++) {
        expandedRun = runSoFar + s[i];  //  a
        if (wordsHash.get(expandedRun, dummyValue)) {    //  b
            runSoFar = expandedRun;
            continue;
        }
        
        //  c
        wordsHash.get(runSoFar, runSoFarValue);
        numbers.push_back(runSoFarValue);   //  d
        wordsHash.touch(runSoFar);  //  e: touch it regardless (if it's permanent)
        runSoFar = "";
        
        //  g
        //  using the fill string constructor to convert char to string
        wordsHash.get(string(1, s[i]), charValue);
        numbers.push_back(charValue);   //  h
        
        if (!wordsHash.isFull()) {  //  i
            wordsHash.set(expandedRun, nextFreeID);
            nextFreeID++;
        } else {    //  j
            wordsHash.discard(dummyKey, discardedID);
            wordsHash.set(expandedRun, discardedID);
        }
    }
    
    if (runSoFar.size() != 0) { //  7
        wordsHash.get(runSoFar, runSoFarValue);
        numbers.push_back(runSoFarValue);
    }
    
    numbers.push_back(capacity);    //  8
    
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
	return false;  // This compiles, but may not be correct
}
