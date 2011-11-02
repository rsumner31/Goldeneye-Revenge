//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Map.h - This is the header file for the map class
//	
//////////////////////////////////////////////////////////////

class Map
{
	public:
		int objectCount;	
		int aiCount;

		Model *mapObjects[1024];
		AI *mapAI[1024];
		Model *landscape;

		Map::Map();
		Map::~Map();

		void Map::loadSettings(char *filePath);
		void Map::drawMap();
		void Map::generateAllBuffers();

	private:		
};