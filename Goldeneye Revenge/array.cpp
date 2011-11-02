//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	array.cpp - This file will contain conversion functions
//	for lists to arrays
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

void listToSetting(list<Setting> *_setting, Setting dst[])
{
	list<Setting>::iterator it;
	int size = ARRAY_LENGTH(_setting);

	int count = 0;
	for (it = _setting->begin(); it != _setting->end(); ++it, count++)
	{
		if (count >= size) break;

		dst[count] = Setting(it->groupName, it->name, it->data);
		debug("", dst[count]);
	}	
}

//////////////////////////////////////////////////////////////
//
//	This function checks if a string exists within a list of
//	strings
//
//////////////////////////////////////////////////////////////
bool inList(string search, list<string> *listString)
{
	list<string>::iterator it;

	for (it = listString->begin(); it != listString->end(); ++it)
	{
		if (search.compare(it->c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}