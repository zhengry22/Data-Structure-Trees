#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <random>
using namespace std;
class treedata
{
public:
    treedata(int num=8500001,int cmdnum=1000001)
    //treedata(int num = 200)
    {
        
        next = 0;
        datanum = num;
        randdata = new unsigned int[datanum];
        for (int i = 0; i < num; i++)
        {
            randdata[i] = i;
        }
        shuffle();
        dataintree= new unsigned int[cmdnum];
        treedatalen = 0;
        std::random_device rd;
        gen= std::mt19937(rd());
    
    }
    ~treedata()
    {
        delete[]randdata;
        delete[]dataintree;
    }

    void init_cmd(int anum, int bnum, int cnum)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i < (anum * 100 / (anum + bnum + cnum)))
                cmd[i] = 'A';
            else if (i < ((anum+bnum) * 100 / (anum + bnum + cnum)))
                cmd[i] = 'B';
            else if (i < ((anum + bnum+cnum/2) * 100 / (anum + bnum + cnum)))
                cmd[i] = 'C';
            else 
                cmd[i] = 'D';
        }
    
    }
    unsigned int insnode()
    {

        dataintree[treedatalen] = randdata[next++];
        return   dataintree[treedatalen++];

    }

    unsigned int delnode()
    {
        unsigned int retval;
        std::uniform_int_distribution<> randindex(0, treedatalen-1);
        int rndidx;
        rndidx = randindex(gen);
        if (treedatalen == 0)
            return -1;
        else if (treedatalen == 1)
        {
            treedatalen--;
            return dataintree[0];
        }
        else
        {
            retval = dataintree[rndidx];
            dataintree[rndidx] = dataintree[treedatalen-1];
            treedatalen--;
            return retval;
        }
    
    }
    unsigned int searchnode()
    {
		if (treedatalen == 0)
           return  randdata[0];
        std::uniform_int_distribution<> randindex(0, treedatalen - 1);
        int rndidx;
        rndidx = randindex(gen);
        return dataintree[rndidx];
       

    }
    unsigned int searchalldata()  //the data may not in tree
    {
        std::uniform_int_distribution<> randindex(0, datanum - 1);
        int rndidx;
        rndidx = randindex(gen);
        return randdata[rndidx];


    }
    void gendata(int n)
    {
        std::uniform_int_distribution<> randindex(0, 99);
        int rndidx;
        cout << n <<endl;
        for (int i = 0; i < n; i++)
        {
            rndidx = randindex(gen);
            switch (cmd[rndidx])
            {
            case 'A':
                cout << "A " << insnode()<<endl;
                break;
            case 'B':
                if (treedatalen >= 1)
                    cout << "B " << delnode()<<endl;
                else
                    cout << "A " << insnode()<<endl;
                break;
            case 'C':   //search the data in tree
                cout << "C " << searchnode() << endl;
                break;
            case 'D':  //data may not in the tree
                cout << "C " << searchalldata() << endl;
                break;
            default:
                break;
            }


        }
        //cout << cmd;
    
    
    }
private:
    void shuffle()
    {
 
        std::uniform_int_distribution<> randindex(0, datanum-1);
        for (int i = 0; i < datanum; i++)
        {
            int rndidx;
            unsigned int tmp;
            rndidx = randindex(gen);
            tmp = randdata[i];
            randdata[i] = randdata[rndidx];
            randdata[rndidx] = tmp;
        }
    
    }
    unsigned int* randdata;
    unsigned int* dataintree;
    int treedatalen;
    int datanum;
    int next;
    char cmd[100];
    std::mt19937 gen;
    
};
int main(int argc, char* argv[])
{
    treedata tdata;
    int total = 1000000;
    int Anum = 50;
    int Bnum = 30;
    int Cnum = 20;

    if (argc != 5) {
        cout << "command as : gentreedata.exe  [cmdnumber] [A percent] [B percent] [C percent]\n the percent number do not include %\n" << std::endl;
           // return 1;
    }
    else
    {
        // 将命令行参数转换为整数
        total = atoi(argv[1]);
        Anum = atoi(argv[2]);
        Bnum = atoi(argv[3]);
        Cnum = atoi(argv[4]);
    }
    tdata.init_cmd(Anum, Bnum, Cnum);
    tdata.gendata(total);

}

