#include <iostream> /* C++ header file */

/**
 * `main` is the entry point of an executable file
 * @param argc, short for "argument count"
 * @param argv, short for "argument vector", holds the content of arguments
 */
int main(int argc, char* argv[]) {
	/* NOTE: the first argument is always the name of the executable file */
	std::cout<<"running: " << argv[0] <<std::endl;
	std::cout<<"  received " << (argc - 1) <<" argumnets from user"<<std::endl;
	
	/* display all user input arguments to console line by line */
	for (int i = 1; i < argc; i++) {
		std::cout<<"    argv["<< i <<"]: "<<argv[i]<<std::endl;
	}

	return 0;
}
