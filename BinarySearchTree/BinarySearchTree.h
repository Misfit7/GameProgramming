#pragma once
#include <bits/stdc++.h>

using namespace std;

struct Bdata
{
    int x, y, z;
    Bdata() :x(0), y(0), z(0) {
    };
    Bdata(int x, int y, int z) :x(0), y(0), z(0) {
        this->x = x;
        this->y = y;
        this->z = z;
    };
    bool eq(const Bdata& b);
    bool cmp(const Bdata& b);
    void print();
};

template<typename T>
class BinarySearchTree
{
public:
    BinarySearchTree();
    BinarySearchTree(T* d, BinarySearchTree<T>* l = nullptr, BinarySearchTree<T>* r = nullptr, BinarySearchTree<T>* p = nullptr);
    bool insertBST(T* e, BinarySearchTree<T>*& b);
    bool insertBST(T* e, BinarySearchTree<T>*& b, BinarySearchTree<T>* p);
    bool removeBST(T* e, BinarySearchTree<T>*& b);
    void inOrderBST();
    BinarySearchTree<T>* findBST(T* v);
    ~BinarySearchTree() {};

private:
    T* data;
    BinarySearchTree<T>* left, * right, * parent;
};

template<typename T>
BinarySearchTree<T>::BinarySearchTree() : left(nullptr), right(nullptr), parent(nullptr)
{
    data = new T();
}

template<typename T>
BinarySearchTree<T>::BinarySearchTree(T* d, BinarySearchTree<T>* l, BinarySearchTree<T>* r, BinarySearchTree<T>* p) : data(d), left(l), right(r), parent(p)
{

}

template<typename T>
bool BinarySearchTree<T>::insertBST(T* e, BinarySearchTree<T>*& b, BinarySearchTree<T>* p)
{
    if (b == nullptr) {
        b = new BinarySearchTree<T>(e);
        b->parent = p;
        if (b == nullptr)
        {
            cout << "Memory allocation failed!" << endl;
            exit(1);
        }
        return true;
    }
    else if (b->data->cmp(*e)) {
        b->insertBST(e, b->left, b);
    }
    else if (!b->data->cmp(*e)) {
        b->insertBST(e, b->right, b);
    }
    else
    {
        return false;
    }
}

template<typename T>
bool BinarySearchTree<T>::insertBST(T* e, BinarySearchTree<T>*& b)
{
    return insertBST(e, b, nullptr);
}

template<typename T>
bool BinarySearchTree<T>::removeBST(T* e, BinarySearchTree<T>*& b)
{
    BinarySearchTree<T>* tmp, * res;
    res = b->findBST(e);
    if (res)
    {
        if (res->left && res->right)
        {
            tmp = res->right;
            while (tmp->left)
            {
                tmp = tmp->left;
            }
            res->data = tmp->data;
            tmp->parent->left = nullptr;
            delete tmp;

        }
        else if (!res->left && !res->right)
        {
            res->parent->left == res ? res->parent->left = nullptr : res->parent->right = nullptr;
            delete res;
        }
        else if (res->right) {
            res->parent->right = res->right;
        }
        else if (res->left)
        {
            res->parent->left = res->left;
        }
        return true;
    }
    else
    {
        return false;
    }
}

template<typename T>
void BinarySearchTree<T>::inOrderBST()
{
    if (this)
    {
        this->left->inOrderBST();
        this->data->print();
        this->right->inOrderBST();
    }
}

template<typename T>
BinarySearchTree<T>* BinarySearchTree<T>::findBST(T* v)
{
    if (this == nullptr)
    {
        return nullptr;
    }
    else if (this->data->eq(*v))
    {
        return this;
    }
    else if (this->data->cmp(*v))
    {
        return this->left->findBST(v);
    }
    else if (!this->data->cmp(*v))
    {
        return this->right->findBST(v);
    }
}

bool Bdata::cmp(const Bdata& b)
{
    if (this->x > b.x && this->y > b.y && this->z > b.z)
    {
        return true;
    }
    return false;
}

bool Bdata::eq(const Bdata& b)
{
    if (this->x == b.x && this->y == b.y && this->z == b.z)
    {
        return true;
    }
    return false;
}

void Bdata::print()
{
    if (this) {
        cout << this->x << " " << this->y << " " << this->z << endl;
    }
}
