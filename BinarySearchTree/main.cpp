#include <bits\stdc++.h>
#include "BinarySearchTree.h"

using namespace std;

int main() {
    BinarySearchTree<Bdata>* BST = new BinarySearchTree<Bdata>();

    for (int i = 1; i <= 6; i++)
    {
        Bdata* e = new Bdata(i, i, i);
        BST->insertBST(e, BST);
    }

    BST->inOrderBST();

    Bdata* tmp = new Bdata(2, 2, 2);

    if (BST->findBST(tmp))
    {
        cout << "1" << endl;
    }
    tmp->x = 7, tmp->y = 7, tmp->z = 7;
    cout << BST->insertBST(tmp, BST) << endl;
    BST->inOrderBST();

    Bdata* tmp1 = new Bdata(5, 5, 5);
    cout << BST->removeBST(tmp1, BST) << endl;
    BST->inOrderBST();
    Bdata* tmp2 = new Bdata(7, 7, 7);

    cout << BST->removeBST(tmp2, BST) << endl;
    BST->inOrderBST();

    return 0;
}