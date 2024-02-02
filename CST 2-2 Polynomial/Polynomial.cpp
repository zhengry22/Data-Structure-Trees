#include<iostream>
#include<stdio.h>
using namespace std;
#define N_OPTR 7
#define MY_MOD 1000000007

class Poly {
private:
    long long poly[65] = { 0 };
    int higest_pow = 0;
public:
    Poly() {}

    Poly(long long coeff, int pow = 0) {
        this->poly[pow] = coeff;
        this->higest_pow = pow;
    }

    long long& operator[](int index) {
        if (index < 0 || index >= 65) {
            return poly[0];
        }
        return poly[index];
    }

    void operator+=(const Poly& f) {
        bool flag = false;
        int max_pow = this->higest_pow > f.higest_pow ? this->higest_pow : f.higest_pow;
        for (int i = max_pow; i >= 0; i--) {
            this->poly[i] = this->poly[i] + f.poly[i];
            this->poly[i] %= MY_MOD;
            this->poly[i] += MY_MOD;
            this->poly[i] %= MY_MOD;
            if (this->poly[i] != 0 && !flag) {
                flag = true;
                this->higest_pow = i;
            }
        }
        if (!flag) {
            this->higest_pow = 0;
        }
    }

    void operator-=(const Poly& f) {
        bool flag = false;
        int max_pow = this->higest_pow > f.higest_pow ? this->higest_pow : f.higest_pow;
        for (int i = max_pow; i >= 0; i--) {
            this->poly[i] = this->poly[i] - f.poly[i];
            this->poly[i] %= MY_MOD;
            this->poly[i] += MY_MOD;
            this->poly[i] %= MY_MOD;
            if (this->poly[i] != 0 && !flag) {
                flag = true;
                this->higest_pow = i;
            }
        }
        if (!flag) {
            this->higest_pow = 0;
        }
    }

    void operator*=(const Poly& f) {
        Poly ret;
        int new_highest_pow = this->higest_pow + f.higest_pow;
        for (int i = this->higest_pow; i >= 0; i--) {
            if (poly[i] == 0) {continue;}
            for (int j = f.higest_pow; j >= 0; j--) {
                if (f.poly[j] == 0) {continue;}
                ret[i + j] += this->poly[i] * f.poly[j];
                ret[i + j] %= MY_MOD;
                ret[i + j] += MY_MOD;
                ret[i + j] %= MY_MOD;
            }
        }
        for (int i = new_highest_pow; i >= 0; i--) {
            this->poly[i] = ret[i];
        }
        this->higest_pow = new_highest_pow;
    }

    void operator^=(const Poly& f) {
        int pow = f.poly[0]; // power
        Poly ret(1, 0);
        for (int i = 0; i < pow; i++) {
            ret *= *this;
        }
        for (int i = ret.higest_pow; i >= 0; i--) {
            this->poly[i] = ret[i];
        }
        this->higest_pow = ret.higest_pow;
    }

    void show() {
        for (int i = this->higest_pow; i >= 0; i--) {
            printf("%lld ", poly[i]);
        }
        printf("\n");
    }
};

struct Pair {
    long long coeff;
    int pow;
};

union MyUnion {
    char operator_;
    Pair my_pair;
};

struct Op {
    MyUnion op;
    int number_type; // True for number, False for operands
};


template<class T>
class Stack {
private:
    T* container;
    int stack_ptr;
public:
    Stack() {
        stack_ptr = 0;
    }

    void allocate(int n) {
        container = new T[n];
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

    inline T* top_ptr() {
        return &container[stack_ptr-1];
    }

    inline T* subtop_ptr() {
        return &container[stack_ptr-2];
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
    bool mult_omit = false;
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
        oprd.allocate(467010);
        optr.allocate(500010);
        expression = new char[1000010];
        exp_ptr = 0;
        mult_omit = false;
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
        if (isnumber) { ret.number_type = 2; }
        else if (expression[exp_ptr] == 'x') { ret.number_type = 1; }
        else { ret.number_type = 0; }// Is the next token we read an operand or an operator?
        if (ret.number_type == 2) {
            ret.op.my_pair = Pair{0, 0};
            while (expression[exp_ptr] >= '0' && expression[exp_ptr] <= '9') {
                //ret.op.number = ret.op.number * 10 + (expression[exp_ptr] - '0');
                ret.op.my_pair.coeff *= 10;
                ret.op.my_pair.coeff += expression[exp_ptr] - '0';
                exp_ptr++;
            }
        }
        else if (ret.number_type == 1) {
            // mult_omit?
            if (exp_ptr != 0 && ((expression[exp_ptr - 1] >= '0' && expression[exp_ptr - 1] <= '9')
                || expression[exp_ptr - 1] == ')') && !mult_omit) {
                ret.number_type = 0; // returning '*'
                ret.op.operator_ = '*';
                mult_omit = true;
            }
            else {
                ret.op.my_pair = Pair{1, 1};
                mult_omit = false;
                exp_ptr++;
            }
        }
        else {
            // mult_omit?
            if (exp_ptr != 0 && expression[exp_ptr] == '(' && ((expression[exp_ptr - 1] >= '0' && expression[exp_ptr - 1] <= '9')
                || expression[exp_ptr - 1] == 'x') && !mult_omit) {
                ret.op.operator_ = '*';
                mult_omit = true;
            }
            else {
                ret.op.operator_ = expression[exp_ptr];
                exp_ptr++;
                mult_omit = false;
            }
        }
        return ret;
    }

    void Parsing() {
        optr.push('\0');
        Op new_op = GetNextOp();
        while (1) {
            if (new_op.number_type != 0) {
                T obj(new_op.op.my_pair.coeff, new_op.op.my_pair.pow);
                oprd.push(obj);
                new_op = GetNextOp();
            }
            else {
                // 3 cases. First we need to get stack top.
                // Compare the operator we push 
                char top_ = optr.top();
                char result = pri[optr_map(top_)][optr_map(new_op.op.operator_)]; // Find from the form
                char operator_;
                T *a, *b;             
                switch (result) {
                case '>':
                    a = oprd.top_ptr();
                    b = oprd.subtop_ptr();
                    switch (top_) {
                        case '+':
                            *b += *a;
                            break;
                        case '-':
                            *b -= *a;
                            break;
                        case '*':
                            *b *= *a;
                            break;
                        case '^':
                            *b ^= *a;
                            break;
                    }
                    oprd.pop();
                    optr.pop();
                    break;
                case '<':
                    optr.push(new_op.op.operator_);
                    new_op = GetNextOp();
                    break;
                case '=':
                    if (new_op.op.operator_ == '\0') {
                        oprd.top().show();
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
    Polynomial<Poly> p;
    p.Input();
    p.Parsing();
    return 0;
}