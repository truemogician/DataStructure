#include "pch.h"
#include "CppUnitTest.h"
#include "..\BinaryTree\HuffmanTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HuffmanTreeTest {
	using namespace My::Tree;
	TEST_CLASS(HuffmanTreeTest) {
	private:
		char charset[7] = { 'A','B','C','D','E','F','G' };
		int count[7] = { 60,45,13,69,14,5,3 };
	public:
		TEST_METHOD(create) {
			LinkList<Frequency<char>> freqList;
			for (int i = 0; i < 7; ++i)
				freqList.append(Frequency<char>(charset[i], count[i]));
			auto huffmanTree = HuffmanTree<char>::createHuffmanTree(move(freqList));
			Assert::AreEqual<string>("10", huffmanTree->getCode('A'));
			string content = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDEEEEEEEEEEEEEEFFFFFGGG";
			auto huffmanTreeB = HuffmanTree<char>::createHuffmanTree(content.c_str(), content.length());
			Assert::AreEqual<string>("00100", huffmanTreeB->getCode('G'));
		}
		TEST_METHOD(generateTable) {
			LinkList<Frequency<char>> freqList;
			for (int i = 0; i < 7; ++i)
				freqList.append(Frequency<char>(charset[i], count[i]));
			auto huffmanTree = HuffmanTree<char>::createHuffmanTree(move(freqList));
			auto table = huffmanTree->generateCodeTable();
			Assert::AreEqual<string>("10", table['A']);
			Assert::AreEqual<string>("01", table['B']);
			Assert::AreEqual<string>("0011", table['C']);
			Assert::AreEqual<string>("11", table['D']);
			Assert::AreEqual<string>("000", table['E']);
			Assert::AreEqual<string>("00101", table['F']);
			Assert::AreEqual<string>("00100", table['G']);
			auto reverseTable = huffmanTree->generateReverseCodeTable();
			Assert::AreEqual<char>('A', reverseTable["10"]);
			Assert::AreEqual<char>('B', reverseTable["01"]);
			Assert::AreEqual<char>('C', reverseTable["0011"]);
			Assert::AreEqual<char>('D', reverseTable["11"]);
			Assert::AreEqual<char>('E', reverseTable["000"]);
			Assert::AreEqual<char>('F', reverseTable["00101"]);
			Assert::AreEqual<char>('G', reverseTable["00100"]);
		}
		TEST_METHOD(getAverageBitwidth) {
			LinkList<Frequency<char>> freqList;
			for (int i = 0; i < 7; ++i)
				freqList.append(Frequency<char>(charset[i], count[i]));
			auto huffmanTree = HuffmanTree<char>::createHuffmanTree(move(freqList));
			Assert::AreEqual<int>(23, (int)floor(huffmanTree->getAverageBitwidth() * 10));
		}
		TEST_METHOD(encodeAndDecode) {
			string content = "AAAACDDDDDDDAAAAABBBBABBBBBBBCAAAAAAABBBAAAAAAAAAAAAAAAAAAAAAAAAABBAAAAAAAAAAAABBBBBBBBBBBBBDDDDDDEBBBBBBBBBBCCCCCCAAAAAABBBBBBCCCCCDDDDDDDDDDDDDDEEEEDDDDDDDDDDDDDDDDDDDDDDDDDDDDDEEEEEEFDDDDDDDDDDDDDEEEFFFFGGG";
			auto huffmanTree = HuffmanTree<char>::createHuffmanTree(content.c_str(), content.length());
			auto encodeResult = huffmanTree->encode(content.c_str(), content.length());
			auto decodeResult = huffmanTree->decode(encodeResult);
			Assert::IsTrue(content == string(decodeResult.toArray(), decodeResult.getLength()));
		}
	};
}
