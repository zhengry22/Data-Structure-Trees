#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<ctime>

//#define MYDEBUG

#ifdef MYDEBUG
#include<queue>
#include<string>
#endif
using namespace std;

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
    Node(T key, Node* parent = nullptr, Node* left_child = nullptr, Node* right_child = nullptr):
        key(key), parent(parent), left_child(left_child), right_child(right_child) {}
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

            cout << "(" << tmp.key << ", " << ((tmp.left_child == nullptr) ? "^" : "#") << ((tmp.right_child == nullptr) ? "^" : "#") << ") ";
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

BST<int> my_tree;

int main() {
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
    return 0;
}