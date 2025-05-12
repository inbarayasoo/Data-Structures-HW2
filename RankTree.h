#ifndef OUR_WET_2_RANKTREE_H
#define OUR_WET_2_RANKTREE_H

#include <iostream>
#include <cassert>


template <class Key, class T>
class RankTree {
public:
    enum class RankTreeError {SUCCESS, ALLOCATION_ERROR, INVALID_INPUT, FAILURE};
    RankTree();

    RankTree(const RankTree &other);

    RankTree &operator=(const RankTree &other);

    ~RankTree();

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

    /************************* Extra functions *************************/
    void addExtra(const Key& key1, const Key& key2, double extra);

    double getRank(const Key& key) const;

    void initExtras();


private:
    class Node;

    enum class Step {NO_STEP, LEFT, RIGHT};

    Node *m_root;

    int m_size;

    int height(Node *node) const;

    int getNodeBalanceFactor(Node *node) const;

    void updateHeight(Node *node);

    void rotateLeft(Node *&node);

    void rotateRight(Node *&node);

    void rebalance(Node *&node);

    void insertAux(const Key &key, const T &data, Node *&node, double extra);

    Node *findMin(Node *node) const;

    void removeAux(const Key &key, Node *&node);

    void clearAux(Node *&node);

    //TODO: Keep copy if deleted in the middle
    Node *copy(Node *node);

    void fillElementsInArrayInOrderAux(RankTree::Node *node, T *array, int &index, int size) const;

    void fillKeysInArrayInOrderAux(RankTree::Node *node, Key* array, int &index, int size) const;

    void printInOrder(Node *node) const;

    Node* getMaxLessThan(const Key& key) const;

    /************************* Extra functions *************************/

    void addExtraAux(const Key& key, double extra, Node*& node, Step previousStep, Step currentStep);

    void lastAddExtraAux(Node*& node, double extra, Step previousStep);

    double getExtraAux(const Key& key, Node* node) const;

    void initExtrasAux(Node* node);

    static Step nextStep(const Key& currentKey, const Key& targetKey);

};

template<class Key, class T>
void RankTree<Key, T>::initExtrasAux(RankTree::Node *node) {
    if (node == nullptr) {
        return;
    }
    initExtrasAux(node->m_left);
    node->m_extra = 0;
    initExtrasAux(node->m_right);
}

template<class Key, class T>
void RankTree<Key, T>::initExtras() {
    initExtrasAux(m_root);
}

template<class Key, class T>
double RankTree<Key, T>::getExtraAux(const Key &key, RankTree::Node *node) const {
    if (node == nullptr) {
        return 0;
    }
    if (node->m_key == key) {
        return node->m_extra;
    }
    if (node->m_key < key) {
        return getExtraAux(key, node->m_right) + node->m_extra;
    }
    else {
        return getExtraAux(key, node->m_left) + node->m_extra;
    }
}

template<class Key, class T>
double RankTree<Key, T>::getRank(const Key &key) const {
    return getExtraAux(key, m_root);
}


template<class Key, class T>
typename RankTree<Key, T>::Step RankTree<Key, T>::nextStep(const Key &currentKey, const Key &targetKey) {
    if (currentKey < targetKey) {
        return Step::RIGHT;
    }
    if (currentKey > targetKey) {
        return Step::LEFT;
    }
    else {
        return Step::NO_STEP;
    }
}


template<class Key, class T>
typename RankTree<Key, T>::Node* RankTree<Key, T>::getMaxLessThan(const Key& key) const {
    Node* current = m_root;
    Node* maxNode = nullptr;
    while (current != nullptr) {
        if (current->m_key < key) {
            maxNode = current;
            current = current->m_right;
        } else {
            current = current->m_left;
        }
    }
    return maxNode;
}

template<class Key, class T>
void RankTree<Key, T>::addExtra(const Key &key1, const Key &key2, double extra) {
//    assert(key1 <= key2);

    if (m_root == nullptr) {
        return;
    }

    Node* maxNodeKey2 = getMaxLessThan(key2);

    if (maxNodeKey2 == nullptr) {
        return;
    }
    Step currentStep = nextStep(m_root->m_key, maxNodeKey2->m_key);
    addExtraAux(maxNodeKey2->m_key, extra, m_root, Step::NO_STEP, currentStep);

    Node* maxNodeKey1 = getMaxLessThan(key1);
    if (maxNodeKey1 == nullptr) {
        return;
    }
    currentStep = nextStep(m_root->m_key, maxNodeKey1->m_key);
    addExtraAux(maxNodeKey1->m_key, -extra, m_root, Step::NO_STEP, currentStep);
}



template<class Key, class T>
void RankTree<Key, T>::addExtraAux(const Key &key, double extra, RankTree::Node *&node,
                                   Step previousStep, Step currentStep) {
//    assert(node != nullptr);
    if (currentStep == Step::NO_STEP) {
        lastAddExtraAux(node, extra, previousStep);
        return;
    }

    else if (currentStep == Step::RIGHT) {
        Step stepTo = nextStep(node->m_right->m_key, key);
        if (previousStep != Step::RIGHT) {
            node->m_extra += extra;
        }
        addExtraAux(key, extra, node->m_right, currentStep, stepTo);
        return;
    }

//    assert(currentStep == Step::LEFT);
    Step stepTo = nextStep(node->m_left->m_key, key);
    if (previousStep == Step::RIGHT) {
        node->m_extra -= extra;
    }
    addExtraAux(key, extra, node->m_left, currentStep, stepTo);
}

template<class Key, class T>
void RankTree<Key, T>::lastAddExtraAux(RankTree::Node *&node, double extra, Step previousStep) {

    if (previousStep != Step::RIGHT) {
        node->m_extra += extra;
    }

    if (node->m_right != nullptr) {
        node->m_right->m_extra -= extra;
    }
}


template<class Key, class T>
int RankTree<Key, T>::getSize() const {
//    assert(m_size >= 0);
    return m_size;
}


template<class Key, class T>
void RankTree<Key, T>::fillElementsInArrayInOrder(T* array, int size) const {
    int index = 0;
    fillElementsInArrayInOrderAux(m_root, array, index, size);
}

template<class Key, class T>
void RankTree<Key, T>::fillElementsInArrayInOrderAux(RankTree::Node *node, T *array, int &index, int size) const {
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
void RankTree<Key, T>::fillKeysInArrayInOrder(Key* array, int size) const {
    int index = 0;
    fillKeysInArrayInOrderAux(m_root, array, index, size);
}

template<class Key, class T>
void RankTree<Key, T>::fillKeysInArrayInOrderAux(RankTree::Node *node, Key* array, int &index, int size) const {
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
RankTree<Key, T>::RankTree() :
        m_root(nullptr),
        m_size(0)
{}


template<class Key, class T>
RankTree<Key, T>::RankTree(const RankTree &other) :
        RankTree()
{
    m_root = copy(other.m_root);
    m_size = other.m_size;
}

template<class Key, class T>
RankTree<Key, T>::~RankTree()
{
    clear();
}


template<class Key, class T>
RankTree<Key, T> &RankTree<Key, T>::operator=(const RankTree &other)
{
    if (this != &other) {
        clear();
        m_root = copy(other.m_root);
        m_size = other.m_size;
    }
    return *this;
}

template<class Key, class T>
void RankTree<Key, T>::insert(const Key& key, const T &data)
{
    if (contains(key)) {
        return;  // Key already exists, no need to insert
    }

    insertAux(key, data, m_root, 0);
    ++m_size;
}

template<class Key, class T>
bool RankTree<Key, T>::contains(const Key& key) const
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
T* RankTree<Key, T>::find(const Key &key) const {
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
T *RankTree<Key, T>::findMin() const {
    Node* current = m_root;
    while (current != nullptr && current->m_left != nullptr) {
        current = current->m_left;
    }
    return (current != nullptr) ? &(current->m_data) : nullptr;
}

template<class Key, class T>
void RankTree<Key, T>::remove(const Key& key)
{
    if (!contains(key)) {
        return;  // Key not found, no need to remove
    }

    removeAux(key, m_root);
    --m_size;
}

template<class Key, class T>
bool RankTree<Key, T>::isEmpty() const
{
    return m_root == nullptr;
}

template<class Key, class T>
void RankTree<Key, T>::clear()
{
    clearAux(m_root);
}

template<class Key, class T>
void RankTree<Key, T>::printInOrder() const
{
    printInOrder(m_root);
    std::cout << std::endl;
}

template<class Key, class T>
int RankTree<Key, T>::height(RankTree::Node* node) const
{
    return (node == nullptr) ? -1 : node->m_height;
}

template<class Key, class T>
int RankTree<Key, T>::getNodeBalanceFactor(RankTree::Node* node) const
{
    return height(node->m_left) - height(node->m_right);
}

template<class Key, class T>
void RankTree<Key, T>::updateHeight(RankTree::Node* node)
{
    node->m_height = 1 + std::max(height(node->m_left), height(node->m_right));
}


template<class Key, class T>
void RankTree<Key, T>::rotateLeft(RankTree::Node*& node) {
    Node* rightChild = node->m_right;
    Node* leftChildOfRightChild = rightChild->m_left;

    if (leftChildOfRightChild != nullptr) {
        leftChildOfRightChild->m_extra += rightChild->m_extra;
    }

    rightChild->m_extra += node->m_extra;
    node->m_extra -= rightChild->m_extra;

    node->m_right = rightChild->m_left;
    rightChild->m_left = node;
    updateHeight(node);
    updateHeight(rightChild);
    node = rightChild;
}

template<class Key, class T>
void RankTree<Key, T>::rotateRight(RankTree::Node *&node) {

    Node* leftChild = node->m_left;

    Node* rightChildOfLeftChild = leftChild->m_right;

    if (rightChildOfLeftChild != nullptr) {
        rightChildOfLeftChild->m_extra += leftChild->m_extra;
    }

    leftChild->m_extra += node->m_extra;
    node->m_extra -= leftChild->m_extra;

    node->m_left = leftChild->m_right;
    leftChild->m_right = node;
    updateHeight(node);
    updateHeight(leftChild);
    node = leftChild;
}

template<class Key, class T>
void RankTree<Key, T>::rebalance(RankTree::Node *&node) {
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
void RankTree<Key, T>::insertAux(const Key& key, const T& data, RankTree::Node *&node, double extra) {
    if (node == nullptr) {
        node = new Node(key, data, extra);
    } else if (key < node->m_key) {
        insertAux(key, data, node->m_left, extra - node->m_extra);
    } else {
        insertAux(key, data, node->m_right, extra - node->m_extra);
    }
    rebalance(node);
}

template<class Key, class T>
typename RankTree<Key, T>::Node *RankTree<Key, T>::findMin(RankTree::Node *node) const {
    if (node->m_left == nullptr) {
        return node;
    }
    return findMin(node->m_left);
}



template<class Key, class T>
void RankTree<Key, T>::removeAux(const Key& key, RankTree::Node *&node) {
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
void RankTree<Key, T>::clearAux(RankTree::Node *&node) {
    if (node == nullptr) {
        return;
    }
    clearAux(node->m_left);
    clearAux(node->m_right);
    delete node;
    node = nullptr;
}

template<class Key, class T>
typename RankTree<Key, T>::Node* RankTree<Key, T>::copy(RankTree::Node* node)
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
void RankTree<Key, T>::printInOrder(RankTree::Node *node) const {
    if (node == nullptr) {
        return;
    }
    printInOrder(node->m_left);
    std::cout << node->m_key << " ";
    printInOrder(node->m_right);
}



template <class Key, class T>
class RankTree<Key, T>::Node {
private:
    Key m_key;
    T m_data;
    Node *m_left;
    Node *m_right;
    int m_height;
    double m_extra;

    explicit Node(const Key& key, const T &data, double extra = 0);
    ~Node() = default;

    friend class RankTree<Key, T>;
};


template<class Key, class T>
RankTree<Key, T>::Node::Node(const Key& key, const T& data, double extra) :
        m_key(key),
        m_data(data),
        m_left(nullptr),
        m_right(nullptr),
        m_height(0),
        m_extra(extra)
{}





#endif //OUR_WET_2_RANKTREE_H
