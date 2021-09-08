#pragma once
#include <iostream>
#include <fstream>
#include "AliasMacro.h"
namespace My{
	using std::cin;
	using std::cout;
	using std::endl;
	CLASS_ALIAS(std::istream, InStream);
	CLASS_ALIAS(std::ostream, OutStream);
	CLASS_ALIAS(std::ifstream, InFileStream);
	CLASS_ALIAS(std::ofstream, OutFileStream);
	CLASS_ALIAS(std::fstream, FileStream);
}