/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    // your code here
    Node*& found = find(root, key);
    return found->value;

}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    // Your code here
    if (subtree == nullptr || subtree->key == key) {
        return subtree;
    }
    if (key >= subtree->key) {
        return find(subtree->right, key);
    }
    return find(subtree->left, key);
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if (subtree == nullptr) {
        subtree = new Node(key, value);
        return;
    }
    if (key < subtree->key) {
        insert(subtree->left, key, value);
    } else if (key > subtree->key) {
        insert(subtree->right, key, value);
    }
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    std::swap(first->key, second->key);
    std::swap(first->value, second->value);
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    if (key < subtree->key) {
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
    } else {
        // leaf
        if (subtree->left == nullptr && subtree->right == nullptr) {
            delete subtree;
            subtree = nullptr;
        }
        // right child
        else if (subtree->left == nullptr) {
            Node* temp = subtree;
            subtree = subtree->right;
            delete temp;
        }
        // left child
        else if (subtree->right == nullptr) {
            Node* temp = subtree;
            subtree = subtree->left;
            delete temp;
        }
        // 2 child
        else {
            Node* iop = subtree->left;
            while (iop->right != nullptr) {
                iop = iop->right;
            }
            swap(subtree, iop);
            remove(subtree->left, key);
        }
    }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K, V> tree;
    for (const auto& b : inList) {
        tree.insert(b.first, b.second);
    }
    return tree;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    std::vector<int> heights(inList.size(), 0);

    int len = inList.size();
    std::vector<int> spots(len);
    
    for (int i = 0; i < len; i++) {
        spots[i] = i;
    }
    std::vector<std::pair<K, V>> perms(len);
    for (int i = 0; i < len; i++) {
        perms[i] = inList[spots[i]];
    }
    BST<K, V> tree = listBuild(perms);
    int h = tree.height();
    if (h >= (int)heights.size()) {
        heights.resize(h + 1);
    }
    heights[h]++;

    while (std::next_permutation(spots.begin(), spots.end())) {
        for (int i = 0; i < len; ++i) {
            perms[i] = inList[spots[i]];
        }
        tree = listBuild(perms);
        h = tree.height();
        if (h >= (int)heights.size()) {
            heights.resize(h + 1);
        }
        heights[h]++;
    }

    return heights;
}