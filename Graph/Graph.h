#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include "../Alias/FunctionAlias.h"
#include "../Alias/UtilityAlias.h"
#include "../MultipleTree/MultipleTree.h"
#include "../CommonLibrary/Class.h"
namespace My {
	namespace Graph {
		using My::LinearList::LinkList;
		using My::LinearList::Queue;
		using My::Tree::MultipleTree;
		template <class TKey, class TValue>
		using Map = std::map<TKey, TValue>;
		template <class TValue>
		using Vector = std::vector<TValue>;
		template <class TValue, class Comparer=std::less<TValue>>
		using Set = std::set<TValue, Comparer>;
		template <class TIndex>
		class DisjointSet {
		private:
			Map<TIndex, TIndex> parent;
			TIndex find(const TIndex &index) {
				auto result = parent.find(index);
				return result == parent.end() ?
					index : parent[index] = find(result->second);
			}
		public:
			void join(const TIndex &a, const TIndex &b) {
				auto parentA = find(a), parentB = find(b);
				if (parentA != parentB)
					parent[parentA] = parentB;
			}
			bool query(const TIndex &a, const TIndex &b) {
				return find(a) == find(b);
			}
		};
		template <bool directed, class TIndex, class TEdge, class TNode>
		class GraphBasis {
		protected:
			template <class T> struct DefaultArgument {
				static T disconnectedValue() { return T(); }
				static T edgeValue() { return T(); }
			};
			template <> struct DefaultArgument<bool> {
				static bool disconnectedValue() { return false; }
				static bool edgeValue() { return true; }
			};

			template <bool connectionOnly> struct ReturnType;
			template<> struct ReturnType<true> { typedef bool type; };
			template<> struct ReturnType<false> { typedef TEdge type; };
		public:
			class Edge;
			class Node {
			public:
				TIndex index;
				TNode value;
				int indegree = 0;
				int outdegree = 0;
				LinkList<Edge> adjacentList;
				Node(const TIndex &index = TIndex()) :index(index), value(), adjacentList() {}
				Node(const TIndex &index, const TNode &value) :index(index), value(value), adjacentList() {}
				Node(const Node &node) :index(node.index), value(node.value), indegree(node.indegree),
					outdegree(node.outdegree), adjacentList(node.adjacentList) {}
				Node(Node &&node) noexcept :index(node.index), value(move(node.value)), indegree(move(node.indegree)),
					outdegree(move(node.outdegree)), adjacentList(move(node.adjacentList)) {}
				Node &operator = (const Node &node) {
					index = node.index, value = node.value;
					indegree = node.indegree, outdegree = node.outdegree;
					return *this;
				}
				Node &operator = (Node &&node) noexcept {
					index = move(node.index), value = move(node.value);
					indegree = move(node.indegree), outdegree = move(node.outdegree);
					return *this;
				}
				Edge &operator [] (const int &index) {
					return adjacentList[index];
				}
				const Edge &operator [] (const int &index) const {
					return adjacentList[index];
				}
			};
			class Edge {
			public:
				//raw index of nodes
				int src, dst;
				TEdge value;
				Edge() :src(), dst(), value() {}
				Edge(const int &src, const int &dst, const TEdge &value) :
					src(src), dst(dst), value(value) {}
				Edge(const int &src, const int &dst, TEdge &&value = TEdge()) :
					src(src), dst(dst), value(move(value)) {}
				Edge(const Edge &edge) :src(edge.src), dst(edge.dst), value(edge.value) {}
				Edge &operator = (const Edge &edge) {
					src = edge.src;
					dst = edge.dst;
					value = edge.value;
					return *this;
				}
				Edge(Edge &&edge) noexcept :src(move(edge.src)), dst(move(edge.dst)), value(move(edge.value)) {}
				Edge &operator = (Edge &&edge) noexcept {
					src = move(edge.src);
					dst = move(edge.dst);
					value = move(edge.value);
					return *this;
				}
				bool operator < (const Edge &edge) const {
					return value < edge.value;
				}
				bool operator > (const Edge &edge) const {
					return value > edge.value;
				}
			};
		protected:
			Vector<Node>nodes;
			Map<TIndex, int>indices;
			inline int index(const Node *const &pNode) const {
				return int(pNode - nodes.data());
			}
			inline int index(const Node &node) const {
				return int(&node - nodes.data());
			}
			inline const Node &find(const TIndex &index) const {
				auto result = indices.find(index);
				if (result == indices.end())
					throw OutOfRangeException("Index doesn't exist, considering using findOrInsert");
				return nodes[result->second];
			}
			inline Node &find(const TIndex &index) {
				auto result = indices.find(index);
				if (result == indices.end())
					throw OutOfRangeException("Index doesn't exist, considering using findOrInsert");
				return nodes[result->second];
			}
			Node &findOrInsert(const TIndex &index) {
				auto result = indices.find(index);
				if (result == indices.end())
					return addNode(index);
				return nodes[result->second];
			}
		public:
			GraphBasis() :nodes(), indices() {}
			GraphBasis(int nodeSize) :nodes(nodeSize), indices() {}
			GraphBasis(const GraphBasis &g) :nodes(g.nodes), indices(g.indices) {}
			GraphBasis(GraphBasis &&g) :nodes(move(g.nodes)), indices(move(g.indices)) {}
			Node &addNode(const TIndex &index, const TNode &value) {
				int newIndex = (int)nodes.size();
				indices[index] = newIndex;
				nodes.push_back(Node(index, value));
				return nodes[newIndex];
			}
			Node &addNode(const TIndex &index, TNode &&value = TNode()) {
				int newIndex = (int)nodes.size();
				indices[index] = newIndex;
				nodes.push_back(Node(index, move(value)));
				return nodes[newIndex];
			}
			void addEdge(const TIndex &src, const TIndex &dst, const TEdge &value, bool twoWay = false) {
				findOrInsert(src), findOrInsert(dst);
				auto &srcNode = find(src), &dstNode = find(dst);
				++srcNode.outdegree, ++dstNode.indegree;
				srcNode.adjacentList.append(Edge(index(srcNode), index(dstNode), value));
				if (twoWay) {
					dstNode.adjacentList.append(Edge(index(dstNode), index(srcNode), value));
					++dstNode.outdegree, ++srcNode.indegree;
				}
			}
			void addEdge(const TIndex &src, const TIndex &dst, TEdge &&value=DefaultArgument<TEdge>::edgeValue(), bool twoWay = false) {
				findOrInsert(src), findOrInsert(dst);
				auto &srcNode = find(src), &dstNode = find(dst);
				++srcNode.outdegree, ++dstNode.indegree;
				if (twoWay) {
					dstNode.adjacentList.append(Edge(index(dstNode), index(srcNode), value));
					++dstNode.outdegree, ++srcNode.indegree;
				}
				srcNode.adjacentList.append(Edge(index(srcNode), index(dstNode), move(value)));
			}
			inline Node &getNode(const TIndex &index) {
				return nodes[indices[index]];
			}
			inline const Node &getNode(const TIndex &index) const{
				return nodes[indices[index]];
			}
			LinkList<Node *> findNodes(const Function<bool(const Node &)> &predict) {
				LinkList<Node *> result;
				for (int i = 0; i < nodes.size(); ++i)
					if (predict(nodes[i]))
						result.append(&nodes[i]);
				return move(result);
			}
			inline int getInnerIndex(const TIndex &index) const {
				auto result = indices.find(index);
				return result->second;
			}
			inline int countNodes() const {
				return (int)nodes.size();
			}
			int countEdges() const {
				int result = 0;
				for (auto i = nodes.begin(); i != nodes.end(); ++i)
					result += i->adjacentList.getLength();
				return result;
			}
			const TIndex *getIndicesMapping() const {
				TIndex *result = new TIndex[nodes.size()];
				for (auto i = indices.begin(); i != indices.end(); ++i)
					result[i->second] = i->first;
				return result;
			}
			bool isConnected(const TIndex &src, const TIndex &dst) const {
				auto srcNode = find(src);
				for (auto i = srcNode.adjacentList.begin(); i != srcNode.adjacentList.end(); ++i)
					if (nodes[i->dst].index == dst)
						return true;
				return false;
			}
			template <bool connectionOnly=true>
			typename ReturnType<connectionOnly>::type **toAdjacencyMatrix(const typename ReturnType<connectionOnly>::type &disconnectedValue=DefaultArgument<typename ReturnType<connectionOnly>::type>::disconnectedValue()) const;
			template <>
			bool ** toAdjacencyMatrix<true>(const bool &disconnectedValue) const{
				int size = (int)nodes.size();
				bool **result = new bool *[size];
				for (int i = 0; i < size; ++i) {
					result[i] = new bool[size];
					memset(result[i], disconnectedValue, sizeof(bool) * size);
				}
				for (auto i = nodes.begin(); i != nodes.end(); ++i) {
					for (auto j = i->adjacentList.begin(); j != i->adjacentList.end(); ++j)
						result[j->src][j->dst] = true;
				}
				return result;
			}
			template <>
			TEdge **toAdjacencyMatrix<false>(const TEdge &disconnectedValue) const {
				int size = (int)nodes.size();
				TEdge **result = new TEdge *[size];
				for (int i = 0; i < size; ++i) {
					result[i] = new TEdge[size];
					for (int j = 0; j < size; ++j)
						result[i][j] = disconnectedValue;
				}
				for (auto i = nodes.begin(); i != nodes.end(); ++i) {
					for (auto j = i->adjacentList.begin(); j != i->adjacentList.end(); ++j)
						result[j->src][j->dst] = j->value;
				}
				return result;
			}
			Map<TIndex, Pair<bool, TEdge>> getShortestPaths(const TIndex &src) const {
				struct PathResult {
				public:
					enum class Flag {
						Calculated,Infinite,InSet
					};
					TEdge distance = TEdge();
					Flags<Flag, 3> flags;
					PathResult() { flags.set(Flag::Infinite, true); }
					bool operator < (const PathResult &pr) const {
						return distance < pr.distance;
					}
				};
				using Flag = typename PathResult::Flag;
				int size = (int)nodes.size();
				int source = indices.find(src)->second;
				PathResult *result = new PathResult[size];
				result[source].flags.set(Flag::Infinite, false);
				Set<PathResult *, PointerLess<PathResult>>set;
				set.insert(result + source);
				bool debug = false;
				while (!set.empty()) {
					PathResult *cur = *set.begin();
					result[cur - result].flags.set(Flag::Calculated,true).set(Flag::InSet,false);
					set.erase(set.begin());
					auto &adjList = nodes[cur - result].adjacentList;
					for (auto i = adjList.begin(); i != adjList.end(); ++i) {
						PathResult &dst = result[i->dst];
						TEdge newDistance = cur->distance + i->value;
						if (dst.flags.get(Flag::Infinite))
							dst.flags.set(Flag::Infinite,false);
						else if (newDistance < dst.distance) {
							if (dst.flags.get(Flag::Calculated))
								return Map<TIndex, Pair<bool, TEdge>>();
						}
						else continue;
						if (dst.flags.get(Flag::InSet))
							set.erase(set.find(&dst));
						else
							dst.flags.set(Flag::InSet,true);
						dst.distance = newDistance;
						set.insert(&dst);
					}
				}
				Map<TIndex, Pair<bool, TEdge>>ret;
				for (int i = 0; i < size; ++i)
					ret[nodes[i].index] = makePair(result[i].flags.get(Flag::Infinite), move(result[i].distance));
				delete[] result;
				return ret;
			}
			inline const Node &operator [](const int &innerIndex) const {
				return nodes[innerIndex];
			}
			inline Node &operator [](const int &innerIndex) {
				return nodes[innerIndex];
			}
		};
		template <bool directed, class TIndex = int, class TEdge = bool, class TNode = bool>
		class Graph;
		template <class TIndex, class TEdge, class TNode>
		class Graph<true, TIndex, TEdge, TNode> :public GraphBasis<true, TIndex, TEdge, TNode> {		
		public:
			using Super = GraphBasis<true, TIndex, TEdge, TNode>;
			using Node = typename Super::Node;
			using Edge = typename Super::Edge;
			Graph() :Super() {}
			Graph(int nodeSize) :Super(nodeSize) {}
			Graph(const Graph &g) :Super(g) {}
			Graph(Graph &&g) :Super(move(g)) {}
			LinkList<const Node *> getTopologicalOrder() const {
				int size = (int)this->nodes.size();
				int *tmpIndegree = new int[size];
				const Node *startNode = nullptr;
				for (int i = 0; i < size; ++i) {
					tmpIndegree[i] = this->nodes[i].indegree;
					if (this->nodes[i].indegree == 0)
						startNode = &this->nodes[i];
				}
				LinkList<const Node *>result;
				if (!startNode)
					return result;
				Queue<const Node *>queue;
				queue.push(startNode);
				while (!queue.empty()) {
					const auto top = queue.pop();
					result.append(top);
					for (auto i = top->adjacentList.begin(); i != top->adjacentList.end(); ++i) {
						if (!--tmpIndegree[i->dst])
							queue.push(&this->nodes[i->dst]);
					}
				}
				delete[] tmpIndegree;
				return result;
			}
			inline bool isAcyclic() const {
				return getTopologicalOrder().getLength() == this->countNodes();
			}
		};
		template <class TIndex, class TEdge, class TNode>
		class Graph<false, TIndex, TEdge, TNode> :public GraphBasis<false, TIndex, TEdge, TNode> {
		public:
			using Super = GraphBasis<false, TIndex, TEdge, TNode>;
			using Node = typename Super::Node;
			using Edge = typename Super::Edge;
			class SpanningTreeNode {
			public:
				TIndex index;
				TNode nvalue;
				TEdge evalue;
				SpanningTreeNode(const TIndex &index) :index(index), nvalue(), evalue() {}
				SpanningTreeNode(const TIndex &index, const TNode &nvalue, const TEdge &evalue = TEdge()) :
					index(index), nvalue(nvalue), evalue(evalue) {}
				SpanningTreeNode(const TIndex &index, TNode &&nvalue, TEdge &&evalue = TEdge()) :
					index(index), nvalue(move(nvalue)), evalue(move(evalue)) {}
			};
			using Tree = MultipleTree<SpanningTreeNode>;
		private:
			void getSpanningTree(const Node &node, SharedPointer<Tree> &pTree, DisjointSet<TIndex> &set) const {
				for (auto i = node.adjacentList.begin(); i != node.adjacentList.end(); ++i) {
					auto &dst = this->nodes[i->dst];
					auto &evalue = i->value;
					if (!set.query(node.index, dst.index)) {
						set.join(node.index, dst.index);
						pTree->children.append(makeShared<Tree>(SpanningTreeNode(dst.index, dst.value, evalue)));
						getSpanningTree(dst, pTree->children.last(), set);
					}
				}
			}
		public:
			Graph() :Super() {}
			Graph(int nodeSize) :Super(nodeSize) {}
			Graph(const Graph &g) :Super(g) {}
			Graph(Graph &&g) :Super(move(g)) {}
			void addEdge(const TIndex &src, const TIndex &dst, const TEdge &value) {
				Super::addEdge(src, dst, value, true);
			}
			void addEdge(const TIndex &src, const TIndex &dst, TEdge &&value= Super::template DefaultArgument<TEdge>::edgeValue()) {
				Super::addEdge(src, dst, move(value), true);
			}
			int countEdges() const {
				return Super::countEdges() >> 1;
			}
			bool isConnected(const TIndex &a, const TIndex &b) const {
				return Super::isConnected(a, b) && Super::isConnected(b, a);
			}
			int countConnectedComponents() const {
				int result = 0;
				int size = (int)this->nodes.size();
				bool *visited = new bool[size];
				memset(visited, false, sizeof(bool) * size);
				Queue<int>queue;
				for (int i = 0; i < size; ++i) {
					if (!visited[i]) {
						++result;
						queue.push(i);
						while (!queue.empty()) {
							int top = queue.pop();
							visited[top] = true;
							auto &adjList = this->nodes[top].adjacentList;
							for (auto j = adjList.begin(); j != adjList.end(); ++j)
								if (!visited[j->dst])
									queue.push(j->dst);
						}
					}
				}
				return result;
			}
			inline SharedPointer<Tree> getSpanningTree() const {
				return getSpanningTree(this->nodes[0].index);
			}
			bool isEulerCircuit() const {
				int oddNodeCount = 0;
				for (auto i = this->nodes.begin(); i != this->nodes.end(); ++i)
					if ((i->indegree + i->outdegree) & 1) 
						++oddNodeCount;
				return oddNodeCount == 0 || oddNodeCount == 2;
			}
			SharedPointer<Tree> getSpanningTree(const TIndex &rootIndex) const {
				const Node &root = this->nodes[this->indices.find(rootIndex)->second];
				SharedPointer<Tree> result = makeShared<Tree>(SpanningTreeNode(root.index, root.value));
				SharedPointer<Tree> cur = result;
				DisjointSet<TIndex> set;
				getSpanningTree(root, cur, set);
				return move(result);
			}
			inline Pair<TEdge,SharedPointer<Tree>> getMinimumSpanningTree() const {
				return getMinimumSpanningTree(this->nodes[0].index);
			}
			Pair<TEdge,SharedPointer<Tree>> getMinimumSpanningTree(const TIndex &rootIndex) const {
				int edgeCount = this->countEdges(), count = 0;
				auto edges = new const Edge * [(size_t)edgeCount << 1];
				for (auto i = this->nodes.begin(); i != this->nodes.end(); ++i) {
					for (auto j = i->adjacentList.begin(); j != i->adjacentList.end(); ++j)
						edges[count++] = &*j;
				}
				std::sort(edges, edges + ((size_t)edgeCount << 1), [](const Edge *a, const Edge *b) {return *a < *b; });
				DisjointSet<int> set;
				Graph spanningGraph;
				TEdge totalWeight = TEdge();
				for (int i = 0; i < (edgeCount << 1); ++i) {
					if (!set.query(edges[i]->src, edges[i]->dst)) {
						set.join(edges[i]->src, edges[i]->dst);
						spanningGraph.addEdge(this->nodes[edges[i]->src].index, this->nodes[edges[i]->dst].index, edges[i]->value);
						totalWeight = totalWeight + edges[i]->value;
					}
				}
				return Pair<TEdge,SharedPointer<Tree>>(move(totalWeight),spanningGraph.getSpanningTree(rootIndex));
			}
		};
	}
}