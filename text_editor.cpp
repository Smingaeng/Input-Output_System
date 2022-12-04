#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

#define LIMIT_LINE 20
#define LIMIT_BYTE 75
string order;
string order_1, order_2, order_3 = "";
string console = "";
int textSize = 0, textLine = 0;
string console_num = "숫자를 입력하세요";


//Model
class TextEditor { 
public:
	list<string> text;
	list<int> lineList;
	list<list<string>::iterator> pageIter;
	list<string>::iterator startIter;
	list<string>::iterator endIter;
	int line, word;

	TextEditor() {
		//Construct
		line = 0;
		word = 0;
	}
	int get_line() {
		return line;
	}
	void setSize() { //line and byte size update
		int setLine = 0;
		textSize = 0, textLine = 0;
		for (list<string>::iterator it = text.begin(); it != text.end(); ++it) {
			if (setLine + size(*it) > LIMIT_BYTE) {
				textLine++;
				setLine = 0;
			}
			setLine += size(*it);
			textSize += size(*it);
			if (setLine < LIMIT_BYTE) {
				setLine++;
				textSize++;
			}
		}
		word = setLine;
	}
	void setOrder(string order) {
		int temp = 2;
		int flag = 0;
		for (int i = 2; i < order.length(); i++) {
			if (order[i] == ',' || order[i] == ')') { //콤마 또는 괄호 단위로 끊기
				if (flag == 0) {
					order_1 = order.substr(temp, i - temp);
					temp = i + 1;
					flag++;
				}
				else if (flag == 1) {
					order_2 = order.substr(temp, i - temp);
					temp = i + 1;
					flag++;
				}
				else {
					order_3 = order.substr(temp, i - temp);
					break;
				}
			}
		}
	} //parameters Separation
	void showPage() {
		endIter = startIter;
		int printLine = 1, printWord = 0;
		printf("%.2d| ", printLine);
		while (printLine <= LIMIT_LINE && endIter != text.end()) {
			if (printWord + size(*endIter) > LIMIT_BYTE) {
				printWord = 0, line++; printLine++;
				if (printLine <= LIMIT_LINE) {
					printf("\n%.2d| ", printLine); //01 ~ 20 | 표현
				}
			}
			else {
				printWord += size(*endIter);
				cout << *endIter;
				if (printWord < LIMIT_BYTE) {
					printWord++;
					cout << " ";
				}
				endIter++;
			}
		}
		cout << endl;
	}
	void nextPageOperation() {
		pageIter.push_back(startIter);
		lineList.push_back(line - LIMIT_LINE);
		if (line + LIMIT_LINE <= textLine) {
			startIter = endIter;
		}
		else {
			int setWord = 0, setLine = 0;
			while (setLine <= textLine - line) {
				if (setWord + size(*startIter) > LIMIT_BYTE) {
					setWord = 0, setLine++;
				}
				else {
					setWord += size(*startIter);
					if (setWord < LIMIT_BYTE) {
						setWord++;
					}
					++startIter;
				}
			}
			line = textLine - LIMIT_LINE + 1;
		}
		showPage();
	}
	void previousPageOperation() {
		if (pageIter.size() != 0) {
			startIter = pageIter.back();
			line = lineList.back();
			pageIter.pop_back();
			lineList.pop_back();
		}
		else {
			startIter = text.begin();
			if (line <= 2 * LIMIT_LINE) {
				line = 0;
			}
			else {
				if (line + LIMIT_LINE > textLine) {
					line++;
				}
				line = line - 2 * LIMIT_LINE;
				int setWord = 0, setLine = 0;
				while (setLine < line) {
					if (setWord + size(*startIter) > LIMIT_BYTE) {
						setWord = 0, setLine++;
					}
					else {
						setWord += size(*startIter);
						if (setWord < LIMIT_BYTE) {
							setWord++;
						}
						++startIter;
					}
				}
			}
		}
		showPage();
	}
	void insertOperation(string order) {
		setOrder(order);
		for (int i = 0; i < order_1.length(); i++) {
			if (order_1[i] < '0' || order_1[i] > '9') {
				console = console_num;
				return;
			}
		}

		for (int i = 0; i < order_2.length(); i++) {
			if (order_2[i] < '0' || order_2[i] > '9') {
				console = console_num;
				return;
			}
		}

		int line = stoi(order_1), byte = stoi(order_2); //stringToInt
		if (line > LIMIT_LINE) {
			console = order_1 + "번째 line은 존재하지 않습니다. 재입력해주세요.";
			return;
		}
		if (byte > LIMIT_BYTE) {
			console = "index " + order_2 + "은 존재하지 않습니다. 재입력해주세요.";
			return;
		}
		string insertString = order_3;

		int insertLine = 0, insertWord = 0;
		endIter = startIter;
		while (insertLine < line - 1) {
			if (insertWord + size(*endIter) > LIMIT_BYTE) {
				insertWord = 0, insertLine++;
			}
			else {
				insertWord += size(*endIter);
				if (insertWord < LIMIT_BYTE && insertLine != line - 1) {
					insertWord++;
				}
				endIter++;
			}
		}

		while (insertWord + size(*endIter) < byte) {
			insertWord += size(*endIter) + 1;
			endIter++;
		}

		if (insertWord + size(*endIter) > LIMIT_BYTE) {
			text.insert(endIter, insertString);
		}

		else {
			string newString = *endIter;
			newString.insert(byte - insertWord, insertString);
			text.insert(endIter, newString);
			if (endIter == startIter) {
				startIter--;
			}
			text.erase(endIter);
		}

		setSize();
		int tempLine = line;
		showPage();
		line = tempLine;
	}
	void changeOperation(string order) {
		setOrder(order);
		string before = order_1;
		string after = order_2;
		startIter = text.begin();
		while (startIter != text.end()) {
			if (!(*startIter).compare(before)) {
				text.erase(startIter++);
				text.insert(startIter, after);
			}
			else {
				startIter++;
			}
		}
		setSize();
		startIter = text.begin();
		line = 0;
		showPage();
	}
	void searchOperation(string order) {
		setOrder(order);
		string findString = order_1;
		pageIter.clear();
		lineList.clear();
		startIter = text.begin();
		line = 1;
		bool searchOk = false;
		int searchWord = 1;
		while (startIter != text.end()) {
			if (searchWord + size(*startIter) > LIMIT_BYTE) {
				searchWord = 0, line++;
			}
			else {
				if (!(*startIter).compare(findString)) {
					if (searchWord + word >= LIMIT_BYTE) {
						line--;
					}
					searchOk = true;
					break;
				}
				searchWord += size(*startIter);
				if (searchWord < LIMIT_BYTE) {
					searchWord++;
				}
				++startIter;
			}
		}
		if (!searchOk) {
			console = "해당 단어는 존재하지 않습니다.";
			startIter = text.begin();
			line = 0;
		}
		showPage();
	}
	void terminateOperation() {
		ofstream saveFile;
		saveFile.open("test.txt");
		if (saveFile.fail())
		{
			cout << "save file opening failed" << endl;
			exit(1);
		}
		if (!saveFile.fail()) {
			startIter = text.begin();
			for (; startIter != text.end(); ++startIter) {
				saveFile << *startIter << " ";
			}
			saveFile.close();
		}
	}
};

//View
class View { 
public:
	void printMenu() {
		cout << "n: 다음페이지, p: 이전페이지, i: 삽입, d: 삭제, c: 변경, s: 찾기, t: 저장후 종료" << endl;
	}

	void printDivideSpace() {
		cout << "---------------------------------------------------------------------------------" << endl;
	}

	void printUserInput() {
		cout << "입력: ";
	}

	void printConsole(string console) {
		cout << "(콘솔메시지) " << console << endl;
	}
};

//Controller
class Controller { 
private: 
	TextEditor textEditor;
	View view;

public:
	Controller(TextEditor _textEditor, View _view) {
		textEditor = _textEditor;
		view = _view;
	}
	void initialization() {
		console = "";
		order_1 = "";
		order_2 = "";
		order_3 = "";
	}
	void run() {
		ifstream readFile;
		list<string> textList;

		readFile.open("test.txt");

		if (readFile.fail())
		{
			cout << "input file opening failed" << endl;
			exit(1);
		}

		string word;
		if (!readFile.fail()) {
			while (readFile >> word) {
				textList.push_back(word);
			}

			readFile.close();
			TextEditor textEditor;
			textEditor.setSize();
			textEditor.text = textList;
			textEditor.startIter = textEditor.text.begin();
			textEditor.endIter = textEditor.text.begin();
			textEditor.setSize();
			textEditor.showPage();

			do {
				view.printDivideSpace();
				view.printMenu();
				view.printDivideSpace();
				view.printConsole(console);
				initialization();
				view.printDivideSpace();
				view.printUserInput();
				getline(cin, order);
				switch (order[0]) {
				case 'n':
					if (textEditor.get_line() >= textLine) {
						console = "This is the last page.";
					}
					else {
						view.printDivideSpace();
						textEditor.nextPageOperation();
					}
					break;
				case 'p':
					if (textEditor.get_line() <= LIMIT_LINE) {
						console = "This is the first page.";
					}
					else {
						view.printDivideSpace();
						textEditor.previousPageOperation();
					}
					break;
				case 'i':
					view.printDivideSpace();
					textEditor.insertOperation(order);
					break;
				case 'c':
					view.printDivideSpace();
					textEditor.changeOperation(order);
					break;
				case 's':
					view.printDivideSpace();
					textEditor.searchOperation(order);
					break;
				case 't':
					view.printDivideSpace();
					textEditor.terminateOperation();
					return;
				default:
					console = "존재하지 않는 명령어입니다. 다시 입력해 주십시오.";
				}

			}
			while (!cin.eof());
		}
		return;
	}

};


int main() {
	TextEditor textEditor;
	View view;
	Controller controller(textEditor, view);
	controller.run();
}