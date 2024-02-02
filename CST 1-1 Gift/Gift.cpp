#include<iostream>
#include<fstream>
#define DEBUG_MODE
using namespace std;

struct Gift_Pair {
    int x;
    int y;
};

long long* sum_left, * sum_right;
int array_ptr = 0;

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

int power_of_2(int exp) {
    int answer = 1;
    for (int i = 0; i < exp; i++) {
        answer *= 2;
    }
    return answer;
}

void List_Subset(Gift_Pair* gifts, int num, int sum, long long* sums) {
    if (num == 1) {
        sums[array_ptr] = sum + gifts[0].x;
        array_ptr++;
        sums[array_ptr] = sum + gifts[0].y;
        array_ptr++;
    }
    else {
        // we'll do it the recursive way
        Gift_Pair* gifts_left = new Gift_Pair[num - 1];
        for (int i = 1; i < num; i++) {
            gifts_left[i - 1] = gifts[i];
        }
        List_Subset(gifts_left, num - 1, sum + gifts[0].x, sums);
        List_Subset(gifts_left, num - 1, sum + gifts[0].y, sums);
    }
}

int main() {
    int n, price;
    long long answer = 0;
    cin >> n >> price;
    Gift_Pair* gifts = new Gift_Pair[n];
    for (int i = 0; i < n; i++) {
        cin >> gifts[i].x >> gifts[i].y;
    }
    
    // First divide the two parts in half.
    int half = n / 2;
    Gift_Pair* left_gifts = new Gift_Pair[half];
    for (int i = 0; i < half; i++) {
        left_gifts[i] = gifts[i];
    }

    // Calculate all the sums, there should be 2^half of them.
    int left_size = power_of_2(half);
    sum_left = new long long[left_size];
    List_Subset(left_gifts, half, 0, sum_left);
    array_ptr = 0;
    // Now, all the sums should be stored in array. We should sort the array.
    quick_sort(sum_left, left_size);

    // In the same way, we calculate the sums of the right side, and store them in another array.
    Gift_Pair* right_gifts = new Gift_Pair[n - half];
    for (int i = half; i < n; i++) {
        right_gifts[i - half] = gifts[i];
    }
#ifdef DEBUG_MODE
    cout << "the right gifts are: ";
    for (int i = 0; i < n - half; i++) {
        cout << right_gifts[i].x << right_gifts[i].y << " ";
    }
#endif
    int right_size = power_of_2(n - half);
    sum_right = new long long[right_size];
    List_Subset(right_gifts, n - half, 0, sum_right);

    // For each number in the sum_right array, we tend to find the greatest number in the sum_left array
    // that's smaller than the difference between price and this number.

#ifdef DEBUG_MODE
    cout << "the sorted left array is: " << endl;
    for (int i = 0; i < left_size; i++) {
        cout << sum_left[i] << ' ';
    }
    cout << "the sorted right array is: " << endl;
    for (int i = 0; i < right_size; i++) {
        cout << sum_right[i] << ' ';
    }
#endif

    for (int i = 0; i < right_size; i++) {
        answer += search(sum_left, 0, left_size - 1, price - sum_right[i]) + 1;
    }
    cout << answer;
    return 0;
}