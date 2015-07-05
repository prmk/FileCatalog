/////////////////////////////////////////////////////////////////////////////
// DisplayHandler.cpp - Displays the input and the output on the console     //
//																		   //
// ver 1.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Pramukh Shenoy, 2015                                        //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS L501x											   //
// Application: OOD Project, 2015	                                       //
// Author:      Pramukh Shenoy,Syracuse University                         //
//              (315) 746-0822, pbantwal@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
NOTE:
Test Stubs for this package needs data from the Executive and hence,
all the functions cannot be tested.
*/

#include "DisplayHandler.h"
#include <iostream>
#include <string>
using namespace FileCatalog;

//Displays a title on the console
void Display::title(const string& title, char ch)
{
	cout << endl << title;
	cout << endl << string(title.size() + 2, ch);
	cout << endl;
}

////prints the vector of strings on the console
//void Display::print(vector<string>& vector_)
//{
//	vector<string>::iterator pt;
//	for (pt = vector_.begin(); pt != vector_.end(); ++pt)
//		cout << (*pt).c_str() << endl;
//	cout << endl;
//}

//prints the info to the user
void Display::showInfo(const string& info)
{
	cout << info << endl;
}

//shows the contents of the datastore to the user
void Display::showDataStore(DataStore dataStore)
{
	dataStore.showContents();
}

//shows a summary of the contents of the datastore
void Display::showSummary(DataStore dataStore)
{
	dataStore.showSummary();
}

//shows the duplicate files to the user
void Display::showDuplicateFiles(DataStore dataStore)
{
	dataStore.showDuplicateFiles();
}

//shows the results of the search text
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
	display.title("Test Stub", '=');

	display.showInfo("End of Test Stub");
}

#endif