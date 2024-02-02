#pragma warning (disable:4996)
#include<iostream>
#include<stdio.h>
using namespace std;
#define N_OPTR 7

union MyUnion {
    char operand;
    long long number;
};

struct Op {
    MyUnion op;
    bool is_number; // True for number, False for operands
};

template<class T>
class Stack {
private:
    T* container;
    int stack_ptr;
public:
    Stack(int n = 1000000) {
        container = new T[n];
        stack_ptr = 0;
    }

    ~Stack() {
        delete[] container;
    }

    inline void push(T elem) {
        container[stack_ptr] = elem;
        stack_ptr++;
    }

    inline T pop() {
        T top = container[stack_ptr - 1];
        stack_ptr--;
        return top;
    }

    inline T top() {
        return container[stack_ptr - 1];
    }

    inline bool empty() {
        return stack_ptr == 0;
    }
};

template<class T>
class Polynomial {
private:
    Stack<T> oprd;
    Stack<char> optr;
    int exp_ptr; // Which char in expression we are reading currently
    char* expression;
    const char pri[N_OPTR][N_OPTR] = {
        /*              +      -      *      ^      (      )      \0 */
        /* --  + */    '>',   '>',   '<',   '<',   '<',   '>',   '>',
        /*     - */    '>',   '>',   '<',   '<',   '<',   '>',   '>',
        /*     * */    '>',   '>',   '>',   '<',   '<',   '>',   '>',
        /*     ^ */    '>',   '>',   '>',   '>',   '<',   '>',   '>',
        /*     ( */    '<',   '<',   '<',   '<',   '<',   '=',   ' ',
        /*     ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
        /* -- \0 */    '<',   '<',   '<',   '<',   '<',   ' ',   '='
    };
public:
    Polynomial() {
        expression = new char[1000010];
        exp_ptr = 0;
    }

    ~Polynomial() {
        delete[] expression;
    }

    void Input() {
        scanf("%s", expression);
    }

    int optr_map(char sign) {
        // Generate mapping number
        switch (sign) {
        case '+':
            return 0;
        case '-':
            return 1;
        case '*':
            return 2;
        case '^':
            return 3;
        case '(':
            return 4;
        case ')':
            return 5;
        case '\0':
            return 6;
        }

    }

    Op GetNextOp() {
        Op ret;
        bool isnumber = expression[exp_ptr] >= '0' && expression[exp_ptr] <= '9';
        ret.is_number = isnumber ? true : false; // Is the next token we read an operand or an operator?
        if (isnumber) {
            ret.op.number = 0;
            while (expression[exp_ptr] >= '0' && expression[exp_ptr] <= '9') {
                ret.op.number = ret.op.number * 10 + (expression[exp_ptr] - '0');
                exp_ptr++;
            }
        }
        else {
            ret.op.operand = expression[exp_ptr];
            exp_ptr++;
        }
        return ret;
    }

    T Calculate(T a, T b, char sign) {
        // a pops first, b pops second
        switch (sign) {
        case '+':
            return a + b;
        case '-':
            return b - a;
        case '*':
            return a * b;
        case '^':
            T answer = T(1);
            for (int i = 0; i < a; i++) {
                answer *= b;
            }
            return answer;
        }
    }

    void Parsing() {
        optr.push('\0');
        Op new_op = GetNextOp();
        while (1) {
            if (new_op.is_number) {
                oprd.push(new_op.op.number);
                new_op = GetNextOp();
            }
            else {
                // 3 cases. First we need to get stack top.
                // Compare the operator we push 
                char top_ = optr.top();
                char result = pri[optr_map(top_)][optr_map(new_op.op.operand)]; // Find from the form
                T new_result, a, b;
                char operator_;
                switch (result) {
                case '>':
                    a = oprd.pop();
                    b = oprd.pop();
                    operator_ = optr.pop();
                    new_result = Calculate(a, b, operator_);
                    oprd.push(new_result);
                    break;
                case '<':
                    optr.push(new_op.op.operand);
                    new_op = GetNextOp();
                    break;
                case '=':
                    if (new_op.op.operand == '\0') {
                        printf("%lld", oprd.top());
                        return;
                    }
                    optr.pop();
                    new_op = GetNextOp();
                    break;
                case ' ':
                    break;
                }
            }
        }
    }
};

int main() {
    Polynomial<long long> p;
    p.Input();
    p.Parsing();
    return 0;
}