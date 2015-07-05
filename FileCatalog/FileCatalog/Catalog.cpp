/////////////////////////////////////////////////////////////////////////////
// Catalog.cpp - Interacts with the File Manager, Command Line Parser,       //
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

#include "Catalog.h"

using namespace FileCatalog;
using namespace std;

//parse command line function calls informs the CLParser package to 
//analyze the input
void Catalog::parseCL(int argc, char *argv[])
{
	commandLineParser.analyzeCommandLine(argc, argv);
	currdir = commandLineParser.path;
}

//the print function prints the data provided by the user
void Catalog::printCL()
{
	commandLineParser.print();
}

//returns the patterns provided by the user
vector<string> Catalog::filePattern()
{
	return commandLineParser.patterns;
}

//tells whether the recursive option is provided by the user
bool Catalog::recursiveSearch()
{
	return commandLineParser.recursiveSearch;
}

//tells whether the duplicate option is provided by the user
bool Catalog::duplicateFiles()
{
	return commandLineParser.duplicateFiles;
}

//tells whether the search option is provided by the user
bool Catalog::search()
{
	return commandLineParser.search;
}

//returns the search text provided by the user
string Catalog::searchText()
{
	return commandLineParser.searchText;
}

//returns the user path set by the user
string Catalog::userpath()
{
	return commandLineParser.path;
}

//get the list of directories from the current directory
vector<string> Catalog::getDirectories()
{
	vector<string> patterns = filePattern();
	vector<string>::iterator pt;
	vector<string> directories;
	vector<string> temp;
	for (pt = patterns.begin(); pt != patterns.end(); ++pt)
	{
		string path = currdir;
		string pattern = (*pt).c_str();
		temp = Directory::getDirectories(path, pattern);
		directories.insert(directories.end(), temp.begin(), temp.end());
	}

	return directories;
}

//scans for the list of files in the current directory
//if the recursive option is set, scans for the files recursivelye
//passes the file name and the path name to the DataStore to be saved
void Catalog::scanforFiles()
{
	vector<string> patterns = filePattern();
	vector<string>::iterator pt;
	vector<string> files;
	vector<string>::iterator ft;
	for (pt = patterns.begin(); pt != patterns.end(); ++pt)
	{
		while (true)
		{

			if (Directory::exists(currdir))
			{
				Directory::setCurrentDirectory(currdir);
				break;
			}
			else
				Directory::setCurrentDirectory("..");
		}

		string path = Directory::getCurrentDirectory();
		string pattern = (*pt).c_str();
		files = Directory::getFiles(path, pattern);
		for (ft = files.begin(); ft != files.end(); ++ft)
			saveData((*ft), path);
	}

	if (recursiveSearch())
	{
		vector<string> directories = Directory::getDirectories();
		vector<string>::iterator it;

		for (it = directories.begin(); it != directories.end(); ++it)
		{
			currdir = (*it);
			if (currdir != "." && currdir != "..")
				scanforFiles();
		}
	}
}

//saves the data in the DataStore
void Catalog::saveData(const string& file, const string& dir)
{
	dataStore.save(file, dir);
}

//returns a copy of the dataStore
DataStore Catalog::store()
{
	return dataStore;
}

//scans for the list of files containing the search text
//creates a map of the result containing the full file path
//and the positions it has been found at
map<string, set<int>> Catalog::searchforText()
{
	string searchText = Catalog::searchText();
	map<string, list<set<string>::iterator>>::iterator it;
	map<string, set<int>> results;

	for (it = dataStore.begin(); it != dataStore.end(); ++it)
	{
		string fileName = (*it).first;
		set<string> path = dataStore.getPath(fileName);
		set<string>::iterator pt;
		for (pt = path.begin(); pt != path.end(); ++pt)
		{
			string fullpath = (*pt) + "\\" + fileName;
			File file(fullpath);
			file.open(File::in);
			if (file.isGood())
			{
				set<int> found = file.findString(searchText);
				results[fullpath] = found;
			}
			file.close();
		}
	}

	return results;
}

//is called during the console input of the file pattern and 
//the text to be searched.
//it divides the patterns provided into individual pattern
void Catalog::processSearchPattern(const string& patterns)
{
	searchPatterns.clear();
	size_t searchPosition = 0;
	int pos = -1;
	while (searchPosition < patterns.length())
	{
		pos = patterns.find(".", searchPosition);
		if (pos >= 0)
		{
			searchPosition = pos + 1;
			int end = patterns.find(" ", pos);
			if (end < 0)
				end = patterns.size();
			string patt = patterns.substr(pos-1, end);
			searchPatterns.push_back(patt);
		}
		else
			searchPosition++;
	}

	if (searchPatterns.size() == 0)
		searchPatterns = filePattern();
}

//is called during the console input of the file pattern and 
//the text to be searched.
//scans for the list of files containing the search text
//creates a map of the result containing the full file path
//and the positions it has been found at
map<string, set<int>> Catalog::searchforText(string searchText)
{
	map<string, list<set<string>::iterator>>::iterator it;
	map<string, set<int>> results;

	for (it = dataStore.begin(); it != dataStore.end(); ++it)
	{
		string fileName = (*it).first;
		if (matchPattern(fileName) == true)
		{
			set<string> path = dataStore.getPath(fileName);
			set<string>::iterator pt;
			for (pt = path.begin(); pt != path.end(); ++pt)
			{
				string fullpath = (*pt) + "\\" + fileName;
				set<string>::iterator pattiter;
				File file(fullpath);
				file.open(File::in);
				if (file.isGood())
				{
					set<int> found = file.findString(searchText);
					results[fullpath] = found;
				}
				file.close();
			}
		}
	}

	return results;
}

//is called during the console input of the file pattern and 
//the text to be searched.
//checks if the file pattern matches the input pattern
//provided by the user.
bool Catalog::matchPattern(const string& fileName)
{
	vector<string>::iterator it;
	for (it = searchPatterns.begin(); it != searchPatterns.end(); ++it)
	{
		string patt = (*it).substr(1);
		int found = fileName.find(patt);
		if (found > 0)
			return true;
	}

	if (searchPatterns.at(0) == "*.*")
		return true;
	return false;
}


#ifdef TEST_CATALOG

int main(int argc, char *argv[])
{
	cout << "Catalog Test Stub " << endl;
	cout << "===================" << endl << endl;

	Catalog catalog;

	cout << "Command Line Arguements " << endl;
	cout << "-------------------------" << endl;
	catalog.parseCL(argc, argv);
	catalog.printCL();

	cout << "Contents of the DataStore " << endl;
	cout << "-------------------------" << endl;
	catalog.scanforFiles();
	catalog.store().showContents();

	cout << "Summary of the DataStore" << endl;
	cout << "-------------------------" << endl;
	catalog.store().showSummary();

	if (catalog.duplicateFiles())
	{
		cout << "Duplicate Files List" << endl;
		cout << "-------------------------" << endl;
		catalog.store().showDuplicateFiles();
	}
}

#endif