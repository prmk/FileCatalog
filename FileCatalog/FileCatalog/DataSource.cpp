/////////////////////////////////////////////////////////////////////////////
// DataStore.cpp - Provides the structure to store the data			       //
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

#include "DataStore.h"
#include <iostream>
#include <list>

using namespace std;
using namespace FileCatalog;

//saves the file name and the paths in the datastore
//it stores the file name and the path only once in the datastore
void DataStore::save(const string& filename, const string& path)
{
	PathIter pt = paths.find(path);
	if (pt == paths.end())
	{
		pair<PathIter, bool> ret;
		ret = paths.insert(path);
		pt = ret.first;			// pt now points to the element inserted in above line
	}

	iterator st = store.find(filename);
	if (st == store.end())
	{
		ListofIters lt;
		lt.push_back(pt);
		store[filename] = lt;
	}
	else
	{
		(*st).second.push_back(pt);
	}
}


void DataStore::save(const string& filespec)
{

}

//prints the contents of the datastore to the console
void DataStore::showContents()
{
	showSummary();

	iterator it;
	for (it = store.begin(); it != store.end(); ++it)
	{
		cout << "File \t-> " << (*it).first << endl;
		cout << "Paths ";
		ListofIters lt = (*it).second;
		ListIter listIter;
		string pathNames;
		for (listIter = lt.begin(); listIter != lt.end(); ++listIter)
		{
			PathIter pt = *listIter;
			string pathName = (*pt);
			pathNames += "\t-> " + pathName + ",\n";
		}
		pathNames = pathNames.substr(0, pathNames.length() - 2);
		cout << pathNames;
		cout << endl << endl;
	}

	showSummary();
}

//prints a summary of the datastore to the console
void DataStore::showSummary()
{
	cout << "Summary :: The DataStore contains " << store.size() << " file(s) in " << paths.size() << " path(s)" << endl << endl;
}

//lists out the duplicate files names along with the path names 
void DataStore::showDuplicateFiles()
{
	bool flag = true;
	iterator it;
	for (it = store.begin(); it != store.end(); ++it)
	{
		size_t size = ((*it).second).size();
		if (size > 1)
		{
			flag = false;
			cout << "File \t-> " << (*it).first << endl;
			cout << "Paths ";
			ListofIters lt = (*it).second;
			ListIter listIter;
			string pathNames;
			for (listIter = lt.begin(); listIter != lt.end(); ++listIter)
			{
				PathIter pt = *listIter;
				string pathName = (*pt);
				pathNames += "\t-> " + pathName + ",\n";
			}
			pathNames = pathNames.substr(0, pathNames.length() - 2);
			cout << pathNames;
			cout << endl << endl;
		}
	}
	if (flag)
		cout << "Yayy!! No Duplicate files found!!" << endl;
}

//for a given path, returns the set of its paths 
set<string> DataStore::getPath(File file)
{
	Paths path;
	iterator it;

	for (it = store.begin(); it != store.end(); ++it)
	{
		if ((*it).first == file)
		{
			ListofIters lt = (*it).second;
			ListIter li;
			for (li = lt.begin(); li != lt.end(); ++li)
			{
				PathIter pt = *li;
				path.insert(*pt);
			}
		}
	}

	return path;
}

#ifdef TEST_DATASTORE

int main(int argc, char *argv[])
{
	cout << "Demonstrating DataStore Functionality" << endl;
	cout << "=====================================" << endl;
	cout << endl;

	DataStore dataStore;

	cout << "Inserting filename1.txt under C:/pathname1" << endl;
	dataStore.save("filename1.txt", "C:/pathname1");
	cout << "Inserting filename1.txt under C:/pathname2" << endl;
	dataStore.save("filename1.txt", "C:/pathname2");
	cout << "Inserting filename2.txt under C:/pathname2" << endl;
	dataStore.save("filename2.txt", "C:/pathname2");
	cout << endl;

	cout << "Contents of the DataStore" << endl;
	cout << "----------------------------" << endl;
	dataStore.showContents();
	cout << endl;

	cout << "Duplicate entries in the datastore" << endl;
	cout << "------------------------------------" << endl;
	dataStore.showDuplicateFiles();
	cout << endl;

	cout << "Summary of the datastore" << endl;
	cout << "--------------------------" << endl;
	dataStore.showSummary();
	cout << endl;

	cout << "Paths for filename1.txt" << endl;
	cout << "--------------------------" << endl;
	set<string> path = dataStore.getPath("filename1.txt");
	set<string>::iterator it;
	for (it = path.begin(); it != path.end(); ++it)
		cout << *it << endl;
	cout << endl;

}

#endif