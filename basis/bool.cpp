#include <iostream> /* C++ 标准输入输出头文件 */

int main(int argc, char* argv[]) {
    // 直接赋值
    bool t = true;
    bool f = false;

    // 数值类型到布尔类型的隐式类型转换
    bool pi = 3.14;
    bool zero = 0.0;

    std::cout << std::boolalpha; // 不要将布尔值转化为数字
    std::cout << "variable 't' is " << t << std::endl;
    std::cout << "variable 'f' is " << f << std::endl;
    std::cout << "variable 'pi' is " << pi << std::endl;
    std::cout << "variable 'zero' is " << zero << std::endl;

    return 0;
}

