
#include "KeySpec.h"


KeySpec::KeySpec()
{
	#include ".\resources\KeySpecInitialize.h"
}

KeySpec::~KeySpec()
{
	;
}

void KeySpec::Product::Initialize()
{
	for (unsigned int i=0; i<data.size(); ++i)
	{
		lookup_int[data[i].id]=i;
		lookup_string[data[i].name]=i;
	}
}

void KeySpec::ProductList::Initialize()
{
	for (unsigned int i=0; i<data.size(); ++i)
	{
		lookup_int[data[i].id]=i;
		lookup_string[data[i].name]=i;
	}
}

void KeySpec::HeaderList::Initialize()
{
	for (unsigned int i=0; i<data.size(); ++i)
	{
		lookup_int[data[i].id]=i;
		lookup_string[data[i].name]=i;
	}
}