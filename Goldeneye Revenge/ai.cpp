//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	ai.cpp - This file will control all the enemies and their
//	movements/decisions
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the class constructor
//
//////////////////////////////////////////////////////////////
AI::AI()
{			
	this->mode = PATROL;

	weaponInfo.currentAmmo = 30;
	weaponInfo.damage = 1.0f;
	weaponInfo.fireMode = AUTO;
	weaponInfo.maxAmmoPerClip = 30;
	weaponInfo.totalAmmo = 200;

	aiInfo.health = 150;
	aiInfo.armour = 50;

	isWalking = false;
	model = NULL;

	__aiList->push_back(this);
}

int AI::getID()
{
	return model->getID();
}

//////////////////////////////////////////////////////////////
//
//	This function will load the settings
//
//////////////////////////////////////////////////////////////
void AI::loadSettings(char *filePath)
{
	//	Get the gun's settings
	aiSettings = new Settings(filePath);
	
	//	Load the model
	model = new Model();
	model->loadModel((char *)aiSettings->getSetting("main", "model").data.c_str());	
	
	//	Load the texutre/s
	string texturesPath = aiSettings->getSetting("main", "textures").data;
	if (texturesPath != "")
	{
		Settings *textures = new Settings((char *)texturesPath.c_str());
		model->loadTextures(textures);
	}
	else
	{
		model->loadTexture((char *)aiSettings->getSetting("main", "texture").data.c_str());		
	}

	//	Get the weapon pos
	float posX = atof((char *)aiSettings->getSetting("position", "x").data.c_str());
	float posY = atof((char *)aiSettings->getSetting("position", "y").data.c_str());
	float posZ = atof((char *)aiSettings->getSetting("position", "z").data.c_str());
	position = Vector3(posX, posY, posZ);

	//	Get any rotation vectors
	float rotateA = atof((char *)aiSettings->getSetting("rotate", "a").data.c_str());
	float rotateX = atof((char *)aiSettings->getSetting("rotate", "x").data.c_str());
	float rotateY = atof((char *)aiSettings->getSetting("rotate", "y").data.c_str());
	float rotateZ = atof((char *)aiSettings->getSetting("rotate", "z").data.c_str());
	rotation = Vector4(rotateX, rotateY, rotateZ, rotateA);

	//	Get any resize vectors
	float resizeX = atof((char *)aiSettings->getSetting("resize", "x").data.c_str());
	float resizeY = atof((char *)aiSettings->getSetting("resize", "y").data.c_str());
	float resizeZ = atof((char *)aiSettings->getSetting("resize", "z").data.c_str());
	scale = Vector3(resizeX, resizeY, resizeZ);

	//	Set the resize and rotate vectors
	model->originalPosition = position;
	this->originalPosition = position;
	model->resize(scale);
	model->rotate(rotation);

	//	Get the gun fire mode
	string _fireMode = aiSettings->getSetting("main", "firemode").data;

	if (_fireMode == "single")
	{
		weaponInfo.fireMode = SINGLE;
	}
	else if (_fireMode == "burst")
	{
		weaponInfo.fireMode = BURST;
	}
	else if (_fireMode == "auto")
	{
		weaponInfo.fireMode = AUTO;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will generate the buffers
//
//////////////////////////////////////////////////////////////
void AI::genBuffers()
{
	model->genBuffers();
}

//////////////////////////////////////////////////////////////
//
//	This function will generate the texture
//
//////////////////////////////////////////////////////////////
void AI::genTexture()
{	
	model->genTexture();
}

int AI::getText()
{
	return 1;
}

//////////////////////////////////////////////////////////////
//
//	This function will be called to handle what the AI does
//
//////////////////////////////////////////////////////////////
int x = 0;
void AI::execute()
{		
	this->decideMode();
	model->drawModel();

	if (!isWalking)
	{
		isWalking = true;
		model->playFrames(0, 25, true);
	}
}

void AI::decideMode()
{
	//	If the ai is within 100 units of the player, set mode
	//	to suspicious

	//	if the ai is within 50 units of the player, set mode
	//	to engage

	//	if the ai is outside 100 units of the player, set mode
	//	to patrol
	this->mode = PATROL;
	this->patrol();
}

int modifier = 1;
void AI::patrol()
{
	//if (lockMilliseconds(10.0f))
	{
		//	Set the new position to walk to and gradually walk to it
		if (position.y < -10 || position.y > 10)
		{
			modifier = modifier * -1;		
		}

		//position.y = model->originalPosition.y -= MOVE_SPEED * modifier;//Vector3(x -= 1, 0, 0);
	
		//this->model->position(position);
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will handle when the ai is shot/shoots
//
//////////////////////////////////////////////////////////////
void AI::getShot(WeaponInfo _weaponInfo)
{
	float totalDamage = _weaponInfo.damage;
	
	aiInfo.armour -= totalDamage;

	if (aiInfo.armour < 0)
	{
		float remainder = 0 - aiInfo.armour;
		aiInfo.health -= remainder;

		aiInfo.armour = 0;
	}
	else if (aiInfo.armour == 0)
	{
		aiInfo.health -= totalDamage;
	}

	if (aiInfo.health <= 0)
	{
		mode = DEAD;
		aiInfo.health = 0;
		aiInfo.armour = 0;
	}

	debug("AI Info", aiInfo);
}