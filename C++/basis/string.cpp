#include <iostream> /* C++ 标准输入输出头文件 */
#include <cstring>  /* C++ 字符串头文件 */

int main(int argc, char* argv[]) {
    // 字符
    char a = 'a';
    char tab = '\t';
    char back_slash = '\'';
    
    // 字符串
    std::string char_tab = "a\t";
    std::string backslash_newline = "\'\n";

    // 字符串操作符
    std::string literal = char_tab + backslash_newline;

    std::cout << a << tab << back_slash << '\n';
    std::cout << literal;

    // 修改字符串
    literal[0] = 'b';
    std::cout << literal[0] << literal[1] << literal[2] << literal[3];

    // 中文字符和字符串
    char chinese_char_array[] = "我是一个汉字字符数组";
    std::string chinese_string = "我是一个汉字字符串";
    std::cout << "chinese_char_array" << ": " << chinese_char_array << std::endl;
    std::cout << "chinese_string" << ": " << chinese_string << std::endl;

    return 0;
}

