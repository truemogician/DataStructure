#include "Graph.h"
using My::Graph::Graph;
using My::LinearList::Queue;
using std::cin;
using std::cout;
using std::endl;
Graph<false>graph;
void searchConnectedComponent(const int &n, bool **const &matrix, bool *const &visited, int cur, bool first=false) {
	visited[cur] = true;
	if (!first)
		putchar(' ');
	cout << cur;
	for (int i = 0; i < n; ++i)
		if (matrix[cur][i] && !visited[i])
			searchConnectedComponent(n, matrix, visited, i);
}
int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; ++i)
		graph.addNode(i);
	int src, dst;
	for (int i = 0; i < m; ++i) {
		cin >> src >> dst;
		graph.addEdge(src, dst);
	}
	auto matrix = graph.toAdjacencyMatrix();
	bool *visited = new bool[n];
	memset(visited, false, sizeof(bool) * n);
	for (int i = 0; i < n; ++i) {
		if (!visited[i]) {
			putchar('{');
			searchConnectedComponent(n, matrix, visited, i, true);
			putchar('}');
		}
	}
	putchar('\n');
	memset(visited, false, sizeof(bool) * n);
	Queue<int>queue;
	for (int i = 0; i < n; ++i)
		if (!visited[i]) {
			bool first = true;
			putchar('{');
			queue.push(i);
			while (!queue.empty()) {
				auto top = queue.pop();
				if (visited[top])
					continue;
				visited[top] = true;
				if (!first)
					putchar(' ');
				cout << top;
				first = false;
				for (int j = 0; j < n; ++j)
					if (matrix[top][j] && !visited[j])
						queue.push(j);
			}
			putchar('}');
		}
	putchar('\n');
	return 0;
}