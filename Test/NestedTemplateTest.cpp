template <class T, bool Param1, int Param2>
class Class {
public:
	T value;
	bool boolean = Param1;
	int integer = Param2;
};
template <class ...TParams>
class Wrapper {
public:
	template <class TValue, TParams ...params>
	class Base {
	public:
		Class<TValue, params...> value;
	};
};
template <class T, bool Param1, int Param2>
class Derived :public Wrapper<bool, int>::Base<T, Param1, Param2> {};
int main() {
	Derived<int,true,0> test;
	return 0;
}