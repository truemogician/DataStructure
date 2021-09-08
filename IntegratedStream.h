#pragma once
#include <sys/stat.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
namespace My {
	using std::string;
	using std::map;
	using std::cin;
	using std::cout;
	using std::ifstream;
	using std::ofstream;
	inline bool fileExists(const string &name) {
		struct stat buffer;
		return stat(name.c_str(), &buffer) == 0;
	}
	class OutputStream {
	private:
		ofstream *curStream;
		map<string, ofstream> fileStreams;
	public:
		bool echo = true;
		OutputStream() :curStream(nullptr) {}
		bool add(string filePath) {
			if (!fileExists(filePath))
				return false;
			fileStreams.insert(std::make_pair(filePath, ofstream(filePath)));
			return true;
		}
		ofstream *currentFile() {
			return curStream;
		}
		bool isStandard() const {
			return curStream == nullptr;
		}
		void selectStandard() {
			curStream = nullptr;
		}
		bool selectFile(string filePath) {
			if (fileStreams.find(filePath) == fileStreams.end())
				return false;
			curStream = &fileStreams[filePath];
			return true;
		}
		bool reopen(string filePath) {
			if (fileStreams.find(filePath) == fileStreams.end())
				return false;
			fileStreams[filePath].open(filePath);
			return true;
		}
		OutputStream &put(char ch) {
			curStream ? curStream->put(ch) : cout.put(ch);
			return *this;
		}
		template <class T>
		OutputStream &operator << (const T &value) {
			curStream ? *curStream << value : cout << value;
			return *this;
		}
		using BasicOstream = std::basic_ostream<std::ostream::char_type, std::ostream::traits_type>;
		OutputStream &operator << (BasicOstream &(*func)(BasicOstream &)) {
			curStream ? *curStream << func : cout << func;
			return *this;
		}
	};
	class InputStream {
	private:
		ifstream *curStream;
		map<string, ifstream> fileStreams;
		string getFileName(string path) {
			int start, end = 0;
			for (start = (int)path.length() - 1; start >= 0; start--) {
				if (!end && path[start] == '.')
					end = start;
				if (path[start] == '\\')
					break;
			}
			return path.substr(start, (size_t)end - start);
		}
	public:
		bool echo = true;
		OutputStream echoOut;
		InputStream() :curStream(nullptr) {}
		bool add(string filePath) {
			if (!fileExists(filePath))
				return false;
			fileStreams.insert(std::make_pair(filePath, ifstream(filePath)));
			return true;
		}
		ifstream *currentFile() {
			return curStream;
		}
		bool isStandard() const {
			return curStream == nullptr;
		}
		void selectStandard() {
			curStream = nullptr;
		}
		bool selectFile(string filePath) {
			if (fileStreams.find(filePath) == fileStreams.end())
				return false;
			curStream = &fileStreams[filePath];
			return true;
		}
		bool reopen(string filePath) {
			if (fileStreams.find(filePath) == fileStreams.end())
				return false;
			fileStreams[filePath].open(filePath);
		}
		InputStream &getLine(char *str, std::streamsize count) {
			curStream ? curStream->getline(str, count) : cin.getline(str, count);
			return *this;
		}
		int get() {
			return curStream ? curStream->get() : cin.get();
		}
		template <class T>
		InputStream &operator >> (T &value) {
			if (!curStream)
				cin >> value;
			else {
				if (echo) {
					auto start = curStream->tellg();
					*curStream >> value;
					if (curStream->peek() == '\n') {
						curStream->get();
						if (curStream->peek() == '\r')
							curStream->get();
					}
					else if (curStream->peek() == '\r') {
						curStream->get();
						if (curStream->peek() == '\n')
							curStream->get();
					}
					auto end = curStream->tellg();
					curStream->clear();
					curStream->seekg(start);
					while (curStream->tellg() != end) {
						char ch = curStream->get();
						if (ch == -1)
							break;
						echoOut.put(ch);
					}
				}
				else
					*curStream >> value;
			}
			return *this;
		}
	};
}