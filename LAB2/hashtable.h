#include <cstring>
#include <cstdio>
const int TABLE_SIZE = 499883; // = 124970 * 4 + 3
extern int probe;
struct hash_entry{
    // As the name suggests, this class provides an entry to the actual container. The two fields below are respectively the key and value.
    char* my_string; // Key, which is the string we are to insert.
    int my_data; // The other parameter related to the key. We don't need that as a hash parameter.
    hash_entry(){
        my_string = NULL;my_data = 0;
    }
    hash_entry(char* str, int data):my_data(data){
        my_string = new char[strlen(str)+1];
        strcpy(my_string,str);
    }
    // using a certain string and int to init hash_entry
};

struct overflow_area: public hash_entry{
    int my_next;
    overflow_area() : hash_entry(), my_next(-1) {}
    overflow_area(char* str, int data, int next) : hash_entry(str, data), my_next(next) {}
};


struct hashing_strategy{
    virtual int operator()(char* str, int N)=0;
};

// Below are the 3 hashing methods.
struct naive_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};
// The worst hashing method that doesn't even use all the letters.
struct bad_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};  
// The bad hashing method that uses all the letters but provides an uneven distribution.
struct good_hashing: public hashing_strategy{
    int operator()(char* str, int N) override;
};
// The good hashing method that users all the letters and provides an even distribution at the same time.

struct collision_strategy{
    virtual void init()=0;// pure virtual function
    virtual int operator()(hash_entry* Table, int table_size, int last_choice)=0;
};
struct linear_probe: public collision_strategy{
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
// Linear_probe: if the place we are to push our string is occupied, then put it in the next place(mod table_size)
struct bilateral_square_probe: public collision_strategy{
    int previous_step = 0;
    int next_step = 1;
    int sign = 1;
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
struct overflow_area_probe: public collision_strategy{
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};



struct hashtable{
    hash_entry* Table;
    overflow_area* Overflow;
    int* previous;
    int* enter;
    int table_size;
    hashing_strategy* my_hashing; // 如果改为hashing_strategy my_hashing, 即不是用指针作为hashtable的成员, 而是让hashing_strategy结构体直接作为hashtable的成员. 会发生什么bug?
    collision_strategy* my_collision;
    int overflow_ptr = 0;
    hashtable(int size, hashing_strategy* hashing, collision_strategy* collision)
        :table_size(size),my_hashing(hashing),my_collision(collision)
    {
        Table = new hash_entry[table_size];
        Overflow = new overflow_area[table_size * 3];
        previous = new int[table_size];
        enter = new int[table_size];
        memset(previous, -1, sizeof(int) * table_size);
        memset(enter, -1, sizeof(int) * table_size);
        overflow_ptr = 0;
    }
    bool insert(hash_entry entry){ 
        int last_choice = (*my_hashing)(entry.my_string,table_size); 
        my_collision->init();
        if(probe != 2) {
            while(Table[last_choice].my_string!=NULL){ // loop infinitely? return false when no more space?
                last_choice = (*my_collision)(Table, table_size, last_choice);
            }
            Table[last_choice] = entry;
            return true;
        } else {
            if (Table[last_choice].my_string == NULL) {
                Table[last_choice] = entry;
                return true;
            } else {

                if(enter[last_choice] == -1) {
                    enter[last_choice] = overflow_ptr;
                }

                Overflow[overflow_ptr].my_string = entry.my_string;
                Overflow[overflow_ptr].my_data = entry.my_data;
                
                if (previous[last_choice] != -1) {
                    Overflow[previous[last_choice]].my_next = overflow_ptr;
                }

                previous[last_choice] = overflow_ptr;
                overflow_ptr++;
                overflow_ptr %= (table_size * 3);
                return true;
            }
        }
    }
    int query(char* query_string){
        int last_choice = (*my_hashing)(query_string,table_size); 
        my_collision->init();
        if (probe != 2) {
            while(Table[last_choice].my_string!=NULL && 
                strcmp(Table[last_choice].my_string, query_string)!=0){ // 未处理的情况: 哈希表已满?
                last_choice = (*my_collision)(Table, table_size, last_choice);
            }
            if(Table[last_choice].my_string == NULL){
                return -1;
            }
            else {
                return Table[last_choice].my_data;
            }
        } else {
            if (Table[last_choice].my_string!=NULL && 
                strcmp(Table[last_choice].my_string, query_string)==0){
                //Hit at the beginning
                return Table[last_choice].my_data;
            } else {
                int finder = enter[last_choice];
                if (finder == -1) return -1;
                while (finder != -1 && Overflow[finder].my_string != NULL &&
                    strcmp(Overflow[finder].my_string, query_string)!=0){

                    finder = Overflow[finder].my_next;
                }
                if (finder == -1) {
                    return -1;
                } else {
                    return Overflow[finder].my_data;
                }
            }
        }
    }
};
