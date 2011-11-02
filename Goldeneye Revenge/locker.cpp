//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Locker.cpp - This class will be incharge of handling FPS
//	and millisecond locks within the game
//
//////////////////////////////////////////////////////////////

//	Include all the needed headers
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the default class constructor
//
//////////////////////////////////////////////////////////////
Locker::Locker()
{

}

bool Locker::lockMilliseconds(float milliseconds)
{
	float _currentTime = GetTickCount();

	if ((_currentTime - lastMilliseconds) > (float)milliseconds)
	{		
		lastMilliseconds = _currentTime;
		return true;
	}

	return false;
}

bool Locker::lockFPS(float fps)
{
	if (fps > 0)
	{		
		float _currentTime = GetTickCount() * 0.001f; 
		
		if ((_currentTime - lastFPS) > (1.0f / fps))
		{			
			lastFPS = _currentTime;	
			return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}