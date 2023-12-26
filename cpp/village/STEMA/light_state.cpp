#include <iostream>

/*************************************************************/
bool is_light_on_by_remainder(int m) {
    return (m % 2 == 1);
}

bool is_light_on_by_simulation(int m) {
    if (m <= 0) {
        return false;
    } else {
        return !is_light_on_by_simulation(m - 1);
    }
}

void print_light_state(bool is_on) {
    if (is_on) {
        printf("%d\n", 1);
    } else {
        printf("%d\n", 0);
    }
}

/*************************************************************/
int main(int argc, char* argv[]) {
    int M;
    bool answer;

    // scanf 函数读取用户输入
    // & 是“取地址”运算符
    // 此句表示把读到的整数存放在变量 m 所在的位置处
    scanf("%d", &M);

    // 通过“余数判定法”得到灯的开-闭状态
    answer = is_light_on_by_remainder(M);

    // 按题目要求输出灯的状态
    print_light_state(answer);
    
    return 0;
}
