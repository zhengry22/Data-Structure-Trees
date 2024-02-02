#include "feature.h"
#include <stdio.h>
#include <cstdlib>

#include <iostream>
//#define MYDEBUG
#ifdef MYDEBUG
#include <queue>
#endif
using namespace std;
#define SIZE 20

int abs(int w) {
    return w > 0 ? w : -w;
}

struct Node {
    int value = 0;
    long long sum = 0;
    int w;
    int x;
    int tree_size = 0;
    int non_minus_size = 0;
    Node* father = nullptr; // Default that it has no father
    Node* left_child = nullptr; // Default no left child
    Node* right_child = nullptr; // Default no right child
};

struct Pair {
    int w;
    int x;
    int rank;
};

void merge_sort(Pair* num, int l, int r)
{
    int middle = (l + r) / 2, lp = l, rp = middle + 1;
    Pair* tmp;
    //when to return 
    if (l == r)
        return;
    //first, merge the left and right part
    merge_sort(num, l, middle);
    merge_sort(num, middle + 1, r);
    //create a new array to dispose number
    tmp = new Pair[r - l + 1];
    int flag = 0;
    while ((lp <= middle) && (rp <= r))
    {
        if (abs(num[lp].w) <= abs(num[rp].w)) // sort according to |w|
        {
            tmp[flag] = num[lp];
            lp++;
        }
        else
        {
            tmp[flag] = num[rp];
            rp++;
        }
        flag++;
    }
    if (lp <= middle)
    {
        for (int i = lp; i <= middle; i++)
        {
            tmp[flag] = num[i];
            flag++;
        }
    }
    if (rp <= r)
    {
        for (int i = rp; i <= r; i++)
        {
            tmp[flag] = num[i];
            flag++;
        }
    }
    //put the numbers in tmp into num
    for (int i = 0, j = l; i < r - l + 1; i++, j++)
    {
        num[j] = tmp[i];
    }
    delete[] tmp;
}


class BST {
private:
    Node container[1000100];

    int size;
public:
    Node* root = nullptr;
    Pair nums[500010];
    Node* pointers[500010];
    BST() {
        root = &container[0];
    }

    ~BST() {
        root = nullptr;
    }

    void Build(int l, int r, int index) {
        // Start with a heap structure
        int mid = (l + r) >> 1;

        if (r - l < 0) {
            return;
        }
        container[index].w = nums[mid].w;
        int w = container[index].w;
        container[index].x = nums[mid].x;
        pointers[nums[mid].rank] = &container[index];
        if ((index - 1) >> 1 >= 0) {
            container[index].father = &container[(index - 1) >> 1];
        }
        container[index].value = w > 0 ? w : -w;
        size = index;
        if (r - l < 1) {
            container[index].sum = (long long)w * (long long)container[index].x;
            container[index].tree_size = 1;
            container[index].non_minus_size = w >= 0 ? 1 : 0;
            return;
        }

        int lt = (index << 1) + 1, rt = (index << 1) + 2;

        Build(l, mid - 1, lt);
        Build(mid + 1, r, rt);

        container[index].sum = container[lt].sum + container[rt].sum + ((long long)container[index].w * (long long)container[index].x);
        container[index].tree_size = container[lt].tree_size + container[rt].tree_size + 1;
        container[index].non_minus_size = container[lt].non_minus_size + container[rt].non_minus_size;
        if (container[lt].tree_size != 0) {
            container[index].left_child = &container[lt];
        }
        if (container[rt].tree_size != 0) {
            container[index].right_child = &container[rt];
        }
        if (container[index].w >= 0) {
            container[index].non_minus_size++;
        }
    }

    void PrintTree() {
        int sign = 1;
        for (int i = 0; i <= size; i++) {
            cout << "index: " << i << ", " << container[i];
            if (i + 1 == sign) {
                cout << endl;
                sign <<= 1;
                sign += 1;
            }
        }
    }

    int Getsize(Node* ptr) {
        if (ptr == nullptr) {
            return 0;
        }
        else {
            return ptr->tree_size;
        }
    }

    int GetNonMinusSize(Node* ptr) {
        if (ptr == nullptr) {
            return 0;
        }
        else {
            return ptr->non_minus_size;
        }
    }

    int GetMinusSize(Node* ptr) {
        if (ptr == nullptr) {
            return 0;
        }
        else {
            return ptr->tree_size - ptr->non_minus_size;
        }
    }

    long long GetSum(Node* ptr) {
        if (ptr == nullptr) {
            return 0;
        }
        else {
            return ptr->sum;
        }
    }

    void Traverse(Node* ptr) {
        Node* lc = ptr->left_child;
        Node* rc = ptr->right_child;
        if (lc != nullptr) {
            Traverse(lc);
        }
        cout << "(" << ptr->w << "," << ptr->x << ") ";
        if (rc != nullptr) {
            Traverse(rc);
        }
    }

#ifdef MYDEBUG
    void BFS() {
        string st;
        cout << endl << "BFS: " << endl;
        queue<Node> myqueue;
        myqueue.push(*root);
        int h = -1;
        while (!myqueue.empty()) {
            Node tmp = myqueue.front();
            myqueue.pop();
            int depth = 0;
            Node ancestor = tmp;
            while (ancestor.father != nullptr) {
                ancestor = *ancestor.father;
                depth++;
            }
            if (h != depth) {
                cout << endl;
                h = depth;
            }

            cout << "(" << tmp.w << "," << tmp.x << "," << tmp.sum << "," << tmp.tree_size << "," << tmp.non_minus_size << ","
                << ((tmp.left_child == nullptr) ? "^" : "#") << ((tmp.right_child == nullptr) ? "^" : "#") << ") ";
            if (tmp.left_child != nullptr) {
                myqueue.push(*tmp.left_child);
            }
            if (tmp.right_child != nullptr) {
                myqueue.push(*tmp.right_child);
            }
        }
        cout << endl;
    }
#endif

    Node* Search_weight(int obj, Node* ptr) {
        if (obj == ptr->value) {
            return ptr;
        }
        if (obj < ptr->value && ptr->left_child != nullptr) {
            return Search_weight(obj, ptr->left_child);
        }
        if (obj > ptr->value && ptr->right_child != nullptr) {
            return Search_weight(obj, ptr->right_child);
        }
        return ptr; // Search failed
    }

    Node* Search_kth_abs(int k, Node* ptr) {
        int right = Getsize(ptr->right_child);
        if (right >= k) {
            return Search_kth_abs(k, ptr->right_child);
        }
        if (right == k - 1) {
            return ptr;
        }
        return Search_kth_abs(k - right - 1, ptr->left_child);
    }

    Node* Search_kth(int k, Node* ptr) {
        //First determine whether the kth number is non_minus or not
        if (k <= ptr->non_minus_size) {
            // >= 0
            return Search_kth_non_minus(k, ptr);
        }
        else {
            // < 0
            int minus = ptr->tree_size - ptr->non_minus_size; // number of minus numbers
            return Search_kth_minus(k-ptr->non_minus_size, ptr);
        }
    }

    Node* Search_kth_non_minus(int k, Node* ptr) {
        int right = GetNonMinusSize(ptr->right_child);
        if (right >= k) {
            return Search_kth_non_minus(k, ptr->right_child);
        }
        if (right == k - 1 && ptr->w >= 0) {
            return ptr;
        }
        int m = k - right;
        m = (ptr->w >= 0) ? m - 1 : m;
        return Search_kth_non_minus(m, ptr->left_child);
    }

    Node* Search_kth_minus(int k, Node* ptr) {
        int left = GetMinusSize(ptr->left_child);
        if (left >= k) {
            return Search_kth_minus(k, ptr->left_child);
        }
        if (left == k - 1 && ptr->w < 0) {
            return ptr;
        }
        int m = k - left;
        m = (ptr->w < 0) ? m - 1 : m;
        return Search_kth_minus(m, ptr->right_child);
    }
};

class SplayTree : public BST {
public:
    void attachAsLC(Node* a, Node* b) {
        // Forget about father pointer first.
        b->left_child = a;
        if (a != nullptr) {
            a->father = b;
        }
    }

    void attachAsRC(Node* a, Node* b) {
        a->right_child = b;
        if (b != nullptr) {
            b->father = a;
        }
    }

    bool isLChild(Node* a) {
        // If a is not the root, whether a is its father's left child
        if (a->father != nullptr) {
            if ((a->father)->left_child == a) {
                return true;
            }
        }
        return false;
    }

    bool isRChild(Node* a) {
        // If a is not the root whether a is its father's right child
        if (a->father != nullptr) {
            if ((a->father)->right_child == a) {
                return true;
            }
        }
        return false;
    }

    void UpdateNode(Node* ptr) {
        //tree_size
        ptr->tree_size = Getsize(ptr->left_child) + Getsize(ptr->right_child) + 1;
        //non_minus_size
        ptr->non_minus_size = (ptr->w >= 0) ? GetNonMinusSize(ptr->left_child) + GetNonMinusSize(ptr->right_child) + 1
            : GetNonMinusSize(ptr->left_child) + GetNonMinusSize(ptr->right_child);
        //sum
        ptr->sum = GetSum(ptr->left_child) + GetSum(ptr->right_child) + (long long)ptr->w * (long long)ptr->x;
    }

    void Splay(Node* v) {
        while ((v->father != nullptr) && (v->father->father != nullptr)) {
            Node* p = v->father;
            Node* g = p->father;
            if (isLChild(v)) {
                if (isLChild(v->father)) {
                    this->attachAsLC(p->right_child, g);
                    this->attachAsLC(v->right_child, p);
                    v->father = g->father;
                    if (v->father != nullptr) {
                        if (v->father->left_child == g) {
                            v->father->left_child = v;
                        }
                        else {
                            v->father->right_child = v;
                        }
                    }
                    this->attachAsRC(p, g);
                    this->attachAsRC(v, p);
                }
                else {
                    this->attachAsLC(v->right_child, p);
                    this->attachAsRC(g, v->left_child);
                    v->father = g->father;
                    if (v->father != nullptr) {
                        if (v->father->left_child == g) {
                            v->father->left_child = v;
                        }
                        else {
                            v->father->right_child = v;
                        }
                    }
                    this->attachAsLC(g, v);
                    this->attachAsRC(v, p);
                }
            }
            else {
                if (isLChild(v->father)) {
                    this->attachAsRC(p, v->left_child);
                    this->attachAsLC(v->right_child, g);
                    v->father = g->father;
                    if (v->father != nullptr) {
                        if (v->father->left_child == g) {
                            v->father->left_child = v;
                        }
                        else {
                            v->father->right_child = v;
                        }
                    }
                    this->attachAsRC(v, g);
                    this->attachAsLC(p, v);
                }
                else {
                    this->attachAsRC(g, p->left_child);
                    this->attachAsRC(p, v->left_child);
                    v->father = g->father;
                    if (v->father != nullptr) {
                        if (v->father->left_child == g) {
                            v->father->left_child = v;
                        }
                        else {
                            v->father->right_child = v;
                        }
                    }
                    this->attachAsLC(g, p);
                    this->attachAsLC(p, v);
                }
            }
            UpdateNode(g);
            UpdateNode(p);
            UpdateNode(v);
        }
        // If v's father is the root, then splay(single rotation) one more time.
        if ((v->father != nullptr) && (v->father->father == nullptr)) {
            // Splay only once
            Node* p = v->father;
            if (isLChild(v)) {
                this->attachAsLC(v->right_child, p);
                v->father = p->father;
                this->attachAsRC(v, p);
            }
            else {
                this->attachAsRC(p, v->left_child);
                v->father = p->father;
                this->attachAsLC(p, v);
            }
            UpdateNode(p);
            UpdateNode(v);
        }
        this->root = v;
    }

    void RemoveAndInsert(int rank, int obj) {
        // Remove
        Node* v = pointers[rank];
        Splay(v);
        Node* left = v->left_child;
        Node* right = v->right_child;
        if (left != nullptr) {
            left->father = nullptr;
            v->left_child = nullptr;
        }
        if (right != nullptr) {
            right->father = nullptr;
            v->right_child = nullptr;
            Splay(Search_kth_abs(right->tree_size, right));
            this->root->left_child = left;
            if (left != nullptr) {
                left->father = this->root;
            }
        }
        else {
            this->root = left;
        }
        // Insert
        int absobj = abs(obj);
        if (this->root == nullptr) {
            this->root = v;
            v->w = obj;
            v->value = absobj;
            v->father = nullptr;
            UpdateNode(v);
            return;
        }
        Node* newptr = Search_weight(absobj, this->root);
        Splay(newptr);
        UpdateNode(newptr);
        UpdateNode(v);
        // Replace v with obj
        v->w = obj;
        v->value = absobj;
        if (absobj > newptr->value) {
            attachAsLC(newptr, v);
            attachAsRC(v, newptr->right_child);
            newptr->right_child = nullptr;
            v->father = nullptr;
            this->root = v;
        }
        else {
            attachAsRC(v, newptr);
            attachAsLC(newptr->left_child, v);
            newptr->left_child = nullptr;
            v->father = nullptr;
            this->root = v;
        }
        UpdateNode(newptr);
        UpdateNode(v);
    }
};

SplayTree mytree;

void init(int n, const int *w, const int *x){
    // nothing to init
    for (int i = 0; i < n; i++) {
        mytree.nums[i].w = w[i+1];
        mytree.nums[i].x = x[i+1];
        mytree.nums[i].rank = i;
    }
    merge_sort(mytree.nums, 0, n-1);
    mytree.Build(0, n-1, 0);
    // mytree.Traverse(mytree.root);
    // mytree.BFS();
}
	
void modify_weight(int i, int w){
    mytree.RemoveAndInsert(i - 1, w);
     //mytree.Traverse(mytree.root);
     //mytree.BFS();
}
	
void modify_value(int k, int x){
    Node *a = mytree.Search_kth(k, mytree.root);
    a->x = x;
    mytree.Splay(a);
     //mytree.Traverse(mytree.root);
     //mytree.BFS();
}

long long calculate(int k){
    if (k == mytree.root->tree_size) {
        return mytree.root->sum;
    }
    mytree.Splay(mytree.Search_kth_abs(k+1, mytree.root));
     //mytree.Traverse(mytree.root);
     //mytree.BFS();
    return mytree.root->right_child->sum;
}