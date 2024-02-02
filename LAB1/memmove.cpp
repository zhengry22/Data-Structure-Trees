#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World!";
    char buffer[20];

    // 将字符串从源位置复制到目标位置
    memmove(buffer, str, strlen(str) + 1);

    printf("Original string: %s\n", str);
    printf("Copied string: %s\n", buffer);

    return 0;
}
