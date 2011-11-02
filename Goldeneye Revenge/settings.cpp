//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Settings.cpp - This file will handle the opening and
//	processing of ini files.
//
//////////////////////////////////////////////////////////////

//	Include all the needed headers
#include "main.h"
#include <iostream>
#include <fstream>
#include <memory.h>

//////////////////////////////////////////////////////////////
//
//	This is the default class constructor
//
//////////////////////////////////////////////////////////////
Settings::Settings()
{
	Settings::Settings("settings.ini");
}

Settings::Settings(char *_fileName)
{ 
	//	Make sure the file exists
	struct stat fileInfo;
	if (stat(_fileName, &fileInfo) != 0)
	{
		string errorStr = "Could not load settings from ";
		errorStr += _fileName;
		error((char *)errorStr.c_str());
	}

	fileName = _fileName;
	std::ifstream stream;
	stream.open(_fileName);	

	loopPointer = 0;
	if (stream.is_open())
	{
		string lastGroupName = "";

		while (!stream.eof())
		{					
			char data[256];
			stream.getline(data, 256);
			string _data = string(data);

			//	Remove any commenting from the INI, comments start with ; and end before the new line
			int commentPos = _data.find_first_of(';');
			int endPos = _data.length();

			if (commentPos > -1)
			{
				_data = _data.substr(0, commentPos);
			}

			if (_data.find_first_of('[') != std::string::npos)
			{
				int endBracket = _data.find_first_of(']');
				lastGroupName = _data.substr(1, endBracket - 1);				
				continue;
			}			

			if (_data != "")
			{
				std::vector<std::string> splitData = split(_data, '=');	

				if (splitData.size() > 0)
				{										
					Setting setting = Setting(lastGroupName, splitData.at(0), splitData.at(1));
					settings[loopPointer] = Setting(setting);

					loopPointer++;
				}
			}
		}
	}

	stream.close();
}

Settings::~Settings()
{
	
}

//////////////////////////////////////////////////////////////
//
//	This function gets the group list
//
//////////////////////////////////////////////////////////////
list<string>* Settings::getGroupList()
{
	list<string> *groupNames = new list<string>;

	for each(Setting setting in settings)
	{
		if (setting.groupName == "")
		{
			break;
		}

		if (!inList(setting.groupName, groupNames))
		{
			groupNames->push_back(setting.groupName);
		}
	}

	return groupNames;
}

//////////////////////////////////////////////////////////////
//
//	This function gets a setting value from the group name
//	and the name you are searching for 
//
//////////////////////////////////////////////////////////////
Setting Settings::getSetting(char *_groupName, char *_name)
{
	for each(Setting setting in settings)
	{
		if (setting.name == _name && setting.groupName == _groupName)
		{
			return setting;
		}
	}

	return Setting("", "", "");
}

//////////////////////////////////////////////////////////////
//
//	This function writes the settings to the file
//
//////////////////////////////////////////////////////////////
void Settings::writeSetting(char *_groupName, char *_name, char *_data, bool updateFile)
{
	bool updated = false;
	string lastGroupName;

	int count = 0;
	for each(Setting setting in settings)
	{
		if (setting.name == _name && setting.groupName == _groupName)
		{
			settings[count].data = _data; 
			updated = true;
		}

		count++;
	}

	if (!updated)
	{
		settings[loopPointer].groupName = _groupName;
		settings[loopPointer].name = _name;
		settings[loopPointer].data = _data;
		loopPointer++;
	}
	
	if (updateFile)
	{
		ofstream stream(fileName);

		if (stream.is_open())
		{
			for each(Setting setting in settings)
			{	
				if (setting.data == "")
				{
					break;
				}

				if (lastGroupName != setting.groupName)
				{
					stream << "[" + setting.groupName + "]";
					stream << "\r\n";
					lastGroupName = setting.groupName;
				}				

				stream << setting.name;
				stream << "=";
				stream << setting.data;
				stream << "\r\n";
			}
		}

		stream.close();
	}
}