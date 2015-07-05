/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Interaction between the user and the system			   //
//																		   //
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
#include "Executive.h"
#include <iostream>
#include <string>
using namespace FileCatalog;
using namespace std;

int main(int argc, char *argv[])
{
	Executive exec;
	exec.display.title("                          FILE CATALOG                          ", '=');

	exec.FileCatalog.parseCL(argc, argv);

	exec.display.title("Command Line Arguments");
	exec.FileCatalog.printCL();

	exec.display.title("DataStore Contents");
	exec.FileCatalog.scanforFiles();
	exec.display.showDataStore(exec.FileCatalog.store());

	if (!exec.FileCatalog.duplicateFiles() && !exec.FileCatalog.recursiveSearch() && !exec.FileCatalog.search())
	{
		exec.display.title("Summary");
		exec.display.showSummary(exec.FileCatalog.store());
	}

	if (exec.FileCatalog.duplicateFiles())
	{
		exec.display.title("Duplicate Files List");
		exec.display.showDuplicateFiles(exec.FileCatalog.store());
	}
	if (exec.FileCatalog.search())
	{
		exec.display.title("Search Result for text '" + exec.FileCatalog.searchText() + "'");
		exec.display.showSearchResults(exec.FileCatalog.searchforText());
	}


	while (true)
	{
		cout << endl << "Press ENTER to exit" << endl;

		string searchtext;
		string patterns;
		cout << "Enter the search text:";
		getline(cin, searchtext);
		if (searchtext == "\n" || searchtext == "")
			break;
		else
		{
			cout << "Enter the patterns (Can be left blank)";
			getline(cin, patterns);
			exec.FileCatalog.processSearchPattern(patterns);
			exec.display.title("Search Result for text '" + searchtext + "'");
			exec.display.showSearchResults(exec.FileCatalog.searchforText(searchtext));
		}
	}

}