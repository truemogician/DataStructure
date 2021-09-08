#include "Graph.h"
using My::Graph::Graph;
using std::cin;
using std::cout;
using std::endl;
int main() {
	int type, n, m;
	cin >> type >> n >> m;
	switch (type) {
		case 1: {
			Graph<true, char>graph;
			char ch;
			for (int i = 0; i < n; ++i) {
				cin >> ch;
				cout << ch << ' ';
				graph.addNode(ch);
			}
			cout << endl;
			char src, dst;
			for (int i = 0; i < m; ++i) {
				cin >> src >> dst;
				graph.addEdge(src, dst);
			}
			auto adjMatrix = graph.toAdjacencyMatrix();
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j)
					printf("%4d", adjMatrix[i][j]);
				putchar('\n');
			}
			for (int i = 0; i < n; ++i) {
				cout << graph[i].index << "-->";
				for (auto j = graph[i].adjacentList.begin(true); j != graph[i].adjacentList.end(true); --j)
					cout << j->dst << ' ';
				cout << endl;
			}
			break;
		}
		case 2: {
			Graph<true, char, int>graph;
			char ch;
			for (int i = 0; i < n; ++i) {
				cin >> ch;
				cout << ch << ' ';
				graph.addNode(ch);
			}
			cout << endl;
			char src, dst;
			int value;
			for (int i = 0; i < m; ++i) {
				cin >> src >> dst >> value;
				graph.addEdge(src, dst, value);
			}
			auto adjMatrix = graph.toAdjacencyMatrix<false>();
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j)
					printf("%4d", adjMatrix[i][j]);
				putchar('\n');
			}
			for (int i = 0; i < n; ++i) {
				cout << graph[i].index << "-->";
				for (auto j = graph[i].adjacentList.begin(true); j != graph[i].adjacentList.end(true); --j)
					cout << j->dst << ',' << j->value << ' ';
				cout << endl;
			}
			break;
		}
		case 3: {
			Graph<false, char>graph;
			char ch;
			for (int i = 0; i < n; ++i) {
				cin >> ch;
				cout << ch << ' ';
				graph.addNode(ch);
			}
			cout << endl;
			char src, dst;
			for (int i = 0; i < m; ++i) {
				cin >> src >> dst;
				graph.addEdge(src, dst);
			}
			auto adjMatrix = graph.toAdjacencyMatrix();
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j)
					printf("%4d", adjMatrix[i][j]);
				putchar('\n');
			}
			for (int i = 0; i < n; ++i) {
				cout << graph[i].index << "-->";
				for (auto j = graph[i].adjacentList.begin(true); j != graph[i].adjacentList.end(true); --j)
					cout << j->dst << ' ';
				cout << endl;
			}
			break;
		}
		case 4: {
			Graph<false, char, int>graph;
			char ch;
			for (int i = 0; i < n; ++i) {
				cin >> ch;
				cout << ch << ' ';
				graph.addNode(ch);
			}
			cout << endl;
			char src, dst;
			int value;
			for (int i = 0; i < m; ++i) {
				cin >> src >> dst >> value;
				graph.addEdge(src, dst, value);
			}
			auto adjMatrix = graph.toAdjacencyMatrix<false>();
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j)
					printf("%4d", adjMatrix[i][j]);
				putchar('\n');
			}
			for (int i = 0; i < n; ++i) {
				cout << graph[i].index << "-->";
				for (auto j = graph[i].adjacentList.begin(true); j != graph[i].adjacentList.end(true); --j)
					cout << j->dst << ',' << j->value << ' ';
				cout << endl;
			}
			break;
		}
	}
}