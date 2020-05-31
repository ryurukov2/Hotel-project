#include "Room.h"
static string lines[100];
static 	Helper h2;
/**@file
@brief
Main and only file
*/


void openF(string inputStr)
/**the open/edit(insert line, edit line, delete line), close, save, save as commands work through this function. */
{
	inputStr = h2.removeFirstWord(inputStr);
	cout << endl << inputStr << endl;
	string fileName = h2.firstWord(inputStr);
	fstream myS;
	string line;
	int i = 0;
	myS.open(fileName);
	while (!myS.eof()) {
		getline(myS, line);
		lines[i] = line;
		cout << i + 1 << ". " << lines[i] << endl;
		i++;
	}
	myS.close();

	string inp;
	int addcounter = 0;
	cout << "1. remove a line\n 2. change a line\n 3. insert a line\n Close = exit open function.\n";
	do {


		getline(cin, inp);
		int ins = h2.strToInt(inp);
		if (ins == 1) {
			int nrLine;
			cout << "Enter the number of the line you want to remove.\n";
			cin >> nrLine;
			lines[nrLine - 1] = "";
		}
		else if (ins == 2) {
			int nrLine;
			cout << "Enter the number of the line you want to change.\n";
			cin >> nrLine;
			string change;
			cout << "Type the new line.\n";
			cin.ignore();
			getline(cin, change);
			cout << change << endl;
			lines[nrLine - 1] = change;
		}
		else if (ins == 3) {
			addcounter++;
			cout << "Enter the number of the line you want to insert after.\n";
			int nrLine;
			cin >> nrLine;
			if (nrLine > i + 1) {
				return;
			}
			string addline;
			cout << "Enter your line: ";
			cin.ignore();
			getline(cin, addline);
			for (int j = 0; j < i - nrLine + 1; j++) {
				lines[i - j + 1] = lines[i - j];
			}
			lines[nrLine] = addline;
		}
		else if (h2.compareStr(h2.firstWord(inp), "close") == 0) {
			for (int o = 0; o < 100; o++) {
				lines[o] = "";
				system("CLS");
				cout << "\nUse open again to access files.\n";
				return;
			}
		}
		else if (h2.compareStr(h2.firstWord(inp), "save") == 0) {
			myS.open("temp.txt", ios::out | ios::app);
			for (int k = 0; k < i + addcounter; k++) {
				int l = 0;
				if (lines[k] == "") {
					l++;
					continue;
				}
				lines[k - l] = lines[k];
				myS << lines[k] << endl;
			}
			myS.close();
			if (remove(fileName.c_str()) != 0) {
				cout << "Can't remove file.\n";
			}
			if (rename("temp.txt", fileName.c_str()) != 0) {
				cout << "Can't rename file.\n";

			}
		}
		else if (h2.compareStr(h2.firstWord(inp), "save as") == 0) {
			cout << "\n save as \n";
			string inp1 = h2.removeFirstWord(h2.removeFirstWord(inp));
			myS.open(inp1, ios::out | ios::app);
			for (int k = 0; k < i + addcounter; k++) {
				int l = 0;
				if (lines[k] == "") {
					l++;
					continue;
				}
				lines[k - l] = lines[k];
				myS << lines[k] << endl;
			}
			myS.close();
		}
		else if (h2.compareStr(h2.firstWord(inp), "help") == 0) {
			system("CLS");
			cout << "\n open <file>	opens <file> \nclose			closes currently opened file \nsave			saves the currently open file\nsave as <file>	saves the currently open file in <file> \nhelp			prints this information";
		}
		else if (h2.compareStr(h2.firstWord(inp), "exit") == 0) {
			exit(0);
		}
	} while (true);

}


int main() {
	string input;
	Room room;


	//The commented part is for adding new rooms. Only needed in the first start.
	/*int n, i=0;
	cin >> n;
	room.addRooms(n);

	ifstream strm;
	string f;
	strm.open("rooms.txt");
	while (!strm.eof()) {
		getline(strm, f);
	
		if (i == 6) {
			cout << "Beds:" <<  h2.getBeds(f);
		}
		i++;
	}
	strm.close();*/
	do {
		cout << "Checkin, checkout, availability, report, find, find!, unavailable." << endl;
		getline(cin, input);
		string firstW = h2.firstWord(input);
		if (h2.compareStr(firstW, "checkin") == 0) {
			room.checkin(input);

		}
		if (h2.compareStr(firstW, "checkout") == 0) {
			room.checkout(input);

		}
		if (h2.compareStr(firstW, "availability") == 0) {
			room.availability(input);

		}
		if (h2.compareStr(firstW, "report") == 0) {
			room.report(input);

		}
		if (h2.compareStr(firstW, "find") == 0) {
			room.find(input);

		}
		if (h2.compareStr(firstW, "unavailable") == 0) {
			room.unavailable(input);

		}
		if (h2.compareStr(firstW, "open") == 0) {
			openF(input);

		}

	} while (input != "exit");

	return 0;
}