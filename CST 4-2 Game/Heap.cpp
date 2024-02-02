#include <iostream>
using namespace std;

template<typename T>
void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

template<typename T>
class Heap{ // Heap with the smallest element on the root
public:
    T container[100010];
    int size = 0;
    Heap() {}
    ~Heap() {}

    int PercolateUp(int rank) {
        while (rank > 0) {
            int father_rank = (rank-1) >> 1;
            if (container[father_rank] > container[rank]) {
                swap(&container[father_rank], &container[rank]);
                rank = father_rank;
                father_rank = (rank-1) >> 1;
                continue;
            } 
            break;      
        }
        return rank;
    }

    int PercolateDown(int rank) {
        int lc = (rank<<1) + 1, rc = (rank<<1) + 2;
        while (lc < size) {
            int min;
            if (rc < size) { // Can go both ways
                // Judge whether there are ways to percolate
                min = container[lc] < container[rc] ? lc : rc;
            } else {
                min = lc;
            }
            if (container[min] < container[rank]) {
                // Percolate down
                swap(&container[rank], &container[min]);
                rank = min;
                lc = (rank<<1) + 1, rc = (rank<<1) + 2;
                continue;
            }
            break;
        }
        return rank;
    }

    int Insert(T node) {
        container[size] = node;
        size++;
        return PercolateUp(size-1);
    }

    int DelMin() {
        if (size == 0) return -1;
        swap(&container[0], &container[size-1]);
        size--;
        return PercolateDown(0);
    }

    void PrintTree() {
        int sign = 1;
        for (int i = 0; i < size; i++) {
            cout << "index: " << i << ", " << container[i] << " ";
            if (i + 1 == sign) {
                cout << endl;
                sign <<= 1;
                sign += 1;
            }
        }
        cout << endl;
    }
};

Heap<long long> my_heap;

int main() {
    int n;
    cin >> n;
    for(int i = 0; i < n; i++) {
        long long k;
        cin >> k;
        my_heap.Insert(k);
    }
    my_heap.PrintTree(); 
    cout << endl;
    for(int i = 0; i < n; i++) {
        my_heap.DelMin();
        my_heap.PrintTree();
    }
    return 0;
}