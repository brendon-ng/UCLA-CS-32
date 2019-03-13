#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct Node {
        char label;
        std::vector<ValueType> values;
        std::vector<Node*> children;
    };
    
    Node* root;
    
    void destructTree(Node* cur);    // Destructor recursive helper function
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly, typename Trie<ValueType>::Node* cur, int keyIndex) const;    // Find recursive helper function
};

// Constructor
template<typename ValueType>
Trie<ValueType>::Trie() {
    root = new Node();
}

// Destructor
template<typename ValueType>
Trie<ValueType>::~Trie() {
    destructTree(root);
}

// Reset
template<typename ValueType>
void Trie<ValueType>::reset() {
    destructTree(root);
    root = new Node();
}

// Insert
template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
    Node* cur = root;
    
    // Traverse the tree according to key
    for(int i=0; i < key.size(); i++){
        int c =0;
        int children = cur->children.size();
        for(; c < children; c++){
            if (cur->children[c]->label == key[i]) {
                cur = cur->children[c];
                break;
            }
        }
        
        // If no children node with the correct letter label
        if(c >= children) {
            // Insert new node into tree
            Node* newNode = new Node;
            newNode->label = key[i];
            cur->children.push_back(newNode);
            cur = newNode;
        }
    }
    
    // Push the value onto the correct node
    cur->values.push_back(value);
}

// Find
template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
    std::vector<ValueType> blank; // Create blank vector to return in special cases
    
    if(key.size() == 0)
        return blank;
    
    // First character must match
    for(int i=0; i < root->children.size(); i++){
        if(root->children[i]->label == key[0])
            return find(key, exactMatchOnly, root->children[i], 1);
    }
    return blank;
    
}

// Find helper function
template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly, typename Trie<ValueType>::Node* cur, int keyIndex) const {
    
    // If we have reached our target node, return the values in this node
    if(keyIndex == key.size()){
        return cur->values;
    }
    
    std::vector<ValueType> returnVec; // Initialize vector of things to return
    for(int i=0; i < cur->children.size(); i++){
        // If character matches the child node, continue to traverse down the tree recursively
        if(cur->children[i]->label == key[keyIndex]){
            std::vector<ValueType> childVec = find(key, exactMatchOnly, cur->children[i], keyIndex+1);
            returnVec.insert(returnVec.end(), childVec.begin(), childVec.end());
        }
        else if (!exactMatchOnly){ // If ONE mis-match is allowed
            // Continue to traverse but do not allow any more mis-matches
            std::vector<ValueType> childVec2 = find(key, false, cur->children[i], keyIndex+1);
            returnVec.insert(returnVec.end(), childVec2.begin(), childVec2.end());
        }
        // If neither condition applies, returnVec remains an empty vector
    }
    
    return returnVec;
    
}


// Destructor Helper Function
template<typename ValueType>
void Trie<ValueType>::destructTree(Node* cur) {
    // Use recursive post order traversal
    for(int i=0; i < cur->children.size(); i++){
        destructTree(cur->children[i]);
    }
    
    delete cur;
}

#endif // TRIE_INCLUDED
