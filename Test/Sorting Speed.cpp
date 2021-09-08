#include <ctime>
#include "../Sorting/Sorting.h"
#include "../Random/Random.h"
using namespace My;
using namespace My::Algorithm;
const int N = 10000;
int array[N];
int main(){
	srand(time(0));
	for (int i = 0; i < N; ++i)
		array[i] = Random<int>::get(0,100);
	auto start = clock();
	Sorting<int>::insertion(array, array + N);
	printf("%d\n", clock() - start);
	return 0;
}