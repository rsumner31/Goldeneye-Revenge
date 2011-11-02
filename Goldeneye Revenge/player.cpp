/////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	player.cpp - This file will handle all the player functions
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

void updatePlayerAmmo();
TextString *ammo;

//////////////////////////////////////////////////////////////
//
//	Player class constructor
//
//////////////////////////////////////////////////////////////
Player::Player()
{
	isReloading = false;
	_maxWeapons = 0;
	_currentWeapon = 0;
	loadSettings("player.ini");

	playerInfo.health = atoi(playerSettings->getSetting("main", "health").data.c_str());
	playerInfo.armour = atoi(playerSettings->getSetting("main", "armour").data.c_str());	

	health = new Object();	
	health->color(50, 194, 61, 255);

	armour = new Object();	
	armour->color(112, 165, 254, 255);

	underHealth = new Object();	
	underHealth->color(150, 0, 0, 100);	

	underArmour = new Object();	
	underArmour->color(50, 65, 215, 100);

	ammo = new TextString();	
	ammo->scaleMag = 1.3f;
	ammo->setFontFace(courierNew);
	ammo->setCharacerSpacing(12);
	ammo->color(Colour(0x000000));	

	for (int soundsLoop = 0; soundsLoop < 8; soundsLoop++)
	{
		ricochet[soundsLoop] = new Sound();
		ricochet[soundsLoop]->loadSound((char *)string("sound/fx/ricochet_" + toString(soundsLoop + 1) + ".wav").c_str());	
	}
}

Player::~Player()
{	

}

//////////////////////////////////////////////////////////////
//
//	This function returns the current weapon
//
//////////////////////////////////////////////////////////////
Weapon* Player::currentWeapon()
{
	return weapons[_currentWeapon];
}

//////////////////////////////////////////////////////////////
//
//	This function draws the current weapon
//
//////////////////////////////////////////////////////////////
void Player::drawCurrentWeapon()
{
	weapons[_currentWeapon]->drawWeapon();
}

//////////////////////////////////////////////////////////////
//
//	This function draws the player HUD
//
//////////////////////////////////////////////////////////////
void Player::drawHUD()
{
	//	Re-calculate all of the correct sizes and positions
	float healthY = SCREEN_HEIGHT / 45;

	health->size(screenRatio.width * (playerInfo.health * 3), screenRatio.height * 15);
	health->position(20.0f, SCREEN_HEIGHT - (screenRatio.height * 20));
	armour->size(screenRatio.width * (playerInfo.armour * 3), screenRatio.height * 15);
	armour->position(20.0f, health->getPosition().y - (screenRatio.height * 20.0f));

	underHealth->size(screenRatio.width * 300, screenRatio.height * 15);
	underHealth->position(20.0f, SCREEN_HEIGHT - (screenRatio.height * 20));
	underArmour->size(screenRatio.width * 300, screenRatio.height * 15);
	underArmour->position(20.0f, health->getPosition().y - (screenRatio.height * 20.0f));
	
	underHealth->drawObject();
	underArmour->drawObject();

	health->drawObject();
	armour->drawObject();

	if (weapons[_currentWeapon]->gunHUD != NULL)
	{
		//	We have to set the position on every frame incase
		//	the user changes the screen resolution
		weapons[_currentWeapon]->gunHUD->position(20.0f, SCREEN_HEIGHT - (SCREEN_HEIGHT / 45) - 50.0f - weapons[_currentWeapon]->gunHUD->getSize().height);
		weapons[_currentWeapon]->gunHUD->drawObject();
	}

	if (weapons[_currentWeapon]->gunBullet != NULL)
	{
		//	We have to set the position on every frame incase
		//	the user changes the screen resolution
		weapons[_currentWeapon]->gunBullet->position(SCREEN_WIDTH - (weapons[_currentWeapon]->gunBullet->getSize().width + 20.0), SCREEN_HEIGHT - (SCREEN_HEIGHT / 45) - (weapons[_currentWeapon]->gunBullet->getSize().height));
		weapons[_currentWeapon]->gunBullet->drawObject();

		string text;
		text = toString(weapons[_currentWeapon]->weaponInfo.currentAmmo) + "/";
		text += toString(weapons[_currentWeapon]->weaponInfo.totalAmmo);
		
		ammo->position(weapons[_currentWeapon]->gunBullet->getPosition().x - (ammo->getCalculatedSize().width + (weapons[_currentWeapon]->gunBullet->getSize().width * 1.5)), SCREEN_HEIGHT - (SCREEN_HEIGHT / 45) - 20.0f, 0);
		ammo->setText(text);
		ammo->drawString();		
	}	
}

//////////////////////////////////////////////////////////////
//
//	This function fires the player's current weapon
//
//////////////////////////////////////////////////////////////
int reloadTimer = 0;
bool isFiring = false;
void Player::fireWeapon(int buttonPressed)
{
	if (GAME_MODE != IN_GAME) return;

	//	Make sure that the current time is not within the reloading
	//	time
	if (reloadTimer < (int)GetTickCount() - ((weapons[_currentWeapon]->reloadEndFrame - weapons[_currentWeapon]->reloadStartFrame) * 60))
	{				
		//	If left mouse down
		if (buttonPressed == 0)
		{		
			//	Fire the weapon
			weapons[_currentWeapon]->fire(updatePlayerAmmo);
			isFiring = true;
		}
			
		//	If left mouse up
		if (buttonPressed == 3 && isFiring)
		{
			isFiring = false;
				
			//	If the fire mode is auto, reset the state so
			//	we can loop again
			if (weapons[_currentWeapon]->weaponInfo.fireMode == AUTO)
			{
				//	We need to wait until the animation is finished
				//	before we can reset the state
				weapons[_currentWeapon]->gunModel->playFrames(weapons[_currentWeapon]->gunModel->getCurrentFrame(), weapons[_currentWeapon]->fireEndFrame);
				
				if (weapons[_currentWeapon]->gunModel->getCurrentFrame() >= weapons[_currentWeapon]->fireEndFrame)
				{
					weapons[_currentWeapon]->resetState();				
				}			
			}
		}			
	}
}

//////////////////////////////////////////////////////////////
//
//	This function updates the player ammo
//
//////////////////////////////////////////////////////////////
void updatePlayerAmmo()
{
	//	If the current ammo is less than or = to 0
	//	reload the weapon
	if (PLAYER->currentWeapon()->weaponInfo.currentAmmo <= 0)
	{		
		isFiring = false;
		PLAYER->reloadWeapon();
	}
	else
	{
		PLAYER->checkFire(middle.x, middle.y);

		PLAYER->currentWeapon()->decAmmo(1);
	}
}

//////////////////////////////////////////////////////////////
//
//	This function reloads the current weapon
//
//////////////////////////////////////////////////////////////
void Player::reloadWeapon()
{
	//	If the current reload timer is out of the
	//	reload timer then reload
	if (reloadTimer < (int)GetTickCount() - 1000 && !isFiring)
	{
		reloadTimer = GetTickCount();

		isReloading = true;		
		weapons[_currentWeapon]->reload();	

		//	Get how much needs to be added to the clip
		int difference = weapons[_currentWeapon]->weaponInfo.maxAmmoPerClip - weapons[_currentWeapon]->weaponInfo.currentAmmo;

		if (difference > weapons[_currentWeapon]->weaponInfo.totalAmmo)
		{
			difference = weapons[_currentWeapon]->weaponInfo.totalAmmo;
		}

		//	Update the clip and total ammo
		weapons[_currentWeapon]->weaponInfo.totalAmmo -= difference;
		weapons[_currentWeapon]->weaponInfo.currentAmmo += difference;
		isReloading = false;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will check the player's fire with the world
//	objects
//
//////////////////////////////////////////////////////////////
void Player::checkFire(int x, int y)
{
	int objectsFound = 0;
	int viewPortCoords[4] = {0};
	unsigned int selectedBuffer[32] = {0};

	//	Init a buffer for the selected objects
	glSelectBuffer(32, selectedBuffer);

	//	Get the view port coordinates
	glGetIntegerv(GL_VIEWPORT, viewPortCoords);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

		//	Set the render mode to select
		glRenderMode(GL_SELECT);
		glLoadIdentity();

		//	Set the click matrix and multiply it by the perspective
		//	view
		gluPickMatrix(x, y, 2, 2, viewPortCoords);
		gluPerspective(70.0, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.005f, 1000.0f);		

		glMatrixMode(GL_MODELVIEW);
		renderGame();

		//	Get the objects 
		objectsFound = glRenderMode(GL_RENDER);

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	//	Here we find out the closest object we clicked on
	if (objectsFound > 0)
	{
		int lowestDepth = selectedBuffer[1];
		int selectedObject = selectedBuffer[3];

		//	Loop through the objects and compare them to the stack
		for (int objectCount = 0; objectCount < (int)objectsFound; objectCount++)
		{
			if ((int)selectedBuffer[(objectCount * 4) + 1] < (int)lowestDepth)
			{
				lowestDepth = selectedBuffer[(objectCount * 4) + 1];
				selectedObject = selectedBuffer[(objectCount * 4) + 3];
			}
		}

		//	We need to find object associated with this ID
		//	Store the ID with the object type in a list
		//	and switch between it. 
		//	AI or Model then we can do what ever with it
		//	ai -> health -= gun damage
		//	model -> nothing (add new model of bullet hole?)
		list<AI*>::iterator it;
		for (it = __aiList->begin(); it != __aiList->end(); ++it)
		{
			if (it._Ptr->_Myval->getID() == selectedObject)
			{
				//	Play player hurt sound
				it._Ptr->_Myval->getShot(PLAYER->currentWeapon()->weaponInfo);
				
				return;
			}
		}

		list<Model*>::iterator modelIt;
		for (modelIt = __modelList->begin(); modelIt != __modelList->end(); ++modelIt)
		{
			if (modelIt._Ptr->_Myval->getID() == selectedObject)
			{
				int id = random(0, 9);	

				//	Play ricochet sound
				if (id < 8)
				{
					debug("ricochet", "yes");
					ricochet[id]->updateVolume(soundVolume);
					ricochet[id]->playSound();								
				}

				return;
			}
		}
		
	}	
}

//////////////////////////////////////////////////////////////
//
//	This function gets the players position
//
//////////////////////////////////////////////////////////////
Vector3 Player::getPlayerPos()
{
	return playerPosition;
}

//////////////////////////////////////////////////////////////
//
//	This function generates the weapon's textures
//
//////////////////////////////////////////////////////////////
void Player::generateWeaponTextures()
{
	//	Loop through the ammount of weapons loaded and generate
	//	their texture ID's
	for (int gunId = 0; gunId < _maxWeapons; gunId++)
	{				
		weapons[gunId]->genTexture();		
	}
}

//////////////////////////////////////////////////////////////
//
//	This function loads in the player settings
//
//////////////////////////////////////////////////////////////
void Player::loadSettings(char *fileName)
{
	playerSettings = new Settings(fileName);
	
	//	Get the position of the player from the ini
	float posX = atof(playerSettings->getSetting("position", "x").data.c_str());
	float posY = atof(playerSettings->getSetting("position", "y").data.c_str());
	float posZ = atof(playerSettings->getSetting("position", "z").data.c_str());
	playerPosition = Vector3(posX, posY, posZ);

	//	Get the current weapon from the ini
	_currentWeapon = atoi(playerSettings->getSetting("main", "currentweapon").data.c_str());

	//	Loop through template setting weapon<int> and load a new 
	//	weapon from it's value
	for (int loop = 0;; loop++)
	{
		string weaponStr = "weapon";
		weaponStr += toString(loop);
		string weaponPath = playerSettings->getSetting("inventory", (char *)weaponStr.c_str()).data;

		if (weaponPath == "")
		{
			break;
		}
		else
		{
			//	Load the weapon
			weapons[loop] = new Weapon();
			weapons[loop]->loadSettings((char *)weaponPath.c_str());
			if (weapons[loop]->gunHUD != NULL)
			{				
				weapons[loop]->gunHUD->position(20.0f, SCREEN_HEIGHT - (SCREEN_HEIGHT / 45) - 50.0f - weapons[loop]->gunHUD->getSize().height);
				weapons[loop]->gunHUD->color(255, 255, 255, 255);
				weapons[loop]->gunHUD->setTransparency(true);
			}

			//	Check if there is a bullet avatar set
			if (weapons[loop]->gunBullet != NULL)
			{				
				weapons[loop]->gunBullet->color(255, 255, 255, 255);
				weapons[loop]->gunBullet->setTransparency(true);
			}

			weaponStr += "ammo";
			weapons[loop]->weaponInfo.totalAmmo = toInt(playerSettings->getSetting("inventory", (char *)weaponStr.c_str()).data);
			_maxWeapons++;
		}		
	}
}

//////////////////////////////////////////////////////////////
//
//	This function sets the current weapon
//
//////////////////////////////////////////////////////////////
void Player::setCurrentWeapon(int weaponId)
{
	_currentWeapon = _maxWeapons - 1 < weaponId ? _maxWeapons -1 : (weaponId < 0 ? 0 : weaponId);
	playerSettings->writeSetting("main", "currentweapon", (char *)toString(_currentWeapon).c_str(), false);
}

void Player::nextWeapon()
{
	int id;

	if (_currentWeapon >= _maxWeapons - 1)
	{
		id = 0;
	}
	else
	{
		id = _currentWeapon + 1;
	}

	setCurrentWeapon(id);
}

void Player::previousWeapon()
{
	int id;

	if (_currentWeapon <= 0)
	{
		id = _maxWeapons - 1;
	}
	else
	{
		id = _currentWeapon - 1;
	}

	setCurrentWeapon(id);
}

//////////////////////////////////////////////////////////////
//
//	This function will test a collision with a model
//
//////////////////////////////////////////////////////////////
bool Player::collisionTest(Model *model, char key)
{
	Vector3 min = model->minBounds;
	Vector3 max = model->maxBounds;
	
	//	Get the middle point of the object
	Vector3 middle = (max - min) / 2;

	//	Calculate the distance between the player's position and the object					
	Vector3 pos = CAMERA.getAxis() - middle;					

	if ((playerPosition.x >= min.x - 0.5) && (playerPosition.x < max.x + 0.5))		
	{					
		if ((playerPosition.y >= min.y - 0.5) && (playerPosition.y < max.y + 0.5))				
		{							
			if ((playerPosition.z >= min.z - 0.5) && (playerPosition.z < max.z + 0.5))		
			{		
				//	What value is closer to 0
				float x = CAMERA.getAxis().x;
				float z = CAMERA.getAxis().z;
				float dir = MIN(abs(x), abs(z));

				//	If moving left
				if (key == 'a')
				{
					moveY((MOVE_SPEED) * 5);
				}
					
				if (key == 'd')
				{
					moveY(-((MOVE_SPEED) * 5));
				}	
					
				if (key == 'w')
				{						
					moveX(-(MOVE_SPEED));						
				}
					
				if (key == 's')
				{
					moveX((MOVE_SPEED));
				}

				return true;
			}
		}
	}
	
	return false;
}

bool Player::collisionTest(Map *map, char key)
{	
	for (int objCount = 0; objCount < map->objectCount; objCount++)
	{	
		if (collisionTest(map->mapObjects[objCount], key) == true)
		{
			return true;
		}
	}
	
	return false;     
}

//////////////////////////////////////////////////////////////
//
//	This function will set the position of the player
//
//////////////////////////////////////////////////////////////
void Player::setPosition(Vector3 position)
{	
	CAMERA.setPosition(position);
	playerPosition = position;
}

void Player::setPosition(float x, float y, float z)
{		
	setPosition(Vector3(x, y, z));
}

//////////////////////////////////////////////////////////////
//
//	This function will move the player on the x axis
//
//////////////////////////////////////////////////////////////
void Player::moveX(float speed)
{	
	CAMERA.moveCameraX(speed);
	playerPosition = CAMERA.cameraPosition;
}

//////////////////////////////////////////////////////////////
//
//	This function will move the player on the y axis
//
//////////////////////////////////////////////////////////////
void Player::moveY(float speed)
{
	CAMERA.moveCameraY(speed);
	playerPosition = CAMERA.cameraPosition;
}