#include <iostream>
using std::cout;
using std::endl;
constexpr int foo(int a, int b) {
    return a + b;
}
template <int result>
struct Result {
    static const int value = result;
};
template <int (*func)(int,int)>
struct Function {
    template <int a, int b>
    struct Execute {
        static const int result=Result<func(a,b)>::value;
    };
};
int main(){
    cout << Function<foo>::Execute<1, 2>::result;
    return 0;
}