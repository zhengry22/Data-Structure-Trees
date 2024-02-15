#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<ctime>

//#define MYDEBUG

#define TIME_EXPENSE_ESTIMATE 1

#ifdef TIME_EXPENSE_ESTIMATE
#include <chrono>
#endif

#ifdef MYDEBUG
#include<queue>
#include<string>
#endif
#define stature(p) ((p != nullptr) ? p->black_height : 0)
using namespace std;

int my_max(int a, int b) { return a > b ? a : b; }

enum Color {
    BLACK,
    RED,
};

template<typename T>
void my_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
class Node {
public:
    T key;
    int height = 0; // height of tree
    Node* left_child = nullptr;
    Node* right_child = nullptr;
    Node* parent = nullptr;
    int black_height = 0;
    Color node_color = RED; // The node initially inserted is red
    Node(T key, Node* parent = nullptr, Node* left_child = nullptr, Node* right_child = nullptr, int black_height = 0) :
        key(key), parent(parent), left_child(left_child), right_child(right_child), black_height(0) {}
    ~Node() {}
};

template<typename T>
class BinTree {
protected:

public:
    Node<T>* root = nullptr;
#ifdef MYDEBUG
    void BFS() {
        if (this->root == nullptr) return;
        string st;
        cout << endl << "BFS: " << endl;
        queue<Node<T>> myqueue;
        myqueue.push(*root);
        int h = -1;
        while (!myqueue.empty()) {
            Node<T> tmp = myqueue.front();
            myqueue.pop();
            int depth = 0;
            Node<T> ancestor = tmp;
            while (ancestor.parent != nullptr) {
                ancestor = *ancestor.parent;
                depth++;
            }
            if (h != depth) {
                cout << endl;
                h = depth;
            }

            cout << "(" << tmp.key << ", " << tmp.node_color << ", " << tmp.black_height << ", " << ((tmp.left_child == nullptr) ? "^" : "#") << ((tmp.right_child == nullptr) ? "^" : "#") << ") ";
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

    void Traverse(Node<T>* ptr) {
        if (this->root == nullptr) return;
        Node<T>* lc = ptr->left_child;
        Node<T>* rc = ptr->right_child;
        if (lc != nullptr) {
            Traverse(lc);
        }
        cout << "(" << ptr->key << ") ";
        if (rc != nullptr) {
            Traverse(rc);
        }
    }

    void InTraverse() {
        Traverse(this->root);
    }
};

template<typename T>
class BST : public BinTree<T> {
protected:
    Node<T>* _hot = nullptr;
    T search_key = -1;
public:
    BST() {}

    ~BST() {}

    void printSearchKey() {
        cout << this->search_key << endl;
    }

    int getHeight(Node<T>* x) {
        return (x == nullptr) ? -1 : x->height;
    }

    int updateNodeHeight(Node<T>* x) {
        if (x != nullptr) {
            int left_height = getHeight(x->left_child);
            int right_height = getHeight(x->right_child);
            x->height = left_height > right_height ? left_height + 1 : right_height + 1;
            return x->height;
        }
        return -1;
    }

    void updateHeightAbove(Node<T>* x) {
        Node<T>* ptr = x;
        while (ptr != nullptr) {
            updateNodeHeight(ptr);
            ptr = ptr->parent;
        }
    }

    bool HasLChild(Node<T>*& x) {
        if (x == nullptr || x->left_child == nullptr) {
            return false;
        }
        return true;
    }

    bool HasRChild(Node<T>*& x) {
        if (x == nullptr || x->right_child == nullptr) {
            return false;
        }
        return true;
    }

    bool isLChild(Node<T>* a) {
        // If a is not the root, whether a is its father's left child
        if (a->parent != nullptr) {
            if ((a->parent)->left_child == a) {
                return true;
            }
        }
        return false;
    }

    bool isRChild(Node<T>* a) {
        // If a is not the root whether a is its father's right child
        if (a->parent != nullptr) {
            if ((a->parent)->right_child == a) {
                return true;
            }
        }
        return false;
    }

    Node<T>* succ(Node<T>*& x) {
        Node<T>* s = x;
        if (x->right_child != nullptr) {
            s = x->right_child;
            while (HasLChild(s)) {
                s = s->left_child;
            }
        }
        else {
            while (isRChild(s)) {
                s = s->parent;
            }
            s = s->parent;
        }
        return s;
    }

    Node<T>*& search(const T& e) {
        search_key = -1;
        if (this->root == nullptr || this->root->key == e) {
            _hot = nullptr;
            search_key = (this->root == nullptr) ? -1 : this->root->key;
            return this->root;
        }
        if (this->root->key <= e) {
            search_key = this->root->key;
        }
        for (_hot = this->root; ; ) {
            Node<T>*& v = (e < _hot->key) ? _hot->left_child : _hot->right_child;
            if (v == nullptr || e == v->key) {
                if (!(v == nullptr) && v->key <= e) {
                    this->search_key = v->key;
                }
                return v;
            }
            if (v->key <= e) {
                this->search_key = v->key;
            }
            _hot = v;
        }
    }

    Node<T>* insert(const T& e) {
        Node<T>*& x = search(e);
        if (x == nullptr) {
            x = new Node<T>(e, this->_hot);
            // x's info
            if (x->parent != nullptr) {
                // In the OJ, the testcases never reaches here!
                x->parent->key < e ? x->parent->right_child = x : x->parent->left_child = x;
            }
            updateHeightAbove(x);
        }
        return x;
    }

    Node<T>* removeAt(Node<T>*& x, Node<T>*& hot) {
        Node<T>* w = x;
        Node<T>* succ = nullptr;
        if (!HasLChild(x)) succ = x = x->right_child;
        else if (!HasRChild(x)) succ = x = x->left_child;
        else {
            w = this->succ(w); // Not nullptr
            my_swap(x->key, w->key);
            Node<T>* u = w->parent;
            (u == x ? u->right_child : u->left_child) = succ = w->right_child;
        }
        hot = w->parent;
        if (succ != nullptr) { succ->parent = hot; }
        delete w;
        w = nullptr;
        return succ;
    }

    bool remove(const T& e) {
        Node<T>*& x = search(e);
        if (x == nullptr) {
            return false;
        }
        removeAt(x, _hot);
        updateHeightAbove(_hot);
        return true;
    }
};

template<typename T>
class RB_Tree : public BST<T> {
protected:
    bool isBlack(Node<T>* x) {
        // Root nodes are supposed to be black nodes, but the function won't be called during only the first node is inserted.
        if (x == nullptr || x->node_color == BLACK) return true;
        return false;
    }

    bool isRed(Node<T>* x) {
        return (x != nullptr) && (x->node_color == RED);
    }

    int updateHeight(Node<T>* x) {
        return x->black_height = isBlack(x) + my_max(stature(x->left_child), stature(x->right_child));
    }

    Node<T>* connect34(Node<T>* a, Node<T>* b, Node<T>* c, Node<T>* T0, Node<T>* T1, Node<T>* T2, Node<T>* T3) {
        a->left_child = T0; if (T0 != nullptr) T0->parent = a;
        a->right_child = T1; if (T1 != nullptr) T1->parent = a;
        c->left_child = T2; if (T2 != nullptr) T2->parent = c;
        c->right_child = T3; if (T3 != nullptr) T3->parent = c;
        b->left_child = a; a->parent = b;
        b->right_child = c; c->parent = b;
        this->updateNodeHeight(a);
        this->updateNodeHeight(c);
        this->updateNodeHeight(b);
        updateBlackHeight(a);
        updateBlackHeight(c);
        updateBlackHeight(b);
        return b;
    }

    int GetBlackHeight(Node<T>* v) {
        return (v != nullptr) ? v->black_height : 0;
    }

    int updateBlackHeight(Node<T>* x) {
        if (x != nullptr) {
            int left_height = GetBlackHeight(x->left_child);
            int right_height = GetBlackHeight(x->right_child);
            if (x->node_color == BLACK) {
                x->black_height = left_height > right_height ? left_height + 1 : right_height + 1;
            }
            else {
                x->black_height = left_height > right_height ? left_height: right_height;
            }
            return x->black_height;
        }
        return -1;
    }

    Node<T>* rotateAt(Node<T>* v) {
        Node<T>* p = v->parent;
        Node<T>* g = p->parent;
        if (this->isLChild(p)) //zig
            if (this->isLChild(v)) { //zig-zig
                p->parent = g->parent;
                return connect34(v, p, g, v->left_child, v->right_child, p->right_child, g->right_child);
            }
            else { //zig-zag
                v->parent = g->parent;
                return connect34(p, v, g, p->left_child, v->left_child, v->right_child, g->right_child);
            }
        else //zag
            if (this->isRChild(v)) { //zag-zag
                p->parent = g->parent;
                return connect34(g, p, v, g->left_child, p->left_child, v->left_child, v->right_child);
            }
            else {
                v->parent = g->parent;
                return connect34(g, v, p, g->left_child, v->left_child, v->right_child, p->right_child);
            }
    }

    bool isRoot(Node<T>* x) {
        return (x != nullptr && x->parent == nullptr);
    }

    Node<T>*& uncle(Node<T>* x) {
        if (x == nullptr || x->parent == nullptr || x->parent->parent == nullptr) return x;
        if (this->isLChild(x->parent)) {
            return x->parent->parent->right_child;
        }
        else return x->parent->parent->left_child;
    }

    bool BlackHeightUpdated(Node<T>* x) {
        if (GetBlackHeight(x->left_child) == GetBlackHeight(x->right_child)) {
            return true;
        }
        return false;
    }

    void solveDoubleRed(Node<T>* x) {
        if (isRoot(x)) {
            this->root->node_color = BLACK;
            this->root->black_height++;
            return;
        }
        // Else, we discuss the color of p's sibling
        Node<T>* p = x->parent;
        if (isBlack(p)) return; // Which means no need for double red correction
        Node<T>* g = p->parent;
        Node<T>* u = uncle(x);
        if (isBlack(u)) {
            // Case 1 of solve double red
            // if u and p are on the same side:
            if (this->isLChild(x) == this->isLChild(p)) {
                p->node_color = BLACK;
            }
            else { x->node_color = BLACK; }
            // And g's color must turn from Red to Black
            g->node_color = RED;
            Node<T>* gg = g->parent; // gg maybe the root! 
            bool isleft = this->isLChild(g);
            Node<T>* r = rotateAt(x);
            // Connect the two nodes. But we need to judge whether gg is a nullptr, since g may be the root.
            r->parent = gg;
            if (gg != nullptr) {
                isleft ? (gg->left_child = r) : (gg->right_child = r);
            }
            else {
                this->root = r;
            }
        }
        else {
            // Case 2 of solve double red
            p->node_color = BLACK; p->black_height++; // Change p into black node
            u->node_color = BLACK; u->black_height++; // Change u into black node
            g->node_color = RED; // Change g into red node, note that this may cause recursion 
            solveDoubleRed(g);
        }
    }

    void solveDoubleBlack(Node<T>* r) {
        Node<T>* p = r ? r->parent : this->_hot;
        if (p == nullptr) return;
        Node<T>* s = ((r == p->left_child) ? p->right_child : p->left_child);
        if (isBlack(s)) {
            Node<T>* t = nullptr;
            if (isRed(s->right_child)) t = s->right_child;
            if (isRed(s->left_child)) t = s->left_child;
            if (t != nullptr) {
                // If s has at least 1 red child
                Color old_color = p->node_color;
                Node<T>* gg = p->parent;
                bool isleft = this->isLChild(p);
                Node<T>* b = rotateAt(t);
                b->parent = gg;
                if (gg != nullptr) {
                    isleft ? (gg->left_child = b) : (gg->right_child = b);
                }
                else {
                    this->root = b;
                }
                if (this->HasLChild(b)) { b->left_child->node_color = BLACK; updateHeight(b->left_child); }
                if (this->HasRChild(b)) { b->right_child->node_color = BLACK; updateHeight(b->right_child); }
                b->node_color = old_color;
                updateHeight(b);
            }
            else {
                // No red child
                s->node_color = RED;
                s->black_height--;
                if (isRed(p)) {
                    p->node_color = BLACK;
                }
                else {
                    p->black_height--; 
                    solveDoubleBlack(p);
                }
            }
        } 
        else {
            s->node_color = BLACK;
            p->node_color = RED;
            Node<T>* t = this->isLChild(s) ? s->left_child : s->right_child;
            this->_hot = p;
            Node<T>* gg = p->parent;
            bool isleft = this->isLChild(p);
            Node<T>* b = rotateAt(t);
            b->parent = gg;
            if (gg != nullptr) {
                isleft ? (gg->left_child = b) : (gg->right_child = b);
            }
            else {
                this->root = b;
            }
            solveDoubleBlack(r);
        }
    }
public:
    Node<T>* insert(const T& e) {
        Node<T>*& x = this->search(e); 
        if (x != nullptr) return x;
        x = new Node<T>(e, this->_hot, nullptr, nullptr, 0); // Create a new red node
        // 
        Node<T>* xOld = x;
        solveDoubleRed(x); 
        return xOld;
    }

    bool remove(const T& e) {
        Node<T>*& x = this->search(e);
        if (x == nullptr) return false;
        Node<T>* r = this->removeAt(x, this->_hot);
        // We can return directly if it's an empty tree
        if (this->root == nullptr) return true;
        if (this->_hot == nullptr) {
            this->root->node_color = BLACK;
            updateHeight(this->root);
            return true;
        }
        // Consider the deleted node (p) and its right child
        if (BlackHeightUpdated(this->_hot)) return true; // p is red and p->rc is black
        if (isRed(r)) { // p is black and p->rc is red
            r->node_color = BLACK;
            r->black_height++;
            return true;
        }
        // And the situation where p and p->rc are both black
        solveDoubleBlack(r);
        return true;
    }
};

RB_Tree<int> my_tree;

int main() {
#ifdef TIME_EXPENSE_ESTIMATE
	auto start_time = chrono::high_resolution_clock::now();
#endif
    int n;
    cin >> n;
    char sign;
    int key;
    for (int i = 0; i < n; i++) {
        cin >> sign >> key;
        switch (sign) {
        case 'A':
            my_tree.insert(key);
#ifdef MYDEBUG
            cout << endl << "---------------------" << endl;
            my_tree.BFS();
            my_tree.InTraverse();
            cout << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
#endif
            break;
        case 'B':
            my_tree.remove(key);
#ifdef MYDEBUG
            cout << endl << "---------------------" << endl;
            my_tree.BFS();
            my_tree.InTraverse();
            cout << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
#endif
            break;
        case 'C':
            my_tree.search(key);
            my_tree.printSearchKey();
            break;
        default:
            break;
        }

    }
#ifdef TIME_EXPENSE_ESTIMATE
	auto end_time = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
	cout << "Program Exeute Time: " << duration.count() << " us" << endl;
#endif
    return 0;
}