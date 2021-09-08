#include <fstream>
using std::ifstream;
int main() {
	ifstream input("input.txt");
	printf(input.is_open() ? "true" : "false");
	return 0;
}