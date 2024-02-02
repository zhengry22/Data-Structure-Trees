#include<iostream>
using namespace std;

struct Node {
    int weight;

    // Below are the elements of UnionFindAndLeftHeap
    int unionfind_size = 1; // Size of union find
    // Pointer system for UnionFindAndLeftHeap
    Node *unionfind_ptr = this;

    // Below are the elements of LeftHeap
    Node *heap_root = this; // This points to the heap root of the unionfind
    Node *heap_parent = nullptr; // Default the parent is a nullptr
    Node *heap_left_child = nullptr;
    Node *heap_right_child = nullptr;
    int npl = 1; // Default it has no left or right child
};

template<typename T>
void my_swap(T* a, T* b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

class LeftHeap {
    // We may have to save multiple heaps in one object ultimately
private:
    Node nodes[1000010];
    int size = 0;
public:
    LeftHeap() {}
    ~LeftHeap() {}
    // Below are the interfaces for UnionFind
    void Input(int n) {
        // At the beginning, all the nodes are isolated, each forming a single-sized leftheap.
        size = n;
        for (int i = 1; i <= size; i++) {
            cin >> nodes[i].weight;
        }
    }
    Node *Merge(Node *a, Node *b) { // a and b stands for root
        // Basis: if a is null or b is null
        if (a == nullptr) {
            return b;
        }
        if (b == nullptr) {
            return a;
        }
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
        return b;
    }
    void MergeWithRank(int u, int v) {
        Merge()
    }
};

LeftHeap leftheap;

int main() {
    int n;
    cin >> n;
    leftheap.Input(n);
}