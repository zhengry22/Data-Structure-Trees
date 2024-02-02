#pragma warning (disable:4996)
#include<cstdio>
#include<iostream>
using namespace std;
#define MOD 1000000007;

template<typename T>
void my_swap(T* a, T* b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template<typename T>
class Heap { // Heap with the smallest element on the root
public:
    T container[100010];
    int size = 0;
    Heap() {}
    ~Heap() {}

    int PercolateUp(int rank) {
        while (rank > 0) {
            int father_rank = (rank - 1) >> 1;
            if (container[father_rank] > container[rank]) {
                my_swap(&container[father_rank], &container[rank]);
                rank = father_rank;
                father_rank = (rank - 1) >> 1;
                continue;
            }
            break;
        }
        return rank;
    }

    int PercolateDown(int rank) {
        int lc = (rank << 1) + 1, rc = (rank << 1) + 2;
        while (lc < size) {
            int min;
            if (rc < size) { // Can go both ways
                // Judge whether there are ways to percolate
                min = container[lc] < container[rc] ? lc : rc;
            }
            else {
                min = lc;
            }
            if (container[min] < container[rank]) {
                // Percolate down
                my_swap(&container[rank], &container[min]);
                rank = min;
                lc = (rank << 1) + 1, rc = (rank << 1) + 2;
                continue;
            }
            break;
        }
        return rank;
    }

    int Insert(T node) {
        container[size] = node;
        size++;
        return PercolateUp(size - 1);
    }

    int DelMin() {
        if (size == 0) return -1;
        int rank = container[0].rank;
        my_swap(&container[0], &container[size - 1]);
        size--;
        PercolateDown(0);
        return rank;
    }

    void PrintTree() {
        int sign = 1;
        for (int i = 0; i < size; i++) {
            cout << "index: " << i << ", " << "min: " << container[i].min_length << " rank: " << container[i].rank;
            if (i + 1 == sign) {
                cout << endl;
                sign <<= 1;
                sign += 1;
            }
        }
        cout << endl;
    }
};

struct Node {
    int time = 0;
    long long min_length = -1;
    long long path_number = 0;
    bool found = false;
};

struct HeapNode {
    long long min_length = 0;
    int rank = 0;
    bool operator<(const HeapNode& other) const {
        return min_length < other.min_length;
    }

    bool operator>(const HeapNode& other) const {
        return min_length > other.min_length;
    }
};

struct Node_with_heap {
    int time = 0;
    long long min_length = -1;
    long long path_number = 0;
    bool found = false;
    HeapNode* heap_ptr = nullptr;
};

struct Node_in_adj {
    Node_in_adj* next = nullptr;
    int rank = 0; // rank of the node in nodes
};

struct Ptr_pair {
    Node_in_adj* entry = nullptr;
    Node_in_adj* tail = nullptr;
};

Node_with_heap nodes[100010];
Ptr_pair list_ptr[100010];
Heap<HeapNode> my_heap;
Node_in_adj adj_list[200020]; // Note that edges can be bi-directional.
int games, paths, list_length = 0;

void Input() {
    scanf("%d %d", &games, &paths);
    for (int i = 1; i <= games; i++) {
        scanf("%d", &(nodes[i].time));
    }
    int v1, v2;
    for (int i = 0; i < paths; i++) {
        scanf("%d %d", &v1, &v2);
        // add v2 to v1
        if (list_ptr[v1].entry == nullptr) {
            list_ptr[v1].entry = &adj_list[list_length];
        }
        else {
            list_ptr[v1].tail->next = &adj_list[list_length];
        }
        list_ptr[v1].tail = &adj_list[list_length];
        adj_list[list_length].rank = v2;
        list_length++;
        // add v1 to v2
        if (list_ptr[v2].entry == nullptr) {
            list_ptr[v2].entry = &adj_list[list_length];
        }
        else {
            list_ptr[v2].tail->next = &adj_list[list_length];
        }
        list_ptr[v2].tail = &adj_list[list_length];
        adj_list[list_length].rank = v1;
        list_length++;
    }
}

void Dijkstra() {
    HeapNode a = { nodes[1].time, 1 };
    nodes[1].heap_ptr = &my_heap.container[my_heap.Insert(a)];
    nodes[1].min_length = nodes[1].time;
    nodes[1].path_number = 1;
    while (!nodes[games].found) {
        // The minimum node in heap should be found and deleted
        int lifted = my_heap.DelMin();
        nodes[lifted].found = true;
        nodes[lifted].heap_ptr = nullptr;
        // For all the nodes adjacent to node[lifted], update heap
        Node_in_adj* ptr = list_ptr[lifted].entry;
        while (ptr != nullptr) {
            if (nodes[ptr->rank].found) {
                ptr = ptr->next;
                continue;
            }
            bool changed = false;
            long long new_length = nodes[lifted].min_length + (long long)nodes[ptr->rank].time;
            // Update Node info
            if (nodes[ptr->rank].min_length == -1 || new_length < nodes[ptr->rank].min_length) {
                nodes[ptr->rank].min_length = new_length;
                nodes[ptr->rank].path_number = nodes[lifted].path_number;
                changed = true;
            }
            else if (new_length == nodes[ptr->rank].min_length) {
                nodes[ptr->rank].path_number += nodes[lifted].path_number;
                nodes[ptr->rank].path_number %= MOD;
            } // else, no need to change
            if (nodes[ptr->rank].heap_ptr != nullptr) {
                // Already in heap
                if (changed) {
                    int heap_rank = nodes[ptr->rank].heap_ptr - my_heap.container;
                    my_heap.container[heap_rank].min_length = nodes[ptr->rank].min_length;
                    nodes[ptr->rank].heap_ptr = &my_heap.container[my_heap.PercolateUp(heap_rank)];
                }
            }
            else {
                HeapNode tmp = { nodes[ptr->rank].min_length, ptr->rank };
                nodes[ptr->rank].heap_ptr = &my_heap.container[my_heap.Insert(tmp)];
            }
            ptr = ptr->next;
            //my_heap.PrintTree();
        }
    }
    printf("%lld\n", nodes[games].min_length);
    printf("%lld", nodes[games].path_number);
}

int main() {
    Input();
    Dijkstra();
    return 0;
}

