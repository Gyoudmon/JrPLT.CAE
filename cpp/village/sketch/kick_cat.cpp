#include <iostream>

/**
 * C++ 必须从一开始就引入类型，
 * 因此零基础初学者可以从 Racket 或 Python 等动态语言开始。
 * 趁学生对语言还没有形成概念，先借其他语言弄个有正反馈的版本，
 * 再开始繁琐的“编辑-编译-运行”流程。
 * 但类型安全方案要留到学生入门之后再说。
 */

// 定义函数：
void 踢(const char* 猫, const char* 脚, const char* 谁) {
    // 根据函数签名，还原自然语言描述
    printf("%s用%s脚踢了%s, %s很气愤\n", 谁, 脚, 猫, 猫);
}

// 定义类和方法(用 struct 可默认 public)
struct Person {
    void 踢(const char* 弱者, const char* 脚) {
        ::踢(弱者, 脚, this->name);
    }

    const char* name;
};

// 门槛不算高，但是极其折磨人
int main(int argc, char* argv[]) {
    Person Cpp{"C++"}, 老板{"老板"}, 老爸{"老爸"}, 孩子{"孩子"};

    Cpp.踢(老板.name, "右");
    老板.踢(老爸.name, "左");
    老爸.踢(孩子.name, "左");
    孩子.踢("猫", "左");

    return 0;    
}
