#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // 设置种子以确保每次运行生成不同的随机数序列
    srand(static_cast<unsigned>(time(nullptr)));

    int n = rand() % ((1 << 30) - 1) + 1; // 1 ≤ n < 2^31
    int m = rand() % 200000 + 1; // 1 ≤ m ≤ 200,000

    std::cout << n << " " << m << std::endl;

    for (int i = 0; i < m; ++i) {
        char op_type = (rand() % 2 == 0) ? 'H' : 'Q';
        int rand_i = rand() % n + 1; // 1 ≤ i ≤ n
        int rand_j = rand() % (n - rand_i + 1) + rand_i; // i ≤ j ≤ n

        std::cout << op_type << " " << rand_i << " " << rand_j << std::endl;
    }

    return 0;
}
