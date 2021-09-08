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
	lnk.unify();
	cout << lnk << endl;
	return 0;
}