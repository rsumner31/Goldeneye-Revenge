//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Joystick.cpp - This class will be used to handle the I/O
//	of the joystick
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the class constructor
//
//////////////////////////////////////////////////////////////
Joystick::Joystick()
{
	joystickCoordinates = Vector2(middle.x, middle.y);
	_controllerNum = 0;
	sensitivity = 0.7f;
	useJoystick = true;
}

//////////////////////////////////////////////////////////////
//
//	This function updates the coordinates of the joystick cursor
//
//////////////////////////////////////////////////////////////
void Joystick::updateCoordinates(float x, float y)
{
	joystickCoordinates.x = x;
	joystickCoordinates.y = y;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the joystick's coordinates
//
//////////////////////////////////////////////////////////////
Vector2 Joystick::getCoodinates()
{
	return joystickCoordinates;
}

//////////////////////////////////////////////////////////////
//
//	This function gets the state of the joystick
//
//////////////////////////////////////////////////////////////
XINPUT_STATE Joystick::getState()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

//////////////////////////////////////////////////////////////
//
//	This function disconnects the jyostick
//
//////////////////////////////////////////////////////////////
void Joystick::disconnect()
{
	this->useJoystick = false;
}

void Joystick::connect()
{
	this->useJoystick = true;
}

bool Joystick::isUsable()
{
	return this->useJoystick & this->isConnected();
}

//////////////////////////////////////////////////////////////
//
//	This function checks if the joystick is connected or not
//
//////////////////////////////////////////////////////////////
bool Joystick::isConnected()
{
	// Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    DWORD result = XInputGetState(_controllerNum, &_controllerState);

    if (result == ERROR_SUCCESS)
    {
        return true;
    }
    
    return false;
}

//////////////////////////////////////////////////////////////
//
//	This function viabrates the controller
//
//////////////////////////////////////////////////////////////
void Joystick::viabrate(int leftVal, int rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(_controllerNum, &Vibration);
}

//////////////////////////////////////////////////////////////
//
//	This function gets the normalized coordinates from the 
//	left stick
//
//////////////////////////////////////////////////////////////
Vector2 Joystick::getCoordsFromLStick()
{
	XINPUT_GAMEPAD buttonValues = this->getState().Gamepad;
	float leftX = buttonValues.sThumbLX;
	float leftY = buttonValues.sThumbLY;

	//	For x, the values range from -32768 to +32767. 0 is centered
	//	- values are down or left, + values are up or right
	float magnitude = sqrt(leftX * leftX + leftY * leftY);
	float normalX = leftX / magnitude;
	float normalY = leftY / magnitude;
	
	return Vector2(normalX, normalY);
}

//////////////////////////////////////////////////////////////
//
//	This function gets the normalized coordinates from the
//	right stick
//
//////////////////////////////////////////////////////////////
Vector2 Joystick::getCoordsFromRStick()
{
	XINPUT_GAMEPAD buttonValues = this->getState().Gamepad;
	float rightX = buttonValues.sThumbRX;
	float rightY = buttonValues.sThumbRY;

	//	For x, the values range from -32768 to +32767. 0 is centered
	//	- values are down or left, + values are up or right
	float magnitude = sqrt(rightX * rightX + rightY * rightY);
	float normalX = rightX / magnitude;
	float normalY = rightY / magnitude;
	
	return Vector2(normalX, normalY);
}

//////////////////////////////////////////////////////////////
//
//	This function gets the magnitude of the stick from its
//	coordinates
//
//////////////////////////////////////////////////////////////
float Joystick::getStickMagnitude(int stick)
{
	//	Get the button values
	XINPUT_GAMEPAD buttonValues = this->getState().Gamepad;

	float x, y;

	//	See what stick we want to test it on
	switch (stick)
	{
		case XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE:
		{
			x = buttonValues.sThumbLX;
			y = buttonValues.sThumbLY;

			break;
		}

		case XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE:
		{
			x = buttonValues.sThumbRX;
			y = buttonValues.sThumbRY;
			
			break;
		}

		default:
		{
			return 0;
		}
	}

	//	Magnitude the coordinates
	float magnitude = sqrt(x * x + y * y);
	float normalizedMagnitude = 0;

	//	Work out the normalized magnitudes
	if (magnitude > stick)
	{
		if (magnitude > 32767) 
		{
			magnitude = 32767;
		}

		magnitude -= stick;

		normalizedMagnitude = magnitude / (32767 - stick);
	}
	else
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	return normalizedMagnitude;
}