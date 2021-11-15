#include <iostream> /* C++ 标准输入输出头文件 */

/**
 * 程序入口必须命名为 `main`
 * @param argc, "argument count" 的缩写，即"参数个数"的意思
 * @param argv, "argument vector" 的缩写, 即"参数数组"的意思，包含命令行的所有输入参数
 */
int main(int argc, char* argv[]) {
    std::string name; /* 字符串变量，姓名 */
    int coding_age;   /* 整数变量，已经学了多长时间编程 */

    /* 请告诉我你的英文名字 （注意，现在还不能输入输出中文名字) */
	std::cout<<"Please tell me your English name: ";
	std::cin>>name;

	/* 你好啊，[名字]，那你学过几年编程了？ */
	std::cout<<"Hello, "<<name;
	std::cout<<". How many years have you been learning coding? ";
	std::cin>>coding_age;
	/* 好棒，已经学了[n]年了啊。那么请继续精益求精！*/
	std::cout<<"Great, already "<<coding_age<<" years. ";
	std::cout<<"Please keep endeavouring for a better you!"<<std::endl;

	return 0;
}
