#define _CRT_SECURE_NO_WARNINGS
#include "Graph.h"
using My::Graph::Graph;
using std::cin;
using std::cout;
using std::endl;
Graph<false, int, int>graph;
int main() {
	int n, m, s;
	cin >> n >> m >> s;
	for (int i = 1; i <= n; ++i)
		graph.addNode(i);
	int src, dst, value;
	for (int i = 0; i < m; ++i) {
		cin >> src >> dst >> value;
		if (cin.eof())
			break;
		graph.addEdge(src, dst, value);
	}
	auto result = graph.getShortestPaths(s);
	for (int i = 1; i <= n; ++i)
		cout << (result[i].first ? 2147483647 : result[i].second) << ' ';
	cout << endl;
	return 0;
}