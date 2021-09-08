#include "HuffmanTree.h"
using namespace My::Tree;
using std::string;
int main() {
	string content = "AAAACDDDDDDDAAAAABBBBABBBBBBBCAAAAAAABBBAAAAAAAAAAAAAAAAAAAAAAAAABBAAAAAAAAAAAABBBBBBBBBBBBBDDDDDDEBBBBBBBBBBCCCCCCAAAAAABBBBBBCCCCCDDDDDDDDDDDDDDEEEEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDEEEEEEFDDDDDDDDDDDDDEEEFFFFGGG";
	auto huffmanTree = HuffmanTree<char>::createHuffmanTree(content.c_str(), (int)content.length());
	auto table = huffmanTree->generateCodeTable();
	for (auto i = table.begin(); i != table.end(); ++i)
		std::cout << i->first << ' ' << i->second << std::endl;
	return 0;
}