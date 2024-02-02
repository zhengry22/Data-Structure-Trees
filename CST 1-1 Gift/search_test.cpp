#include<iostream>
using namespace std;

int search(long long* num, int l, int r, int obj)
{   
    // Under what circumstance should we consider entering the while loop?
    while ((l < r) && (num[l] <= obj) && (num[r] > obj)) {
        int mid = (l + r) / 2;
        if (obj < num[mid])
        {
            r = mid - 1;
        }
        else // obj >= num[mid]
        {
            l = mid;
        }
    }
    if (num[l] > obj)
        return -1;
    if (num[r] <= obj)
        return r;
    if (l == r)
        return l;
}

void quick_sort(long long* p, int n) {
	if (n <= 1)
		return;
	int t = p[n - 1], l = 0, r = n - 2;
	do {
		for (; r >= l; l++)
			if (p[l] > t)
				break;
		for (; r >= l; r--)
			if (p[r] < t)
				break;
		if (r >= l)
			swap(p[l], p[r]);
	} while (r >= l);
	swap(p[l], p[n - 1]);
	quick_sort(&p[0], l);
	quick_sort(&p[l + 1], n - l - 1);
}

int main(){
    int n, obj;
    long long *array;
    cin >> n >> obj;
    array = new long long[n];
    for (int i = 0; i < n; i++){
        cin >> array[i];
    }
    quick_sort(array, n);
    for (int i = 0; i < n; i++){
        cout << array[i] << ' ';
    }
    cout << "search result is: " << search(array, 0, n-1, obj);
    return 0;
}