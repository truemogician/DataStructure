#pragma once
#include "BinaryTree.h"
#include <optional>
#include <map>
namespace My {
	namespace Tree {
		using std::string;
		using std::map;
		template <class T>
		using Optional = std::optional<T>;
		template <class T>
		struct Frequency {
			Optional<T> value;
			int count;
			Frequency() :count(0) {}
			Frequency(const T &value, const int &count = 0) :value(value), count(count) {}
			Frequency(T &&value, const int &count = 0) :value(move(value)), count(count) {}
			Frequency operator + (const Frequency &freq) const {
				Frequency result;
				result.count = count + freq.count;
				return move(result);
			}
			inline bool operator < (const Frequency &freq) const {
				return count < freq.count;
			}
			Frequency &operator = (const Frequency &freq) {
				value = freq.value;
				count = freq.count;
				return *this;
			}
		};
		template <class T>
		class HuffmanTree :public BinaryTree<Frequency<T>> {
			using Super = BinaryTree<Frequency<T>>;
			using Self = HuffmanTree<T>;
			using SuperSharedPointer = SharedPointer<Super>;
			using SelfSharedPointer = SharedPointer<Self>;
		private:
			static void getAverageBitwidth(const Super *const &ptr, int &result, int &length) {
				if (ptr->value.value) {
					result += length * ptr->value.count;
					--length;
					return;
				}
				if (ptr->leftChild)
					getAverageBitwidth(ptr->leftChild.get(), result, ++length);
				if (ptr->rightChild)
					getAverageBitwidth(ptr->rightChild.get(), result, ++length);
				--length;
				return;
			}
			static string getCode(const T &value, const Super *const &ptr, bool fromRight) {
				if (ptr->value.value == value)
					return fromRight ? "1" : "0";
				else {
					string result = "";
					if (ptr->leftChild)
						result = getCode(value, ptr->leftChild.get(), false);
					if (result == "" && ptr->rightChild)
						result = getCode(value, ptr->rightChild.get(), true);
					if (result != "")
						result = (fromRight ? "1" : "0") + result;
					return result;
				}
			}
			static void generateCodeTable(const Super *const &ptr, map<T, string> &table, string &code) {
				if (ptr->value.value) {
					table[ptr->value.value.value()] = code;
					code = code.substr(0, code.length() - 1);
					return;
				}
				generateCodeTable(ptr->leftChild.get(), table, code += "0");
				generateCodeTable(ptr->rightChild.get(), table, code += "1");
				code = code.substr(0, code.length() - 1);
			}
			static void generateReverseCodeTable(const Super *const &ptr, map<string, T> &table, string &code) {
				if (ptr->value.value) {
					table[code] = ptr->value.value.value();
					code = code.substr(0, code.length() - 1);
					return;
				}
				generateReverseCodeTable(ptr->leftChild.get(), table, code += "0");
				generateReverseCodeTable(ptr->rightChild.get(), table, code += "1");
				code = code.substr(0, code.length() - 1);
			}
		public:
			static SelfSharedPointer createHuffmanTree(LinkList<Frequency<T>> &&freqTable) {
				freqTable.sort();
				LinkList<SelfSharedPointer> indexTable;
				for (auto i = freqTable.begin(); i != freqTable.end(); ++i) {
					indexTable.append(makeShared<HuffmanTree>(*i));
				}
				while (indexTable.getLength() > 1) {
					auto &nodeA = indexTable[0];
					auto &nodeB = indexTable[1];
					auto newNode = makeShared<HuffmanTree>(nodeA->value + nodeB->value, nodeA, nodeB);
					nodeA->parent = std::static_pointer_cast<Super>(newNode);
					nodeB->parent = nodeA->parent;
					indexTable.remove(0).remove(0);
					int index = 0;
					for (auto i = indexTable.begin(); i != indexTable.end(); ++i, ++index)
						if (newNode->value < (*i)->value) {
							indexTable.insert(index, newNode);
							break;
						}
					if (index == indexTable.getLength())
						indexTable.append(newNode);
				}
				return move(indexTable[0]);
			}
			static SelfSharedPointer createHuffmanTree(const LinkList<T> &content) {
				map<T, int> freqMap;
				for (auto i = content.begin(); i != content.end(); ++i) {
					if (freqMap.find(*i) == freqMap.end())
						freqMap[*i] = 1;
					else
						++freqMap[*i];
				}
				LinkList<Frequency<T>> freqList;
				for (auto i = freqMap.begin(); i != freqMap.end(); ++i)
					freqList.append(Frequency(i->first, i->second));
				return move(createHuffmanTree(move(freqList)));
			}
			inline static SelfSharedPointer createHuffmanTree(const T *const &content, const int &length) {
				return createHuffmanTree(LinkList<T>(content, length));
			}
			double getAverageBitwidth() const {
				int summary = 0;
				int auxLength = 0;
				getAverageBitwidth(this, summary, auxLength);
				return (double)summary / this->value.count;
			}
			string encode(const LinkList<T> &content) {
				auto table = generateCodeTable();
				string result = "";
				for (auto i = content.begin(); i != content.end(); ++i)
					result += table[*i];
				return result;
			}
			string encode(const T *const &content, const int &length) {
				auto table = generateCodeTable();
				string result = "";
				for (auto i = 0; i < length; ++i)
					result += table[content[i]];
				return result;
			}
			LinkList<T> decode(const char *const &content, const int &length) {
				LinkList<T> result;
				Super *tmp;
				for (int i = 0; i < length;) {
					tmp = this;
					for (; i < length && !tmp->value.value; ++i) {
						if (content[i] == '0')
							tmp = tmp->leftChild.get();
						else if (content[i] == '1')
							tmp = tmp->rightChild.get();
						else {
							char message[1 << 6];
							sprintf_s(message, "Unexpected character %c detected at index %d", content[i], i);
							throw InvalidArgumentException(message);
						}
					}
					if (!tmp->value.value)
						throw InvalidArgumentException("Content ends incorrectly");
					result.append(tmp->value.value.value());
				}
				return move(result);
			}
			inline LinkList<T> decode(const string &content) {
				return decode(content.c_str(), content.length());
			}

			HuffmanTree(const Frequency<T> &value, const SuperSharedPointer &leftChild = nullptr,
				const SuperSharedPointer &rightChild = nullptr, const SuperSharedPointer &parent = nullptr) :
				Super(value, leftChild, rightChild, parent) {}
			inline string getCode(const T &value) const {
				return getCode(value, this, false).substr(1);
			}
			inline map<T, string> generateCodeTable() const {
				map<T, string> result;
				string auxString = "";
				generateCodeTable(this, result, auxString);
				return move(result);
			}
			inline map<string, T> generateReverseCodeTable() const {
				map<string, T> result;
				string auxString = "";
				generateReverseCodeTable(this, result, auxString);
				return move(result);
			}
		};
	}
}