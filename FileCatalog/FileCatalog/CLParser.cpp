/////////////////////////////////////////////////////////////////////////////
// CLParser.cpp - Command Line Parser.  								   //
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

#include "CLParser.h"
#include <iostream>
#include <string>
using namespace std;
using namespace FileCatalog;

//constructor for the CLParser to set the default values 
//and to define options
CLParser::CLParser()
{
	path = ".";
	patterns.push_back("*.*");
	recursiveSearchCommand = "/s";
	duplicateFilesCommand = "/d";
	searchCommand = "/f";
	recursiveSearch = false;
	duplicateFiles = false;
	search = false;
}

//deconstructor to free memory
CLParser::~CLParser()
{

}

//checks for duplicates and store the patterns in the patterns field
void CLParser::addPattern(const string& patt)
{
	if (patterns.size() == 1 && patterns[0] == "*.*")
		patterns.pop_back();
	patterns.push_back(patt);
}

//code the analyze the command line arguements
void CLParser::analyzeCommandLine(int argc, char *argv[])
{
	try
	{
		for (int count = 1; count < argc; count++)
		{
			string str = argv[count];
			if (compare(str, recursiveSearchCommand))
				recursiveSearch = true;
			else if (compare(str, duplicateFilesCommand))
				duplicateFiles = true;
			else if (compare(str, searchCommand))
			{
				if (count < argc-1)
				{
					search = true;
					searchText = argv[++count];
				}
			}
			else if (str.find("*") != string::npos)
				addPattern(str);
			else
				path = str;
		}
	}
	catch (exception& e)
	{
		cout << "Exception has occurred " << e.what() << endl;
	}
}

//does a case insensitive comparison between two strings
bool CLParser::compare(const string& str1, const string& str2)
{
	for (string::size_type i = 0; i < str1.length(); i++)
	{
		if (tolower(str1[i]) != tolower(str2[i]))
			return false;
	}
	return true;
}

//prints the options provided by the user to the console
void CLParser::print()
{
	PattIter pt;

	cout << "The path is " << path << endl;
	cout << "The file patterns are ";
	for (pt = patterns.begin(); pt != patterns.end(); ++pt)
		cout << (*pt).c_str() << " ";
	cout << endl;
	cout << "The recursive search command is available: ";
	if (recursiveSearch)
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
	cout << "The duplicate file search command is available: ";
	if (duplicateFiles)
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
	cout << "The search text command is available: ";
	if (search)
	{
		cout << "Yes" << endl;
		cout << "and the search text is '" << searchText << "'" << endl;
	}
	else
		cout << "No" << endl;

	cout << endl;

}

#ifdef TEST_CLPARSER

int main(int argc, char *argv[])
{
	CLParser parser;

	cout << "Testing Command Line Parser" << endl;
	cout << "===========================" << endl << endl;
	parser.analyzeCommandLine(argc, argv);
	parser.print();
}

#endif