#include <iostream>
#include <string>
#include <ctime>
#include "Random.h"

#define BRANCH(T) \
else if (type == #T)\
	cout << Random<T>::get(0) << endl

using std::cin;
using std::cout;
using std::endl;
using std::string;
using namespace My::Algorithm;
int main(){
	srand(time(0));
	string type;
	while (true){
		std::getline(cin, type);
		if (type == "bool")
			cout << Random<bool>::get() << endl;
		BRANCH(char);
		BRANCH(unsigned char);
		BRANCH(short);
		BRANCH(unsigned short);
		BRANCH(int);
		BRANCH(unsigned int);
		BRANCH(long long);
		BRANCH(unsigned long long);
		BRANCH(float);
		BRANCH(double);
		BRANCH(long double);
		else break;
	}
}