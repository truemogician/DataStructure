#include "Stack.h"
#include <tuple>
using namespace My;
using std::tuple;
using std::make_tuple;
using std::get;
using std::tuple_element;
using std::endl;
template <typename R, typename... Args>
class BreakPoint {
public:
	bool trackBack = false;
	R *result;
	BreakPoint *father;
	tuple<Args...> parameters;
	BreakPoint(BreakPoint *father = nullptr) :trackBack(false), result(new R), father(father) {}
	BreakPoint(BreakPoint *father, Args ...args) :BreakPoint(father) {
		parameters = make_tuple<Args...>(std::move(args)...);
	}
	template <size_t index>
	typename tuple_element<index, tuple<Args...>>::type &parameter() {
		return get<index>(parameters);
	}
	template <size_t index>
	typename tuple_element<index, tuple<Args...>>::type parameter() const {
		return get<index>(parameters);
	}
};
int main() {
	using FactorialBreakPoint = BreakPoint<int, int>;
	Stack<FactorialBreakPoint>systemStack(1 << 5);
	int n;
	cin >> n;
	systemStack.push(FactorialBreakPoint(nullptr, n));
	int answer = 0;
	while (!systemStack.empty()) {
		auto &cur = systemStack.top();
		int &arg = cur.parameter<0>();
		if (arg == 1) {
			*cur.father->result = *cur.result = 1;
			systemStack.pop();
		}
		else if (!cur.trackBack) {
			cur.trackBack = true;
			systemStack.push(FactorialBreakPoint(&cur, arg - 1));
		}
		else {
			*cur.result *= arg;
			if (cur.father)
				*cur.father->result = *cur.result;
			else
				answer = *cur.result;
			systemStack.pop();
		}
	}
	cout << answer << endl;
	return 0;
}