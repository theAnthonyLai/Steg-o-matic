// We have not yet produced the test driver main routine for you.


#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include "provided.h"
#include "HashTable.h"
using namespace std;

unsigned int computeHash(int key) {
    return key;
}

unsigned int computeHash(std::string key) {
    int result = 0;
    for (int i = 0; i < key.size(); i++)
        result += (i+1)*key[i];
    return result;
}

void test();
void test2();
int main()
{
	cout << "Test driver not yet written." << endl;

	// string text;
	// if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text))
	//	cout << "Error hiding!" << endl;
	// string msg;
	// if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", msg))
	// 	cout << "Error revealing!" << endl;
    
    test();
    test2();
}

void test() {
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

unsigned int computeHash(int* a)
{
    return 1;
}

void test2()
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