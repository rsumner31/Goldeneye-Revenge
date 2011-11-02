//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Main.cpp - Main app entry file for the program, will contain
//	All function setups and window handling
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

Object *menuBackground, *loading;
Settings *settings;

//	Fonts
FontFace *courierNew, *courierNew32;

//	This is where we actually initialize the extern variables
Camera CAMERA;
Player *PLAYER;
HINSTANCE g_hInstance;
HWND g_hWnd;
HDC g_hDC;
HGLRC g_hRC;
RECT g_windowRect;

//	This is the speed you move every second
float SPEED = 2; 
//	This is the speed you move every frame = speed / FPS
float MOVE_SPEED = .005f;
//	We have to lock the FPS at 60 otherwise higher framerates, 
//	cause the game to work alot faster I.E camera move speed
float FPS_LOCK = 0;
int FPS = 0;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int SCREEN_BPP;
float soundVolume = 0.5f;
//	Create a joystick instance
Joystick *joystick;

int GAME_MODE = IN_MENU;
int GAME_STATE = GAME_LOADING;
int MENU_ID = MENU1;
bool FULL_SCREEN = true;
bool IS_RUNNING = true;
int currentTime;
Vector2 middle;
Size screenRatio;
Size maxScreenRes;
Size maxGameRes;
char keys[32];
//	Use the thumb pad to move with the joystick?
bool useThumbPad;
int selectedMovementMode;	// 0 for WASD, 4 for Arrow Keys

//	Initialize the AI and model reference list
list<AI*> *__aiList;
list<Model*> *__modelList;

//	Here we initialize the temporary pointer functions for the 
//	openGL extention library
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0;
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;
PFNGLACTIVETEXTUREARBPROC pglActiveTextureARB = 0;
PFNGLGENERATEMIPMAPPROC pglGenerateMipmap = 0;

//////////////////////////////////////////////////////////////
//
//	This function displays an error popup, then closes the 
//	game.
//
//////////////////////////////////////////////////////////////
void error(char *message)
{
	MessageBox(g_hWnd, message, "Fatal Error", MB_ICONERROR | MB_OK);
	PostQuitMessage(0);
	exit(0);

	return;
}

//////////////////////////////////////////////////////////////
//
//	This function initializes the window and calls
//	The initializeGL function
//
//////////////////////////////////////////////////////////////
bool initialize(HWND hWnd)
{	
	__aiList = new list<AI*>;
	__modelList = new list<Model*>;		

	//	Set the key system to WASD
	keys[0] = 87;	//	W
	keys[1] = 65;	//	A
	keys[2] = 83;	//	S
	keys[3] = 68;	//	D
	keys[4] = 38;	//	Up
	keys[5] = 37;	//	Left
	keys[6] = 40;	//	Down
	keys[7] = 39;	//	Right

	useThumbPad = true;
	selectedMovementMode = 0;

	//	Set the global HWND and get the client rectangle
	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_windowRect);

	//	Set the middle points of the screen
	if (FULL_SCREEN)
	{
		middle.x = SCREEN_WIDTH >> 1;
		middle.y = SCREEN_HEIGHT >> 1;
	}
	else
	{
		RECT screenRect;
		GetWindowRect(g_hWnd, &screenRect);

		middle.x = (SCREEN_WIDTH - screenRect.left) >> 1;
		middle.y = (SCREEN_HEIGHT - screenRect.top) >> 1;
	}

	//	Initialize the joystick
	joystick = new Joystick();

	//	Get the values from the settings
	soundVolume = atof(settings->getSetting("sound", "volume").data.c_str());
	bool useJoystick = toBool(settings->getSetting("joystick", "usejoystick").data);
	joystick->sensitivity = atof(settings->getSetting("joystick", "sensitivity").data.c_str());	
	useThumbPad = toInt(settings->getSetting("joystick", "keymode").data) == 0 ? true : false;
	selectedMovementMode = toInt(settings->getSetting("keyboard", "keymode").data) == 0 ? 0 : 4;
	CAMERA.invertYAxis = toBool(settings->getSetting("control", "inverty").data);

	if (useJoystick)
	{
		joystick->connect();
	}
	else
	{
		joystick->disconnect();
	}
	
	//	Initialize the graphics and audio libraries
	initializeAL();		
	//	Load the textures and init the ARB functions
	initializeGL(g_windowRect.right, g_windowRect.bottom);	

	//	Load the fonts
	courierNew = new FontFace();
	courierNew->loadFont("graphics/textures/fonts/courier_new_16.tga", 32, 36);
	courierNew->genFont();

	courierNew32 = new FontFace();
	courierNew32->loadFont("graphics/textures/fonts/courier_new_32.tga", 32, 36);
	courierNew32->genFont();

	//	Load the loading textures and objects
	menuBackground = new Object();		
	menuBackground->loadTexture("graphics/textures/background.tga");
	menuBackground->size(SCREEN_WIDTH, SCREEN_HEIGHT);
	menuBackground->genTexture(GL_REPEAT);

	//	Start the loading of the textures and sounds, show the
	//	loading screen	
	HANDLE threadHandle = 0;
	threadHandle = CreateThread(NULL, 0, loadMenu, NULL, 0, NULL);
	
	//	Show the loading screen
	while (GAME_STATE == GAME_LOADING)
	{
		renderScreen();
	}

	//	The thread has finished so lets close it and finish
	//	loading the game
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);

	//	Now all the textures have been coppied to RAM, we need
	//	to generate their ID's so we can actually use them
	generateMenuTextures();		
	ShowCursor(false);	

	return true;
}

void deInitialize()
{
	if (g_hRC)											
	{
		// This frees our rendering memory and sets everything back to normal
		wglMakeCurrent(NULL, NULL);						

		// Delete our OpenGL Rendering Context	
		wglDeleteContext(g_hRC);						
	}
	
	if (g_hDC) 
	{
		// Release our HDC from memory
		ReleaseDC(g_hWnd, g_hDC);						
	}
		
	// Free the window class
	UnregisterClass(CLASS_NAME, g_hInstance);		

	PostQuitMessage(0);	
	exit(0);
}

//////////////////////////////////////////////////////////////
//
//	This function is a thread which will get called to load
//	the game whilst we display a loading screen
//
//////////////////////////////////////////////////////////////
bool gameHasBeenLoaded = false;
DWORD WINAPI loadGame(LPVOID lParam)
{		
	loadGameSounds();
	loadGameTextures();

	gameHasBeenLoaded = true;
	GAME_STATE = GAME_RUNNING;
	return 0;
}

//////////////////////////////////////////////////////////////
//
//	This function is a thread which will get called to load
//	the game whilst we display a loading screen
//
//////////////////////////////////////////////////////////////
DWORD WINAPI loadMenu(LPVOID lParam)
{	
	loadMenuSounds();	
	loadMenuTextures();

	GAME_STATE = GAME_RUNNING;
	return 0;
}

//////////////////////////////////////////////////////////////
//
//	This function is the main loop where we will check for any
//	User input or mouse moves
//
//////////////////////////////////////////////////////////////
WPARAM mainLoop()
{
	MSG msg;	
	Locker joystickLocker, movementLocker, gameLocker;
	
	//	While the game is running, exectue all these functions
	while (IS_RUNNING)
	{	
		//	Check if the joystick is connected
		if (joystick->isUsable())
		{	
			//	Check for joystick input every 10ms
			//if (lockMilliseconds(10))
			{
				checkJoystickInput();
			}

			//	Check for if the joystick is firing
			if (GAME_STATE != GAME_PAUSED && GAME_MODE == IN_GAME)
			{
				static bool pressed;

				//	If the right trigger is all the way down
				if (joystick->getState().Gamepad.bRightTrigger > 200)
				{
					if (pressed != true)
					{
						PLAYER->fireWeapon(0);
						pressed = true;
					}			
				}
				else if (joystick->getState().Gamepad.bRightTrigger == 0)
				{
					//	If the trigger is up and it was just down
					if (pressed == true)
					{
						PLAYER->fireWeapon(3);
						pressed = false;
					}
				}
			}
			else if (GAME_MODE == IN_MENU || GAME_STATE == GAME_PAUSED)
			{
				//	We have to update the coords for the joystick as if we 
				//	are using a mouse
				float mag = joystick->getStickMagnitude(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);		

				if (mag > 0 && joystickLocker.lockMilliseconds(0.1f))
				{
					//	Get the coordinates
					Vector2 joystickPos = Vector2
					(
						(joystick->getStickMagnitude(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) * joystick->getCoordsFromLStick().x, 
						(joystick->getStickMagnitude(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) * joystick->getCoordsFromLStick().y
					);

					Vector2 joystickCoords = joystick->getCoodinates();

					//	Make sure the joystick coords do not go off screen
					if (joystickCoords.x < 0)
					{
						joystickPos.x = 0;
						joystickCoords.x = 0;
					}
					else if (joystickCoords.x > SCREEN_WIDTH)
					{
						joystickPos.x = 0;
						joystickCoords.x = SCREEN_WIDTH;
					}

					if (joystickCoords.y < 0)
					{
						joystickPos.y = 0;
						joystickCoords.y = 0;
					}
					else if (joystickCoords.y > SCREEN_HEIGHT)
					{
						joystickPos.y = 0;
						joystickCoords.y = SCREEN_HEIGHT;
					}

					joystickPos = joystickPos * (joystick->sensitivity * screenRatio.width) * 10.0;
					joystick->updateCoordinates(joystickPos.x + joystickCoords.x, joystickCoords.y - joystickPos.y);				
				}				

				//	We also need to detect a right trigger click emulating
				//	a mouse click
				XINPUT_STATE state = joystick->getState();
				static bool _menuTriggerPress;

				//	Check for trigger press or A button press for click
				if (state.Gamepad.bRightTrigger > 200 || state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
				{
					if (_menuTriggerPress != true)
					{
						checkMouse(0, joystick->getCoodinates().x, joystick->getCoodinates().y);
						_menuTriggerPress = true;
					}
				}
				else if (state.Gamepad.bRightTrigger <= 0 || !state.Gamepad.wButtons)
				{
					if (_menuTriggerPress == true)
					{
						checkMouse(3, joystick->getCoodinates().x, joystick->getCoodinates().y);
						_menuTriggerPress = false;
					}
				}
			}
		}

		//	This detects if the current window the user is using
		//	is the game. If it isnt, then we stop the camera from
		//	controlling the mouse otherwise the user cant use it
		//	when they are not in the game.
		if (GetForegroundWindow() != g_hWnd)
		{			
			//GAME_STATE = GAME_PAUSED;
			CAMERA.moveByMouse = false;
		}
		else
		{		
			if (GAME_MODE != IN_MENU)
			{				
				CAMERA.moveByMouse = true;
			}
		}		

		//	If a message is sent, handle that
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)
			{
				IS_RUNNING = false;
				PostQuitMessage(0);

				break;
			}

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		else if (gameLocker.lockFPS(FPS_LOCK))
		{						
			//	Check for movement every 10ms
			if (movementLocker.lockMilliseconds(10.0f) && GAME_STATE == IN_GAME)
			{
				checkMovement();	
				
				//	Move the camera based on mouse/joystick
				CAMERA.moveCameraByMouse();
			}

			//	Render the opengl stage
			renderScreen();
		}
	}
	
	return(0);	
}

//////////////////////////////////////////////////////////////
//
//	This function handles the stage resize everytime the window
//	changes size
//
//////////////////////////////////////////////////////////////
void resizeScene(int width, int height)
{
	//	If the width or height = 0, set as 1 to stop it from 
	//	dividing by zero
	height = height == 0 ? 1 : height;
	width = width == 0 ? 1 : width;

	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	//	Update the screen ratio
	screenRatio.width = (float)SCREEN_WIDTH / 800.0;
	screenRatio.height = (float)SCREEN_HEIGHT / 600.0;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//////////////////////////////////////////////////////////////
//
//	This function sets up the pixel formatting for openGL
//
//////////////////////////////////////////////////////////////
bool setupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	int pixelFormat;

	pixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDescriptor.nVersion = 1;

	//	Set the right openGL flags
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
	
	//	We want a standard mask
    pixelFormatDescriptor.dwLayerMask = PFD_MAIN_PLANE;
	
	//	We want an RGBA pixel type
    pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	
	//	We need to set our colour bits
    pixelFormatDescriptor.cColorBits = SCREEN_BPP;
    pixelFormatDescriptor.cDepthBits = SCREEN_BPP;
	
	//	No special bitplanes needed
    pixelFormatDescriptor.cAccumBits = 0;
	
	//	We do not need stencil bits
    pixelFormatDescriptor.cStencilBits = 0;

	if ((pixelFormat = ChoosePixelFormat(hdc, &pixelFormatDescriptor)) == false)
	{
		error("Could not initialize pixel format");
		return false;
	}

	if (SetPixelFormat(hdc, pixelFormat, &pixelFormatDescriptor) == false)
	{
		error("Could not set pixel format");
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////
//
//	This function sets up the window
//
//////////////////////////////////////////////////////////////
HWND setupWindow(HINSTANCE hInstance)
{
	//	Get the screen size from the config file
	Setting width = settings->getSetting("main", "width");	
	Setting height = settings->getSetting("main", "height");	
	maxScreenRes = Size((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN));
	maxGameRes = Size(1920.0, 1200.0);

	//	Make sure their max screen resolution is not larger
	//	than the max game resolution
	if (maxScreenRes.width >= maxGameRes.width && maxScreenRes.height >= maxGameRes.height)
	{
		maxScreenRes = maxGameRes;
	}

	//	Check of the width isset in settings
	if (width.data != "")
	{		
		SCREEN_WIDTH = toInt((char *)width.data.c_str());
		//	Make sure the requested width is not greater than the screen res
		SCREEN_WIDTH > maxScreenRes.width ? SCREEN_WIDTH = maxScreenRes.width : NULL;
	}
	else
	{
		SCREEN_WIDTH = maxScreenRes.width;
		settings->writeSetting("main", "width", (char *)toString(SCREEN_WIDTH).c_str());
	}

	//	Check of the height isset in settings
	if (height.data != "")
	{
		SCREEN_HEIGHT = toInt((char *)height.data.c_str());
		//	Make sure the requested height is not greater than the screen res
		SCREEN_HEIGHT > maxScreenRes.height ? SCREEN_HEIGHT = maxScreenRes.height : NULL;
	}
	else
	{
		SCREEN_HEIGHT = maxScreenRes.height;
		settings->writeSetting("main", "height", (char *)toString(SCREEN_HEIGHT).c_str());
	}

	//	Set the change in ratio for the textures to scale properly
	screenRatio.width = (float)SCREEN_WIDTH / 800.0;
	screenRatio.height = (float)SCREEN_HEIGHT / 600.0;

	HWND hWnd;
	DWORD dwStyle;	
	WNDCLASS windowClass;
	
	//	Create our window class
	memset(&windowClass, 0, sizeof(WNDCLASS));
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WinProc;
	windowClass.hInstance = g_hInstance;	
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW); 
	windowClass.hbrBackground = 0;
	windowClass.lpszClassName = CLASS_NAME;
	
	if (!RegisterClass(&windowClass))
	{
		error("Could not register class");
		return NULL;
	}

	//	Always start in full screen
	Setting fullscreen = settings->getSetting("main", "fullscreen");

	if (fullscreen.data == "")
	{
		settings->writeSetting("main", "fullscreen", "true");
	}

	//	If full screen is true in the config, toggle full screen
	bool fullScreen = toBool(fullscreen.data);
	if (fullScreen)
	{
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		toggleFullScreen(true);		
	}
	else
	{
		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_THICKFRAME;			
		FULL_SCREEN = false;
	}

	//	Create the window
	g_hInstance = hInstance;

	RECT windowRect;
	windowRect.left	= 0;
	windowRect.right = SCREEN_WIDTH;
	windowRect.top = 0;
	windowRect.bottom = SCREEN_HEIGHT;

	//	Adjust the window rectangle of the program
	AdjustWindowRect(&windowRect, dwStyle, false);
	hWnd = CreateWindow
	(
		CLASS_NAME, 
		APP_TITLE, 
		dwStyle, 
		0, 
		0, 
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, 
		NULL, 
		g_hInstance, 
		NULL
	);

	if (!hWnd)
	{
		error("Could not create window handler");
		return NULL;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	SetFocus(hWnd);	

	return hWnd;
}

//////////////////////////////////////////////////////////////
//
//	This function handles the toggle to full screen method
//
//////////////////////////////////////////////////////////////
HWND prevHwnd;
void toggleFullScreen(bool fullscreen)
{
	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = SCREEN_WIDTH;
	windowRect.top = 0;
	windowRect.bottom = SCREEN_HEIGHT;	

	//	Create a null instance
	DEVMODE dmSettings;
	memset(&dmSettings, 0, sizeof(DEVMODE));

	if (fullscreen == true)
	{		
		DWORD dwExStyle = 0;
		DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	

		if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmSettings))
		{
			error("Could not enum display settings");
			return;
		}
								
		SetWindowLongA(g_hWnd, -16, dwStyle);
		AdjustWindowRect(&windowRect, dwStyle, false);			
		ShowWindow(g_hWnd, SW_SHOW);
		SetForegroundWindow(g_hWnd);
		SetFocus(g_hWnd);

		dmSettings.dmPelsWidth = SCREEN_WIDTH;
		dmSettings.dmPelsHeight = SCREEN_HEIGHT;	
		dmSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;	
	
		int result = ChangeDisplaySettings(&dmSettings, CDS_FULLSCREEN);	
		if (result != DISP_CHANGE_SUCCESSFUL)
		{
			error("Could not toggle to fullscreen mode");
			return;
		}
		
		resizeScene(SCREEN_WIDTH, SCREEN_HEIGHT);	
		FULL_SCREEN = true;
	}
	else
	{				
		int result = ChangeDisplaySettings(0, 0);	
		if (result != DISP_CHANGE_SUCCESSFUL)
		{
			error("Could not toggle to windowed mode");
			return;
		}
		
		DWORD dwExStyle = WS_EX_CLIENTEDGE;
		DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;				
		SetWindowLongA(g_hWnd, -16, dwStyle);					
		AdjustWindowRect(&windowRect, dwStyle, false);
		
		ShowWindow(g_hWnd, SW_SHOW);		
		SetForegroundWindow(g_hWnd);
		SetFocus(g_hWnd);		

		resizeScene(SCREEN_WIDTH, SCREEN_HEIGHT);		
		FULL_SCREEN = false;
	}	

	SetWindowPos(g_hWnd, NULL, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL);

	//	Calculate the screen's middle point
	if (FULL_SCREEN)
	{
		middle.x = SCREEN_WIDTH >> 1;
		middle.y = SCREEN_HEIGHT >> 1;
	}
	else
	{
		RECT screenRect;
		GetWindowRect(g_hWnd, &screenRect);

		middle.x = (SCREEN_WIDTH - screenRect.left) >> 1;
		middle.y = (SCREEN_HEIGHT - screenRect.top) >> 1;
	}	
}

//////////////////////////////////////////////////////////////
//
//	This function resizes the window and opengl scene
//
//////////////////////////////////////////////////////////////
void resizeWindow(int width, int height)
{		
	toggleFullScreen(false);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	
	MoveWindow(g_hWnd, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, true);	

	toggleFullScreen(true);
	resizeScene(width, height);
	setMenuPositions();

	//	If the game has been loaded once, reset the positions
	if (gameHasBeenLoaded)
	{
		setGamePositions();
	}
}

//////////////////////////////////////////////////////////////
//
//	We need to initialize the entry point for the game, for 
//	This we will use WINAPI WinMain
//
//////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevHinstance, LPSTR arguments, int showCmd)
{	
	//	Get our settings
	settings = new Settings("settings.ini");

	g_hInstance = hinstance;		 
	currentTime = time(0);
	HWND hWnd;

	if ((hWnd = setupWindow(hinstance)) == NULL)
	{
		return 0;
	}
	else
	{
		initialize(hWnd);
		
		return mainLoop();
	}

	return 0;
}

//////////////////////////////////////////////////////////////
//
//	This is the function that will handle all user input and
//	Window messaging
//
//////////////////////////////////////////////////////////////
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG lResult = 0; 
    PAINTSTRUCT paintStruct;

    switch (uMsg)
	{ 
		case WM_ACTIVATE:
		{			
			break;
		}

		//	On resize
		case WM_SIZE:
		{			
			RECT windowRect;
			windowRect.left = 0;
			windowRect.right = SCREEN_WIDTH;
			windowRect.top = 0;
			windowRect.bottom = SCREEN_HEIGHT;
			AdjustWindowRectEx(&windowRect, NULL, false, NULL);	

			return 0; 
		}

		//	Window repaint
		case WM_PAINT:	
		{		
			BeginPaint(hWnd, &paintStruct);	
			EndPaint(hWnd, &paintStruct);

			break;
		}

		//	Keyup
		case WM_KEYUP:
		{
			switch (wParam)
			{
				case 80:
				case VK_ESCAPE:
				{									
					togglePause();

					break;
				}
			}

			break;
		}

		//	Keydown
		case WM_KEYDOWN:
		{
			switch(wParam) 
			{				
				case 70:
				{
					if (FULL_SCREEN)
					{
						toggleFullScreen(false);
					}
					else
					{
						toggleFullScreen(true);
					}		

					break;
				}

				default:
				{				
					checkInputs(wParam);				

					break;
				}
			}

			break;
		}

		//	When the user closes the program
		case WM_CLOSE:			
		{			
			deInitialize();			

			break; 
		}

		//	This checks for system commands such as
		//	minimize and maximize
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_MINIMIZE:
				{
					CAMERA.moveByMouse = false;
					break;
				}
			}
		}

		//	Left mouse button down
		case WM_LBUTTONDOWN:
		{		
			if (!joystick->isUsable())
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(g_hWnd, &mousePos);
				checkMouse(0, mousePos.x, mousePos.y);		
			}

			break;
		}

		//	Left button up
		case WM_LBUTTONUP:
		{
			if (!joystick->isUsable())
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(g_hWnd, &mousePos);
				checkMouse(3, mousePos.x, mousePos.y);		
			}

			break;
		}

		//	Right button down
		case WM_RBUTTONDOWN:
		{
			if (GAME_STATE != GAME_PAUSED)
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(g_hWnd, &mousePos);
				checkMouse(1, mousePos.x, mousePos.y);
			}

			break;
		}
		
		//	Right button up
		case WM_RBUTTONUP:
		{
			if (GAME_STATE != GAME_PAUSED)
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(g_hWnd, &mousePos);
				checkMouse(4, mousePos.x, mousePos.y);
			}
			break;
		}
		  
		//	Middle mouse button down
		case WM_MBUTTONDOWN:
		{
			if (GAME_STATE != GAME_PAUSED)
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(g_hWnd, &mousePos);
				checkMouse(2, mousePos.x, mousePos.y);
			}

			break;
		}

		//	Middle mouse button up
		case WM_MBUTTONUP:
		{
			if (GAME_STATE != GAME_PAUSED)
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(g_hWnd, &mousePos);
				checkMouse(5, mousePos.x, mousePos.y);
			}
			break;
		}

		//	Get the max min info for the window
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO *mmi = (MINMAXINFO *) lParam;
			mmi->ptMinTrackSize.x = mmi->ptMaxTrackSize.x = SCREEN_WIDTH; // Width
			mmi->ptMinTrackSize.y = mmi->ptMaxTrackSize.y = SCREEN_HEIGHT; // Height

			break;
		}

		default:	
		{		
			lResult = DefWindowProc(hWnd, uMsg, wParam, lParam); 		
			break; 
		}
    } 
 	
    return lResult;
}

//////////////////////////////////////////////////////////////
//
//	This function converts ints to strings
//
//////////////////////////////////////////////////////////////
std::string toString(int num)
{
	std::string message;
	std::stringstream stringStream;
	stringStream << num;
	message = stringStream.str();

	return message;
}

std::string toString(bool data)
{
	if (data)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

std::string toString(float num)
{
	char *message = new char[30];
	sprintf(message, "%.10g", num);

	return (std::string)message;
}

int toInt(char *str)
{
	return toInt(string(str));
}

int toInt(string str)
{
	std::stringstream stringStream(str);
	int num;

	if ((stringStream >> num).fail())
	{
		return 0;
	}

	return num;
}

bool toBool(string str)
{
	if (str.compare("true") == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function splits a string by another
//
//////////////////////////////////////////////////////////////
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
{	
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) 
	{
        elems.push_back(item);
    }

    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

//////////////////////////////////////////////////////////////
//
//	This function prints text out to the screen using openGL
//
//////////////////////////////////////////////////////////////
void glPrintText2i(int x, int y, std::string str, FontFace *font, int fontSpace)
{
	static TextString *text = new TextString();
	{
		text->setFontFace(font);	
		text->setCharacerSpacing(fontSpace);				
		text->position(Vector3(x, y, 0));	
	}

	text->setText(str);
	text->drawString();
	delete text;
}

//////////////////////////////////////////////////////////////
//
//	This function provides a random number in the range of 2
//	numbers
//
//////////////////////////////////////////////////////////////
int random(int lowest, int highest)
{
	srand(GetTickCount() + rand());
	
	int randomInteger;
	int range = (highest - lowest) + 1;
	
	for (int i = 0; i < 20; i++)
	{
		randomInteger = lowest + int(range * rand() / (RAND_MAX + 1.0));
	}

	return randomInteger;
}