//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	debug.cpp - This file will handle output to console functions
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This function outputs string data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, string data)
{
	cout << "[" + tag + "] " + data;
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs char pointer data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, char *data)
{
	cout << "[" + tag + "] " + data;
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs vector2 data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, Vector2 data)
{
	cout << "[" + tag + "] ";
	cout << "X: " + toString(data.x);
	cout << ", Y: " + toString(data.y);
	cout << "\n";
}

void debug(string tag, Vector2 *data)
{
	cout << "[" + tag + "] ";
	cout << "X: " + toString(data->x);
	cout << ", Y: " + toString(data->y);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs vector3 data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, Vector3 data)
{
	cout << "[" + tag + "] ";
	cout << "X: " + toString(data.x);
	cout << ", Y: " + toString(data.y);
	cout << ", Z: " + toString(data.z);
	cout << "\n";
}

void debug(string tag, Vector3 *data)
{
	cout << "[" + tag + "] ";
	cout << "X: " + toString(data->x);
	cout << ", Y: " + toString(data->y);
	cout << ", Z: " + toString(data->z);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs vector4 data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, Vector4 data)
{
	cout << "[" + tag + "] ";
	cout << "X: " + toString(data.x);
	cout << ", Y: " + toString(data.y);
	cout << ", Z: " + toString(data.z);
	cout << ", Angle: " + toString(data.z);
	cout << "\n";
}

void debug(string tag, Vector4 *data)
{
	cout << "[" + tag + "] ";
	cout << "X: " + toString(data->x);
	cout << ", Y: " + toString(data->y);
	cout << ", Z: " + toString(data->z);
	cout << ", Angle: " + toString(data->z);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs int data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, int data)
{
	cout << "[" + tag + "] ";	
	cout << toString(data);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs float data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, float data)
{
	cout << "[" + tag + "] ";	
	cout << toString(data);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs size data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, Size data)
{
	cout << "[" + tag + "] ";	
	cout << "Width: " + toString(data.width);
	cout << ", Height: " + toString(data.height);
	cout << ", Depth: " + toString(data.depth);
	cout << "\n";
}

void debug(string tag, Size *data)
{
	cout << "[" + tag + "] ";	
	cout << "Width: " + toString(data->width);
	cout << ", Height: " + toString(data->height);
	cout << ", Depth: " + toString(data->depth);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs settinge data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, Setting data)
{
	cout << "[" + tag + "] ";	
	cout << "Group: " + data.groupName;
	cout << ", Name: " + data.data;
	cout << ", Data: " + data.name;
	cout << "\n";
}

void debug(string tag, Setting *data)
{
	cout << "[" + tag + "] ";	
	cout << "Group: " + data->groupName;
	cout << ", Name: " + data->data;
	cout << ", Data: " + data->name;
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs waypoint data to the console
//
//////////////////////////////////////////////////////////////
void debug(string tag, Waypoint data)
{
	cout << "[" + tag + "] ";	
	debug("Position", data.position);
	debug(", Up", data.up);
	debug(", View", data.view);
	debug(", Speed", data.speed);
	cout << "\n";
}

void debug(string tag, Waypoint *data)
{
	cout << "[" + tag + "] ";	
	debug("Position", data->position);
	debug(", Up", data->up);
	debug(", View", data->view);
	debug(", Speed", data->speed);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs joystick data
//
//////////////////////////////////////////////////////////////
void debug(string tag, XINPUT_GAMEPAD data)
{
	cout << "[" + tag + "] ";	
	cout << "Left trigger: " + data.bLeftTrigger;
	cout << ", Right trigger: " + data.bRightTrigger;
	cout << ", sThumbLX: " + data.sThumbLX;
	cout << ", sThumbLY: " + data.sThumbLY;
	cout << ", sThumbRX: " + data.sThumbRX;
	cout << ", sThumbRY: " + data.sThumbRY;
	cout << ", wButtons: " + data.wButtons;
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs player info data
//
//////////////////////////////////////////////////////////////
void debug(string tag, PlayerInfo data)
{
	cout << "[" + tag + "] ";	
	cout << "Health: " + toString(data.health);
	cout << ", Armour: " + toString(data.armour);
	cout << "\n";
}

//////////////////////////////////////////////////////////////
//
//	This function outputs player info data
//
//////////////////////////////////////////////////////////////
void debug(string tag, Matrix4 data)
{
	cout << "[" + tag + "] ";	
	cout << "\n";
	cout << "[ " + toString((float)data.mat[0][0]) + ", " + toString((float)data.mat[0][1]) + ", " + toString((float)data.mat[0][2])  + ", " + toString((float)data.mat[0][3]);
	cout << "\n";
	cout << "[ " + toString((float)data.mat[1][0]) + ", " + toString((float)data.mat[1][1]) + ", " + toString((float)data.mat[1][2])  + ", " + toString((float)data.mat[1][3]);
	cout << "\n";
	cout << "[ " + toString((float)data.mat[2][0]) + ", " + toString((float)data.mat[2][1]) + ", " + toString((float)data.mat[2][2])  + ", " + toString((float)data.mat[2][3]);
	cout << "\n";
	cout << "[ " + toString((float)data.mat[3][0]) + ", " + toString((float)data.mat[3][1]) + ", " + toString((float)data.mat[3][2])  + ", " + toString((float)data.mat[3][3]);
	cout << "\n";
}