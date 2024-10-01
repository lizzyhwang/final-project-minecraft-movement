// Support Code written by Michael D. Shah
// Last Updated: 1/21/17
// Please do not redistribute without asking permission.

// std libraries
#include <iostream>
#include <string>

// Functionality that we created
#include "SDLGraphicsProgram.hpp"

int main(int argc, char** argv){

	// Warn the user they need to specify at least one argument
    // We detect this by checking the size of the argument count (argc)
    // if(argc < 2){
    //     std::cout << "Please specify a .obj model" << std::endl;
    //     return 0;
    // }
	std::string filepath;
	filepath = "media/island/island_small.obj";
	//filepath = "media/plaza/plaza_smaller.obj";
	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280,720, filepath);
	// Run our program forever
	mySDLGraphicsProgram.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
