//
//  HashTable.h
//  Steg-o-matic
//
//  Created by Anthony Lai on 3/8/15.
//  Copyright (c) 2015 Yu-Kuan (Anthony) Lai. All rights reserved.
//

#ifndef Steg_o_matic_HashTable_h
#define Steg_o_matic_HashTable_h

template <typename KeyType, typename ValueType>
class HashTable
{
public:
    HashTable(unsigned int numBuckets, unsigned int capacity);
    ~HashTable();
    bool isFull() const;
    bool set(const KeyType& key, const ValueType& value, bool permanent = false);
    bool get(const KeyType& key, ValueType& value) const;
    bool touch(const KeyType& key);
    bool discard(KeyType& key, ValueType& value);
    
private:
    //  We prevent a HashTable from being copied or assigned by declaring the
    //  copy constructor and assignment operator private and not implementing them.
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);
    
    struct Node {
        KeyType key;
        ValueType value;
        Node* next;
        Node* orderPrev;
        Node* orderNext;
    };
    
    //  private member functions
    unsigned int getBucketNum(const KeyType& key) const;
    bool getNodePtr(const KeyType& key, Node*& ptr) const;
    
    
    Node* m_buckets;    //  dynamically allocated array
    unsigned int m_nBuckets;
    unsigned int m_nCapacity;
    unsigned int m_nUsed;
    Node* m_leastRecent;
    Node* m_mostRecent;
    
};

//  TO_DO implement/move this to cpp
unsigned int computeHash(std::string key);

template <typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int numBuckets, unsigned int capacity)
 :m_nBuckets(numBuckets), m_nCapacity(capacity), m_nUsed(0)
{
    m_buckets = new Node*[numBuckets];
    m_leastRecent = nullptr;
    m_mostRecent = nullptr;
}

//  TO_DO destructor here

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::isFull() const
{
    return m_nUsed == m_nCapacity;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::set(const KeyType& key, const ValueType& value, bool permanent)
{
    Node* keyPtr;
    if (!getNodePtr(key, keyPtr) && isFull())
        return false;
    
    
}

//  private member/helper function
template <typename KeyType, typename ValueType>
unsigned int HashTable<KeyType, ValueType>::getBucketNum(const KeyType& key) const
{
    unsigned int computeHash(KeyType);  //  prototype
    unsigned int hashNum = computeHash(key);
    unsigned int bucketNum = hashNum % m_buckets;
    return bucketNum;
}

template <typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::getNodePtr(const KeyType& key, Node*& ptr) const
{
    unsigned int keyBucketNum = getBucketNum(key);
    ptr = m_buckets[keyBucketNum];
    while (ptr != nullptr) {
        if (ptr->key == key)
            return true;
        ptr = ptr->next;
    }
    //  not found
    return false;
}



#endif
