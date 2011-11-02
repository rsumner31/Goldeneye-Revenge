//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	camera.cpp - This file will handle all of the camera 
//	operations and main views
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

Waypoint wayPoints[4096];
int _count = 1;

//////////////////////////////////////////////////////////////
//
//	This function is the class constructor, it will initialize
//	all of the appropriate variables
//
//////////////////////////////////////////////////////////////
Camera::Camera()
{
	Vector3 _cameraPosition = Vector3(0.0, 0.0, 0.0);
	Vector3 _cameraView = Vector3(0.0, 1.0, 0.5);
	Vector3 _cameraUpVector = Vector3(0.0, 0.0, 1.0);
	Vector4 _rotationVector = Vector4(0, 0, 0, 0);
	//wayPoints = new list<Waypoint>;

	cameraView = _cameraView;
	cameraPosition = _cameraPosition;
	cameraUpVector = _cameraUpVector;
	rotationVector = _rotationVector;

	originalPos = cameraPosition;

	invertYAxis = false;
	moveByMouse = true;
}

//////////////////////////////////////////////////////////////
//
//	This is the class destructor
//
//////////////////////////////////////////////////////////////
Camera::~Camera()
{

}

//////////////////////////////////////////////////////////////
//
//	This function will push a way point onto the list stack
//
//////////////////////////////////////////////////////////////
void Camera::addWaypoint(Waypoint wPoint)
{
	//	The first pos of the camera will always be the current
	wayPoints[0].position = cameraPosition;	

	//	We need to calculate indevidual steps between the last
	//	pos and the new pos, using the base of FPS_LOCK
	Vector3 newPos = ((wPoint.position - wayPoints[_count - 1].position) / FPS_LOCK);

	for (int i = 0; i < FPS_LOCK; i++)
	{
		wayPoints[_count++].position = wayPoints[_count - 1].position + newPos;
	}

	return;
}

//////////////////////////////////////////////////////////////
//
//	This function will calculate the perpendicular vector
//	from 2 given vectors by taking the cross product
//
//////////////////////////////////////////////////////////////
Vector3 Camera::cross(Vector3 vector1, Vector3 vector2)
{
	Vector3 vector;
	vector.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
	vector.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
	vector.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

	return vector;
}

//////////////////////////////////////////////////////////////
//
//	This function will render the camera
//
//////////////////////////////////////////////////////////////
void Camera::drawCamera()
{
	gluLookAt
	(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraView.x, cameraView.y, cameraView.z,
		cameraUpVector.x, cameraUpVector.y, cameraUpVector.z
	);	
}

//////////////////////////////////////////////////////////////
//
//	This function returns the camera view
//
//////////////////////////////////////////////////////////////
Vector3 Camera::getCameraView()
{
	return (cameraView - cameraPosition);		
}

//////////////////////////////////////////////////////////////
//
//	This function returns the camera's rotation
//
//////////////////////////////////////////////////////////////
Vector4 Camera::getRotation()
{
	return rotationVector;
}

//////////////////////////////////////////////////////////////
//
//	This function the magnitudes a vector
//
//////////////////////////////////////////////////////////////
float Camera::magnitude(Vector3 vector)
{
	return (float) sqrt((vector.x * vector.x) +
						(vector.y * vector.y) +
						(vector.z * vector.z));
}

//////////////////////////////////////////////////////////////
//
//	This function moves the camera forward or backward depending
//	on the speed
//
//////////////////////////////////////////////////////////////
void Camera::moveCameraX(float speed)
{
	Vector3 view = getCameraView();

	cameraPosition.x += view.x * speed;
	cameraPosition.z += view.z * speed;

	cameraView.x += view.x * speed;
	cameraView.z += view.z * speed;
}

void Camera::moveCameraY(float speed)
{	
	//	We need to find out what plain the camera is on
	//	by finding the perpendicular cross of the direction
	//	it is facing
	Vector3 axis = getAxis();

	cameraPosition.x += (axis.x * speed);
	cameraPosition.z += (axis.z * speed);
	cameraView.x += (axis.x * speed);
	cameraView.z += (axis.z * speed);
}

int frameCount = 0;
//////////////////////////////////////////////////////////////
//
//	This function will move the camera through the way point
//	stack
//
//////////////////////////////////////////////////////////////
void Camera::moveThroughWaypoints(bool loop)
{
	moveByMouse = false;

	//	We have reached the end of the array
	if (frameCount >= _count && !loop)
	{
		return;
	}
	else if (frameCount >= _count && loop)
	{
		frameCount = 0;
	}

	cameraPosition = wayPoints[frameCount++].position;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the camera's axis
//
//////////////////////////////////////////////////////////////
Vector3 Camera::getAxis()
{
	Vector3 axis;
	axis = cross(getCameraView(), cameraUpVector);
	axis = normalize(axis);

	return axis;
}

//////////////////////////////////////////////////////////////
//
//	This function will tell the camera to be handled by the mouse
//
//////////////////////////////////////////////////////////////
void Camera::moveCameraByMouse()
{
	float angleX = 0;
	float angleY = 0;
	float currentRotationX = 0;
	float lastRotationX = 0;
	float modifier = 0;
	Vector3 axis = getAxis();

	if (moveByMouse && GAME_STATE != GAME_PAUSED)
	{
		if (joystick->isUsable())
		{
			static Vector2 pos;
			Vector2 newPos = joystick->getCoordsFromRStick();
			newPos = (newPos * joystick->sensitivity) * 5.0;
		
			float mag = joystick->getStickMagnitude(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

			if ((pos.x == newPos.x && pos.y == newPos.y) || mag == 0)
			{
				return;
			}

			//	Distance from the new post and last pos
			angleX = (newPos.x - pos.x) / 100.0f;
			angleY = (newPos.y - pos.y) / 100.0f;
			
			angleY *= (invertYAxis ? -1 : 1);

			lastRotationX = currentRotationX;
			currentRotationX += mag;		
		}
		else
		{		
			POINT mousePoint;
			GetCursorPos(&mousePoint);

			if (mousePoint.x == middle.x && mousePoint.y == middle.y)
			{
				return;
			}
			
			//	Invert the mouse or not
			SetCursorPos(middle.x, middle.y);
			angleX = (middle.x - mousePoint.x) / 1000.0f;
			angleY = (middle.y - mousePoint.y) / 1000.0f;

			angleY *= (invertYAxis ? -1 : 1);
			lastRotationX = currentRotationX;
			currentRotationX += angleY;									
		}

		if (currentRotationX > 1.0f)
		{
			currentRotationX = 1.0f;

			if (lastRotationX != 1.0f)
			{				
				modifier = 1.0f - lastRotationX;
			}
		}
		else if (currentRotationX < -1.0f)
		{
			currentRotationX = -1.0f;

			if (lastRotationX != -1.0f)
			{
				modifier = -1 - lastRotationX;
			}
		}
		else
		{
			modifier = angleY;
		}

		rotateView(angleX, 0, 1, 0);
		rotateView(modifier, axis.x, axis.y, axis.z);	
	}
}

//////////////////////////////////////////////////////////////
//
//	This function returns a normalized vector length of 1
//
//////////////////////////////////////////////////////////////
Vector3 Camera::normalize(Vector3 vector)
{
	float mag = magnitude(vector);
	vector = vector / mag;

	return vector;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the position of the camera
//
//////////////////////////////////////////////////////////////
void Camera::setPosition(float x, float y, float z)
{		
	cameraPosition.x += x;
	cameraPosition.z += z;

	cameraView.x += x;
	cameraView.z += z;
}

void Camera::setPosition(Vector3 vector)
{		
	cameraPosition.x += vector.x;
	cameraPosition.z += vector.z;

	cameraView.x += vector.x;
	cameraView.z += vector.z;
}

//////////////////////////////////////////////////////////////
//
//	This function will control where the camera is positioned
//
//////////////////////////////////////////////////////////////
void Camera::positionCamera(float px, float py, float pz, float vx, float vy, float vz, float ux, float uy, float uz)
{
	Vector3 position = Vector3(px, py, pz);
	Vector3 view = Vector3(vx, vy, vz);
	Vector3 upVector = Vector3(ux, uy, uz);

	cameraPosition = position;
	cameraView = view;
	cameraUpVector = upVector;
}

//////////////////////////////////////////////////////////////
//
//	This function rotates the view based on a vector and angle
//
//////////////////////////////////////////////////////////////
void Camera::rotateView(float angle, Vector3 vector)
{
	rotateView(angle, vector.x, vector.y, vector.z);
}

void Camera::rotateView(float angle, float x, float y, float z)
{
	Vector3 view = getCameraView();
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	Vector3 newView;
	//	This will calculate the x value for the new view
	newView.x  = (cosTheta + (1 - cosTheta) * x * x) * view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta) * view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta) * view.z;
	
	//	This will calculate the y value for the new view
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta) * view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y) * view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta) * view.z;
	
	//	This will calculate the z value for the new view
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta) * view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta) * view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z) * view.z;	

	//	Stop the player from going backwards when moving
	//	absolute up or absolute down	
	if (newView.y >= 5 || newView.y <= -5)
	{
		return;
	}

	cameraView = cameraPosition + newView;
	Vector3 difference = originalPos - newView;

	//	In order to get the rotation matrix, we need to have the original matrix
	//	and the new rotated matrix, and find the difference, then we need to calculate
	//	the angle from the difference.
	float newangle = cos(-1.0) * (((view.x * newView.x) + (view.y + newView.y) + (view.z + newView.z)) / magnitude(getCameraView()));
	rotationVector = Vector4(difference.x, difference.y, difference.z, newangle);
}