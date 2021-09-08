#include "pch.h"
#include "CppUnitTest.h"
#include "../Graph/Graph.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphTest {
	using namespace My;
	using My::LinearList::LinkList;
	using My::Graph::Graph;
	TEST_CLASS(GraphTest) {
	public:
		Graph<false, int, int>undirected;
		Graph<true, int, int>directed;
		void initialize(bool idDirected) {
			if (idDirected) {
				directed.addEdge(1, 2, 8);
				directed.addEdge(3, 1, 9);
				directed.addEdge(1, 5, 1);
				directed.addEdge(2, 4, 7);
				directed.addEdge(3, 4, 2);
				directed.addEdge(3, 5, 6);
			}
			else {
				undirected.addEdge(1, 2, 8);
				undirected.addEdge(1, 3, 9);
				undirected.addEdge(1, 5, 1);
				undirected.addEdge(2, 4, 7);
				undirected.addEdge(3, 4, 2);
				undirected.addEdge(3, 5, 6);
			}
		}
		TEST_METHOD(size) {
			initialize(false);
			Assert::AreEqual<int>(5, undirected.countNodes());
			Assert::AreEqual<int>(6, undirected.countEdges());
			initialize(true);
			Assert::AreEqual<int>(5, directed.countNodes());
			Assert::AreEqual<int>(6, directed.countEdges());
		}
		TEST_METHOD(adjacencyMatrix) {
			initialize(false);
			bool expUndir[5][5] = {
				{0,1,1,0,1},
				{1,0,0,1,0},
				{1,0,0,1,1},
				{0,1,1,0,0},
				{1,0,1,0,0}
			};
			auto result = undirected.toAdjacencyMatrix();
			auto mapping = undirected.getIndicesMapping();
			for (int i = 0; i < 5; ++i)
				for (int j = 0; j < 5; ++j)
					Assert::AreEqual<bool>(expUndir[mapping[i] - 1][mapping[j] - 1], result[i][j]);
			initialize(true);
			int expDir[5][5] = {
				{0,8,0,0,1},
				{0,0,0,7,0},
				{9,0,0,2,6},
				{0,0,0,0,0},
				{0,0,0,0,0}
			};
			auto rslt = directed.toAdjacencyMatrix<false>();
			for (int i = 0; i < 5; ++i)
				for (int j = 0; j < 5; ++j)
					Assert::AreEqual<int>(expDir[mapping[i] - 1][mapping[j] - 1], rslt[i][j]);
		}
		TEST_METHOD(topologicalOrder) {
			initialize(true);
			LinkList<int> expected(3, 1, 2, 5, 4);
			auto result = directed.getTopologicalOrder();
			for (int i = 0; i < 5; ++i)
				Assert::AreEqual<int>(expected[i], result[i]->index);
			Assert::IsTrue(directed.isAcyclic());
			directed.addEdge(4, 1, 10);
			Assert::IsFalse(directed.isAcyclic());
		}
		TEST_METHOD(minimunSpanningTree) {
			initialize(false);
			auto result = undirected.getMinimumSpanningTree(3);
			Assert::AreEqual<int>(16, result.first);
			Assert::AreEqual<int>(3, result.second->getHeight());
			Assert::AreEqual<int>(2, result.second->getWidth());
		}
		TEST_METHOD(shortestPath) {
			initialize(true);
			auto result = directed.getShortestPaths(1);
			Assert::IsTrue(result[3].first);
			Assert::AreEqual<int>(0, result[1].second);
			Assert::AreEqual<int>(8, result[2].second);
			Assert::AreEqual<int>(15, result[4].second);
			Assert::AreEqual<int>(1, result[5].second);
		}
		TEST_METHOD(connectedComponent) {
			initialize(false);
			undirected.addEdge(6, 7, -1);
			Assert::AreEqual<int>(2, undirected.countConnectedComponents());
		}
	};
}
