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
    void rotateLeft(AVLNode<Key, Value>* head);
    void rotateRight(AVLNode<Key, Value>* head);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
  if (BinarySearchTree<Key, Value>::root_ == nullptr)
  {
    BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    //balance will be 0 on the root
    return;
  }
  AVLNode<Key, Value>* prevNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
  AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
  while (currNode != nullptr)
  {
    if (currNode->getKey() == new_item.first)
    {
      currNode->setValue(new_item.second);
      return;
    }
    else if (currNode->getKey() > new_item.first)
    {
      prevNode = currNode;
      currNode = currNode->getLeft();
    }
    else
    {
      prevNode = currNode;
      currNode = currNode->getRight();
    }
  }
  AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, prevNode);
  if (prevNode->getKey() > new_item.first)
  {
    prevNode->setLeft(temp);
    currNode = prevNode->getLeft();
  }
  else
  {
    prevNode->setRight(temp);
    currNode = prevNode->getRight();
  }
  //new node has been created, pointed to by currNode, prevNode is parent
  //now balance

  AVLNode<Key, Value>* parent = currNode->getParent();
  AVLNode<Key, Value>* grandParent = parent->getParent();

  if (parent->getLeft() != nullptr && parent->getRight() != nullptr)
  {
    parent->setBalance(0);
    return;
  }
  if (currNode == parent->getLeft())
  {
    parent->updateBalance(-1);
  }
  else
  {
    parent->updateBalance(1);
  }
  while (grandParent != nullptr)
  {
    if (parent == grandParent->getLeft())
    {
      grandParent->updateBalance(-1);
    }
    else
    {
      grandParent->updateBalance(1);
    }

    if (grandParent->getBalance() == 0)
      break;
    if (grandParent->getBalance() > 1)
    {
      if (currNode == parent->getLeft())
      {
        rotateRight(parent);
      }
      rotateLeft(grandParent);
      break;
    }
    if (grandParent->getBalance() < -1)
    {
      if (currNode == parent->getRight())
      {
        rotateLeft(parent);
      }
      rotateRight(grandParent);
      break;
    }

    currNode = parent;
    parent = grandParent;
    grandParent = grandParent->getParent();
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
  AVLNode<Key, Value>* currNode = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
  if (currNode == nullptr)
  {
    return;
  }
  AVLNode<Key, Value>* parent = currNode->getParent();

  BinarySearchTree<Key, Value>::removeHelp(currNode);
  //begin loop

  if (parent == nullptr)
  {
    return;
  }
  AVLNode<Key, Value>* grandParent = parent->getParent();

  if (parent->getLeft() != nullptr && parent->getRight() != nullptr)
  {
    parent->setBalance(0);
    return;
  }
  if (currNode == parent->getLeft())
  {
    parent->updateBalance(-1);
  }
  else
  {
    parent->updateBalance(1);
  }
  while (grandParent != nullptr)
  {
    if (parent == grandParent->getLeft())
    {
      grandParent->updateBalance(-1);
    }
    else
    {
      grandParent->updateBalance(1);
    }

    if (grandParent->getBalance() > 1)
    {
      if (currNode == parent->getLeft())
      {
        rotateRight(parent);
      }
      rotateLeft(grandParent);
    }
    else if (grandParent->getBalance() < -1)
    {
      if (currNode == parent->getRight())
      {
        rotateLeft(parent);
      }
      rotateRight(grandParent);
    }

    currNode = parent;
    parent = grandParent;
    grandParent = grandParent->getParent();
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* head)
{
  AVLNode<Key, Value>* left = head;
  AVLNode<Key, Value>* top = head->getRight();

  left->setRight(top->getLeft());
  top->setParent(left->getParent());
  top->setLeft(left);
  left->setParent(top);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* head)
{
  AVLNode<Key, Value>* right = head;
  AVLNode<Key, Value>* top = head->getLeft();

  right->setLeft(top->getRight());
  top->setParent(right->getParent());
  top->setRight(right);
  right->setParent(top);
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
