#include "Graph.h"
using My::Graph::Graph;
using std::cin;
using std::cout;
using std::endl;
Graph<false, int, int>graph;
int main() {
	int n;
	cin >> n;
	int value;
	for (int i=0; i<n; ++i)
		for (int j = 0; j < n; ++j) {
			cin >> value;
			if (i < j)
				graph.addEdge(i, j, value);
		}
	cout << graph.getMinimumSpanningTree().first << endl;
	return 0;
}