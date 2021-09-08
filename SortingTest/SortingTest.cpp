#include "pch.h"
#include "CppUnitTest.h"
#include <ctime>
#include <algorithm>
#include "../Sorting/Sorting.h"
#include "../Random/Random.h"
#include "../Alias/StreamAlias.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace My;
using namespace My::Algorithm;

namespace SortingTest{
	typedef int Type;
	int length = 10000;
	bool writeToFile = true;
	enum class StabilityTest{
		None, Increasing, Decreasing
	};
	StabilityTest stability = StabilityTest::Decreasing;
	char *fileName = "input8.txt";
	Type *arr;
	Type *expected;

	void initialize(){
		srand((unsigned int)time(0));
		arr = new Type[length];
		expected = new Type[length];
		for (int i = 0; i < length; ++i)
			arr[i] = Random<Type>::get();
		if (stability == StabilityTest::Increasing)
			std::sort(arr, arr + length);
		else if (stability == StabilityTest::Decreasing)
			std::sort(arr, arr + length, [](const Type &a, const Type &b){return a > b; });
		if (writeToFile){
			InFileStream ifstream(fileName);
			if (!ifstream.good()){
				ifstream.close();
				OutFileStream ofstream(fileName);
				ofstream << length << endl;
				for (int i = 0; i < length; ++i)
					ofstream << arr[i] << ' ';
				ofstream << endl;
			}
		}
		
		memcpy(expected, arr, sizeof(Type) * length);
		std::sort(expected, expected + length);
	}
	TEST_CLASS(SortingTest){
	public:
		TEST_CLASS_INITIALIZE(init){
			initialize();
		}
		/*TEST_METHOD_CLEANUP(assert){
			for (int i = 0; i < length; ++i)
				Assert::AreEqual<Type>(expected[i], arr[i]);
		}*/
		TEST_METHOD(bubble){
			Sorting<Type>::bubble(arr, arr + length);
		}
		TEST_METHOD(selection){
			Sorting<Type>::selection(arr, arr + length);
		}
		TEST_METHOD(insertion){
			Sorting<Type>::insertion(arr, arr + length);
		}
		TEST_METHOD(shell){
			Sorting<Type>::shell(arr, arr + length);
		}
		TEST_METHOD(merge){
			Sorting<Type>::merge(arr, arr + length);
		}
		TEST_METHOD(heap){
			Sorting<Type>::heap(arr, arr + length);
		}
		TEST_METHOD(quick){
			Sorting<Type>::quick(arr, arr + length);
		}
		TEST_METHOD(stdsort){
			std::sort(arr, arr + length);
		}
	};
}
