#include<iostream>
#include<string.h>
#include<cstring>
using namespace std;
int N, M, K;
char* a, * b, *long_str, *short_str;
long long* row, * tmp;

bool in_limit = true;

int my_max(int a, int b) {
    return a > b ? a : b;
}

int my_min(int a, int b) {
    return a < b ? a : b;
}

long long LCS(int N, int M) {
    //first calculate base
    for (int i = 0; i <= N; i++) {
        row[i] = 0;
    }
    //iterative process in two fors
    for (int i = 1; i <= M; i++) {//for each row
        bool slide = (i + K == N) ? true : false;
        int left_ = my_max(0, i - K), right_ = my_min(N, i + K); //two boundaries of each cycle 
        for (int j = left_; j <= right_; j++) {
            if (j == left_) {
                tmp[j] = row[j];
            }
            else if (j == right_) {
                //different result 
                if (j != N || (j == N && slide)) {
                    tmp[j] = tmp[j - 1];
                }
                else {
                    if (short_str[j - 1] == long_str[i - 1]) {
                        tmp[j] = row[j - 1] + 1;
                    }
                    else {
                        long long x = tmp[j - 1];
                        long long y = row[j];
                        tmp[j] = x > y ? x : y;
                    }
                }
            }
            else {
                if (short_str[j - 1] == long_str[i - 1]) {
                    tmp[j] = row[j - 1] + 1;
                }
                else {
                    long long x = tmp[j - 1];
                    long long y = row[j];
                    tmp[j] = x > y ? x : y;
                }
            }
        }
        for (int j = left_; j <= right_; j++) {
            row[j] = tmp[j];
            tmp[j] = 0;
        }
    }
    return row[N];
}

int main() {
    cin >> N >> M >> K;
    a = new char[N + 1];
    b = new char[M + 1];
    row = new long long[N + 1];
    tmp = new long long[N + 1];
    cin >> a >> b;
    
    if (N > M) {//we hope N <= M
        int tmp = N;
        N = M;
        M = tmp;
        long_str = new char[M + 1];
        short_str = new char[N + 1];
        long_str = a;
        short_str = b;
    }
    else {
        long_str = new char[M + 1];
        short_str = new char[N + 1];
        long_str = b;
        short_str = a;
    }

    long long lcs = LCS(N, M);
    long long answer = N + M - 2 * lcs;
    if (answer <= K) {
        cout << answer;
    }
    else {
        cout << -1;
    }

    delete[] a;
    delete[] b;
    delete[] row;
    delete[] tmp;
    //delete[] long_str;
    //delete[] short_str;
    return 0;
}