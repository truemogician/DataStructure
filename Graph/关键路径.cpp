#include "Graph.h"
using Graph = My::Graph::Graph<true, int, int>;
using My::LinearList::LinkList;
using std::cin;
using std::cout;
using std::endl;
Graph graph;
void findKeyPath(Graph::Node &cur, int curCost, int &cost, LinkList<int> &stack, LinkList<int> &result) {
	if (!cur.outdegree) {
		if (curCost > cost) {
			cost = curCost;
			result = stack;
			result.append(cur.index);
		}
		return;
	}
	stack.append(cur.index);
	for (auto i = cur.adjacentList.begin(true); i != cur.adjacentList.end(true); --i)
		findKeyPath(graph[i->dst], curCost + i->value, cost, stack, result);
	stack.remove(stack.getLength() - 1);
}
void findKeyPath(int index, int &cost, LinkList<int> &result) {
	LinkList<int> stack;
	findKeyPath(graph[graph.getInnerIndex(index)], 0, cost, stack, result);
}
int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 0; i <= n; ++i)
		graph.addNode(i);
	int src, dst, value;
	for (int i = 0; i < m; ++i) {
		cin >> src >> dst >> value;
		graph.addEdge(src, dst, value);
	}
	for (int i = n; i >= 1; --i) {
		if (!graph.getNode(i).indegree)
			graph.addEdge(0, i, 0);
	}
	if (!graph.isAcyclic()) {
		cout << 0 << endl;
		return 0;
	}
	LinkList<int>result;
	int cost = 0;
	findKeyPath(0, cost, result);
	cout << cost << endl;
	for (int i = 1; i < result.getLength()-1; ++i)
		cout << result[i] << "->" << result[i + 1] << endl;
	return 0;
}