#include"Helper_Functions.cpp"
using namespace std;
/**@file
@brief
Room class header file.
*/
static Helper h;
class Room {
public:
	int roomId = 0, beds = 0, takenBeds = 0;
	int takenStatus = 0;
	string note = "", fromDate = "", toDate = "";
	int lastRoomId()
		/**returns the last room id recorded, needed to add new rooms*/
	{
		ifstream myFiles;
		myFiles.open("rooms.txt");
		if (h.iss_empty(myFiles)) {
			return 0;
		}
		else {
			int lastId = h.getId(h.getLastLine(myFiles));
			cout << lastId << endl;;
			return lastId;

		}
	}
	void addRooms(int n)
		/**adds a new room with <beds> and saves it to file.*/
	{
		ofstream myFile;

		Room room;
		room.roomId = lastRoomId() + 1;
		myFile.open("rooms.txt", ios::app);
		cin >> room.beds;
		room.takenBeds = 0;
		room.takenStatus = 0;
		room.note = "";
		room.fromDate = "";
		room.toDate = "";
		myFile << room.roomId << "*" << room.beds << "*" << room.takenBeds << "*" << room.takenStatus << "*" << room.note << "*" << room.fromDate << "*" << room.toDate << "*\n";
		myFile.close();

	}
	void checkin(string str)
		/**takes <room> <date from> <date to> <note> [<number of people>] and checks them in if the room has enough beds for the people. Stores the note in the file.*/
	{
		//					checkin 6 2020-03-23 2020-03-31 The Simpsons
		Room curr;
		bool isFine = 1;
		curr.note = "";
		string substr = h.removeFirstWord(str);

		string room = "", fromY = "", fromM = "", fromD = "", toY = "", toM = "", toD = "", numberOfPeople;
		int element = 0;
		while (element < 4) {
			if (element == 0) {
				//getting room number
				for (auto x : substr) {
					if (x == ' ') {
						int n = room.length();
						while (isdigit(room[room.length() - n])) {
							n--;
						}
						if (n == 0) {
							break;
						}
						else {
							cout << "Room number should be a number." << endl;
							isFine = 0;
							return;
						}
					}
					else {
						room = room + x;
					}
				}

			}
			else if (element == 1) {
				h.parseDate(substr, fromY, fromM, fromD);
				//getting dates from 
			}
			else if (element == 2) {
				h.parseDate(substr, toY, toM, toD);
				if (!h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(toY), h.strToInt(toM), h.strToInt(toD))) {

					cout << "Checkout date should be after checkin date." << endl;
					isFine = 0;
					return;
				}
				//getting dates to

			}
			else {
				if (h.is_number(substr.substr(substr.length() - 1)) && substr[substr.length() - 2] == ' ') {
					numberOfPeople = substr[substr.length() - 1];
					cout << "people = " << numberOfPeople << endl;
					curr.note = substr.substr(0, substr.length() - 2);

				}
				else {
					curr.note = substr;
				}
			}



			element++;
			substr = h.removeFirstWord(substr);

		}	cout << "From: " << fromY << "-" << fromM << "-" << fromD << endl;
		cout << "To: " << toY << "-" << toM << "-" << toD << endl;
		cout << "note = " << curr.note << endl;
		ifstream strm;
		int i = 0;
		string f;
		strm.open("rooms.txt");
		while (!strm.eof()) {
			getline(strm, f);

			if (i == (h.strToInt(room) - 1)) {
				if (h.getBeds(f) < h.strToInt(numberOfPeople)) {
					isFine = 0;
				}
			}
			i++;
		}
		strm.close();

		if (isFine == 1) {

			curr.roomId = h.strToInt(room);
			curr.takenBeds = h.strToInt(numberOfPeople);

			if (fromM.length() == 1) {
				fromM = "0" + fromM;
			}
			if (fromD.length() == 1) {
				fromD = "0" + fromD;
			}
			curr.fromDate = fromY + "-" + fromM + "-" + fromD;
			if (toM.length() == 1) {
				toM = "0" + toM;
			}
			if (toD.length() == 1) {
				toD = "0" + toD;
			}
			curr.toDate = toY + "-" + toM + "-" + toD;
			curr.takenStatus = 2;

			ifstream istrm;
			istrm.open("rooms.txt");
			ofstream ostrm;
			ostrm.open("rooms1.txt");
			while (!istrm.eof()) {
				getline(istrm, f);

				if (h.strToInt(h.firstWord(f)) == (h.strToInt(room))) {
					curr.beds = h.getBeds(f);
					ostrm << curr.roomId << "*" << curr.beds << "*" << curr.takenBeds << "*" << curr.takenStatus << "*" << curr.note << "*" << curr.fromDate << "*" << curr.toDate << "*\n";
				}
				else {
					ostrm << f << "\n";
				}
			}

			istrm.close();
			ostrm.close();
			if (remove("rooms.txt") != 0) {
				cout << "Cant delete file." << endl;
			}
			if (rename("rooms1.txt", "rooms.txt") != 0) {
				cout << "Cant rename files." << endl;
			}
			cout << "Checkin successful." << endl;
		}
		else {
			cout << "Checkin unsuccessful." << endl;
		}
	}
	void checkout(string str)
		/**checks a room out, takes <room> and resets the taken status and taken beds and note and dates to default. */
	{
		int rId = h.strToInt(h.removeFirstWord(str));
		string linesF[100];
		ifstream myS;
		int i = 0;
		myS.open("rooms.txt");
		while (!myS.eof()) {
			getline(myS, linesF[i]);


			if (h.strToInt(h.firstWord(linesF[i])) == rId) {
				string tempB = h.firstWord(h.removeFirstAst(linesF[i]));
				linesF[i] = to_string(rId) + "*" + tempB + "*0*0***";
			}
			i++;
		}
		myS.close();
		ofstream newS;
		newS.open("rooms1.txt");
		for (int k = 0; k < i; k++) {
			int l = 0;
			if (linesF[k] == "") {
				l++;
				continue;
			}
			linesF[k - l] = linesF[k];
			newS << linesF[k] << endl;
		}
		newS.close();
		if (remove("rooms.txt") != 0) {
			cout << "Can't delete file.";
		}
		if (rename("rooms1.txt", "rooms.txt") != 0) {
			cout << "Can't rename file.";
		}
		cout << "checkout successful" << endl;
	}
	void availability(string str)
		/**takes <date> and prints a list of the rooms available on that date*/
	{
		string checkY = "", checkM = "", checkD = "";
		h.parseDate(h.removeFirstWord(str), checkY, checkM, checkD);
		ifstream myS;
		string line;
		int i = 0;
		myS.open("rooms.txt");
		cout << "Rooms that are free on that day.";
		while (!myS.eof()) {
			string compY = "", compM = "", compD = "", compToY = "", compToM = "", compToD = "";
			getline(myS, line);
			h.parseDate(h.getDateFromRooms(line), compY, compM, compD);
			h.parseDate(h.getDateToRooms(line), compToY, compToM, compToD);

			if (h.compareDates(h.strToInt(checkY), h.strToInt(checkM), h.strToInt(checkD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) ||
				!h.compareDates(h.strToInt(checkY), h.strToInt(checkM), h.strToInt(checkD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD)))
			{
				cout << h.firstWord(line) << endl;
			}


		}
		myS.close();
	}
	void  report(string str)
		/**takes <from> and <to> and prints how many days every room is being used during that period*/
	{
		//report 2020-03-25 2020-04-12
		//       2020-03-23 2020-03-31
		//fromX, toX = input date
		//compX, compToX = line date

		string fromY, fromM, fromD, toY, toM, toD, line;
		h.parseDate(h.firstWord(h.removeFirstWord(str)), fromY, fromM, fromD);
		h.parseDate(h.removeFirstWord(h.removeFirstWord(str)), toY, toM, toD);
		ifstream myS;
		myS.open("rooms.txt");
		while (!myS.eof()) {
			int countDay = 0;
			string compY = "", compM = "", compD = "", compToY = "", compToM = "", compToD = "";
			getline(myS, line);
			h.parseDate(h.getDateFromRooms(line), compY, compM, compD);

			h.parseDate(h.getDateToRooms(line), compToY, compToM, compToD);
			if (h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) &&
				!h.compareDates(h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) &&
				h.compareDates(h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD)))
			{
				// a < c; c < b < d
				countDay = h.countDays(h.strToInt(compY), h.strToInt(compM), h.strToInt(compD), h.strToInt(toY), h.strToInt(toM), h.strToInt(toD));
			}
			else if (!h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) &&
				h.compareDates(h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD)))
			{
				// a > c; b < d
				countDay = h.countDays(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(toY), h.strToInt(toM), h.strToInt(toD));
			}
			else if (!h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) &&
				!h.compareDates(h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD)))
			{
				// d > a > c; b > d
				countDay = h.countDays(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD));
			}
			else if (h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) &&
				!h.compareDates(h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD)))
			{
				//a < c; b > d
				countDay = h.countDays(h.strToInt(compY), h.strToInt(compM), h.strToInt(compD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD));

			}
			if (countDay > 0) {
				cout << "Room " << line.substr(0, line.find("*")) << " is taken for " << countDay << " days." << endl;
			}

		}


		myS.close();
	}
	void find(string str)
		/**takes <beds> <from> and <to> and finds a room with enough beds that is free during that time*/
	{
		//find 4 2020-03-18 2020-03-21
		int beds = h.strToInt(h.firstWord(h.removeFirstWord(str)));
		string fromY, fromM, fromD, toY, toM, toD, line;
		int minRoom = 0, minBed = INT_MAX;
		h.parseDate(h.firstWord(h.removeFirstWord(h.removeFirstWord(str))), fromY, fromM, fromD);
		h.parseDate(h.removeFirstWord(h.removeFirstWord(h.removeFirstWord(str))), toY, toM, toD);
		ifstream myS;
		myS.open("rooms.txt");
		while (!myS.eof()) {
			getline(myS, line);
			string compY = "", compM = "", compD = "", compToY = "", compToM = "", compToD = "";
			int currBeds = h.strToInt(h.firstWord(h.removeFirstAst(line)));
			h.parseDate(h.getDateFromRooms(line), compY, compM, compD);
			h.parseDate(h.getDateToRooms(line), compToY, compToM, compToD);
			string t = h.removeFirstAst(h.removeFirstAst(h.removeFirstAst(line)));
			if (h.strToInt(t.substr(0, t.find("*"))) == 0) {
				if (currBeds >= beds && (currBeds - beds) < minBed) {
					minRoom = h.getId(line);
					minBed = currBeds - beds;
				}
			}
			else if (h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD)) &&
				h.compareDates(h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compY), h.strToInt(compM), h.strToInt(compD))) {
				if (currBeds >= beds && (currBeds - beds) < minBed) {
					minRoom = h.getId(line);
					minBed = currBeds - beds;
				}
			}
			else if (!h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD)) &&
				h.compareDates(!h.strToInt(toY), h.strToInt(toM), h.strToInt(toD), h.strToInt(compToY), h.strToInt(compToM), h.strToInt(compToD))) {
				if (currBeds >= beds && (currBeds - beds) < minBed) {
					minRoom = h.getId(line);
					minBed = currBeds - beds;
				}
			}

		}
		if (minRoom == 0) {
			cout << "No suitable room";
		}
		else {
			cout << "\n Best room:" << minRoom << endl;
		}
		myS.close();
	}

	void unavailable(string str)
		/**takes <room>, <from>, <to> and <note> and marks the room as unavailable for that period with the note*/
	{
		//unavailable 3 2020-03-21 2020-04-20 remont
		string note = "";
		string substr = h.removeFirstWord(str);
		string fromY = "", fromM = "", fromD = "", toY = "", toM = "", toD = "", line;
		int element = 0;
		int room = h.strToInt(h.firstWord(h.removeFirstWord(str)));
		h.parseDate(h.firstWord(h.removeFirstWord(h.removeFirstWord(str))), fromY, fromM, fromD);
		h.parseDate(h.firstWord(h.removeFirstWord(h.removeFirstWord(h.removeFirstWord(str)))), toY, toM, toD);
		note = h.removeFirstWord(h.removeFirstWord(h.removeFirstWord(h.removeFirstWord(str))));
		if (!h.compareDates(h.strToInt(fromY), h.strToInt(fromM), h.strToInt(fromD), h.strToInt(toY), h.strToInt(toM), h.strToInt(toD))) {
			return;
		}

		cout << room << endl;
		cout << fromY << " " << fromM << " " << fromD << endl;
		cout << toY << " " << toM << " " << toD << endl;
		cout << "note = " << note << endl;
		int i = 0;
		ofstream myS;
		myS.open("rooms1.txt");
		ifstream strm;
		strm.open("rooms.txt");

		while (!strm.eof()) {
			getline(strm, line);
			if (i == room - 1) {
				int tempBeds = h.strToInt(h.firstWord(h.removeFirstAst(line)));
				myS << room << "*" << tempBeds << "*0*1*" << note << "*" << fromY << "-" << fromM << "-" << fromD << "*" << toY << "-" << toM << "-" << toD << "*\n";
			}
			else {
				myS << line << "\n";
			}
			i++;
		}
		myS.close();
		strm.close();
		if (remove("rooms.txt") != 0) {
			cout << "Cant delete file." << endl;
		}
		if (rename("rooms1.txt", "rooms.txt") != 0) {
			cout << "Cant rename files." << endl;
		}


	}
};
