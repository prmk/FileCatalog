#ifndef CATALOG_H
#define CATALOG_H
/////////////////////////////////////////////////////////////////////////////
// Catalog.h - Interacts with the File Manager, Command Line Parser,       //
//				and DataStore											   //
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
This module interacts with the File Manager, Command Line Parser
and DataStore packages.

It ties up all the functionality provided by the File Manager,
Command Line Parser and the DataStore packages.

The module is used by the Executive to process the data provided
by the user.


Public Interface
----------------
Catalog catalog;
catalog.parseCL(argc,*argv[]);
catalog.printCL();
vector<string> patt = catalog.filePattern();
bool rec = catalog.recursiveSearch();
bool dup = catalog.duplicateFiles();
bool srch = catalog.search();
string srchTxt = catalog.searchText();
string path = catalog.userpath();
catalog.scanforFiles();
vector<string> dirs = catalog.getDirectories();
catalog.saveData(file,dir);
DataStore store = catalog.store();
map<string,set<int>> result = catalog.searchforText();
catalog.processSearchPattern("*.cpp *.h");
catalog.Catalog::matchPattern("test.txt");
catalog.searchPatterns;


Required Files
--------------
Catalog.h, Catalog.cpp


Build Command
-------------
cl /DTEST_CATALOG Catalog.cpp


Maintenance History
-------------------
ver 1.2:
- Code to handle DataStore

ver 1.0:
- Code to handle the Command Line Parser
- Code to handle Files
*/
#include <map>
#include <set>
#include <vector>
#include "DataStore.h"
#include "CLParser.h"
#include "FileSystem.h"
using namespace std;

namespace FileCatalog
{
	class Catalog
	{
	public:
		void parseCL(int argc, char *argv[]);
		void printCL();
		vector<string> filePattern();
		bool recursiveSearch();
		bool duplicateFiles();
		bool search();
		string searchText();
		string userpath();
		void scanforFiles();
		vector<string> getDirectories();
		void saveData(const string& file, const string& dir);
		DataStore store();
		map<string, set<int>> searchforText();
		map<string, set<int>> searchforText(string searchtext);
		void processSearchPattern(const string& patt);
		bool Catalog::matchPattern(const string& fileName);
		vector<string> searchPatterns;

	private:
		CLParser commandLineParser;
		DataStore dataStore;
		string currdir;
	};
}

#endif