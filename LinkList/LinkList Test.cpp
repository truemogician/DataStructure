#include <Windows.h>
#include "LinkList.h"
#include "..\\IntegratedStream.h"
#define arrayLength(arr) (sizeof(arr)/sizeof(arr[0]))
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define YELLOW (FOREGROUND_RED|FOREGROUND_GREEN)
#define CYAN (FOREGROUND_GREEN|FOREGROUND_BLUE)
#define MAGENTA (FOREGROUND_RED|FOREGROUND_BLUE)
#define WHITE (FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
using namespace My;
using std::string;
using std::map;
using std::endl;
using std::istream;
using std::ifstream;
using std::to_string;
#define toString to_string
const bool fileMode = true;
const string commands = ".\\commands.txt";
enum class Setting {
	autoPrint, SIZE
};
const string settingName[(int)Setting::SIZE] = { "autoPrint" };
bool setting[(int)Setting::SIZE] = { false };
enum class Command {
	help, quit, set, clear, create, copy, drop, rename, activate, list, initialize, print, length, prepend, 
	append, insert, removeAt, removeOne, removeAll, query, sort, reverse, unify, concat, split, SIZE
};
const string commandName[(int)Command::SIZE] = {
	"help","quit","set","clear","create","copy","drop","rename","activate","list","initialize","print","length","prepend",
	"append","insert","removeAt","removeOne","removeAll","query","sort","reverse","unify","concat","split" 
};
const string instructions = ".\\help.txt";
bool setColor(WORD color = WHITE) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute(handle, color);
}
void error(string message) {
	setColor(RED);
	cout << "[Error]" << message << endl;
}
void warning(string message) {
	setColor(YELLOW);
	cout << "[Warn]" << message << endl;
}
void information(string message) {
	setColor(GREEN);
	cout << "[Info]" << message << endl;
}
int main() {
	map<string, SingleLinkList<int>> lnks;
	SingleLinkList<int> *curLnk = nullptr;
	InputStream in;
	if (fileMode == true) {
		in.add(commands);
		in.selectFile(commands);
	}
	information("Use command \"help\" if not familiar with the commands");
	warning("Make sure you type the correct parameters for commands. Program may crack if unexpected parameter received");
	static const int BUF_SIZE = 1 << 10;
	char *buffer = new char[BUF_SIZE];
	while (true) {
		if (!in.isStandard() && in.currentFile()->eof())
			in.selectStandard();
		setColor();
		string commandString;
		int commandId = -1;
		in >> commandString;
		for (int i = 0; i < (int)Command::SIZE; i++)
			if (commandString == commandName[i]) {
				commandId = i;
				break;
			}
		if (commandId == -1) {
			error("Unrecognized command");
			continue;
		}
		Command command = (Command)commandId;
		int value;
		if (command == Command::quit)
			break;
		else if (command == Command::help) {
			static const int BUF_SIZE = 1 << 10;
			char *buffer = new char[BUF_SIZE];
			std::ifstream instr(instructions);
			while (!instr.fail() && !instr.eof()) {
				instr.getline(buffer, BUF_SIZE);
				int length = strlen(buffer);
				setColor(RED);
				for (int i = 0; i < length; i++) {
					if (buffer[i] == ':') {
						setColor(WHITE);
						putchar(buffer[i]);
						setColor(GREEN);
					}
					else if (buffer[i] == '[') {
						setColor(YELLOW);
						putchar(buffer[i]);
					}
					else if (buffer[i] == ']') {
						putchar(buffer[i]);
						setColor(GREEN);
					}
					else
						putchar(buffer[i]);
				}
				putchar('\n');
			}
		}
		else if (command == Command::set or command == Command::clear) {
			bool setValue = (command == Command::set);
			in >> commandString;
			int settingId = -1;
			for (int i = 0; i < (int)Setting::SIZE; i++)
				if (commandString == settingName[i]) {
					settingId = i;
					break;
				}
			if (settingId == -1) {
				error("Unrecognized setting");
				continue;
			}
			setting[settingId] = setValue;
			information("Settings updated");
		}
		else if (command == Command::create) {
			in >> commandString;
			if (lnks.find(commandString) != lnks.cend()) {
				error("Link list already exists");
				continue;
			}
			lnks.insert(makePair(commandString, SingleLinkList<int>()));
			if (!curLnk) {
				curLnk = &lnks[commandString];
				information(commandString + " created and activated");
			}
			else
				information(commandString + " created");
		}
		else if (command == Command::copy) {
			string srcLnk, dstLnk;
			in >> dstLnk >> srcLnk;
			if (lnks.find(srcLnk) == lnks.end()) {
				error("Source link list doesn't exist");
				continue;
			}
			if (lnks.find(dstLnk) == lnks.end())
				information("Destination link list created");
			lnks[dstLnk] = lnks[srcLnk];
			information(srcLnk + " copied to " + dstLnk);
		}
		else if (command == Command::drop) {
			in >> commandString;
			if (lnks.find(commandString) == lnks.cend()) {
				error("Link list doesn't exist");
				continue;
			}
			information(commandString + " dropped");
			if (curLnk == &lnks[commandString]) {
				if (lnks.size() == 1) {
					curLnk = nullptr;
					warning("No available link list left");
				}
				else {
					lnks.erase(commandString);
					curLnk = &(lnks.begin()->second);
					information(lnks.cbegin()->first + " is now activated");
					continue;
				}
			}
			lnks.erase(commandString);
		}
		else if (command == Command::rename) {
			string newName;
			in >> commandString >> newName;
			if (lnks.find(commandString) == lnks.cend()) {
				error("Link list doesn't exist");
				continue;
			}
			if (lnks.find(newName) != lnks.cend()) {
				error("New link list already exists");
				continue;
			}
			auto &dst = lnks[commandString];
			lnks.insert(makePair(newName, std::move(dst)));
			if (curLnk == &dst)
				curLnk = &lnks[newName];
			lnks.erase(commandString);
			information(commandString + " renamed to " + newName);
		}
		else if (command == Command::activate) {
			in >> commandString;
			if (lnks.find(commandString) == lnks.cend()) {
				error("Link list doesn't exist");
				continue;
			}
			curLnk = &lnks[commandString];
			information(commandString + " activated");
		}
		else if (command == Command::list) {
			if (lnks.empty()) {
				error("No available link list");
				continue;
			}
			for (auto i = lnks.begin(); i != lnks.end(); i++) {
				if (&(i->second) == curLnk)
					setColor(YELLOW);
				else
					setColor(GREEN);
				cout << i->first << endl;
			}
		}
		else if (command == Command::initialize) {
			if (!curLnk) {
				error("No link list created");
				char ch;
				while ((ch = getchar()) != '\n' && ch != '\r');
				continue;
			}
			int n;
			setColor(GREEN);
			cout << "length : ";
			setColor();
			in >> n;
			setColor(GREEN);
			cout << "data   : ";
			setColor();
			for (int i = 0; i < n; i++) {
				in >> value;
				curLnk->append(value);
			}
			information("Initialized");
			continue;
		}
		else {
			if (!curLnk || !(*curLnk)) {
				setColor(RED);
				cout << (curLnk ? "Link list not intialized" : "No link list created") << endl;
				char ch;
				while ((ch = getchar()) != '\n' && ch != '\r');
				continue;
			}
			else if (command == Command::print) {
				setColor(GREEN);
				cout << *curLnk << endl;
				continue;
			}
			else if (command == Command::length) {
				setColor(GREEN);
				cout << curLnk->length() << endl;
				continue;
			}
			else if (command == Command::prepend) {
				in >> value;
				curLnk->prepend(value);
			}
			else if (command == Command::append) {
				in >> value;
				curLnk->append(value);
			}
			else if (command == Command::insert) {
				int pos;
				in >> pos >> value;
				if (!curLnk->insert(pos, value))
					error("Out of range");
			}
			else if (command == Command::removeAt) {
				in >> value;
				if (!curLnk->removeAt(value))
					error("Out of range");
			}
			else if (command == Command::removeOne) {
				in >> value;
				int result = curLnk->removeOne(value);
				(result != -1) ? information("Removed at " + toString(result)) : error("Not found");
			}
			else if (command == Command::removeAll) {
				in >> value;
				int result = curLnk->removeAll(value);
				(result) ? information(toString(result) + " element(s) removed") : error("Not found");
			}
			else if (command == Command::query) {
				in >> value;
				auto result = curLnk->query(value);
				setColor(GREEN);
				cout << ((result.first != -1) ? "Found at " + toString(result.first) : "Not found") << endl;
				continue;
			}
			else if (command == Command::sort)
				curLnk->sort();
			else if (command == Command::reverse)
				curLnk->reverse();
			else if (command == Command::unify)
				curLnk->unify();
			else if (command == Command::concat) {
				in >> commandString;
				if (lnks.find(commandString) == lnks.end()) {
					error("Link list doesn't exist");
					continue;
				}
				auto &dst = lnks[commandString];
				if (dst)
					curLnk->concat(dst);
			}
			else if (command == Command::split) {
				int pos;
				in >> pos >> commandString;
				if (lnks.find(commandString) == lnks.end()) 
					information("Destination link list created");
				auto result = curLnk->split(pos);
				if (!result) {
					error("Out of range");
					continue;
				}
				lnks.insert_or_assign(commandString, std::move(*result));
				information("New link list moved to " + commandString);
			}
			if (setting[(int)Setting::autoPrint]) {
				setColor(CYAN);
				cout << "length : " << curLnk->length() << endl << "data   : " << *curLnk << endl;
			}
		}
	}
}