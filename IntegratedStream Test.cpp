#include "IntegratedStream.h"
using namespace My;
using std::endl;
int main() {
	InputStream input;
	OutputStream output;
	string inFile = ".\\test.in";
	string outFile = ".\\test.out";
	string str[3];
	input.add(inFile);
	input.selectFile(inFile);
	input.echoOut.add(outFile);
	input.echoOut.selectFile(outFile);
	input >> str[0];
	input.selectStandard();
	input >> str[1];
	output << str[0] << ' ' << str[1] << endl;
	return 0;
}