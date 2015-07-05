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


	cout << endl << "Press ENTER to exit" << endl;
	while (true)
	{
		string input;
		getline(cin, input);
		if (input == "\n" || input == "")
			break;
	}

}