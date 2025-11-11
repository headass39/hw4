#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int checkHeight(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr) return true;

    return checkHeight(root) != -1;
}

int checkHeight(Node* root) {
    if (root == nullptr) return 0; 
    int leftHeight = checkHeight(root->left);
    int rightHeight = checkHeight(root->right);
    //check for previous failure
    if (leftHeight == -1 || rightHeight == -1)
        return -1;

    //check for failure
    if (root->left != nullptr && root->right != nullptr && leftHeight != rightHeight)
        return -1;

    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
}

