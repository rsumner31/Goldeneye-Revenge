//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	weapon.cpp - This file will handle all the weapon functions
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the main class initializer
//
//////////////////////////////////////////////////////////////
Weapon::Weapon()
{
	weaponInfo.totalAmmo = 0;
	weaponInfo.fireMode = SINGLE;
	weaponInfo.maxAmmoPerClip = 0;
	weaponInfo.currentAmmo = 0;
	weaponInfo.damage = 0;

	gunModel = NULL;
	gunHUD = NULL;
	gunBullet = NULL;
}

Weapon::~Weapon()
{
	delete &gunModel;
}

//////////////////////////////////////////////////////////////
//
//	This function draws the model
//
//////////////////////////////////////////////////////////////
void Weapon::drawWeapon()
{
	gunModel->originalPosition = modelPosition;
	gunModel->drawModel();
}

//////////////////////////////////////////////////////////////
//
//	This function plays the fire frames
//
//////////////////////////////////////////////////////////////
void Weapon::fire(void (*_callback)())
{
	if (weaponInfo.totalAmmo <= 0)
	{
		return;
	}

	if (weaponInfo.currentAmmo == 0)
	{
		//	Put a sound of an empty weapon click here?
		PLAYER->reloadWeapon();
		return;
	}
	else
	{
		//	Clear the sound and check there is a fire sound set
		gunModel->clearSound();
		if (gunSounds[0]->getSoundId() > 0)
		{
			gunSounds[0]->updateVolume(soundVolume);
			gunModel->setSound(gunSounds[0], fireSoundFrame);
		}

		bool loop;
		if (weaponInfo.fireMode == SINGLE)
		{
			loop = false;
		}
		else if (weaponInfo.fireMode == AUTO)
		{
			loop = true;
		}

		//	Play the frames of the gun firing 
		gunModel->playFrames(fireStartFrame, fireEndFrame, loop, _callback, fireSoundFrame);		
	}
}

//////////////////////////////////////////////////////////////
//
//	This function generates the texture ID's
//
//////////////////////////////////////////////////////////////
void Weapon::genTexture()
{
	gunModel->genBuffers();
	gunModel->genTexture();

	if (gunHUD != NULL)
	{
		gunHUD->genTexture();	
	}

	if (gunBullet != NULL)
	{
		gunBullet->genTexture();
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will load the weapon's settings file and
//	set the variables correctly
//
//////////////////////////////////////////////////////////////
void Weapon::loadSettings(char *filePath)
{
	//	Get the gun's settings
	gunSettings = new Settings(filePath);
	//	Load the model
	loadModel((char *)gunSettings->getSetting("main", "model").data.c_str());	
	
	//	Load the texutre/s
	string texturesPath = gunSettings->getSetting("main", "textures").data;
	if (texturesPath != "")
	{
		Settings *textures = new Settings((char *)texturesPath.c_str());
		gunModel->loadTextures(textures);
	}
	else
	{
		loadTexture((char *)gunSettings->getSetting("main", "texture").data.c_str());		
	}

	//	Get the weapon info
	weaponInfo.damage = atof((char *)gunSettings->getSetting("fire", "damage").data.c_str());

	//	Get the start and end frames of the model
	fireStartFrame = toInt(gunSettings->getSetting("fire", "startframe").data);
	fireEndFrame = toInt(gunSettings->getSetting("fire", "endframe").data);
	reloadStartFrame = toInt(gunSettings->getSetting("reload", "startframe").data);
	reloadEndFrame = toInt(gunSettings->getSetting("reload", "endframe").data);
	weaponInfo.maxAmmoPerClip = toInt(gunSettings->getSetting("main", "maxammo").data);
	weaponInfo.currentAmmo = weaponInfo.maxAmmoPerClip;

	gunSounds[0] = new Sound();
	gunSounds[1] = new Sound();

	//	Load the fire sound if set
	string path = gunSettings->getSetting("fire", "sound").data;
	if (path != "")
	{		
		gunSounds[0]->loadSound((char *)path.c_str());		
		fireSoundFrame = toInt(gunSettings->getSetting("fire", "soundframe").data);
	}

	//	Load the reload sound if set
	string path2 = gunSettings->getSetting("reload", "sound").data;
	if (path2 != "")
	{		
		gunSounds[1]->loadSound((char *)path2.c_str());
		reloadSoundFrame = toInt(gunSettings->getSetting("reload", "soundframe").data);
	}

	//	Get the weapon pos
	float posX = atof((char *)gunSettings->getSetting("position", "x").data.c_str());
	float posY = atof((char *)gunSettings->getSetting("position", "y").data.c_str());
	float posZ = atof((char *)gunSettings->getSetting("position", "z").data.c_str());
	modelPosition = Vector3(posX, posY, posZ);

	//	Get any rotation vectors
	float rotateA = atof((char *)gunSettings->getSetting("rotate", "a").data.c_str());
	float rotateX = atof((char *)gunSettings->getSetting("rotate", "x").data.c_str());
	float rotateY = atof((char *)gunSettings->getSetting("rotate", "y").data.c_str());
	float rotateZ = atof((char *)gunSettings->getSetting("rotate", "z").data.c_str());
	modelRotate = Vector4(rotateX, rotateY, rotateZ, rotateA);

	//	Get any resize vectors
	float resizeX = atof((char *)gunSettings->getSetting("resize", "x").data.c_str());
	float resizeY = atof((char *)gunSettings->getSetting("resize", "y").data.c_str());
	float resizeZ = atof((char *)gunSettings->getSetting("resize", "z").data.c_str());
	modelResize = Vector3(resizeX, resizeY, resizeZ);

	//	Set the resize and rotate vectors
	gunModel->resize(modelResize);
	gunModel->rotate(modelRotate);

	//	Get the gun fire mode
	string _fireMode = gunSettings->getSetting("main", "firemode").data;

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

	string weapon2d = gunSettings->getSetting("main", "avatar").data;

	if (weapon2d != "")
	{
		gunHUD = new Object();
		gunHUD->loadTexture((char *)weapon2d.c_str());		
	}

	string bullet2d = gunSettings->getSetting("main", "bulletavatar").data;

	if (bullet2d != "")
	{
		gunBullet = new Object();
		gunBullet->loadTexture((char *)bullet2d.c_str());		
	}
}

//////////////////////////////////////////////////////////////
//
//	This function loads the model
//
//////////////////////////////////////////////////////////////
void Weapon::loadModel(char *filePath)
{
	gunModel = new Model();
	gunModel->loadModel(filePath, false);		
}

//////////////////////////////////////////////////////////////
//
//	This function loads the texture
//
//////////////////////////////////////////////////////////////
void Weapon::loadTexture(char *filePath)
{
	gunModel->loadTexture(filePath);
}

//////////////////////////////////////////////////////////////
//
//	This function plays the reload frames
//
//////////////////////////////////////////////////////////////
void Weapon::reload()
{	
	//	If the current clip is full, return the function
	if (weaponInfo.currentAmmo >= weaponInfo.maxAmmoPerClip)
	{
		return;
	}
	
	//	Clear the sound and use the reload sound if it is set
	gunModel->clearSound();
	if (gunSounds[1]->getSoundId() > 0)
	{
		gunSounds[1]->updateVolume(soundVolume);
		gunModel->setSound(gunSounds[1], reloadSoundFrame);
	}

	//	Play the frames of the reload
	gunModel->playFrames(reloadStartFrame, reloadEndFrame);
}

//////////////////////////////////////////////////////////////
//
//	This function resets the frame of the model
//
//////////////////////////////////////////////////////////////
void Weapon::resetState()
{	
	gunModel->clearSound();
	gunModel->setFrame(0);
}

//////////////////////////////////////////////////////////////
//
//	This function deducts the current ammo in the clip by a
//	set ammount
//
//////////////////////////////////////////////////////////////
void Weapon::decAmmo(int ammount)
{
	weaponInfo.currentAmmo -= ammount;
}