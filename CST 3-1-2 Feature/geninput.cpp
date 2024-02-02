#include <iostream>
#include <random>
using namespace std;
#define data_range 1000
int main()
{
    int n,m;
     std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> n_rand(1, data_range);
    n =n_rand(gen);
   // std::uniform_int_distribution<> n_rand(1, 100000);
    std::uniform_int_distribution<> m_rand(1, data_range);
    m = m_rand(gen);
    cout << n << ' ' << m << endl;
    //n = 10;
    std::uniform_int_distribution<>w_rand(-1000000000, 1000000000);
    std::uniform_int_distribution<>x_rand(-1000, 1000);
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < m;j++)
            cout<< w_rand(gen)<<' '<< x_rand(gen)<<endl;
    }


    std::uniform_int_distribution<>cmd_rand(0, 2);
    char cmd[3] = { 'W','V','C' };
    std::uniform_int_distribution<>range_rand(1, n);
    for (int i = 0; i < m; i++)
    {
        char tc = cmd[cmd_rand(gen)];
        if(tc=='W')
            cout <<tc<<' '<< range_rand(gen)<<' '<< w_rand(gen)<< endl;
        else if (tc == 'V')
            cout << tc << ' ' << range_rand(gen) << ' ' << x_rand(gen) << endl;
        else if (tc == 'C')
            cout << tc << ' ' << range_rand(gen)  << endl;
    }
   // cout << endl;

}

