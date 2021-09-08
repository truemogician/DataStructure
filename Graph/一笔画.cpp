#include "Graph.h"
using My::Graph::Graph;
using My::LinearList::LinkList;
using std::cin;
using std::cout;
using std::endl;
Graph<false>graph;
void findEulerCircuit(int innerIndex, int depth, LinkList<int> &stack, int &nodeCount, int &edgeCount, bool **const matrix) {
	stack.append(graph[innerIndex].index);
	if (depth == edgeCount) {
		for (auto i = stack.begin(); i != stack.end(); ++i)
			cout << *i << ' ';
		cout << endl;
	}
	for (int i = 0; i < nodeCount; ++i) {
		if (matrix[innerIndex][i]) {
			matrix[innerIndex][i] = false;
			matrix[i][innerIndex] = false;
			findEulerCircuit(i, depth + 1, stack, nodeCount, edgeCount, matrix);
			matrix[i][innerIndex] = true;
			matrix[innerIndex][i] = true;
		}
	}
	stack.remove(stack.getLength() - 1);
}
void findEulerCircuit(int srcIndex) {
	auto matrix = graph.toAdjacencyMatrix();
	LinkList<int> stack;
	int nodeCount = graph.countNodes();
	int edgeCount = graph.countEdges();
	findEulerCircuit(graph.getInnerIndex(srcIndex), 0, stack, nodeCount, edgeCount, matrix);
}
int main() {
	int n, m, s;
	cin >> n >> m >> s;
	int src, dst;
	for (int i = 0; i < m; ++i) {
		cin >> src >> dst;
		graph.addEdge(src, dst);
	}
	bool isCircuit = graph.isEulerCircuit();
	if (!isCircuit){
		cout << "Impossible" << endl;
		return 0;
	}
	else {
		auto oddNodes = graph.findNodes([](const typename Graph<false>::Node &node) {
			return (node.indegree + node.outdegree) & 1;
		});
		if (!oddNodes.isEmpty()) {
			bool possible = false;
			for (auto i=oddNodes.begin(); i!=oddNodes.end(); ++i)
				if ((*i)->index == s) {
					possible = true;
					break;
				}
			if (!possible){
				cout << "Impossible" << endl;
				return 0;
			}
		}
	}
	findEulerCircuit(s);
	return 0;
}