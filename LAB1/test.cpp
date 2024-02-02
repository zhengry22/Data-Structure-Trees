#include<iostream>
using namespace std;
#include<string>

string a;

int main() {
    a = "abc";
    cout << a[-100];
    return 0;
}