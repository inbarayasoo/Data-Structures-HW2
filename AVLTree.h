#ifndef WE1_AVLTREE_H
#define WE1_AVLTREE_H

#include <iostream>
#include <cassert>

template <class Key, class T>
class AVLTree {
public:
    AVLTree();

    AVLTree(const AVLTree &other);

    AVLTree &operator=(const AVLTree &other);

    ~AVLTree();

    void insert(const Key &key, const T &data);

    bool contains(const Key &key) const;

    T* find(const Key &key) const;

    T* findMin() const;

    void remove(const Key &key);

    bool isEmpty() const;

    void fillElementsInArrayInOrder(T* array, int size) const;

    void fillKeysInArrayInOrder(Key* array, int size) const;

    void clear();

    int getSize() const;

    void printInOrder() const;


private:
    class Node;

    Node *m_root;

    int m_size;

    int height(Node *node) const;

    int getNodeBalanceFactor(Node *node) const;

    void updateHeight(Node *node);

    void rotateLeft(Node *&node);

    void rotateRight(Node *&node);

    void rebalance(Node *&node);

    void insertAux(const Key &key, const T &data, Node *&node);

    Node *findMin(Node *node) const;

    void removeAux(const Key &key, Node *&node);

    void clearAux(Node *&node);

    //TODO: Keep copy if deleted in the middle
    Node *copy(Node *node);

    void fillElementsInArrayInOrderAux(AVLTree::Node *node, T *array, int &index, int size) const;

    void fillKeysInArrayInOrderAux(AVLTree::Node *node, Key* array, int &index, int size) const;

    void printInOrder(Node *node) const;

};



template<class Key, class T>
int AVLTree<Key, T>::getSize() const {
//    assert(m_size >= 0);
    return m_size;
}


template<class Key, class T>
void AVLTree<Key, T>::fillElementsInArrayInOrder(T* array, int size) const {
    int index = 0;
    fillElementsInArrayInOrderAux(m_root, array, index, size);
}

template<class Key, class T>
void AVLTree<Key, T>::fillElementsInArrayInOrderAux(AVLTree::Node *node, T *array, int &index, int size) const {
    if (node == nullptr || index >= size) {
        return;
    }
    fillElementsInArrayInOrderAux(node->m_left, array, index, size);
    if (index < size) {
        array[index++] = node->m_data;
    }
    fillElementsInArrayInOrderAux(node->m_right, array, index, size);
}

template<class Key, class T>
void AVLTree<Key, T>::fillKeysInArrayInOrder(Key* array, int size) const {
    int index = 0;
    fillKeysInArrayInOrderAux(m_root, array, index, size);
}

template<class Key, class T>
void AVLTree<Key, T>::fillKeysInArrayInOrderAux(AVLTree::Node *node, Key* array, int &index, int size) const {
    if (node == nullptr || index >= size) {
        return;
    }
    fillKeysInArrayInOrderAux(node->m_left, array, index, size);
    if (index < size) {
        array[index++] = node->m_key;
    }
    fillKeysInArrayInOrderAux(node->m_right, array, index, size);
}

template<class Key, class T>
AVLTree<Key, T>::AVLTree() :
    m_root(nullptr),
    m_size(0)
    {}


template<class Key, class T>
AVLTree<Key, T>::AVLTree(const AVLTree &other) :
        AVLTree()
{
    m_root = copy(other.m_root);
    m_size = other.m_size;
}

template<class Key, class T>
AVLTree<Key, T>::~AVLTree()
{
    clear();
}


template<class Key, class T>
AVLTree<Key, T> &AVLTree<Key, T>::operator=(const AVLTree &other)
{
    if (this != &other) {
        clear();
        m_root = copy(other.m_root);
        m_size = other.m_size;
    }
    return *this;
}

template<class Key, class T>
void AVLTree<Key, T>::insert(const Key& key, const T &data)
{
    if (contains(key)) {
        return;  // Key already exists, no need to insert
    }

    insertAux(key, data, m_root);
    ++m_size;
}

template<class Key, class T>
bool AVLTree<Key, T>::contains(const Key& key) const
{
    Node* current = m_root;
    while (current != nullptr) {
        if (current->m_key == key) {
            return true;
        }
        else if (key < current->m_key) {
            current = current->m_left;
        }
        else {
            current = current->m_right;
        }
    }
    return false;
}

template<class Key, class T>
T* AVLTree<Key, T>::find(const Key &key) const {
    Node* current = m_root;
    while (current != nullptr) {
        if (current->m_key == key) {
            return &(current->m_data);
        }
        else if (key < current->m_key) {
            current = current->m_left;
        }
        else {
            current = current->m_right;
        }
    }
    return nullptr;
}

template<class Key, class T>
T *AVLTree<Key, T>::findMin() const {
    Node* current = m_root;
    while (current != nullptr && current->m_left != nullptr) {
        current = current->m_left;
    }
    return (current != nullptr) ? &(current->m_data) : nullptr;
}

template<class Key, class T>
void AVLTree<Key, T>::remove(const Key& key)
{
    if (!contains(key)) {
        return;  // Key not found, no need to remove
    }

    removeAux(key, m_root);
    --m_size;
}

template<class Key, class T>
bool AVLTree<Key, T>::isEmpty() const
{
    return m_root == nullptr;
}

template<class Key, class T>
void AVLTree<Key, T>::clear()
{
    clearAux(m_root);
}

template<class Key, class T>
void AVLTree<Key, T>::printInOrder() const
{
    printInOrder(m_root);
    std::cout << std::endl;
}

template<class Key, class T>
int AVLTree<Key, T>::height(AVLTree::Node* node) const
{
    return (node == nullptr) ? -1 : node->m_height;
}

template<class Key, class T>
int AVLTree<Key, T>::getNodeBalanceFactor(AVLTree::Node* node) const
{
    return height(node->m_left) - height(node->m_right);
}

template<class Key, class T>
void AVLTree<Key, T>::updateHeight(AVLTree::Node* node)
{
    node->m_height = 1 + std::max(height(node->m_left), height(node->m_right));
}


template<class Key, class T>
void AVLTree<Key, T>::rotateLeft(AVLTree::Node*& node) {
    Node *rightChild = node->m_right;
    node->m_right = rightChild->m_left;
    rightChild->m_left = node;
    updateHeight(node);
    updateHeight(rightChild);
    node = rightChild;
}

template<class Key, class T>
void AVLTree<Key, T>::rotateRight(AVLTree::Node *&node) {
    Node *leftChild = node->m_left;
    node->m_left = leftChild->m_right;
    leftChild->m_right = node;
    updateHeight(node);
    updateHeight(leftChild);
    node = leftChild;
}

template<class Key, class T>
void AVLTree<Key, T>::rebalance(AVLTree::Node *&node) {
    if (node == nullptr) {
        return;
    }
    int bf = getNodeBalanceFactor(node);

    if (bf == 2) {
        if (getNodeBalanceFactor(node->m_left) < 0) {
            //Preform LR rotation (LR rotation is left rotation and then right rotation)
            rotateLeft(node->m_left);
        }
        //Preform LL rotation (LL rotation is right rotation)
        rotateRight(node);
    } else if (bf == -2) {
        if (getNodeBalanceFactor(node->m_right) > 0) {
            //Preform RL rotation (RL rotation is right rotation and then left rotation)
            rotateRight(node->m_right);
        }
        //Preform RR rotation (RR rotation is left rotation)
        rotateLeft(node);
    }

    updateHeight(node);
}

template<class Key, class T>
void AVLTree<Key, T>::insertAux(const Key& key, const T& data, AVLTree::Node *&node) {
    if (node == nullptr) {
        node = new Node(key, data);
    } else if (key < node->m_key) {
        insertAux(key, data, node->m_left);
    } else {
        insertAux(key, data, node->m_right);
    }
    rebalance(node);
}

template<class Key, class T>
typename AVLTree<Key, T>::Node *AVLTree<Key, T>::findMin(AVLTree::Node *node) const {
    if (node->m_left == nullptr) {
        return node;
    }
    return findMin(node->m_left);
}



template<class Key, class T>
void AVLTree<Key, T>::removeAux(const Key& key, AVLTree::Node *&node) {
    if (node == nullptr) {
        return;
    } else if (key < node->m_key) {
        removeAux(key, node->m_left);
    } else if (key > node->m_key) {
        removeAux(key, node->m_right);
    } else {
        if (node->m_left == nullptr && node->m_right == nullptr) {
            delete node;
            node = nullptr;
        } else if (node->m_left == nullptr) {
            Node *temp = node;
            node = node->m_right;
            delete temp;
        } else if (node->m_right == nullptr) {
            Node *temp = node;
            node = node->m_left;
            delete temp;
        } else {
            Node *minRight = findMin(node->m_right);
            node->m_key = minRight->m_key;
            node->m_data = minRight->m_data;
            removeAux(minRight->m_key, node->m_right);
        }
    }
    rebalance(node);
}



template<class Key, class T>
void AVLTree<Key, T>::clearAux(AVLTree::Node *&node) {
    if (node == nullptr) {
        return;
    }
    clearAux(node->m_left);
    clearAux(node->m_right);
    delete node;
    node = nullptr;
}

template<class Key, class T>
typename AVLTree<Key, T>::Node* AVLTree<Key, T>::copy(AVLTree::Node* node)
{
    if (node == nullptr) {
        return nullptr;
    }

    Node* new_node = new Node(node->m_key, node->m_data);
    new_node->m_left = copy(node->m_left);
    new_node->m_right = copy(node->m_right);
    new_node->m_height = node->m_height;
    return new_node;
}

template<class Key, class T>
void AVLTree<Key, T>::printInOrder(AVLTree::Node *node) const {
    if (node == nullptr) {
        return;
    }
    printInOrder(node->m_left);
    std::cout << node->m_key << " ";
    printInOrder(node->m_right);
}



template <class Key, class T>
class AVLTree<Key, T>::Node {
private:
    Key m_key;
    T m_data;
    Node *m_left;
    Node *m_right;
    int m_height;

    explicit Node(const Key& key, const T &data);
    ~Node() = default;

    friend class AVLTree<Key, T>;
};


template<class Key, class T>
AVLTree<Key, T>::Node::Node(const Key& key, const T& data) :
        m_key(key),
        m_data(data),
        m_left(nullptr),
        m_right(nullptr),
        m_height(0)
        {}



#endif //WE1_AVLTREE_H
