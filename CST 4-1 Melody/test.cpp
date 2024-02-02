#include <iostream>
using namespace std;

int a[10];

int main() {
    int *b = &a[5];
    b[0] = 5;
    for (int i = 0; i < 10; i++) {
        cout << a[i] << " ";
    }
    return 0;
}