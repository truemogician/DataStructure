#include "Graph.h"
using My::Graph::Graph;
using std::cin;
using std::cout;
using std::endl;
Graph<true,int,int>graph;
int main() {
	int n, m;
	cin >> n >> m;
	int src, dst, value;
	for (int i = 0; i < m; ++i) {
		cin >> src >> dst >> value;
		graph.addEdge(src, dst, value);
	}
	cout << graph.isAcyclic() << endl;
	return 0;
}