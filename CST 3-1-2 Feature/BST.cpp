#include <iostream>
#define MYDEBUG
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

    friend std::ostream& operator<<(std::ostream& os, const Node& obj) {
        /*os << "value: " << obj.value << " sum: " << obj.sum << " w: "
            << obj.w << " x: " << obj.x << " tree_size: " << obj.tree_size <<
            " non_minus_size: " << obj.non_minus_size << " ";
        if (obj.father != nullptr) {
            os << "father: " << obj.father->w << " ";
        } else {
            os << "father: " <<  "ROOT ";
        }
        if (obj.left_child != nullptr) {
            os << "left_child: " << obj.left_child->w << " ";
        } else {
            os << "left_child: " << "null ";
        }
        if (obj.right_child != nullptr) {
            os << "right_child: " << obj.right_child->w << " ";
        }
        else {
            os << "right_child: " << "null ";
        }
        os << endl;*/
        os << "weight is: " << obj.w << " ";
        if (obj.father != nullptr) {
            os << "father: " << obj.father->w << " ";
        }
        else {
            os << "father: " << "ROOT ";
        }
        if (obj.left_child != nullptr) {
            os << "left_child: " << obj.left_child->w << " ";
        }
        else {
            os << "left_child: " << "null ";
        }
        if (obj.right_child != nullptr) {
            os << "right_child: " << obj.right_child->w << " ";
        }
        else {
            os << "right_child: " << "null ";
        }
        os << endl; 
        return os;
    }
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
    Node* container;
    
    int size;
public:
    Node* root = nullptr;
    Pair* nums;
    Node** pointers;
    BST() {
        container = new Node[1000100];
        nums = new Pair[500010];
        root = &container[0];
        pointers = new Node*[500010];
    }

    ~BST() {
        delete[] container;
        delete[] nums;
        delete[] pointers;
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

        Build( l, mid - 1, lt);
        Build( mid + 1, r, rt);

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
            return Search_kth_minus(k, ptr);
        }
    }

    Node* Search_kth_non_minus(int k, Node* ptr) {
        int right = GetNonMinusSize(ptr->right_child);
        if (right >= k) {
            return Search_kth_non_minus(k, ptr->right_child);
        }
        if (right == k - 1) {
            if (ptr->w >= 0) {
                return ptr;
            }
            else {
                return Search_kth_non_minus(1, ptr->left_child);
            }
        }
        else {
            int m = k - right;
            if (ptr->w >= 0) {
                return Search_kth_non_minus(m - 1, ptr->left_child);
            }
            else {
                return Search_kth_non_minus(m, ptr->left_child);
            }
        }
    }

    Node* Search_kth_minus(int k, Node* ptr) {
        int left = GetMinusSize(ptr->left_child);
        if (left >= k) {
            return Search_kth_minus(k, ptr->left_child);
        }
        if (left == k - 1) {
            if (ptr->w < 0) {
                return ptr;
            }
            else {
                return Search_kth_minus(1, ptr->right_child);
            }
        }
        else {
            int m = k - left;
            if (ptr->w < 0) {
                return Search_kth_minus(m - 1, ptr->right_child);
            }
            else {
                return Search_kth_minus(m, ptr->right_child);
            }
        }
    }
};

class SplayTree : public BST {
public:
    void attachAsLC(Node *a, Node *b) {
        // Forget about father pointer first.
        b->left_child = a;
        if (a != nullptr) {
            a->father = b;
        }
    }

    void attachAsRC(Node *a, Node *b) {
        a->right_child = b;
        if (b != nullptr) {
            b->father = a;
        }
    }

    bool isLChild(Node *a) {
        // If a is not the root, whether a is its father's left child
        if (a->father != nullptr) {
            if ((a->father)->left_child == a) {
                return true;
            }
        }
        return false;
    }

    bool isRChild(Node *a) {
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

    void Splay(Node *v) {
        while ((v->father != nullptr) && (v->father->father != nullptr)) {
            Node *p = v->father;
            Node *g = p->father;
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
            Node *p = v->father;
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
        cout << "Splay v" << endl;
        this->Traverse(this->root);
        this->BFS();
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
            cout << "right tree" << endl;
            this->Traverse(this->root);
            this->BFS();
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
        cout << "Search Done" << endl;
        Traverse(this->root);
        BFS();
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
        cout << "Insert done" << endl;
        Traverse(this->root);
        BFS();
    }
};

SplayTree mytree;

int main() {
    int w[SIZE] = { 1, 20, 2, 19, 3, 18, 4, 17, 5, 16, 6, 15, 7, 14, 8, 13, 9, 12, 10, 11 };
    int x[SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    /*int w[SIZE] = { 1000000000, 1000000000, 1000000000, 1000000000, 1000000000, 1000000000, 1000000000, 1000000000, 1000000000, 1000000000};
    int x[SIZE] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};*/
    for (int i = 0; i < SIZE; i++) {
        mytree.nums[i].w = w[i];
        mytree.nums[i].x = x[i];
        mytree.nums[i].rank = i;
    }

    merge_sort(mytree.nums, 0, SIZE - 1);
    for (int i = 0; i < SIZE; i++) {
        cout << mytree.nums[i].w << " ";
    }
    cout << endl;
    mytree.Build(0, SIZE - 1, 0);
    //mytree.PrintTree();
    mytree.Traverse(mytree.root);
    mytree.BFS();
    cout << "1:" << endl;
    mytree.RemoveAndInsert(0, -22);
    mytree.Traverse(mytree.root);
    mytree.BFS();
    cout << "2:" << endl;
    mytree.Splay(mytree.Search_kth(4, mytree.root));
    mytree.Traverse(mytree.root);
    mytree.BFS();
    
    //cout << "2:" << endl;
    //mytree.Splay(mytree.Search_kth(16, mytree.root));
    //cout << endl;
    //mytree.Traverse(mytree.root);
    //mytree.BFS();
    //cout << "3:" << endl;
    //mytree.Splay(mytree.Search_kth(20, mytree.root));
    //cout << endl;
    //mytree.Traverse(mytree.root);
    //mytree.BFS();
    //cout << "4:" << endl;
    //mytree.Splay(mytree.Search_kth(4, mytree.root));
    //cout << endl;
    //mytree.Traverse(mytree.root);
    //mytree.BFS();
    return 0;
}