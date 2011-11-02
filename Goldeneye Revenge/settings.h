//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Settings.h - This is the header file for the settings class
//	
//////////////////////////////////////////////////////////////

class Settings
{
	public:
		Settings::Settings();
		Settings::~Settings();
		Settings::Settings(char *_fileName);	
		list<string>* Settings::getGroupList();
		Setting Settings::getSetting(char *_groupName, char *_name);
		void Settings::writeSetting(char *_groupName, char *_name, char *_data, bool updateFile = true);

	private:		
		char *fileName;
		Setting settings[1024];	
		int loopPointer;
};
