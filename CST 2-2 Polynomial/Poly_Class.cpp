#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;

class Poly {
private:
    long long poly[65] = {0};
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

    void operator+=(const Poly &f) {
        bool flag = false;
        
        for (int i = higest_pow; i >= 0; i--) {
            this->poly[i] = this->poly[i] + f.poly[i];
            if (this->poly[i] != 0 && !flag) {
                flag = true;
                this->higest_pow = i;
            }
        }
        if (!flag) {
            this->higest_pow = 0;
        }
    }

    void operator-=(const Poly &f) {
        bool flag = false;
        for (int i = higest_pow; i >= 0; i--) {
            this->poly[i] = this->poly[i] - f.poly[i];
            if (this->poly[i] != 0 && !flag) {
                flag = true;
                this->higest_pow = i;
            }
        }
        if (!flag) {
            this->higest_pow = 0;
        }
    }

    void operator*=(const Poly &f) {
        Poly ret;
        int new_highest_pow = this->higest_pow + f.higest_pow;
        for(int i = this->higest_pow; i >= 0; i--) {
            for(int j = f.higest_pow; j >= 0; j--) {
                ret[i+j] += this->poly[i]*f.poly[j];
            }
        }
        for(int i = new_highest_pow; i >= 0; i--) {
            this->poly[i] = ret[i];
        }
        this->higest_pow = new_highest_pow;
    }

    void operator^=(const Poly &f) {
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


int main() {
    Poly a(1, 1);
    Poly b(2, 2);
    Poly c(2, 0);
    Poly d;
    d = Poly(1 ,1);
    d.show();
    a*=b;
    a.show();
    a+=b;
    a.show();
    a^=c;
    a.show();
    return 0;
}