//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Camera.h - This file handles all the camera functions,
//	definitions and prototypes
//	
//////////////////////////////////////////////////////////////

class Camera
{
	public:	
		Vector3 cameraPosition;
		Vector3 cameraView; 
		Vector3 cameraUpVector;		
		Vector4 rotationVector;
		Vector3 originalPos;
		bool moveByMouse;
		bool invertYAxis;
		
		//	This is the class constructor and destructor
		Camera::Camera();
		Camera::~Camera();

		//	Here we will prototype all of the functions
				
		void Camera::addWaypoint(Waypoint wPoint);		
		Vector3 Camera::cross(Vector3 vector1, Vector3 vector2);
		void Camera::drawCamera();
		Vector3 Camera::getCameraView();
		Vector4 Camera::getRotation();
		float Camera::magnitude(Vector3 vector);
		void Camera::moveCameraX(float speed);
		void Camera::moveCameraY(float speed);
		void Camera::moveCameraByMouse();
		void Camera::moveThroughWaypoints(bool loop = false);
		Vector3 Camera::normalize(Vector3 vector);
		void Camera::positionCamera(float px, float py, float pz, float vx, float vy, float vz, float ux, float uy, float yz);
		void Camera::rotateView(float angle, Vector3 vector);
		void Camera::rotateView(Vector4 vector);
		void Camera::rotateView(float angle, float x, float y, float z);
		Vector3 Camera::getAxis();
		void Camera::setPosition(float x, float y, float z);
		void Camera::setPosition(Vector3 vector);

	private:
		//list<Waypoint> *wayPoints;
		//list<Waypoint>::iterator wayPointsIterator;
};