#pragma warning (disable:4996)
#include<iostream>
#include<limits.h>
using namespace std;

struct Node {
    int weight;

    // Below are the elements of UnionFindAndLeftHeap
    int unionfind_size = 1; // Size of union find
    // Pointer system for UnionFindAndLeftHeap
    Node* unionfind_ptr = this;

    // Below are the elements of LeftHeap
    Node* heap_root = this; // This points to the heap root
    Node* heap_parent = nullptr; // Default the parent is itself
    Node* heap_left_child = nullptr;
    Node* heap_right_child = nullptr;
    int heap_size = 1; // Default each heap is sized 1 from the beginning
    int npl = 1; // Default it has no left or right child
};

template<typename T>
void my_swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
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
    void Input(int n, int m, int k) {
        size = n;
        K = k;
        for (int i = 1; i <= size; i++) {
            scanf("%d", &nodes[i].weight);
        }
        int u, v;
        for (int i = 1; i <= m; i++) {
            scanf("%d %d", &u, &v);
            Union(u, v);
            u = 0, v = 0;
        }
    }
    Node* Find(Node* a) {
        // Find means find ultimate parent
        if (a->unionfind_ptr == a) {
            return a;
        }
        else {
            a->unionfind_ptr = Find(a->unionfind_ptr);
            return a->unionfind_ptr;
        }
    }

    int GetHeapSize(Node* ptr) {
        if (ptr == nullptr) {
            return 0;
        }
        return ptr->heap_size;
    }

    int GetNpl(Node *ptr) {
        if (ptr == nullptr) {
            return 0;
        }
        return ptr->npl;
    }

    int GetWeight(Node *ptr) {
        if (ptr == nullptr) {
            return INT_MAX;
        }
        return ptr->weight;
    }

    Node* Union(int u, int v) {
        // We assume the node with smaller rank becomes the parent
        if (u > v) {
            my_swap(u, v);
        }
        Node* m = Find(&nodes[v]);
        Node* n = Find(&nodes[u]);

        if (m == n) {
            // No need to update heap and merge heap, return directly
            return n;
        }
        // else, it's necessary to merge heaps and unionfinds

        // Correspondingly, update the information of heap
        Node* root_1 = m->heap_root, * root_2 = n->heap_root;

        // In order to promote efficiency, we need to DelMin the two heaps until it reaches correct size
        int total_size = GetHeapSize(root_1) + GetHeapSize(root_2);
        while (total_size > K) {
            if (GetWeight(root_1) < GetWeight(root_2)) {
                root_1 = DelMin(root_1);
                m->heap_root = root_1;
            }
            else {
                root_2 = DelMin(root_2);
                n->heap_root = root_2;
            }
            total_size--;
        }

        // Union the unionfind
        m->unionfind_ptr = n;
        n->unionfind_size += m->unionfind_size;
        // As the new root, n's heap root is the Merge of root_1 and root_2
        n->heap_root = Merge(root_1, root_2);
        (n->heap_root)->heap_size = total_size < K ? total_size : K;
        return n;
    }

    // Below are the interfaces of LeftHeap

    Node *Merge(Node* a, Node* b) {
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
        a->heap_right_child = Merge(a->heap_right_child, b);
        if (a->heap_right_child != nullptr) {
            (a->heap_right_child)->heap_parent = a;
        }
        if (GetNpl(a->heap_left_child) < GetNpl(a->heap_right_child)) {
            my_swap(a->heap_left_child, a->heap_right_child);
        }
        a->npl = GetNpl(a->heap_right_child) + 1;
        return a;
    }

    void Insert(Node* root, Node* new_node) {
        Merge(root, new_node);
    }

    Node* DelMin(Node* root) {
        if (root == nullptr) return nullptr;
        Node* left_heap = root->heap_left_child;
        if (left_heap != nullptr) {
            left_heap->heap_parent = nullptr;
        }
        Node* right_heap = root->heap_right_child;
        if (right_heap != nullptr) {
            right_heap->heap_parent = nullptr;
        }
        Node* new_root = Merge(left_heap, right_heap);
        return new_root;
    }

    // Below are the interfaces of Operations
    int Query_kth(int u) {
        // Query the kth largest node in the connectivity where nodes[u] lies in
        Node* root_ = Find(&nodes[u])->heap_root;
        if (GetHeapSize(root_) == K) {
            return root_->weight;
        }
        return -1;
    }
};

UnionFindAndLeftHeap component;

int main() {
    int n, m, k, q;
    scanf("%d %d %d %d", &n, &m, &k, &q);
    component.Input(n, m, k);
    int query;
    int u, v;
    for (int i = 1; i <= q; i++) {
        // All the queries
        scanf("%d", &query);
        switch (query) {
        case 1:
            scanf("%d %d", &u, &v);
            component.Union(u, v);
            break;
        case 2:
            scanf("%d", &u);
            printf("%d\n", component.Query_kth(u));
            break;
        }
    }
    return 0;
}