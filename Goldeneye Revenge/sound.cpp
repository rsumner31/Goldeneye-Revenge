//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Sound.cpp - This file handles all sound functions
//
//////////////////////////////////////////////////////////////

//	Include all the needed headers
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the class constructor
//
//////////////////////////////////////////////////////////////
Sound::Sound()
{
	soundId = 0;

	//	Sound positioning
	sourcePosition[0] = 0.0;
	sourcePosition[1] = 0.0;
	sourcePosition[2] = 0.0;

	sourceVelocity[0] = 0.0;
	sourceVelocity[1] = 0.0;
	sourceVelocity[2] = 0.0;

	listenerPosition[0] = 0.0;
	listenerPosition[1] = 0.0;
	listenerPosition[2] = 0.0;

	listenerVelocity[0] = 0.0;
	listenerVelocity[1] = 0.0;
	listenerVelocity[2] = 0.0;

	listenerOrientation[0] = 0.0;
	listenerOrientation[1] = 0.0;
	listenerOrientation[2] = -1.0;
	listenerOrientation[3] = 0.0;
	listenerOrientation[4] = 1.0;
	listenerOrientation[5] = 0.0;
}

Sound::~Sound()
{	
	
}

//////////////////////////////////////////////////////////////
//
//	This function returns the sound's generated id
//
//////////////////////////////////////////////////////////////
ALint Sound::getSoundId()
{	
	if (soundId == NULL)
	{
		return 0;
	}

	return soundId;
}


//////////////////////////////////////////////////////////////
//
//	This function loads the sound and binds it to a buffer ID
//
//////////////////////////////////////////////////////////////
void Sound::loadSound(char* fileName, bool loopSound)
{
	struct stat fileInfo;
	
	if (stat(fileName, &fileInfo) != 0)
	{
		string errorStr = "Could not load file ";
		errorStr += fileName;
		error((char *)errorStr.c_str());
	}

	loopSound = loopSound == true ? AL_TRUE : AL_FALSE;

	alGenBuffers(1, &soundId);

	if(alGetError() != AL_NO_ERROR)
	{
		error("openAL error");
	}

	alutLoadWAVFile(fileName, &format, &data, &size, &freq, &loop);
	alBufferData(soundId, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	//	Bind the buffer with the source.
	alGenSources(1, &soundSource);

	if(alGetError() != AL_NO_ERROR)
	{
		error("openAL error");
	}

	alSourcei(soundSource, AL_BUFFER, soundId);
	alSourcef(soundSource, AL_PITCH, 1.0);	
	alSourcef(soundSource, AL_GAIN, soundVolume);
	alSourcefv(soundSource, AL_POSITION, sourcePosition);
	alSourcefv(soundSource, AL_VELOCITY, sourceVelocity);
	alSourcei(soundSource, AL_LOOPING, loopSound);

	alListenerfv(AL_POSITION, listenerPosition);
	alListenerfv(AL_VELOCITY, listenerVelocity);
	alListenerfv(AL_ORIENTATION, listenerOrientation);
}

//////////////////////////////////////////////////////////////
//
//	This function pauses the sound
//
//////////////////////////////////////////////////////////////
void Sound::pauseSound()
{
	alSourcePause(soundSource);
}

//////////////////////////////////////////////////////////////
//
//	This function updates the volume of the sound
//
//////////////////////////////////////////////////////////////
void Sound::updateVolume(float level)
{
	alSourcef(soundSource, AL_GAIN, level);
}

//////////////////////////////////////////////////////////////
//
//	This function plays the sound
//
//////////////////////////////////////////////////////////////
void Sound::playSound()
{
	alSourcePlay(soundSource);
}

//////////////////////////////////////////////////////////////
//
//	This function sets the listener position
//
//////////////////////////////////////////////////////////////
void Sound::setListenerPosition(float x, float y, float z)
{
	listenerPosition[0] = x;
	listenerPosition[1] = y;
	listenerPosition[2] = x;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the listener orientation
//
//////////////////////////////////////////////////////////////
void Sound::setListenerOrientation(float fx, float fy, float fz, float ux, float uy, float uz)
{
	listenerOrientation[0] = fx;
	listenerOrientation[1] = fy;
	listenerOrientation[2] = fz;
	listenerOrientation[3] = ux;
	listenerOrientation[4] = uy;
	listenerOrientation[5] = uz;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the listener velocity
//
//////////////////////////////////////////////////////////////
void Sound::setListenerVelocity(float x, float y, float z)
{
	listenerVelocity[0] = x;
	listenerVelocity[1] = y;
	listenerVelocity[2] = x;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the source position
//
//////////////////////////////////////////////////////////////
void Sound::setSourcePosition(float x, float y, float z)
{
	sourcePosition[0] = x;
	sourcePosition[1] = y;
	sourcePosition[2] = x;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the source velocity
//
//////////////////////////////////////////////////////////////
void Sound::setSourceVelocity(float x, float y, float z)
{
	sourceVelocity[0] = x;
	sourceVelocity[1] = y;
	sourceVelocity[2] = x;
}

//////////////////////////////////////////////////////////////
//
//	This function stops the sound
//
//////////////////////////////////////////////////////////////
void Sound::stopSound()
{
	alSourceStop(soundSource);
}