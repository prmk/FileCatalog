#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////////////
// DataStore.h - Provides the structure to store the data			       //
// ver 1.0                                                                 //
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
This module provides the structure to store the contents.
The contents include a map of file names and the list of paths it
belongs to.
The module is used by the Catalog and Display packages to fetch and
display the contents


Public Interface
----------------
DataStore dataStore;
PathIter it;
ListofIters lt;
ListIter listIter;
iterator it = Store::iterator;
dataStore.save("fileSpec");
dataStore.save("fileName.txt","C:\temp");
dataStore.showContents();
dataStore.showSummary();
dataStore.showDuplicateFiles();
dataStore.begin();
dataStore.end();
dataStore.getPath("fileName.txt");


Required Files
--------------
DataStore.h, DataStore.cpp


Build Command
-------------
cl /DTEST_DATASTORE DataStore.cpp


Maintenance History
-------------------
ver 1.0:
- Code to define the structure
- Code to insert the values into the structure
*/
#include<vector>
#include<set>
#include<list>
#include<map>
#include<string>

using namespace std;

namespace FileCatalog
{
	class DataStore
	{
	public:
		using Path = string;
		using Paths = set < Path >;
		using PathIter = Paths::iterator;
		using ListofIters = list < PathIter >;
		using ListIter = ListofIters::iterator;
		using File = string;
		using Store = map < File, ListofIters >;
		using iterator = Store::iterator;

		void save(const string& filespec);
		void save(const string& filename, const string& path);
		void showContents();
		void showSummary();
		void showDuplicateFiles();
		iterator begin() { return store.begin(); }
		iterator end() { return store.end(); }
		Paths getPath(File file);

	private:
		Paths paths;
		Store store;
	};
}

#endif