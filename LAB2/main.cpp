#include <cstdio>
#include "hashtable.h"
//#include <ctime>
// The hashing methods we have are bad hashing and good hashing;
// 

int main(){
    //freopen("out3-2-3.txt","w",stdout); // 3 datas, 2 hashings, 3 probings
    //freopen("near.txt","r",stdin);
    //double t1=clock();
    int type;
    char buffer[1000];int data;
    hashtable table(TABLE_SIZE,new good_hashing(),new overflow_area_probe());
    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            printf("%d\n", table.query(buffer));
        }else break;
    }
    //double t2=clock();
    //double result = (t2-t1)/CLOCKS_PER_SEC;//result里存的就是要测时间的代码段的运行时间，单位秒
    //printf("The time we used is: %f\n", result * 1000);
    return 0;
}