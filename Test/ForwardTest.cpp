#include <utility>
#include <iostream>
#include <vector>
using namespace std;
class Foo {
public:
	template <class T>
	Foo(T v) {
		value(std::forward<T>(v));
	}
	void value(const int &v) {
		cout << "lvalue" << endl;
	}
	void value(int &&v) {
		cout << "rvalue" << endl;
	}
};
int main() {
	int a = 0;
	auto bar=Foo(a);
	bar = Foo(0);
	return 0;
}