#include "provided.h"
#include "HashTable.h"
#include <string>
#include <vector>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    //  Create the HashTable
    int capacity = s.size() / 2 + 512;
    if (capacity > 16384)
        capacity = 16384;
    HashTable<string, unsigned short> wordsHash(2 * capacity, capacity);
    
    //  Initialize the permanent ones
    int nextFreeID;
    for (nextFreeID = 0; nextFreeID < 256; nextFreeID++)
        //  using the fill string constructor to convert char to string
        wordsHash.set(string(1, static_cast<char>(nextFreeID)), nextFreeID, true);
    
    //  Defined some variables
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
	//  Base condition check
    if (numbers.size() == 0)
        //  numbers must at least has the HashTable capacity
        return false;
    
    //  Create the HashTable
    int capacity = numbers[numbers.size() - 1];
    HashTable<unsigned short, string> numHash(2 * capacity, capacity);
    
    //  Initialize the permanent ones
    for (int i = 0; i < 256; i++)
        //  using the fill string constructor to convert char to string
        numHash.set(i, string(1, i), true);
    
    //  Defined some variables
    int nextFreeID = 256;
    string runSoFar = "";
    string expandedRun = "";
    string numValue = "";
    string dummyValue = "";
    s = "";
    unsigned short discardedID;
    
    for (int i = 0; i < numbers.size() - 1; i++) {
        if (numbers[i] <= 255) { //  a
            numHash.get(numbers[i], numValue);
            s += numValue;
            if (runSoFar.size() == 0) { //  ii
                runSoFar += numValue;
                continue;
            } else {    //  iii
                expandedRun = runSoFar + numValue;
                if (!numHash.isFull()) {    //  iv
                    numHash.set(nextFreeID, expandedRun);
                    nextFreeID++;
                } else {    //  v
                    numHash.discard(discardedID, dummyValue);
                    numHash.set(discardedID, expandedRun);
                }
                runSoFar = "";  //  vi
                continue;
            }
        } else {    //  b
            if (!numHash.get(numbers[i], numValue))
                //  HashTable does not have this association
                return false;
            
            numHash.touch(numbers[i]);  //  ii
            s += numValue;  //  iii
            runSoFar = numValue;    //  iv
        }
    }
    return true;
}
