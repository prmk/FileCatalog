#include "DisplayHandler.h"
#include <iostream>
#include <string>
using namespace FileCatalog;

void Display::title(const string& title, char ch)
{
	cout << endl << title;
	cout << endl << string(title.size() + 2, ch);
	cout << endl;
}

void Display::print(vector<string>& vector_)
{
	vector<string>::iterator pt;
	for (pt = vector_.begin(); pt != vector_.end(); ++pt)
		cout << (*pt).c_str() << endl;
	cout << endl;
}

void Display::showInfo(const string& info)
{
	cout << info << endl;
}

void Display::showDataStore(DataStore dataStore)
{
	dataStore.showContents();
}

void Display::showDuplicateFiles(DataStore dataStore)
{
	dataStore.showDuplicateFiles();
}

void Display::showSearchResults(map<string, set<int>> searchResults)
{
	map<string, set<int>>::iterator it;
	set<int>::iterator st;
	string positions;
	string output;
	for (it = searchResults.begin(); it != searchResults.end(); ++it)
	{
		for (st = ((*it).second).begin(); st != ((*it).second).end(); ++st)
			positions = positions + to_string(*st) + ", ";
		positions = positions.substr(0, positions.size() - 2);
		if (positions.size() > 0)
		{
			output = output + "File\t\t-> " + (*it).first + "\n";
			output = output + "Position(s)\t -> ";
			output = output + positions + "\n\n";
			positions = "";
		}
	}
	if (output.size() <= 0)
		output = "No matches\n";
	cout << output;
}

#ifdef TEST_DISPLAY

int main(int argc, char *argv[])
{
	Display display;
	display.title("Header");
	display.title("SubHeader", '-');
	display.showInfo("End of Test Stub");
}

#endif