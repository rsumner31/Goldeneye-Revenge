//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Map.cpp - This class will handle the loading of maps
//
//////////////////////////////////////////////////////////////

//	Include all the needed headers
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the main class initializer
//
//////////////////////////////////////////////////////////////
Map::Map()
{
	objectCount = 0;
	aiCount = 0;
}

Map::~Map()
{
	delete &landscape;

	//	Loop through the object array and display the model
	for (int objectLoop = 0; objectLoop < objectCount; objectLoop++)
	{
		delete &mapObjects[objectLoop];		
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will load the settings of the map 
//
//////////////////////////////////////////////////////////////
void Map::loadSettings(char *filePath)
{
	Settings *mapSettings = new Settings(filePath);
	
	landscape = new Model();
	landscape->loadSettings((char *)mapSettings->getSetting("main", "model").data.c_str());	

	//	Loop through template setting object<int>/AI<int> and load a new 
	//	object/AI from it's value
	for (int loop = 0, counter = 0;; loop++, counter++)
	{
		string objectStr = "object";
		objectStr += toString(counter);
		string objectPath = mapSettings->getSetting("objects", (char *)objectStr.c_str()).data;		

		if (objectPath == "")
		{
			break;
		}
		else
		{
			//	Load the object
			mapObjects[loop] = new Model();
			mapObjects[loop]->loadSettings((char *)objectPath.c_str());	

			Settings *modelSettings = mapObjects[loop]->modelSettings;
			if (modelSettings->getSetting("main", "type").data == string("wall"))
			{					
				//	Get the tile count for each vertex
				int tileX = toInt(mapObjects[loop]->modelSettings->getSetting("tile", "x").data);
				int tileY = toInt(mapObjects[loop]->modelSettings->getSetting("tile", "y").data);
				int tileZ = toInt(mapObjects[loop]->modelSettings->getSetting("tile", "z").data);

				//	Create the new tiled objects for the X axis
				float lastPosX = mapObjects[loop]->originalPosition.x;
				for (int tx = 0; tx < tileX; tx++)
				{
					loop++;
					objectCount++;

					mapObjects[loop] = new Model();
					memcpy(mapObjects[loop], mapObjects[loop - 1], sizeof(Model));
					mapObjects[loop]->originalPosition.x = lastPosX + (mapObjects[loop]->maxBounds.x - mapObjects[loop]->minBounds.x) - 0.1;					
					lastPosX = lastPosX + (mapObjects[loop]->maxBounds.x - mapObjects[loop]->minBounds.x) - 0.1;	
				}

				//	Create the new tiled objects for the Y axis
				float lastPosY = mapObjects[loop]->originalPosition.y;
				for (int ty = 0; ty < tileY; ty++)
				{
					loop++;
					objectCount++;

					mapObjects[loop] = new Model();
					memcpy(mapObjects[loop], mapObjects[loop - 1], sizeof(Model));
					mapObjects[loop]->originalPosition.y = lastPosY + (mapObjects[loop]->maxBounds.y - mapObjects[loop]->minBounds.y) - 0.1;					
					lastPosY = lastPosY + (mapObjects[loop]->maxBounds.y - mapObjects[loop]->minBounds.y) - 0.1;	
				}

				//	Create the new tiled objects for the Z axis
				float lastPosZ = mapObjects[loop]->originalPosition.z;
				for (int tz = 0; tz < tileZ; tz++)
				{
					loop++;
					objectCount++;

					mapObjects[loop] = new Model();
					memcpy(mapObjects[loop], mapObjects[loop - 1], sizeof(Model));
					mapObjects[loop]->originalPosition.z = lastPosZ + (mapObjects[loop]->maxBounds.z - mapObjects[loop]->minBounds.z) - 0.1;					
					lastPosZ = lastPosZ + (mapObjects[loop]->maxBounds.z - mapObjects[loop]->minBounds.z) - 0.1;	
				}
			}
			
			delete modelSettings;
		}		

		objectCount++;
	}

	//	Loop through template setting object<int>/AI<int> and load a new 
	//	object/AI from it's value
	for (int loop = 0;; loop++)
	{
		string aiStr = "AI";
		aiStr += toString(loop);
		string aiPath = mapSettings->getSetting("AI", (char *)aiStr.c_str()).data;

		if (aiPath == "")
		{
			break;
		}
		else
		{
			mapAI[loop] = new AI();
			mapAI[loop]->loadSettings((char *)aiPath.c_str());
		}

		aiCount++;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will generate the buffer and texture ID's 
//	for all the objects
//
//////////////////////////////////////////////////////////////
void Map::generateAllBuffers()
{
	landscape->genBuffers();
	landscape->genTexture();

	//	Loop through the object array and gen the buffers + textures
	for (int objectLoop = 0; objectLoop < objectCount; objectLoop++)
	{
		mapObjects[objectLoop]->genBuffers();
		mapObjects[objectLoop]->genTexture();
	}

	for (int aiLoop = 0; aiLoop < aiCount; aiLoop++)
	{
		mapAI[aiLoop]->genBuffers();
		mapAI[aiLoop]->genTexture();
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will draw all the objects
//
//////////////////////////////////////////////////////////////
void Map::drawMap()
{
	landscape->drawModel();

	//	Loop through the object array and display the model
	for (int objectLoop = 0; objectLoop < objectCount; objectLoop++)
	{
		mapObjects[objectLoop]->drawModel();		
	}

	for (int aiLoop = 0; aiLoop < aiCount; aiLoop++)
	{
		mapAI[aiLoop]->execute();		
	}
}