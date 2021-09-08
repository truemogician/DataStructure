#include "LinkList.h"
using namespace My;
int main() {
	SingleLinkList<int> lnk;
	int n, pos, value;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> value;
		lnk.prepend(value);
	}
	cout << lnk << endl;
	cin >> pos >> value;
	auto result=lnk.insert(pos - 1, value);
	if (!result)
		cout << "-1\n";
	else
		cout << lnk << endl;
	cin >> pos;
	bool legal = lnk.removeAt(pos - 1);
	if (!legal)
		cout << "-1\n";
	else
		cout << lnk << endl;
	cin >> value;
	auto res = lnk.query(value);
	if (res.first == -1)
		cout << "-1\n";
	else
		cout << res.first + 1 << endl;
	cout << lnk.length() << endl;
	return 0;
}