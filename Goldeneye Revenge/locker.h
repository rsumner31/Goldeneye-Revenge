//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	locker.h - This is the class header file for the locker
//	utility class
//	
//////////////////////////////////////////////////////////////

class Locker
{
	public:
		Locker::Locker();
		bool Locker::lockMilliseconds(float milliseconds);
		bool Locker::lockFPS(float fps);

	private:		
		float lastMilliseconds;	
		float lastFPS;
};
