//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	ai.h- This is the header file for the artificial intelligence
//	
//////////////////////////////////////////////////////////////

enum aiMode
{
	PATROL = 1,
	ENGAGE = 2,
	SUSPICIOUS = 3,
	DEAD = 4
};

class AI
{
	public:
		AI::AI();
		void AI::execute();
		int AI::getID();
		void AI::loadSettings(char *filePath);
		void AI::genTexture();
		void AI::genBuffers();
		void AI::decideMode();
		void AI::patrol();
		int AI::getText();
		void AI::getShot(WeaponInfo _weaponInfo);

	private:		
		int mode;
		Model *model;
		Settings *aiSettings;

		Vector3 position, scale;
		Vector3 originalPosition;
		Vector4 rotation;
		WeaponInfo weaponInfo;
		PlayerInfo aiInfo;
		bool isWalking;
};
