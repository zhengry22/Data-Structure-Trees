#include "hashtable.h"
int probe = 0;
int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}
// Naive Hashing, which is bad because it only uses the first letter of the string.

int bad_hashing::operator()(char* str, int N){
    int ret = 0;
    if(str == NULL) return 0;
    else {
        int len = strlen(str);
        for (int i = 0; i < len; i++) {
            ret += str[i];
            ret %= N;
        }
    }
    return ret;
}

int good_hashing::operator()(char* str, int N){
    int ret = 0;
    if(str == NULL) return 0;
    else {
        int len = strlen(str);
        long long pow = 1;
        for (int i = 0; i < len; i++) {
            ret += str[i] * (int)pow;
            pow *= 10;
            pow %= N;
            ret %= N;
        }
    }
    return ret;
}

int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}
void linear_probe::init(){
    return;// do nothing
}

int bilateral_square_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    int answer = ((last_choice - sign*(-1)*previous_step*previous_step) % table_size + table_size) % table_size;
    if (sign == 1) {
        previous_step = next_step;
        sign = -1;
    } else {
        next_step += 1;
        sign = 1;
    }
    return answer;
}
void bilateral_square_probe::init(){
    this->previous_step = 0;
    this->next_step = 1;
    this->sign = 1;
    probe = 1;
    return;// do nothing
}

int overflow_area_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    if (last_choice < table_size) {
        return last_choice + table_size;
    } else {
        return (last_choice + 1) % table_size + table_size;
    }
}

void overflow_area_probe::init(){
    probe = 2;
    return;
}