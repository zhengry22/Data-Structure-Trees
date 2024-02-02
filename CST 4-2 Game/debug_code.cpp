#include<iostream>
using namespace std;

struct HeapNode {
    int min_length = 0;
    int rank = 0;
    bool operator<(const HeapNode& other) const {
        return min_length < other.min_length;
    }

    bool operator>(const HeapNode& other) const {
        return min_length > other.min_length;
    }
};

struct Node_with_heap{
    int time = 0;
    long long min_length = -1;
    long long path_number = 0;
    bool found = false;
    HeapNode *heap_ptr = nullptr;
};

Node_with_heap array[1000];

int main() {
    Node_with_heap *a = &array[1];
    cout << a - array;    
}