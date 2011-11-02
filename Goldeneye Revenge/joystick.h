//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	joystick.h - This file handles all the joystick functions,
//	definitions and prototypes
//	
//
//	Left top trigger - prev weapon
//	Right top trigger - next weapon
//	X - reload
//////////////////////////////////////////////////////////////

class Joystick
{
	public:		
		float sensitivity;

		Joystick::Joystick();

		XINPUT_STATE Joystick::getState();
		void Joystick::disconnect();
		void Joystick::connect();
		bool Joystick::isUsable();
		bool Joystick::isConnected();
		void Joystick::viabrate(int leftVal = 0, int rightVal = 0);
		Vector2 Joystick::getCoordsFromLStick();
		Vector2 Joystick::getCoordsFromRStick();
		float Joystick::getStickMagnitude(int stick); 
		void Joystick::updateCoordinates(float x, float y);
		Vector2 Joystick::getCoodinates();

	private:
		XINPUT_STATE _controllerState;
		int _controllerNum;
		Vector2 joystickCoordinates;
		bool useJoystick;
};