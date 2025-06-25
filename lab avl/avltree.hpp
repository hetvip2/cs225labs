/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
template <class K, class V>
typename AVLTree<K, V>::Node* AVLTree<K, V>::getMax(Node* t) const {
    while (t->right != NULL) {
        t = t->right;
    }
    return t;
}

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* r = t->right;
    t->right = r->left;
    r->left = t;

    int leftH = heightOrNeg1(t->left);
    int rightH = heightOrNeg1(t->right);
    t->height = std::max(leftH, rightH) + 1;
    int RL = heightOrNeg1(r->left);
    int RR = heightOrNeg1(r->right);
    r->height = std::max(RL, RR) + 1;

    t = r;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* l = t->left;
    t->left = l->right;
    l->right = t;

    int leftH = heightOrNeg1(t->left);
    int rightH = heightOrNeg1(t->right);
    t->height = std::max(leftH, rightH) + 1;

    int LL = heightOrNeg1(l->left);
    int LR = heightOrNeg1(l->right);
    l->height = std::max(LL, LR) + 1;

    t = l;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);

}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    if (subtree == nullptr) {
        return;
    }
    int leftH = heightOrNeg1(subtree->left);
    int rightH = heightOrNeg1(subtree->right);
    subtree->height = std::max(leftH, rightH) + 1;
    int bal = leftH - rightH;

    if (bal > 1) {
        int LL = heightOrNeg1(subtree->left->left);
        int LR = heightOrNeg1(subtree->left->right);

        if (LL >= LR) {
            rotateRight(subtree);
        } else {
            rotateLeftRight(subtree);
        }

    } else if (bal < -1) {
        int RR = heightOrNeg1(subtree->right->right);
        int RL = heightOrNeg1(subtree->right->left);

        if (RR >= RL) {
            rotateLeft(subtree);
        } else {
            rotateRightLeft(subtree);
        }
    }
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if (subtree == NULL) {
        subtree = new Node(key, value);
    } else if (key < subtree->key) {
        insert(subtree->left, key, value);
    } else if (key > subtree->key) {
        insert(subtree->right, key, value);
    }

    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        // your code here
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);

    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
            Node* prev = getMax(subtree->left);
            swap(subtree, prev);
            remove(subtree->left, key); 
        } else {
            /* one-child remove */
            // your code here
            Node* temp = subtree;
            if (subtree->left != nullptr) {
                subtree = subtree->left;
            } else {
                subtree = subtree->right;
            }
            delete temp;
        }
        // your code here
    }
    if (subtree != NULL) {
        rebalance(subtree);
    } 
}
