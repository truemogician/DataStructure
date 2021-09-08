#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include "Sorting.h"
using namespace My;
using namespace My::Algorithm;
int main(){
	int n;
	(void)scanf("%d", &n);
	int *array = new int[n];
	for (int i = 0; i < n; ++i)
		(void)scanf("%d", array + i);;
	Sorting<int>::quick(array, array + n);
	for (int i = 0; i < n; ++i)
		printf("%d ",array[i]);
	return 0;
}