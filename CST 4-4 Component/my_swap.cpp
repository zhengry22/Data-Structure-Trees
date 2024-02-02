#include <iostream>
using namespace std;

template<typename T>
void my_swap(T** a, T** b) {
    T *temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int a = 4;
    int *p = &a;
    int *q = nullptr;
    my_swap(&p, &q);
    cout << *p;

    return 0;
}
