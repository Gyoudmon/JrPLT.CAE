#include <iostream> /* C++ 标准输入输出头文件 */

/**
 * 程序入口必须命名为 `main`
 * @param argc, "argument count" 的缩写，即"参数个数"的意思
 * @param argv, "argument vector" 的缩写, 即"参数数组"的意思，包含命令行的所有输入参数
 */
int main(int argc, char* argv[]) {
    int coding_age = 2; /* 在定义变量的同时将变量赋值为 2 */

    /* 假设你已经学了两年编程 */
    std::cout<<"假如你已经学了" << coding_age << "年编程。" << std::endl;

    /* 注意：跟在数学中的意义不同，"=" 在 C++ 中表示"赋值"，而不是两边相等 */
    coding_age = coding_age + 3; /* coding_age 自增三年 */

    /* 3年后你的编程年龄就有[m]年啦! */
    std::cout << "那么再过三年，你就已经学了" << coding_age << "年编程啦！" << std::endl;

    return 0;
}

