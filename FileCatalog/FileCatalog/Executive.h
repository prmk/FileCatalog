#ifndef EXECUTIVE_H
#define EXECUTIVE_H
/////////////////////////////////////////////////////////////////////////////
// Executive.h - Interaction between the user and the system			   //
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
/*
Module Operations
-----------------
This module acts an interaction between the user and the system.

It interacts with the Catalog to process the data.

It informs the Display package to show the results to the user


Public Interface
----------------
Executive exec
exec.FileCatalog;
exec.display;


Required Files
--------------
Executive.h, Executive.cpp


Maintenance History
-------------------
ver 1.0:
- Code to call the Catalog
- Code to show the results on the console
*/

#include "Catalog.h"
#include "DisplayHandler.h"

namespace FileCatalog
{
	class Executive
	{
	public:
		Catalog FileCatalog;
		Display display;
	private:
	};
}

#endif