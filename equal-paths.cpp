#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int tHeight(Node* root)
{
    if (root == nullptr)
    {
        return 0;
    }

    int lh = tHeight(root->left);
    int rh = tHeight(root->right);

    if (lh >= rh)
    {
        return lh + 1;
    }
    else 
    {
        return rh + 1;
    }
}


bool equalPaths(Node * root)
{
    if (root == nullptr) return true;
    if (root->left != nullptr && root->right == nullptr)
    {
        return equalPaths(root->left);
    }
    if (root->left == nullptr && root->right != nullptr)
    {
        return equalPaths(root->right);
    }
    if (tHeight(root->left) == tHeight(root->right)) 
        return true;

    return false;
}


