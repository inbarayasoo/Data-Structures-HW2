#ifndef OUR_WET_2_HASHTABLE_H
#define OUR_WET_2_HASHTABLE_H

#include "AVLTree.h"

template<class Key, class T>
class HashTable {
public:
    explicit HashTable(int size = initialSize);

    ~HashTable();

    void insert(Key& key, T& value);

    void remove(Key& key);

    T* find(Key& key) const;

    bool contains(Key& key) const;

    void elementsToArr(T* arr) const;

    int getSize() const;

private:

    AVLTree<Key, T>* m_buckets;
    int m_numOfElements;
    int m_numOfBuckets;

    static const int initialSize = 10;
    static const int rehaseFactor = 2;
    static const int shrinkThreshold = 4;

    int hashFunc(const Key& key) const;
    int getBucketIndex(const Key& key) const;
    T* findKeyInBucket(const int& key, const AVLTree<Key, T>& bucket) const;
    void checkTableSize();
    void rehashToSize(int newTableSize);
    void keysToArr(Key* arr) const;

};

template<class Key, class T>
bool HashTable<Key, T>::contains(Key &key) const {
    int bucketIndex = getBucketIndex(key);
    AVLTree<Key, T>& bucket = m_buckets[bucketIndex];
    return bucket.contains(key);
}

template<class Key, class T>
T* HashTable<Key, T>::find(Key &key) const {
    int bucketIndex = getBucketIndex(key);
    AVLTree<Key, T>& bucket = m_buckets[bucketIndex];
    return findKeyInBucket(key, bucket);
}

template<class Key, class T>
int HashTable<Key, T>::getSize() const {
    return m_numOfElements;
}

template<class Key, class T>
void HashTable<Key, T>::elementsToArr(T* arr) const {
    int index = 0;
    for (int i = 0; i < m_numOfBuckets; ++i) {
        AVLTree<Key, T>& bucket = m_buckets[i];
        bucket.fillElementsInArrayInOrder(arr + index, bucket.getSize());
        index += bucket.getSize();
    }
}

template<class Key, class T>
void HashTable<Key, T>::keysToArr(Key *arr) const {
    int index = 0;
    for (int i = 0; i < m_numOfBuckets; ++i) {
        AVLTree<Key, T>& bucket = m_buckets[i];
        bucket.fillKeysInArrayInOrder(arr + index, bucket.getSize());
        index += bucket.getSize();
    }
}


template<class Key, class T>
HashTable<Key, T>::HashTable(int size) :
    m_buckets(nullptr),
    m_numOfElements(0)
    {
        int numOfBuckets = initialSize;

        if (size > initialSize) {
            numOfBuckets = size;
        }

        m_numOfBuckets = numOfBuckets;
        m_buckets = new AVLTree<Key, T>[m_numOfBuckets];
    }

template<class Key, class T>
void HashTable<Key, T>::insert(Key &key, T &value) {
    int bucketIndex = getBucketIndex(key);
    AVLTree<Key, T>& bucket = m_buckets[bucketIndex];

    T* elem = findKeyInBucket(key, bucket);
    if (elem == nullptr) {
        bucket.insert(key, value);
        ++m_numOfElements;
        checkTableSize();
    }
}

template<class Key, class T>
void HashTable<Key, T>::remove(Key &key) {
    int bucketIndex = getBucketIndex(key);
    AVLTree<Key, T>& bucket = m_buckets[bucketIndex];

    T* elem = findKeyInBucket(key, bucket);
    if (elem != nullptr) {
        bucket.remove(key);
        --m_numOfElements;
        checkTableSize();
    }
}

template<class Key, class T>
int HashTable<Key, T>::hashFunc(const Key& key) const {
    return key % m_numOfBuckets;
}

template<class Key, class T>
int HashTable<Key, T>::getBucketIndex(const Key& key) const {
    return hashFunc(key);
}

template<class Key, class T>
T* HashTable<Key, T>::findKeyInBucket(const int& key, const AVLTree<Key, T>& bucket) const {
    return bucket.find(key);
}

template<class Key, class T>
void HashTable<Key, T>::checkTableSize() {
//    assert(m_numOfElements <= m_numOfBuckets);

    if (m_numOfElements == m_numOfBuckets) {
        rehashToSize(m_numOfBuckets * rehaseFactor);
    }
    else if (m_numOfElements < m_numOfBuckets / shrinkThreshold) {

        if (m_numOfElements < initialSize) {
            return;
        }

        int sizeToRehase = initialSize;

        if (m_numOfBuckets / rehaseFactor > initialSize) {
            sizeToRehase = m_numOfBuckets / rehaseFactor;
            rehashToSize(sizeToRehase);
        }
        rehashToSize(sizeToRehase);
    }
}

template<class Key, class T>
void HashTable<Key, T>::rehashToSize(int newTableSize) {

    auto* newBuckets = new AVLTree<Key, T>[newTableSize];

    T* elementsArray = new T[m_numOfElements];
    Key* keysArray = new Key[m_numOfElements];
    elementsToArr(elementsArray);
    keysToArr(keysArray);

    m_numOfBuckets = newTableSize;

    for (int i = 0; i < m_numOfElements; ++i) {
        int bucketIndex = getBucketIndex(keysArray[i]);
        AVLTree<Key, T>& bucket = newBuckets[bucketIndex];
        bucket.insert(keysArray[i], elementsArray[i]);
    }

    delete[] m_buckets;
    delete[] elementsArray;
    delete[] keysArray;
    m_buckets = newBuckets;
}

template<class Key, class T>
HashTable<Key, T>::~HashTable() {
    delete[] m_buckets;
}


#endif //OUR_WET_2_HASHTABLE_H
