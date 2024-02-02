#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

// 生成指定长度的字母串
std::string generateString(int length) {
    const char letters[] = {'A', 'B', 'C', 'D'};
    std::string result;

    if (length <= 0) {
        std::cerr << "Invalid length specified." << std::endl;
        return result;
    }

    // 设置随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 生成字母串
    for (int i = 0; i < length; ++i) {
        // 随机选择一个字母
        char letter = letters[std::rand() % 4];

        // 检查相邻的三个字母是否相同
        while (i >= 2 && result[i - 1] == letter && result[i - 2] == letter) {
            letter = letters[std::rand() % 4];
        }

        // 将字母添加到结果中
        result += letter;
    }

    return result;
}

int main() {
    int length;
    std::cout << "Enter the length of the string: ";
    std::cin >> length;

    // 生成字母串
    std::string generatedString = generateString(length);

    // 将字母串写入到文件中
    std::ofstream outputFile("input.txt");
    if (outputFile.is_open()) {
        outputFile << generatedString;
        outputFile.close();
        std::cout << "Generated string has been written to 'input.txt'." << std::endl;
    } else {
        std::cerr << "Unable to open output file." << std::endl;
        return 1;
    }

    return 0;
}
