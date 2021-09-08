#include "Graph.h"
using My::Graph::Graph;
using My::LinearList::Queue;
using std::cin;
using std::cout;
using std::endl;
Graph<false>graph;
void searchConnectedComponent(const int &n, bool *const &visited, int cur, bool first = false) {
	visited[cur] = true;
	if (!first)
		putchar(' ');
	cout << cur;
	auto curNode = graph[cur];
	for (auto i=curNode.adjacentList.begin(true); i!=curNode.adjacentList.end(true); --i)
		if (!visited[i->dst])
			searchConnectedComponent(n, visited, i->dst);
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
	bool *visited = new bool[n];
	memset(visited, false, sizeof(bool) * n);
	for (int i = 0; i < n; ++i) {
		if (!visited[i]) {
			putchar('{');
			searchConnectedComponent(n, visited, i, true);
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
				auto curNode = graph[top];
				for (auto j = curNode.adjacentList.begin(true); j != curNode.adjacentList.end(true); --j)
					if (!visited[j->dst])
						queue.push(j->dst);
			}
			putchar('}');
		}
	putchar('\n');
	return 0;
}