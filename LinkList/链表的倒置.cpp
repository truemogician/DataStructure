#include "LinkList.h"
using namespace My;
int main() {
	SingleLinkList<int> lnk;
	int n, value;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> value;
		lnk.append(value);
	}
	cin >> n;
	int left, right;
	while (n--) {
		cin >> left >> right;
		if (left > right || left < 1 || right >lnk.length()) {
			cout << "0" << endl;
			continue;
		}
		auto rightLnk = lnk.split(right);
		auto middleLnk = lnk.split(left - 1);
		middleLnk->reverse();
		lnk.concat(*middleLnk).concat(*rightLnk);
		cout << lnk << endl;
	}
	return 0;
}