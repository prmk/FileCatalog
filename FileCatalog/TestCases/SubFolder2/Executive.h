#ifndef EXECUTIVE_H
#define EXECUTIVE_H
#include "../Catalog/Catalog.h"
#include "../Display/DisplayHandler.h"

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