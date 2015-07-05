#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H
/////////////////////////////////////////////////////////////////////////////
// DisplayHandler.h - Displays the input and the output on the console     //
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
Module Operations
-----------------
This module prints the contents on to the console.

It prints the title for each operation

It prints the contents of the datastore.

It prints the input provided by the user.

The module is used by the Executive.


Public Interface
----------------
Display display;
display.title("Title");
display.print(values);
display.showInfo("Info");
display.showDataStore(dataStore);
display.showSummary(dataStore);
display.showDuplicateFiles(DataStore dataStore);
display.showSearchResults(results);


Required Files
--------------
DisplayHandler.h, DisplayHandler.cpp


Build Command
-------------
cl /DTEST_DISPLAY DisplayHandler.cpp


Maintenance History
-------------------
ver 1.2:
- Code to print duplicate files
- Code to print text search results

ver 1.0:
- Code to print the contents of the datastore
- Code to print the summary
- Code to print the title
*/
#include <iostream>
#include <vector>
#include <list>
#include "DataStore.h"
using namespace std;

namespace FileCatalog
{
	class Display
	{
	public:
		void title(const string& title, char ch = '-');
		void print(vector<string>& vector_);
		void showInfo(const string& info);
		void showDataStore(DataStore dataStore);
		void showSummary(DataStore dataStore);
		void showDuplicateFiles(DataStore dataStore);
		void showSearchResults(map<string, set<int>> results);
	private:
	};
}

#endif