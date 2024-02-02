#include <iostream>
using namespace std;

struct Operation {
    char op_type;
    long long left;
    long long right;
};

long long GetIntersect(long long left1, long long right1, long long left2, long long right2) {
    long long l = left1 > left2 ? left1 : left2;
    long long r = right1 < right2 ? right1 : right2;
    return r - l + 1 > 0 ? r - l + 1 : 0;
}

Operation operations[1000000];

int main() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        cin >> operations[i].op_type >> operations[i].left >> operations[i].right;
        if (operations[i].op_type == 'Q') {
            long long sum = 0;
            for (int j = 0; j < i; j++) {
                if (operations[j].op_type == 'H') {
                    sum += GetIntersect(operations[j].left, operations[j].right, operations[i].left, operations[i].right);
                }  
            }
            cout << sum << endl;
        }
    }
    return 0;
}