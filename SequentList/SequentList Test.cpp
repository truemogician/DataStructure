#include <fstream>
#include <string>
#include <map>
#include <Windows.h>
#include "SequentList.h"
#define arrayLength(arr) (sizeof(arr)/sizeof(arr[0]))
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define YELLOW (FOREGROUND_RED|FOREGROUND_GREEN)
#define CYAN (FOREGROUND_GREEN|FOREGROUND_BLUE)
#define MAGENTA (FOREGROUND_RED|FOREGROUND_BLUE)
#define WHITE (FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE)
using std::string;
using std::map;
using std::endl;
enum class Setting {
	autoPrint,autoCheck,SIZE
};
const string settingName[(int)Setting::SIZE] = { "autoPrint","autoCheck" };
bool setting[(int)Setting::SIZE] = { false };
enum class Command {
	help, set, clear, create, copy, drop, rename, activate, list, initialize, print, resize, length, fill, add,
	insert, orderedInsert, removeAt, removeOne, removeAll, query, sort, reverse, unify, concat, merge,SIZE
};
const string commandName[(int)Command::SIZE] = { "help","set","clear","create","copy","drop","rename","activate","list","initialize","print","resize","length",
"fill","add","insert","orderedInsert","removeAt","removeOne","removeAll","query","sort","reverse","unify","concat","merge" };
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
	map<string, SequentList<int>> seqs;
	SequentList<int> *curSeq = nullptr;
	information("Use command \"help\" if not familiar with the commands");
	warning("Make sure you type the correct parameters for commands. Program may crack if unexpected parameter received");
	while (true) {
		setColor();
		string commandString;
		int commandId = -1;
		cin >> commandString;
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
		if (command == Command::help) {
			std::ifstream instr(instructions);
			static const int BUF_SIZE = 1 << 10;
			char *buffer = new char[BUF_SIZE];
			while (!instr.fail() && !instr.eof()){
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
			cin >> commandString;
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
			cin >> commandString;
			if (seqs.find(commandString) != seqs.cend()) {
				error("Sequent list already exists");
				continue;
			}
			seqs[commandString] = SequentList<int>();
			if (!curSeq) {
				curSeq = &seqs[commandString];
				information(commandString + " created and activated");
			}
			else
				information(commandString + " created");
		}
		else if (command == Command::copy) {
			string srcSeq, dstSeq;
			cin >> dstSeq >> srcSeq;
			if (seqs.find(srcSeq) == seqs.end()) {
				error("Source sequent list doesn't exist");
				continue;
			}
			if (seqs.find(dstSeq) == seqs.end()) 
				information("Destination sequent list created");
			seqs[dstSeq] = seqs[srcSeq];
			information("Copied");
		}
		else if (command == Command::drop) {
			cin >> commandString;
			if (seqs.find(commandString) == seqs.cend()) {
				error("Sequent list doesn't exist");
				continue;
			}
			if (curSeq == &seqs[commandString]) {
				information("Active sequent list dropped");
				if (seqs.size() == 1) {
					curSeq = nullptr;
					warning("No available sequent list left");
				}
				else {
					seqs.erase(commandString);
					curSeq = &(seqs.begin()->second);
					information(seqs.cbegin()->first + " is now activated");
					continue;
				}
			}
			seqs.erase(commandString);
		}
		else if (command == Command::rename) {
			string newName;
			cin >> commandString >> newName;
			if (seqs.find(commandString) == seqs.cend()) {
				error("Sequent list doesn't exist");
				continue;
			}
			if (seqs.find(newName) != seqs.cend()) {
				error("New sequent list already exists");
				continue;
			}
			auto oldSeq = seqs[commandString];
			bool activated = (curSeq == &oldSeq);
			SequentList<int> newSeq(std::move(oldSeq));
			seqs.erase(commandString);
			seqs[newName] = newSeq;
			if (activated)
				curSeq = &newSeq;
			information(commandString + " renamed to " + newName);
		}
		else if (command == Command::activate) {
			cin >> commandString;
			if (seqs.find(commandString) == seqs.cend()) {
				error("Sequent list doesn't exist");
				continue;
			}
			curSeq = &seqs[commandString];
			information(commandString + " activated");
		}
		else if (command == Command::list) {
			if (seqs.empty()) {
				error("No available sequent list");
				continue;
			}
			for (auto i = seqs.begin(); i != seqs.end(); i++) {
				if (&(i->second) == curSeq)
					setColor(YELLOW);
				else
					setColor(GREEN);
				cout << i->first << endl;
			}
		}
		else if (command == Command::initialize) {
			if (!curSeq) {
				error("No sequent list created");
				char ch;
				while ((ch = getchar()) != '\n' && ch != '\r');
				continue;
			}
			int n;
			setColor(GREEN);
			cout << "length : ";
			setColor();
			cin >> n;
			*curSeq = SequentList<int>(n);
			setColor(GREEN);
			cout << "data   : ";
			setColor();
			for (int i = 0; i < n; i++) {
				cin >> value;
				curSeq->add(value);
			}
			information("Initialized");
			continue;
		}
		else {
			if (!curSeq || !(*curSeq)) {
				setColor(RED);
				cout << (curSeq ? "Sequent list not intialized" : "No sequent list created") << endl;
				char ch;
				while ((ch = getchar()) != '\n' && ch != '\r');
				continue;
			}
			else if (command == Command::print) {
				setColor(GREEN);
				cout << *curSeq << endl;
				continue;
			}
			else if (command == Command::length) {
				setColor(GREEN);
				cout << curSeq->length() << endl;
				continue;
			}
			else if (command == Command::resize) {
				cin >> value;
				curSeq->resize(value);
			}
			else if (command == Command::fill) {
				cin >> value;
				curSeq->fill(value);
			}
			else if (command == Command::add) {
				cin >> value;
				curSeq->add(value);
			}
			else if (command == Command::insert) {
				int pos;
				cin >> pos >> value;
				if (!curSeq->insert(pos, value)) 
					error("Out of range");
			}
			else if (command == Command::orderedInsert) {
				cin >> value;
				if (setting[(int)Setting::autoCheck]) {
					if (!curSeq->isOrdered()) {
						error("Sequent list not in increasing order, please sort first");
						continue;
					}
				}
				information("Inserted at " + std::to_string(curSeq->orderedInsert(value)));
			}
			else if (command == Command::removeAt) {
				cin >> value;
				if (!curSeq->removeAt(value)) 
					error("Out of range");
			}
			else if (command == Command::removeOne) {
				cin >> value;
				int result = curSeq->removeOne(value);
				(result != -1)?information("Removed at " + result):error("Not found");
			}
			else if (command == Command::removeAll) {
				cin >> value;
				int result = curSeq->removeAll(value);
				(result)?information(std::to_string(result) + " element(s) removed"):error("Not found");
			}
			else if (command == Command::query) {
				cin >> value;
				int result = curSeq->query(value);
				setColor(GREEN);
				cout << ((result != -1) ? "Found at " + std::to_string(result) : "Not found") << endl;
				continue;
			}
			else if (command == Command::sort)
				curSeq->sort();
			else if (command == Command::reverse)
				curSeq->reverse();
			else if (command == Command::unify)
				curSeq->unify();
			else if (command == Command::concat) {
				cin >> commandString;
				if (seqs.find(commandString) == seqs.cend()) {
					error("Sequent list doesn't exist");
					continue;
				}
				curSeq->concat(seqs[commandString]);
			}
			else if (command == Command::merge) {
				cin >> commandString;
				if (seqs.find(commandString) == seqs.cend()) {
					error("Sequent list doesn't exist");
					continue;
				}
				if (setting[(int)Setting::autoCheck]) {
					if (!curSeq->isOrdered() || !seqs[commandString].isOrdered()) {
						error("Sequent list not in increasing order, please sort first");
						continue;
					}
				}
				curSeq->merge(seqs[commandString]);

			}
			if (setting[(int)Setting::autoPrint]) {
				setColor(CYAN);
				cout << "length : " << curSeq->length() << endl << "data   : " << *curSeq << endl;
			}
		}
	}
}