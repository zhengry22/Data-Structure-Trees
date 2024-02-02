#include <iostream>
#include <stdio.h>
using namespace std;

int partition(long long *array, int left, int right) {
    int l = left, r = right;
    long long pivot = array[left];
    while (left < right) {
        while (left < right && array[right] > pivot) {
            right--;
        }
        if (left < right) {
            array[left] = array[right];
        }
        while (left < right && array[left] <= pivot) {
            left++;
        }
        if (left < right) {
            array[right] = array[left];
        }
    }
    array[left] = pivot;
    //cout << "Index is: " << left << endl;
    //for (int i = l; i <= r; i++) {
    //    cout << array[i] << ' ';
    //}
    return left;
}

int getmid(long long *array, int left, int right) {
    int mid_index = (right+left) >> 1;
    while(true) {
        int pivot = partition(array, left, right);
        // return if pivot is already the mid number
        if (pivot == mid_index) {
            return pivot;
        }
        else {
            // Judge whether the pivot is greater or smaller than mid num
            if (pivot > mid_index) {
                right = pivot - 1;
            }
            else {
                left = pivot + 1;
            }
        }
    }
}

long long array[1000000];

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> array[i];
    }
    int result = getmid(array, 0, n-1);
    for (int i = 0; i < n; i++) {
        cout << array[i] << ' ';
    }
    cout << endl;
    cout << "Index: " << result << endl;
    cout << "Number: " << array[result] << endl;
    return 0;
}