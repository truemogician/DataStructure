#include <iostream>
#include <memory>
#include <list>
#include <optional>
#include "CreateAliasMacro.h"
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;
using std::move;
using std::optional;
class Type {
public:
	int value;
	Type(const int &value=0) :value(value) {}
	Type(const Type &type) :value(type.value){
		cout << "Copy constructor called" << endl;
	}
	Type(Type &&type) noexcept :value(type.value) {
		cout << "Move constructor called" << endl;
	}
	Type &operator= (const Type &type) {
		cout << "Const left referrence assignment called" << endl;
		value = type.value;
		return *this;
	}
	Type &operator=(Type &&type) noexcept{
		cout << "Right referrence assignment called" << endl;
		value = type.value;
		return *this;
	}
	~Type() {
		cout << "Destructor called" << endl;
	}
};
Type foo1(Type &value) {
	return Type(value);
}
Type &&foo2(Type &value) {
	return Type(value);
}
class Base {
public:
	int value = 0;
	virtual void func(int a) {
		cout << a;
	}
	template<class ...Ts>
	void func(int a, Ts ...args) {
		func(a);
		func(args...);
	}
};
class Derived :public Base {
public:
	void func(int a) override {
		cout << "Derived " << a << ' ';
	}
	void test() {
		Base::func(1, 2, 3, 4);
	}
};

void ptrFunc(const shared_ptr<const int> &ptr) {
	cout << ptr.use_count() << endl;
	cout << "const reference pointer, const content";
}
void ptrFunc(const shared_ptr<int> &ptr) {
	cout << "const reference pointer";
}
void ptrFunc(shared_ptr<const int> &ptr) {
	cout << "const content";
}
#define makePointer std::make_shared
#define Pointer std::shared_ptr
int main() {
	/*Type bar1(123);
	cout << foo1(bar1).value << endl;
	cout << foo2(bar1).value << endl;
	Type bar2;
	bar2 = foo1(bar1);
	cout << bar2.value << endl;
	bar2 = foo2(bar1);
	cout << bar2.value << endl;*/
	/*Pointer<Base> bPtr = makePointer<Base>();
	Pointer<Derived> dPtr = std::static_pointer_cast<Derived>(bPtr);
	dPtr->func(1);
	bPtr = nullptr;
	dPtr = nullptr;*/
	/*optional<bool> v1;
	cout << (v1 == true) << (v1 == false) << (v1 == std::nullopt) << endl;*/
	return 0;
}