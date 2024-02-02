#pragma warning(disable:4996);
#include <cstdio>
#include <limits.h>
#include <iostream>
using namespace std;

template<typename T>
void my_swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

class Melody {
private: 
    int next[2000010];
    int j = 0; // The paragraph pointer
    int t = -1; // initial t
public:
    int paragraph[2000010];
    int size = 0;
    Melody() {
        next[0] = -1;
    }

    ~Melody() {}

    void buildNext(int m) {
        //int current_j = j;
        while ( j <size )
            if ( 0 > t || paragraph[t] == paragraph[j] ) {
                ++t; ++j; next[j] = t; 
            } 
            else 
                t = next[t]; 
    }

    int getMaxPresuffix(int length) {

        int maxret = 0;
        int half_length = length >> 1;
        maxret = next[length];
        if (maxret > half_length) {
            maxret = maxret-(length - maxret)*((maxret - half_length) / (length - maxret));
        }
        while ( maxret > half_length) {

            maxret=next[maxret];
        }
        return maxret;
    }

    int getCommonPrefix(int u, int v) {
        // u, v are the retval of getMaxPresuffix
        while (u != v) {
            if (u < v) {
                my_swap(u, v);
            }
            if (next[u] > (u >> 1)) {
                int unit = u - next[u];
                u -= (u-v)/unit;
            } 
            else {
                u = next[u];
            }
        }
        return u;
    }
    
    void Print() {
        for (int i = 0; i <= j; i++) {
            cout << next[i] << " ";
        }
        cout << endl;
    }

    void PrintGetMax() {
        for (int i = 1; i <= j; i++) {
            cout << getMaxPresuffix(i) << " ";
        }
        cout << endl;
    }

};

Melody melody;
int n, m;

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%d", &melody.paragraph[i]);
        melody.size++;
    }
    melody.buildNext(n);
    int operation, query;
    for (int i = 1; i <= m; i++) {
        // All the queries
        scanf("%d", &operation);
        switch (operation) {
        case 1:
            scanf("%d", &query);
            for (int i = 0; i < query; i++) {
                scanf("%d", &melody.paragraph[melody.size]);
                melody.size++;
            }
            melody.buildNext(query);
            break;
        case 2:
            scanf("%d", &query);

            int answer, length;
            scanf("%d", &length);
            answer = melody.getMaxPresuffix(length);

            for (int i = 1; i < query; i++) {
                scanf("%d", &length);
                if (answer == 0) {
                    continue;
                }
                answer = melody.getCommonPrefix(answer, melody.getMaxPresuffix(length));
            }
            printf("%d\n", answer);
            break;
        }
    }
}