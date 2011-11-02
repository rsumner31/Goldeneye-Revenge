//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	weapons.h - This file handles all the weapons constants
//	and functions
//	
//////////////////////////////////////////////////////////////

enum fireModes
{
	SINGLE = 1,
	BURST = 2,
	AUTO = 3
};

class Weapon
{
	public:
		WeaponInfo weaponInfo;
		Vector3 modelPosition;
		Model *gunModel;
		Object *gunHUD;
		Object *gunBullet;

		int fireStartFrame;
		int fireEndFrame;
		int reloadStartFrame;
		int reloadEndFrame;
	
		Weapon::Weapon();		
		Weapon::~Weapon();
		void Weapon::drawWeapon();		
		void Weapon::fire(void (*_callback)());
		int Weapon::getCurrentAmmo();
		int Weapon::getTotalAmmo();
		int Weapon::getMaxAmmo();
		void Weapon::genTexture();
		void Weapon::loadSettings(char *filePath);
		void Weapon::loadModel(char *filePath);
		void Weapon::loadTexture(char *filePath);				
		void Weapon::reload();		
		void Weapon::resetState();
		void Weapon::setTotalAmmo(int count);
		void Weapon::decAmmo(int ammount);

	private:	
		Settings *gunSettings;		
		Vector4 modelRotate;
		Vector3 modelResize;

		char *name;					

		//	Gun sounds
		//	sound 0: fire sound
		//	sound 1: reload sound
		Sound *gunSounds[8];
		int fireSoundFrame;
		int reloadSoundFrame;		
};