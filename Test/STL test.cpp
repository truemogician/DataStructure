#include <iostream>
#include <set>
#include "../CommonLibrary/Class.h"
using namespace My;
using namespace std;
int function(int x) {
	cout << x << endl;
	return x;
}
int main() {
	set<int *, PointerLess<int>>s;
	int arr[4] = { 9,2,1,6 };
	for (int i = 0; i < 4; ++i)
		s.insert(arr + i);
	auto result = s.find(arr+3);
	cout << **result << endl;
	while (!s.empty()) {
		auto begin = s.begin();
		cout << **begin << endl;
		s.erase(begin);
	}
	cout << (function(1) < function(2)) << endl;
	return 0;
}