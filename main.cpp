// We have not yet produced the test driver main routine for you.
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>
#include "provided.h"
#include "HashTable.h"
using namespace std;

void HashTest();
void HashTest2();
void BinaryTest();
void CompressorTest();

int main()
{
	cout << "Test driver not yet written." << endl;

	// string text;
	// if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text))
	//	cout << "Error hiding!" << endl;
	// string msg;
	// if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", msg))
	// 	cout << "Error revealing!" << endl;
    
    //  HashTest();
    //  HashTest2();
    //  BinaryTest();
    CompressorTest();
    cerr << "Passed all tests!!" << endl;
}

void CompressorTest() {
    vector<unsigned short> v1, v2;
    v1.push_back(999);
    string s1 = "dummy";
    Compressor::compress("AAAAAAAAAB", v1);
    assert(v1.size() == 8 && v1[0] == 65 && v1[2] == 256 && v1[4] == 257 && v1[5] == 65 && v1[7] == 517);
    assert(Compressor::decompress(v1, s1) && s1 == "AAAAAAAAAB");
    assert(!Compressor::decompress(v2, s1));
    v1.clear();
    v2.clear();
    s1 = "Dummy stuff";
    v1.push_back(777);
    assert(Compressor::decompress(v1, s1) && s1 == "");
    Compressor::compress("Lionel Messi is the best player in the world!!!", v1);
    assert(Compressor::decompress(v1, s1) && s1 == "Lionel Messi is the best player in the world!!!");
}

void BinaryTest() {
    vector<unsigned short> v1, v2;
    v1.push_back(19);
    assert(BinaryConverter::encode(v1) == "           \t  \t\t");
    assert(BinaryConverter::decode(BinaryConverter::encode(v1), v2) && v2[0] == 19);
    v1.clear();
    v2.clear();
    assert(BinaryConverter::encode(v1) == "");
    assert(BinaryConverter::decode("", v1) && v1.size() == 0);
    unsigned short array[9] = { 17, 25, 24, 99, 123, 45, 222, 98, 77 };
    for (int i = 0; i < 9; i++)
        v1.push_back(array[i]);
    //  stuff v2 with random stuff, make sure decode clears them
    for (int i = 8; i >= 0; i--)
        v2.push_back(array[i]);
    v2.push_back(111);
    assert(BinaryConverter::decode(BinaryConverter::encode(v1), v2) && v2.size() == 9);
    for (int i = 0; i < 9; i++)
        assert(array[i] == v2[i]);
    //  test non-16 multiple and non space/tab
    assert(!BinaryConverter::decode("   ", v1));
    assert(BinaryConverter::decode("                ", v1));
    assert(!BinaryConverter::decode("               t", v1));

}

unsigned int computeHash(int key) {
    return key;
}

unsigned int computeHash(unsigned short key) {
    return key;
}

unsigned int computeHash(std::string key) {
    int result = 0;
    for (int i = 0; i < key.size(); i++)
        result += (i+1)*key[i];
    return result;
}

unsigned int computeHash(int* a)
{
    return 1;
}

void HashTest() {
    HashTable<string, int> nameToAge(11, 200);
    // add a bunch of associations in this order
    nameToAge.set("Carey", 43);
    nameToAge.set("David", 97);
    nameToAge.set("Lily", 18);
    nameToAge.set("Sally", 22);
    nameToAge.set("David", 55);
    
    int value = 999;
    assert(!nameToAge.get("UCLA", value));
    assert(value == 999);
    assert(nameToAge.get("Carey", value) && value == 43);
    assert(nameToAge.get("David", value) && value == 55);
    
    HashTable<string, int>test1(3, 4);
    string key;
    assert(!test1.isFull());
    assert(!test1.touch(""));
    assert(!test1.touch("Hello"));
    assert(!test1.get("", value));
    assert(!test1.get("Hello", value));
    assert(!test1.discard(key, value));
    
    test1.set("Demaryius", 88, true);
    test1.set("Odell", 13, false);
    test1.set("JPP", 90);
    test1.set("Demaryius", 77);
    test1.set("Watt", 99);
    test1.set("JPP", 66);
    test1.set("Odell", 17);
    assert(!test1.set("Suh", 90));
    assert(test1.isFull());
    assert(test1.get("Demaryius", value) && value == 77);
    assert(test1.get("Odell", value) && value == 17);
    assert(!test1.get("Suh", value) && value == 17);
    assert(test1.discard(key, value) && key == "Watt" && value == 99);
    assert(!test1.touch("Watt"));
    assert(!test1.touch("Demaryius"));
    assert(test1.touch("JPP"));
    assert(test1.discard(key, value) && key == "Odell" && value == 17);
    assert(test1.discard(key, value) && key == "JPP" && value == 66);
    assert(!test1.discard(key, value) && key == "JPP" && value == 66);
    
}

void HashTest2()
{

    HashTable<int*, int> aaa(5, 5);
    aaa.touch(nullptr);
    
    HashTable<int, int> swag(5, 5);
    int x = -1, y = -1;
    assert(!swag.isFull());
    assert(!swag.discard(x, y) && x == -1 && y == -1);
    assert(swag.set(3, 3, true));
    assert(!swag.discard(x, y) && x == -1 && y == -1);
    assert(swag.set(0, 0, false));
    assert(swag.set(0, 1, false));
    assert(swag.discard(x, y) && x == 0 && y == 1);
    x = -1, y = -1;
    assert(!swag.discard(x, y) && x == -1 && y == -1);
    x = 0;
    assert(swag.set(0, 5, false));
    assert(swag.get(x, y) && y == 5);
    assert(swag.set(4, 5, false));
    assert(swag.set(5, 6, false));
    assert(swag.set(6, 7, false));
    assert(!swag.set(7, 8, false));
    assert(swag.isFull());
    assert(swag.touch(0));
    assert(swag.discard(x, y) && x == 4 && y == 5);
    assert(swag.set(10, 25, true));
    assert(!swag.set(15, 25, false));
    assert(!swag.set(15, 25, false));
    assert(swag.set(6, 123, true));
    assert(swag.touch(6));
    assert(swag.discard(x, y) && x ==5 && y == 6);
    
    HashTable<string, string> motherfucker(10, 250);
    for (int i = 0; i < 250; i++)
    {
        ostringstream a, b;
        a << i;
        b << i * i * i;
        motherfucker.set(a.str(), b.str(), false);
    }
    assert(motherfucker.isFull());
    cout << "RETRIEVING VALUES FROM HASHTABLE, FROM 0 TO 249" << endl;
    cout << "VALUES ARE THE KEY TO THE THIRD POWER" << endl;
    for (int i = 0; i < 250; i++)
    {
        ostringstream a;
        a << i;
        string b;
        motherfucker.get(a.str(), b);
        cout << a.str() << ", " <<  b << endl;
    }
    for (int i = 249; i >= 0; i--)
    {
        ostringstream a;
        a << i;
        motherfucker.touch(a.str());
    }
    
    cout << "PRINTING KEY VALUE PAIRS IN REVERSE ORDER" << endl;
    for (int i = 0; i < 250; i++)
    {
        string a, b;
        motherfucker.discard(a, b);
        cout << a << ", " << b << endl;
    }
    
    cout << "IF THE ORDER ISN'T FROM 249 to 0 YOU FUCKED UP" << endl;
    
    HashTable<string, int> nameToAge(100, 200);
    nameToAge.set("Carey", 43);
    nameToAge.set("David", 97);
    nameToAge.set("Timothy", 43, true);
    nameToAge.set("Ivan", 28);
    nameToAge.set("Sally", 22);
    nameToAge.set("David", 55);
    nameToAge.touch("Carey");
    string a;
    int b;
    assert(nameToAge.discard(a, b) && a == "Ivan" && b == 28);
    assert(nameToAge.discard(a, b) && a == "Sally" && b == 22);
}