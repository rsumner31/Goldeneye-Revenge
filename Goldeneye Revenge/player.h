//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Player.h - This is the header file for the player class
//	
//////////////////////////////////////////////////////////////

class Player
{
	public:
		int _currentWeapon;
		int _maxWeapons;
		PlayerInfo playerInfo;

		Player::Player();
		Player::~Player();

		Weapon* Player::currentWeapon();
		bool Player::collisionTest(Model *model, char key);
		bool Player::collisionTest(Map *map, char key);
		void Player::checkFire(int x, int y);
		void Player::drawCurrentWeapon();
		void Player::drawHUD();
		void Player::fireWeapon(int buttonPressed);
		Vector3 Player::getPlayerPos();
		void Player::generateWeaponTextures();
		void Player::loadSettings(char *fileName);	
		void Player::setCurrentWeapon(int weaponId);		
		void Player::nextWeapon();
		void Player::previousWeapon();
		void Player::reloadWeapon();										
		void Player::setPosition(Vector3 position);
		void Player::setPosition(float x, float y, float z);
		void Player::moveX(float speed);
		void Player::moveY(float speed);

	private:
		Object *health, *underHealth, *armour, *underArmour;
		Settings *playerSettings;
		
		//	Position
		Vector3 playerPosition;

		//	Inventory
		Weapon *weapons[8];		
		bool isReloading;

		//	Ricochet sounds
		Sound *ricochet[8];
};