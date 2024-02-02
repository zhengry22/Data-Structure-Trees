#include<iostream>
using namespace std;

struct Node {
    int weight;

    // Below are the elements of UnionFindAndLeftHeap
    int unionfind_size = 1; // Size of union find
    // Pointer system for UnionFindAndLeftHeap
    Node *unionfind_ptr = this;

    // Below are the elements of LeftHeap
    Node *heap_root = this; // This points to the heap root
    Node *heap_parent = this; // Default the parent is itself
    Node *heap_left_child = nullptr;
    Node *heap_right_child = nullptr;
    int heap_size = 1; // Default each heap is sized 1 from the beginning
    int npl = 1; // Default it has no left or right child
};

template<typename T>
void my_swap(T* a, T* b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

class UnionFindAndLeftHeap {
private:
    Node nodes[1000010];
    int size = 0;
    int K;
public:
    UnionFindAndLeftHeap() {}
    ~UnionFindAndLeftHeap() {}
    // Below are the interfaces for UnionFind
    void Input(int n) {
        size = n;
        for (int i = 1; i <= size; i++) {
            cin >> nodes[i].weight;
        }
    }
    Node *Find(Node *a) {
        // Find means find ultimate parent
        if (a->unionfind_ptr == a) {
            return a;
        } else {
            a->unionfind_ptr = Find(a->unionfind_ptr);
            return a->unionfind_ptr;
        }
    }
    Node *Union(int u, int v) {
        // We assume the node with smaller rank becomes the parent
        if (u > v) {
            my_swap(&u, &v);
        }
        Node *m = Find(&nodes[v]);
        Node *n = Find(&nodes[u]);

        // Correspondingly, update the information of heap
        Node *root_1 = m->heap_root, *root_2 = n->heap_root;

        // In order to promote efficiency, we need to DelMin the two heaps until it reaches correct size
        while (root_1->heap_size + root_2->heap_size > K) {
            if (root_1->weight < root_2->weight) {
                root_1 = DelMin(root_1);
            }
            else {
                root_2 = DelMin(root_2);
            }
        }

        // Union the unionfind
        m->unionfind_ptr = n;
        if (m != n) {
            n->unionfind_size += m->unionfind_size;
        } 

        // As the new root, n's heap root is the Merge of root_1 and root_2
        n->heap_root = Merge(root_1, root_2);
        return n;
    }
    // Testing UnionFind
    void Print() {
        for (int i = 1; i <= size; i++) {
            cout << "rank: " << i << " father: " << nodes[i].unionfind_ptr - nodes 
            << " weight: " << nodes[i].weight << " size " << nodes[i].unionfind_size << endl;
        }
    }

    // Below are the interfaces of LeftHeap
    Node *Merge(Node *a, Node *b) { // a and b stands for root
        // Basis: if a is null or b is null
        if (a == nullptr) {
            return b;
        }
        if (b == nullptr) {
            return a;
        }
        int size_1 = a->heap_size, size_2 = b->heap_size;
        // Make sure that a's weight is greater than b
        if (a->weight > b->weight) {
            my_swap(a, b);
        }
        // Start merging
        while (a->heap_right_child != nullptr) {
            if ((a->heap_right_child)->weight > b->weight) {
                b->heap_parent = a;
                my_swap(a->heap_right_child, b);
            }
            a = a->heap_left_child;
        }
        // Insert the remainders of b
        a->heap_right_child = b;
        (a->heap_left_child)->heap_parent = a;
        // Backtracking to maintain a LeftHeap
        while (a != nullptr) {
            if (a -> heap_left_child == nullptr || (a->heap_left_child)->npl < (a->heap_right_child)->npl) {
                my_swap(a->heap_left_child, a->heap_right_child);
            }
            a->npl = a->heap_right_child != nullptr ? (a->heap_right_child)->npl + 1 : 1;
            b = a; // Use b to record the previous a
            a = a->heap_parent;
        }
        b->heap_size = size_1 + size_2;
        return b;
    }

    void Heap_Insert(Node *root, Node *new_node) {
        Merge(root, new_node);
    }

    Node *DelMin(Node *root) {
        Node *left_heap = root->heap_left_child;
        if (left_heap != nullptr) {
            left_heap->heap_parent = nullptr;
        }
        Node *right_heap = root->heap_right_child;
        if (right_heap != nullptr) {
            right_heap->heap_parent = nullptr;
        }
        Node *new_root = Merge(left_heap, right_heap); 
        return new_root;
    }

    // Below are the interfaces of Operations
    void Insert(int u, int v) { 
        // Insert a segment between nodes[u] and nodes[v]
        Node *new_parent = Union(u, v);
    }
    void Query_kth(int u) {
        // Query the kth largest node in the connectivity where nodes[u] lies in

    }
};

UnionFindAndLeftHeap unionfind;

int main() {
    int n, k;
    cin >> n;
    unionfind.Input(n);
    cin >> k;
    int u, v;
    for (int i = 0; i < k; i++) {
        cin >> u >> v;
        unionfind.Union(u, v);
    }
    unionfind.Print();
    return 0;
}