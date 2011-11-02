//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	sound.h - This is the header file for the sound class
//	
//////////////////////////////////////////////////////////////

class Sound
{
	public:		
		Sound::Sound();
		Sound::~Sound();		
		void Sound::loadSound(char* fileName, bool loopSound = false);
		ALint Sound::getSoundId();
		void Sound::updateVolume(float level);
		void Sound::pauseSound();
		void Sound::playSound();
		void Sound::setListenerPosition(float x, float y, float z);		
		void Sound::setListenerOrientation(float fx, float fy, float fz, float ux, float uy, float uz);
		void Sound::setListenerVelocity(float x, float y, float z);
		void Sound::setSourcePosition(float x, float y, float z);
		void Sound::setSourceVelocity(float x, float y, float z);		
		void Sound::stopSound();
		
		ALuint soundId;
		ALuint soundSource;				

	private:		
		//	Sound variables				
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;

		//	Sound positioning
		ALfloat sourcePosition[3];
		ALfloat sourceVelocity[3];
		ALfloat listenerPosition[3];
		ALfloat listenerVelocity[3];
		ALfloat listenerOrientation[6];
};
