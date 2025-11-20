#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char

};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void fixBalance_insert(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* new_node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void fixBalance_remove(AVLNode<Key, Value>* node, int diff);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
  //want to make right child the new parent


  AVLNode<Key, Value>* RC = node->getRight();
  AVLNode<Key, Value>* parent = node->getParent();

  //making it so that parent of "node" points to right child of node now
  if(parent != NULL){
    if(parent->getRight() == node){
      //node is right child
      parent->setRight(RC);
    }
    else{
      parent->setLeft(RC);
    }
  }
  //saving RC's left child
  AVLNode<Key, Value>* RCLC = RC->getLeft();

  //swapping relationship between original node and its left child
  RC->setParent(parent);
  RC->setLeft(node);
  node->setParent(RC);

  //setting RC's left child to be node's right child
  node->setRight(RCLC);
  if(RCLC != NULL) RCLC->setParent(node);

  if (parent == NULL) {
    this->root_ = RC;
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
  AVLNode<Key, Value>* LC = node->getLeft();
  AVLNode<Key, Value>* parent = node->getParent();

  if(parent != NULL){
    if(parent->getRight() == node){
      //node is right child
      parent->setRight(LC);
    }
    else{
      parent->setLeft(LC);
    }
  }

  AVLNode<Key, Value>* LCRC = LC->getRight();

  LC->setParent(parent);
  LC->setRight(node);
  node->setParent(LC);

  node->setLeft(LCRC);
  if(LCRC != NULL) LCRC->setParent(node);

  if (parent == NULL) {
    this->root_ = LC;
  }
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;
    //is this line necessary?
    //Node<Key, Value> newItemNode(new_item.first, new_item.second, NULL);

    //if root is null, then insert at root
    if(this->root_ == NULL){
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      return;
    }

    //perform binary search and find where the new node could fit in as a leaf
    //after this we will find whether or not the balance of the tree has been altered
    //and do rotations to make it balanced again
    while(current != NULL){
        parent = current;
        Key currentKey = current->getKey();
        if(new_item.first < currentKey){
            current = current->getLeft();
        }
        else if(currentKey == new_item.first){
          current->setValue(new_item.second);
          return;
        }
        else{
            current = current->getRight();
        }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    }
    else {
        parent->setRight(newNode);
    }
    
    //set balances
    if(parent->getBalance() == 0){
        if (parent->getLeft() == newNode) {
            parent->setBalance(-1);
        }
        else {
            parent->setBalance(1);
        }
        fixBalance_insert(parent, newNode);
    }
    else{
        parent->setBalance(0);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixBalance_insert(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* new_node){
  if (parent == NULL || parent->getParent() == NULL) {
    return;
  }

  AVLNode<Key, Value>* grandparent = parent->getParent();

  if (grandparent->getLeft() == parent) {
    // Parent is in left subtree
    grandparent->updateBalance(-1);  // Always update first

    if (grandparent->getBalance() == 0) {
      // Was +1, now 0 - tree got more balanced, stop
      return;
    }
    else if (grandparent->getBalance() == -1) {
      // Was 0, now -1 - height increased, continue up
      fixBalance_insert(grandparent, parent);
    }
    else { // balance == -2
      // Was -1, now -2 - need to rotate
      if (parent->getLeft() == new_node) {
        // Zig-zig case (left-left)
        rotateRight(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
      }
      else {
        // Zig-zag case (left-right)
        rotateLeft(parent);
        rotateRight(grandparent);
        
        if (new_node->getBalance() == -1) {
          parent->setBalance(0);
          grandparent->setBalance(1);
        }
        else if (new_node->getBalance() == 0) {
          parent->setBalance(0);
          grandparent->setBalance(0);
        }
        else { // new_node->getBalance() == 1
          parent->setBalance(-1);
          grandparent->setBalance(0);
        }
        new_node->setBalance(0);
      }
    }
  }
  else {
    // Parent is right child of grandparent
    grandparent->updateBalance(1);  // Already correct

    if (grandparent->getBalance() == 0) {
      return;
    }
    else if (grandparent->getBalance() == 1) {
      fixBalance_insert(grandparent, parent);
    }
    else { // balance == 2
      if (parent->getRight() == new_node) {
        // Zig-zig case (right-right)
        rotateLeft(grandparent);
        parent->setBalance(0);
        grandparent->setBalance(0);
      }
      else {
        // Zig-zag case (right-left)
        rotateRight(parent);
        rotateLeft(grandparent);
        
        if (new_node->getBalance() == 1) {
          parent->setBalance(0);
          grandparent->setBalance(-1);
        }
        else if (new_node->getBalance() == 0) {
          parent->setBalance(0);
          grandparent->setBalance(0);
        }
        else { // new_node->getBalance() == -1
          parent->setBalance(1);
          grandparent->setBalance(0);
        }
        new_node->setBalance(0);
      }
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == NULL) {
        return;
    }

    // If node has two children, swap with predecessor
    if (node->getLeft() != NULL && node->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(node));
        nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* parent = node->getParent();
    int8_t diff = 0;

    // Calculate diff for parent
    if (parent != NULL) {
        if (parent->getLeft() == node) {
            diff = 1;
        }
        else {
            diff = -1;
        }
    }

    // Get the child (at most one)
    AVLNode<Key, Value>* child = node->getLeft();
    if (child == NULL) {
        child = node->getRight();
    }

    // Update parent pointers
    if (child != NULL) {
        child->setParent(parent);
    }

    if (parent == NULL) {
        this->root_ = child;
    }
    else if (parent->getLeft() == node) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }

    delete node;

    // Fix balance
    fixBalance_remove(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixBalance_remove(AVLNode<Key, Value>* node, int diff)
{
    if (node == NULL) {
        return;
    }

    AVLNode<Key, Value>* parent = node->getParent();
    int ndiff = 0;
    
    if (parent != NULL) {
        if (parent->getLeft() == node) {
            ndiff = 1;
        }
        else {
            ndiff = -1;
        }
    }

    if (diff == -1) {
        // Left subtree shrunk
        if (node->getBalance() + diff == -2) {
            AVLNode<Key, Value>* tallerChild = node->getLeft();
            
            if (tallerChild->getBalance() == -1) {
                // Zig-zig case
                rotateRight(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                fixBalance_remove(parent, ndiff);
            }
            else if (tallerChild->getBalance() == 0) {
                // Zig-zig case
                rotateRight(node);
                node->setBalance(-1);
                tallerChild->setBalance(1);
            }
            else { // tallerChild->getBalance() == 1
                // Zig-zag case
                AVLNode<Key, Value>* grandchild = tallerChild->getRight();
                rotateLeft(tallerChild);
                rotateRight(node);
                
                if (grandchild->getBalance() == 1) {
                    node->setBalance(0);
                    tallerChild->setBalance(-1);
                }
                else if (grandchild->getBalance() == 0) {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                }
                else { // grandchild->getBalance() == -1
                    node->setBalance(1);
                    tallerChild->setBalance(0);
                }
                grandchild->setBalance(0);
                fixBalance_remove(parent, ndiff);
            }
        }
        else if (node->getBalance() + diff == -1) {
            node->setBalance(-1);
        }
        else { // node->getBalance() + diff == 0
            node->setBalance(0);
            fixBalance_remove(parent, ndiff);
        }
    }
    else { // diff == 1
        // Right subtree shrunk
        if (node->getBalance() + diff == 2) {
            AVLNode<Key, Value>* tallerChild = node->getRight();
            
            if (tallerChild->getBalance() == 1) {
                // Zig-zig case
                rotateLeft(node);
                node->setBalance(0);
                tallerChild->setBalance(0);
                fixBalance_remove(parent, ndiff);
            }
            else if (tallerChild->getBalance() == 0) {
                // Zig-zig case
                rotateLeft(node);
                node->setBalance(1);
                tallerChild->setBalance(-1);
            }
            else { // tallerChild->getBalance() == -1
                // Zig-zag case
                AVLNode<Key, Value>* grandchild = tallerChild->getLeft();
                rotateRight(tallerChild);
                rotateLeft(node);
                
                if (grandchild->getBalance() == -1) {
                    node->setBalance(0);
                    tallerChild->setBalance(1);
                }
                else if (grandchild->getBalance() == 0) {
                    node->setBalance(0);
                    tallerChild->setBalance(0);
                }
                else { // grandchild->getBalance() == 1
                    node->setBalance(-1);
                    tallerChild->setBalance(0);
                }
                grandchild->setBalance(0);
                fixBalance_remove(parent, ndiff);
            }
        }
        else if (node->getBalance() + diff == 1) {
            node->setBalance(1);
        }
        else { // node->getBalance() + diff == 0
            node->setBalance(0);
            fixBalance_remove(parent, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
