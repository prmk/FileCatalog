#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <iostream>
#include <vector>
#include <list>
#include "../DataStore/DataStore.h"
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
		void showDuplicateFiles(DataStore dataStore);
		void showSearchResults(map<string,set<int>>);
	private:
	};
}

#endif