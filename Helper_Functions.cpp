#include<iostream>
#include<string>
#include<algorithm>
#include<sstream>
#include<stdio.h>
#include<fstream>

using namespace std;
/**@file
@brief
Helper Functions file.
*/

class Helper {
public:
	string removeFirstAst(string str)
		/**finds the first "*" in the string and deletes the substring before it. I am using "*" as a delimiter between the different types of information in tine file*/
	{
		return str.substr(str.find_first_of("*") + 1);
	}
	string removeFirstWord(string str)
		/** finds the first whitespace and deletes the ssubstring before it.*/
	{
		return str.substr(str.find_first_of(" \t") + 1);
	}



	bool is_number(string str)
		/**checks if a string is a number or not*/
	{
		//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
		string::const_iterator it = str.begin();
		while (it != str.end() && isdigit(*it)) {
			it++;
		}
		return !str.empty() && it == str.end();
	}

	bool iss_empty(ifstream& pFile)
		/**checks if a file is empty*/
	{
		//https://stackoverflow.com/questions/2390912/checking-for-an-empty-file-in-c
		return pFile.peek() == fstream::traits_type::eof();
	}

	bool compareStr(string str, const char* ch)
		/**custom strcmp because I had problems with the default*/
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		const char* compArr = str.c_str();
		return strcmp(ch, compArr);
	}

	int strToInt(string str) {
		if (str == "") {
			return NULL;
		}
		int result = 0;
		for (auto x : str) {
			if (x == '*') {
				break;
			}
			else if (x >= '0' && x <= '9') {
				result = 10 * result + (x - '0');
			}
			else {
				return NULL;
			}
		}
		return result;
	}

	string firstWord(string str)
		/**returns the first word of a string*/
	{
		string saveAs = "save as";
		string word = "", word2 = "";
		for (auto x : str)
		{
			if (x == ' ' || x == '*')
			{
				if (compareStr(word, "save") == 0) {
					string secondW = removeFirstWord(str);
					for (char y : secondW)
					{
						if (y == ' ')
						{
							if (compareStr(word2, "as") == 0) {
								return saveAs;
							}
						}
						else
						{
							word2 = word2 + y;

						}
					}
				}
				if (compareStr(word2, "as") == 0) {
					return saveAs;
				}
				else {
					return word;

				}
			}
			else
			{
				word = word + x;
			}
		}
		return word;

	}

	int getId(string str)
		/**gets the id of the room in the line of the file*/
	{
		return strToInt(firstWord(str));
	}
	int getBeds(string str)
		/**returns how many beds there are in a room, takes the line read from the file*/
	{
		string beds;
		int astCount = 0;
		for (auto x : str) {
			if (x == '*') {
				astCount++;
				if (astCount == 1) {
					continue;
				}
			}
			if (astCount == 0 || x == ' ') {

				continue;
			}

			if (astCount == 1 && x != '*') {
				beds = beds + x;
			}
			if (astCount == 1 && x == '*') {
				return strToInt(beds);
			}

		}
		return strToInt(beds);
	}



	void parseDate(string str, string& y, string& m, string& d)
		/**gets a date in the yyyy-mm-dd format and stores it as 3 different variables to make verification of dates and comparison and counting days possible*/
	{
		int dashCount = 0;

		for (auto x : str) {

			if (dashCount == 0) {
				if (x == '-') {
					int n = y.length();
					if (n == 4) {
						while (isdigit(y[y.length() - n])) {
							n--;
						}
						if (n == 0) {
							dashCount++;
							continue;
						}
						else {
							cout << "1Incorrect year format." << endl;
							return;
						}
					}
					else {
						cout << "6Incorrect year format." << endl;
						return;
					}

				}
				else {
					y = y + x;

				}

			}
			if (dashCount == 1) {

				if (x == '-') {
					int n = m.length();
					if (n == 2) {
						while (isdigit(m[m.length() - n])) {
							n--;
						}
						if (n == 0) {
							dashCount++;
							continue;
						}

						else {
							cout << "2Incorrect month format." << endl;
							return;
						}
					}
					else {
						cout << "3Incorrect month format." << endl;
						return;
					}
				}
				else {
					m = m + x;
				}
			}
			if (dashCount == 2) {

				if (x == ' ') {
					int n = d.length();
					if (n == 2) {
						while (isdigit(d[d.length() - n])) {
							n--;
						}
						if (n == 0) {
							dashCount++;
							continue;
						}

						else {
							cout << "4Incorrect month format." << endl;
							return;
						}
					}
					else {
						cout << "5Incorrect month format." << endl;
						return;
					}

				}
				else {
					d = d + x;
				}
			}

		}

	}



	bool compareDates(int y1, int m1, int d1, int y2, int m2, int d2)
		/**takes the 3 components parseDate "returns" for two dates and returns true if the second one is after the first one.*/
	{
		if (y1 < y2) {
			return true;
		}
		else if (y1 == y2) {
			if (m1 < m2) {
				return true;
			}
			else if (m1 == m2) {
				if (d1 == d2) {
					return true;
				}
				if (d1 < d2) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	string getDateFromRooms(string str)
		/**function to get the date from which a room is checked in from the line of the file*/
	{
		string str1 = removeFirstAst(removeFirstAst(removeFirstAst(removeFirstAst(removeFirstAst(str)))));
		return str1.substr(0, str1.find("*"));

	}
	string getDateToRooms(string str)
		/**function to get the date until which a room is checked in from the line of the file*/
	{
		string str1 = removeFirstAst(removeFirstAst(removeFirstAst(removeFirstAst(removeFirstAst(removeFirstAst(str))))));
		return str1.substr(0, str1.find("*"));
	}

	int checkDays(int fromY, int fromM)
		/**asigns number of days depending on what year and month it is*/
	{

		if (fromM == 1 || fromM == 3 || fromM == 5 || fromM == 7 || fromM == 8 || fromM == 10 || fromM == 12) {
			return 31;
		}
		else if (fromM == 2 && fromY % 4 == 0) {
			return 29;
		}
		else if (fromM == 2 && fromY % 4 != 0) {
			return 28;
		}
		else {
			return 30;
		}
	}
	int countDays(int fromY, int fromM, int fromD, int toY, int toM, int toD)
		/*counts the days from one date until another, needed for Room::report()*/
	{
		int days = 0;
		if (fromY == toY) {

			if (fromM == toM) {
				return toD - fromD;
			}
			else {

				for (int i = fromM; i < toM; i++) {
					int monthDays = checkDays(fromY, fromM);
					days = days + monthDays + (toD - fromD);
				}
				return days;
			}
		}
		else {
			for (int i = fromY; i < toY; i++) {
				if (i % 4 == 0) {
					days = days + 366;
				}
				else {
					days = days + 365;
				}
				for (int k = fromM; k < toM; k++) {
					int monthDays = checkDays(fromY, fromM);
					days = days + monthDays;
				}

			}
			days = days + (toD - fromD);
			return days;
		}
		return days;
	}
	string getLastLine(ifstream& in)
		/**returns the last line of a fiile*/
	{
		string line;
		while (in >> ws && getline(in, line));
		cout << "line: " << line << endl;
		return line;
	}

};