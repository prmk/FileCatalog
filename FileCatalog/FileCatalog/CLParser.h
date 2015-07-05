#ifndef CLPARSER_H
#define CLPARSER_H
/////////////////////////////////////////////////////////////////////////////
// CLParser.h - Command Line Parser.  									   //
//				Analyzes the options provided by the user.				   //
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
This module provides class and the functions required to scan the command line
provided by the user.

It stores the results in the CLParser class.
It also defines the list of options that can be provided by the user.

The module is used by the Catalog package.


Public Interface
----------------
CLParser parser();
Patterns patt ;
PattIter iter ;
parser.recursiveSearch;
parser.duplicateFiles;
parser.search;
parser.searchText;
parser.path;
parser.patterns;


Required Files
--------------
CLParser.h, CLParser.cpp


Build Command
-------------
cl /DTEST_CLPARSER CLParser.cpp


Maintenance History
-------------------
ver 1.0:
- Code to definitions of the options
- Code to analyze the command line arguements

*/
#include <vector>
#include <iostream>
using namespace std;

namespace FileCatalog
{
	class CLParser
	{
	public:
		CLParser();
		~CLParser();
		using Patterns = vector < string >;
		using PattIter = Patterns::iterator;
		bool recursiveSearch;
		bool duplicateFiles;
		bool search;
		string searchText;
		string path;
		Patterns patterns;

		void addPattern(const string& patt);
		void analyzeCommandLine(int argc, char *argv[]);
		void print();
		bool compare(const string& str1, const string& str2);

	private:
		string recursiveSearchCommand;
		string duplicateFilesCommand;
		string searchCommand;
	};
}
#endif