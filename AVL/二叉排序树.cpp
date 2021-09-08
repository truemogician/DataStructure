#define _CRT_SECURE_NO_WARNINGS
#include "./AVL.h"
using My::Tree::Set;
int main() {
	Set<int>set;
	int n;
	(void)scanf("%d", &n);
	int opt, value;
	while (n--) {
		(void)scanf("%d", &opt);
		if (opt != 4)
			(void)scanf("%d", &value);
		switch (opt) {
			case 1:
				set.insert(value);
				break;
			case 2: 
				if (!set.remove(value))
					printf("None\n");
				break;
			case 3:
				printf("%d\n", (int)set.count(value));
				break;
			case 4: {
				auto ptr = set.root;
				while (ptr->leftChild)
					ptr = ptr->leftChild;
				printf("%d\n", ptr->value);
				break;
			}
			case 5: {
				auto result = set.findLess(value);
				if (result)
					printf("%d", result->value);
				else
					printf("None\n");
				break;
			}
		}
	}
	return 0;
}