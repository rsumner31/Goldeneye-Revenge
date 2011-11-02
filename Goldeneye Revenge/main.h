//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Main.h - Here we will include the right libraries and set
//	up any definitions or function prototypes
//
//////////////////////////////////////////////////////////////

//	We need this definition in order to use all the nice VBO
//	functions with GL
#define GL_GLEXT_PROTOTYPES

#pragma comment(lib, "opengl32.lib")	
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "alut.lib")
#pragma comment(lib, "lib3ds.lib")
#pragma comment(lib, "XInput.lib")

//#ifdef DEBUG
	//#pragma comment(linker, "/subsystem:console")
//#else
	//#pragma comment(linker, "/subsystem:windows")
	#pragma comment(linker, "/ENTRY:WinMain")
//#endif

#include <windows.h> 
#include <Xinput.h>
#include <winuser.h> 
#include <sys/stat.h> 
#include <sstream>
#include <time.h>
#include <stdio.h>   
#include <stdlib.h> 
#include <list>
#include <vector>
using namespace std;
#include <iostream>
#include <ostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <regex>
#include <lib3ds/file.h>
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>  
#include <AL\alut.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL\glut.h>
#include <GL\glext.h>
#include "locker.h"
#include "structs.h"
#include "joystick.h"
#include "sound.h"
#include "settings.h"
#include "game.h"
#include "camera.h"
#include "tga.h"
#include "texture.h"
#include "model.h"
#include "object.h"
#include "weapon.h"
#include "ai.h"
#include "map.h"
#include "player.h"
#include "font.h"

//////////////////////////////////////////////////////////////
//
//	Here are the prototypes for all the debug functions
//
//////////////////////////////////////////////////////////////
void debug(string tag, string data);
void debug(string tag, char *data);
void debug(string tag, Vector2 data);
void debug(string tag, Vector2 *data);
void debug(string tag, Vector3 data);
void debug(string tag, Vector3 *data);
void debug(string tag, Vector4 data);
void debug(string tag, Vector4 *data);
void debug(string tag, int data);
void debug(string tag, float data);
void debug(string tag, Size data);
void debug(string tag, Size *data);
void debug(string tag, Setting data);
void debug(string tag, Setting *data);
void debug(string tag, Waypoint data);
void debug(string tag, Waypoint *data);
void debug(string tag, XINPUT_GAMEPAD data);
void debug(string tag, PlayerInfo data);

//////////////////////////////////////////////////////////////
//
//	Here are the prototypes for all the array functions
//
//////////////////////////////////////////////////////////////
void listToSetting(list<Setting> *_setting, Setting dst[]);

//////////////////////////////////////////////////////////////
//
//	Here are my GL addition macros and definitions
//
//////////////////////////////////////////////////////////////

const float GL_COLOR_BLACKf[] = {0.0, 0.0, 0.0, 1.0};
const int GL_COLOR_BLACKi[] = {0, 0, 0, 255};

const float GL_COLOR_WHITEf[] = {1.0, 1.0, 1.0, 1.0};
const int GL_COLOR_WHITEi[] = {255, 255, 255, 255};

const float GL_COLOR_REDf[] = {1.0, 0, 0, 1.0};
const int GL_COLOR_REDi[] = {255, 0, 0, 255};

const float GL_COLOR_BLUEf[] = {0.0, 0.0, 1.0, 1.0};
const int GL_COLOR_BLUEi[] = {0, 0, 255, 255};

const float GL_COLOR_GREENf[] = {0.0, 1.0, 0.0, 1.0};
const int GL_COLOR_GREENi[] = {0, 255, 0, 255};

//	Fonts
extern FontFace *courierNew;
extern FontFace *courierNew32;

//////////////////////////////////////////////////////////////
//
//	Initialize all the needed constants, definitions and 
//	global variables for the game
//
//////////////////////////////////////////////////////////////

#define APP_TITLE "Goldeneye Revenge - 2011 by Callum Taylor"
#define CLASS_NAME "goldeneyeclass"

static int sNextId = 100; 
#define getNextId() ( ++sNextId )

//	Definition functions
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)
#define ARRAY_LENGTH(t) (sizeof(t) / sizeof *(t))

//	Here we register the extern variables and definitions
//	to use the open GL extention library
extern PFNGLGENBUFFERSARBPROC pglGenBuffersARB; 
extern PFNGLBINDBUFFERARBPROC pglBindBufferARB;
extern PFNGLBUFFERDATAARBPROC pglBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB;
extern PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB; 
extern PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB;
extern PFNGLMAPBUFFERARBPROC pglMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB;
extern PFNGLACTIVETEXTUREARBPROC pglActiveTextureARB;

#define glGenBuffersARB pglGenBuffersARB
#define glBindBufferARB pglBindBufferARB
#define glBufferDataARB pglBufferDataARB
#define glBufferSubDataARB pglBufferSubDataARB
#define glDeleteBuffersARB pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB pglMapBufferARB
#define glUnmapBufferARB pglUnmapBufferARB
#define glActiveTextureARB pglActiveTextureARB

//	Define the image status codes
#define IMG_OK 0x1
#define IMG_ERR_NO_FILE 0x2
#define IMG_ERR_MEM_FAIL 0x4
#define IMG_ERR_BAD_FORMAT 0x8
#define IMG_ERR_UNSUPPORTED 0x40

//	Define the model status codes
#define MODEL_OK 0x1
#define MODEL_ERR_NO_FILE 0x2
#define MODEL_ERR_BAD_FORMAT 0x4
#define MODEL_ERR_UNSUPPORTED 0x8

//	These are the extern definitions which means it can be called
//	From any file in the source, it also stops double definitions
//	because the header file has been included more than once IE.
//	a massive loopback.
extern Settings *settings;
extern Object *menuBackground;
extern Object *loading;
extern bool FULL_SCREEN;
extern bool IS_RUNNING;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_BPP;
extern float FPS_LOCK;
extern Camera CAMERA;
extern Player *PLAYER;
extern float MOVE_SPEED;
extern int FPS;
extern float soundVolume;
extern Size maxScreenRes;
extern Joystick *joystick;
extern char keys[32];
extern bool useThumbPad;
extern int selectedMovementMode;

//	Global variables
extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern HDC g_hDC;
extern HGLRC g_hRC;
extern RECT g_windowRect;
extern int currentTime;
extern Vector2 middle;
extern bool isFiring;
extern Size screenRatio;

//	Here we register the externs used to
//	register every generated ID for models
//	such as objects and AI so we can find them
//	when we click on them
extern list<AI*> *__aiList;
extern list<Model*> *__modelList;

//////////////////////////////////////////////////////////////
//
//	Initialize all of the functions a protocols
//
//////////////////////////////////////////////////////////////
void deInitialize();
void exitGame();
void togglePause();
void error(char *message);
WPARAM mainLoop();
bool initialize(HWND hWnd);
DWORD WINAPI loadGame(LPVOID lParam);
DWORD WINAPI loadMenu(LPVOID lParam);
void resizeScene(int width, int height);
bool setupPixelFormat(HDC hdc);
HWND setupWindow(HINSTANCE hinstance);
void resizeWindow(int width, int height);
void toggleFullScreen(bool fullscreen);
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR arguments, int showCmd);
LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

std::string toString(int num);
std::string toString(float num);
std::string toString(bool data);
int toInt(char *str);
int toInt(string str);
bool toBool(string str);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void glPrintText2i(int x, int y, std::string str, FontFace *font, int fontSpace = 16);
bool inList(string search, list<string> *listString);
int random(int lowest, int highest);