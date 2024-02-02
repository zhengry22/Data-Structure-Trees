#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

int* buildNext( char* P ) {
    size_t m = strlen(P), j = 0;
    int* next = new int[m]; int t = next[0] = -1;
    while ( j < m - 1 )
        if ( 0 > t || P[t] == P[j] ) {
            if ( P[++t] != P[++j] )
                next[j] = t;
            else //P[next[t]] != P[t] == P[j]
                next[j] = next[t];
        } 
        else
            t = next[t];
    return next;
}

int* buildNext2( char* P ) {
    size_t m = strlen(P), j = 0;
    int* next = new int[m]; int t = next[0] = -1;
    while ( j < m - 1 )
        if ( 0 > t || P[t] == P[j] ) { //匹配
            ++t; ++j; next[j] = t; //则递增赋值
        } 
        else //否则
            t = next[t]; //继续尝试下一值得尝试的位置
    return next;
}


int main() {
    int n;
    char a[100];
    scanf("%s", a);
    int* next1 = buildNext(a);
    int* next2 = buildNext2(a);
    for (int i = 0; i < strlen(a); i++) {
        cout << next1[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < strlen(a); i++) {
        cout << next2[i] << " ";
    }
    return 0;
}
