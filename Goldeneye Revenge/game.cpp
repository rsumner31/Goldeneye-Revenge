//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	game.cpp - This file will handle all the game rendering
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//	Initialize the FPS variables
int averageFPS = 0;
int fpsCount = 0;

//	Initialize the models vairables
Map *dam;
Object *skyBox;
Object *crosshair;
//	Text strings for the menu
//	TextString 0: Select Mission
//	TextString 1: Options
//	TextString 2: Exit Game
//	TextString 3: Screen Resolution
//	TextString 4: Game Volume
//	TextString 5: Use Joystick
//	TextString 6: Joystick Sensitivity
//	TextString 7: Keyboard Key Mode

//	TextString 9: Invert Y Axis
TextString *menuStrings[10];
//	Buttons for the menu
//	Button 0: back to main menu
//	Button 1: back to Mission screen
//	Button 2: Start Misson
//	Button 3: Resume Game
//	Button 4: Mission 1 button
Object *buttons[5];
//	Menu screens
//	Menu 0: Main Menu
//	Menu 1: Select Mission
//	Menu 2: Mission Brefing
//	Menu 3: Options
Object *menu[8];
//	Paused menu text
//	Paused Menu 0: Resume game
//	Paused Menu 1: Settings
//	Paused Menu 2: Exit
TextString *pausedMenu[8];
//	Initialize the sound variables
//	Sounds 0: Menu background
//	Sounds 1: Page turn
//	Sounds 2: Mission 1 background
Sound *sounds[3];
//	Create an array of available screen resolutions
Size screenRes[8];
//	Create the text string for the current screen resolution
TextString *currentScreenResolution;
//	Create the text string for the current game volume
TextString *currentGameVolume;
//	Create the text string for the use joystick toggler
TextString *useJoystick;
//	Create the text string for the joystick sensitvity
TextString *joystickSensitivity;
int currentScreenRes = 0;
//	Create the text string for the current key mode
TextString *currentKeyMode;
//	Create the text string for the inverted y axis status	
TextString *invertYAxis;

//	Initialize the lighting variables
GLfloat lightAmbient[] = {.5f, .5f, .5f, 1.0f};
GLfloat lightDiffuse[] = {.7f, .7f, .7f, 1.0f};
GLfloat lightSpecular[] = {1, 1, 1, 1};
float lightPos[] = {0, 0, 0, 1.0};

Size menuSize;
Size menuChange;

//////////////////////////////////////////////////////////////
//
//	This function will handle the mouse presses
//	button 0: left mouse down
//	button 1: right mouse down
//	button 2: scroll wheel down
//	button 3: left mouse up
//	button 4: right mouse up
//	button 5: scroll wheel up
//
//////////////////////////////////////////////////////////////
int fireTick = 0;
void checkMouse(int button, int x, int y)
{
	if (button == 3)
	{
		if (GAME_MODE == IN_MENU)
		{
			if (MENU_ID == MENU1)
			{
				//	We need to check each object for button presses
				//	Select Mission
				menuStrings[0]->checkMouse(x, y, button, selectMissionPress);
				//	Options
				menuStrings[1]->checkMouse(x, y, button, optionsButtonPress);
				//	Exit Game
				menuStrings[2]->checkMouse(x, y, button, exitGame);			
			}
			else if (MENU_ID == MENU2)
			{
				//	Back to main menu
				buttons[0]->checkMouse(x, y, button, mainMenu);
				//	Mission 1 button
				buttons[4]->checkMouse(x, y, button, missionBriefing);			
			}
			else if (MENU_ID == MENU3)
			{
				//	Back to Mission screen
				buttons[1]->checkMouse(x, y, button, missionSelection);
				//	Start Game
				buttons[2]->checkMouse(x, y, button, startGame);					
			}
			else if (MENU_ID == OPTIONS)
			{
				//	Back to Main Menu
				buttons[0]->checkMouse(x, y, button, mainMenu);		
				currentScreenResolution->checkMouse(x, y, button, changeScreenRes);
				currentGameVolume->checkMouse(x, y, button, changeGameVolume);
				useJoystick->checkMouse(x, y, button, toggleJoystick);
				joystickSensitivity->checkMouse(x, y, button, changeJoystickSensitivity);
				currentKeyMode->checkMouse(x, y, button, changeKeyMode);
				invertYAxis->checkMouse(x, y, button, changeInvert);
			}
			else if (MENU_ID == GAME_OPTIONS)
			{
				//	Resume Game
				buttons[3]->checkMouse(x, y, button, showPausedMenu);					
				currentScreenResolution->checkMouse(x, y, button, changeScreenRes);
				currentGameVolume->checkMouse(x, y, button, changeGameVolume);
				useJoystick->checkMouse(x, y, button, toggleJoystick);
				joystickSensitivity->checkMouse(x, y, button, changeJoystickSensitivity);
				currentKeyMode->checkMouse(x, y, button, changeKeyMode);
				invertYAxis->checkMouse(x, y, button, changeInvert);
			}
		}

		if (GAME_STATE == GAME_PAUSED)
		{
			//	Resume Game
			pausedMenu[0]->checkMouse(x, y, button, togglePause);
			//	Options
			pausedMenu[1]->checkMouse(x, y, button, showOptionsInGame);
			//	Exit Game
			pausedMenu[2]->checkMouse(x, y, button, exitGame);
		}
	}	

	if (GAME_MODE == IN_GAME && GAME_STATE != GAME_PAUSED)
	{			
		PLAYER->fireWeapon(button);			
	}
}

//////////////////////////////////////////////////////////////
//
//	This function closes the game
//
//////////////////////////////////////////////////////////////
void exitGame()
{
	deInitialize();
}

//////////////////////////////////////////////////////////////
//
//	This function toggles the pause and play state
//
//////////////////////////////////////////////////////////////
void togglePause()
{
	if (GAME_MODE != IN_GAME) return;

	if (GAME_STATE == GAME_PAUSED)
	{
		//	Reset the position of the mouse so the screen dosen't
		//	jump everywhere
		SetCursorPos(middle.x, middle.y);				
		GAME_STATE = GAME_RUNNING;				
	}
	else
	{
		GAME_STATE = GAME_PAUSED;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will handle the other key presses such as
//	menu, pause etc.
//
//////////////////////////////////////////////////////////////
void checkInputs(WPARAM wParam)
{
	switch(wParam)
	{		
		//	Key: R
		case 82:
		{			
			PLAYER->reloadWeapon();
			break;
		}

		//	Key: 1
		case 49:
		{			
			PLAYER->setCurrentWeapon(0);
			break;
		}		

		//	Key: 2
		case 50:
		{
			PLAYER->setCurrentWeapon(1);
			break;
		}

		//	Key: 3
		case 51:
		{
			PLAYER->setCurrentWeapon(2);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////
//
//	This function checks for the inputs made by the joystick
//
//////////////////////////////////////////////////////////////
void checkJoystickInput()
{	
	if (GAME_MODE != IN_GAME) return;

	static int button;	

	if (joystick->getState().Gamepad.wButtons == 0)
	{
		//	Check if the button is not 0
		if (button != 0)
		{
			if (button & XINPUT_GAMEPAD_START)
			{
				//	Toggle the pause menu
				togglePause();
			}
			else if (button & XINPUT_GAMEPAD_X)
			{
				//	Reload the weapon
				PLAYER->reloadWeapon();
			}
			else if (button & XINPUT_GAMEPAD_LEFT_SHOULDER)
			{
				//	Cycle through the weapons backwards
				PLAYER->previousWeapon();
			}
			else if (button & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
				//	Cycle through the weapons forwards
				PLAYER->nextWeapon();
			}

			button = 0;
		}
	}
	else
	{
		button = joystick->getState().Gamepad.wButtons;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function checks for the user movement (keys W, A, S, D)
//	and (UP, LEFT, DOWN, RIGHT)
//
//////////////////////////////////////////////////////////////
void checkMovement()
{
	if (GAME_MODE == IN_GAME)
	{
		//	Check if we are using a joystick or keyboard
		if (joystick->isUsable())
		{		
			if (useThumbPad)
			{
				//	Get the coordinates
				Vector2 joystickPos = Vector2
				(
					(joystick->getStickMagnitude(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * 2) * joystick->getCoordsFromLStick().x, 
					(joystick->getStickMagnitude(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) * 2) * joystick->getCoordsFromLStick().y
				);

				//	For some reason we have to swap the coordinates
				PLAYER->moveY(joystickPos.x * (MOVE_SPEED * 5));
				PLAYER->moveX(joystickPos.y * MOVE_SPEED);		
			}
			else
			{
				XINPUT_GAMEPAD movementState = joystick->getState().Gamepad;
				
				if (movementState.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					if (!PLAYER->collisionTest(dam, 'w'))
					{
						PLAYER->moveX(MOVE_SPEED);		
					}
				}

				if (movementState.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					if (!PLAYER->collisionTest(dam, 's'))
					{
						PLAYER->moveX(-MOVE_SPEED);	
					}
				}

				if (movementState.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
				{
					if (!PLAYER->collisionTest(dam, 'a'))
					{
						PLAYER->moveY(-MOVE_SPEED * 5);		
					}
				}

				if (movementState.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
				{
					if (!PLAYER->collisionTest(dam, 'd'))
					{
						PLAYER->moveY(MOVE_SPEED * 5);		
					}
				}
			}
		}
		else
		{
			if (GetKeyState(keys[0 + selectedMovementMode]) & 0x80) 
			{
				if (!PLAYER->collisionTest(dam, 'w'))
				{
					PLAYER->moveX(MOVE_SPEED);		
				}
			}			

			if (GetKeyState(keys[2 + selectedMovementMode]) & 0x80)
			{
				if (!PLAYER->collisionTest(dam, 's'))
				{
					PLAYER->moveX(-MOVE_SPEED);	
				}
			}	

			if (GetKeyState(keys[1 + selectedMovementMode]) & 0x80)
			{
				if (!PLAYER->collisionTest(dam, 'a'))
				{
					PLAYER->moveY(-MOVE_SPEED * 5);		
				}
			}	

			if (GetKeyState(keys[3 + selectedMovementMode]) & 0x80)
			{
				if (!PLAYER->collisionTest(dam, 'd'))
				{
					PLAYER->moveY(MOVE_SPEED * 5);		
				}
			}	
		}
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will draw the crosshair
//
//////////////////////////////////////////////////////////////
void drawCrosshair()
{	
	//	Here we need to either get the mouse coordinates, or
	//	joysticks.
	Vector2 mousePos;

	if (joystick->isUsable())
	{
		mousePos = joystick->getCoodinates();
	}
	else
	{
		POINT _mousePos;
		GetCursorPos(&_mousePos);
		ScreenToClient(g_hWnd, &_mousePos);
		mousePos.x = _mousePos.x;
		mousePos.y = _mousePos.y;
	}

	crosshair->position(mousePos.x - (crosshair->getSize().width / 2), mousePos.y - (crosshair->getSize().height / 2));
	crosshair->drawObject();
}

//////////////////////////////////////////////////////////////
//
//	This function generates the texture IDs
//
//////////////////////////////////////////////////////////////
void generateMenuTextures()
{			
	menu[0]->genTexture();
	menu[1]->genTexture();
	menu[2]->genTexture();
	menu[3]->genTexture();
	crosshair->genTexture();
		
	buttons[0]->genTexture();
	buttons[1]->genTexture();
	buttons[2]->genTexture();		
	buttons[3]->genTexture();	

	//	Loop through and find what screen resolution in our array is the
	//	one we are currently using
	for (int screenLoop = 0; screenLoop < ARRAY_LENGTH(screenRes); screenLoop++)
	{
		if (screenRes[screenLoop].width == SCREEN_WIDTH && screenRes[screenLoop].height == SCREEN_HEIGHT)
		{
			currentScreenResolution->setText(toString(screenRes[screenLoop].width) + "x" + toString(screenRes[screenLoop].height));
			currentScreenRes = screenLoop;
		}
	}
}

void generateGameTextures()
{
	PLAYER->generateWeaponTextures();
	dam->generateAllBuffers();
	skyBox->genTexture();

	Waypoint wPoint;
	wPoint.position = CAMERA.cameraPosition + Vector3(10, 0, 0);
	wPoint.view = CAMERA.cameraView + Vector3(10, 0, 0);
	wPoint.speed = 0.1;
	CAMERA.addWaypoint(wPoint);

	wPoint.position = CAMERA.cameraPosition + Vector3(25, 10, 0);
	wPoint.view = CAMERA.cameraView + Vector3(20, 0, 0);
	wPoint.up = CAMERA.cameraUpVector + Vector3(10, 0, 0);
	wPoint.speed = 0.1;
	CAMERA.addWaypoint(wPoint);
}

//////////////////////////////////////////////////////////////
//
//	This function initializes the open GL functions so we can
//	use them
//
//////////////////////////////////////////////////////////////
void initializeARB()
{
	if ((glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB")) == NULL)
	{
		error("Your video card does not support glGenBuffersARB.");
	}

    if ((glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB")) == NULL)
	{
		error("Your video card does not support glBindBufferARB.");
	}

	if ((glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB")) == NULL)
	{
		error("Your video card does not support glBufferDataARB.");
	}

    if ((glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB")) == NULL)
	{
		error("Your video card does not support glBuffersSubDataARB");		
	}

	if ((glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB")) == NULL)
	{
		error("Your video card does not support glActiveTextureARB");
	}
}

//////////////////////////////////////////////////////////////
//
//	This function initializes the sound library
//
//////////////////////////////////////////////////////////////
void initializeAL()
{
	//	Initialize openAL
	alutInit(NULL, 0);
	alGetError();
}

//////////////////////////////////////////////////////////////
//
//	This function initializes the GL stage
//
//////////////////////////////////////////////////////////////
void initializeGL(int width, int height)
{	
	g_hDC = GetDC(g_hWnd);

	if (!setupPixelFormat(g_hDC))
	{
		PostQuitMessage(0);
	}

	//	Set the current context to be openGL
	g_hRC = wglCreateContext(g_hDC);
	wglMakeCurrent(g_hDC, g_hRC);

	int pcVer = atof((const char *)glGetString(GL_VERSION)) >= 3.0;

	if (!pcVer)
	{
		error("You need a minimum of OpenGL 3.0 to run this game");
	}
	
	//	Initialize the ARB functions which we need for VBO's	
	initializeARB();
	resizeScene(SCREEN_WIDTH, SCREEN_HEIGHT);	
	
	glEnable(GL_BLEND);
	//	Enable alpha testing to remove alpha channels
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glDisable(GL_BLEND);
	
	//	Enable lighting and texturing
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	//	Set the available screen resolution
	screenRes[0] = Size(800, 600);
	screenRes[1] = Size(1024, 768); 
	screenRes[2] = Size(1280, 720);
	screenRes[3] = Size(1280, 800);
	screenRes[4] = Size(1440, 900);
	screenRes[5] = Size(1600, 900);
	screenRes[6] = Size(1920, 1080);
	screenRes[7] = Size(1920, 1200);

	memset(sounds, NULL, ARRAY_LENGTH(sounds));

	menuSize = Size(SCREEN_HEIGHT - 50, SCREEN_HEIGHT - 50);
	menuChange = Size(menuSize.width / 544.0, menuSize.height / 534.0);
}

//////////////////////////////////////////////////////////////
//
//	This function handles the lighting.
//
//////////////////////////////////////////////////////////////
void initializeLighting()
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);    
}

//////////////////////////////////////////////////////////////
//
//	This function changes the current matrix stack to enable
//	the use of 2D Functions. This is needed for the HUD display
//
//////////////////////////////////////////////////////////////
void loadFor2D()
{
	//	Disable the lighting from the 3d projection otherwise
	//	it messes with out lovely 2d texturing
	glDisable(GL_LIGHTING);

	//	Disable depth text otherwise it wont be 2d
	glDisable(GL_DEPTH_TEST);

	//	Set the matrix mode to projection so we can modify the
	//	projection
    glMatrixMode(GL_PROJECTION);
		
	//	Set the orthographic (2D) view
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);       
    
	//	Set the matrix mode to model view so we can draw the game
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

//////////////////////////////////////////////////////////////
//
//	This function changes the matrix to 3D to enable the user
//	of the 3D functions. This is needed for the game scene
//	rendering
//
//////////////////////////////////////////////////////////////
void loadFor3D()
{	
	//	Enable the required matricies for 3d projection
	glEnable(GL_LIGHTING);	
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_POLYGON_SMOOTH);

	//	Show the lighting
	initializeLighting();

	//	Set the matrix mode to projection so we can modify the
	//	projection
	glMatrixMode(GL_PROJECTION);
	
	//	Set the perspective view
	glLoadIdentity();
	gluPerspective(70.0, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.005f, 1000.0f);		

	//	Set the matrix mode to model view so we can draw the game
	glMatrixMode(GL_MODELVIEW);
	
	//	Clear the stage	
	glLoadIdentity();	
}

//////////////////////////////////////////////////////////////
//
//	This function loads the sounds
//
//////////////////////////////////////////////////////////////
void loadMenuSounds()
{
	sounds[0] = new Sound();
	sounds[0]->loadSound("sound/menu/menu.wav", true);
	sounds[1] = new Sound();
	sounds[1]->loadSound("sound/menu/menuchange.wav");
}

void loadGameSounds()
{
	sounds[2] = new Sound();
	sounds[2]->loadSound("sound/ambient/mission1.wav");	
}

//////////////////////////////////////////////////////////////
//
//	This function loads the needed textures and models
//
//////////////////////////////////////////////////////////////
void loadMenuTextures()
{
	//	Cursor crosshair
	crosshair = new Object();
	crosshair->loadTexture("graphics/textures/crosshair.tga");
	crosshair->setTransparency(true);		
	crosshair->size(52, 52);

	//	Main Menu background
	menu[0] = new Object();
	menu[0]->loadTexture("graphics/textures/menu/menu1.tga");		
	menu[0]->setTransparency(true);

	//	Select Mission background
	menu[1] = new Object();
	menu[1]->loadTexture("graphics/textures/menu/menu2.tga");	
	menu[1]->setTransparency(true);

	//	Mission Briefing background
	menu[2] = new Object();
	menu[2]->loadTexture("graphics/textures/menu/menu3.tga");
	menu[2]->setTransparency(true);

	//	Settings screen background
	menu[3] = new Object();
	menu[3]->loadTexture("graphics/textures/menu/settings.tga");	
	menu[3]->setTransparency(true);

	//	Select Mission text
	menuStrings[0] = new TextString("Select Mission");	
	menuStrings[0]->setFontFace(courierNew32);

	//	Options text
	menuStrings[1] = new TextString("Options");	
	menuStrings[1]->setFontFace(courierNew32);

	//	Exit game text
	menuStrings[2] = new TextString("Exit Game");	
	menuStrings[2]->setFontFace(courierNew32);

	//	Screen Resolution text
	menuStrings[3] = new TextString("Screen Resolution:");
	menuStrings[3]->setFontFace(courierNew32);	

	//	Game Volume Text
	menuStrings[4] = new TextString("Volume:");
	menuStrings[4]->setFontFace(courierNew32);

	//	Use Joystick Text
	menuStrings[5] = new TextString("Use Joystick:");
	menuStrings[5]->setFontFace(courierNew32);

	//	Joystick Sensitivity Text
	menuStrings[6] = new TextString("Joystick Sensitivity:");
	menuStrings[6]->setFontFace(courierNew32);

	//	Keyboard key mode
	menuStrings[7] = new TextString("Key Mode:");
	menuStrings[7]->setFontFace(courierNew32);

	//	Invert Y Axis
	menuStrings[9] = new TextString("Invert Y Axis:");
	menuStrings[9]->setFontFace(courierNew32);
	
	//	Invert Y Axis placeholder text
	invertYAxis = new TextString(CAMERA.invertYAxis ? "Yes" : "No");
	invertYAxis->setFontFace(courierNew32);

	//	Game Volume placeholder text
	currentGameVolume = new TextString();
	currentGameVolume->setFontFace(courierNew32);

	//	Screen Resolution placeholder text
	currentScreenResolution = new TextString();
	currentScreenResolution->setFontFace(courierNew32);

	//	Joystick toggler placeholder text
	useJoystick = new TextString();
	useJoystick->setFontFace(courierNew32);

	//	Keymode placeholder text
	currentKeyMode = new TextString();
	currentKeyMode->setFontFace(courierNew32);

	//	Joystick sensitivity toggler text
	joystickSensitivity = new TextString();
	joystickSensitivity->setFontFace(courierNew32);

	//	Back to main menu
	buttons[0] = new Object();
	buttons[0]->loadTexture("graphics/textures/back.tga");	
	buttons[0]->setTransparency(true);

	//	Back to missions
	buttons[1] = new Object();
	buttons[1]->loadTexture("graphics/textures/back.tga");	
	buttons[1]->setTransparency(true);

	//	Start Mission
	buttons[2] = new Object();
	buttons[2]->loadTexture("graphics/textures/start.tga");	
	buttons[2]->setTransparency(true);	
	
	//	Resume Game
	buttons[3] = new Object();
	buttons[3]->loadTexture("graphics/textures/back.tga");	
	buttons[3]->setTransparency(true);

	//	Mission 1 button
	buttons[4] = new Object();
	buttons[4]->color(255, 255, 255, 0);
	buttons[4]->setTransparency(true);	

	setMenuPositions();
	sounds[0]->playSound();
}

//////////////////////////////////////////////////////////////
//
//	This function sets the menu positions and sizes when the
//	resolution is changed
//
//////////////////////////////////////////////////////////////
void setMenuPositions()
{
	menuSize = Size(SCREEN_HEIGHT - 50, SCREEN_HEIGHT - 50);
	menuChange = Size(menuSize.width / 544.0, menuSize.height / 534.0);

	Vector3 menuPosition = Vector3((SCREEN_WIDTH - (SCREEN_HEIGHT - 50)) / 2, (SCREEN_HEIGHT - (SCREEN_HEIGHT - 50)) / 2, 0);
	Size backSize = Size(SCREEN_HEIGHT / 20, ((57 / 27) * (SCREEN_HEIGHT / 20)));
	Vector3 backPosition = Vector3(menuPosition.x + menuSize.width - (backSize.width * 1.5), menuPosition.y + (backSize.height), 0);
	Vector3 backPosition2 = Vector3(0, menuChange.height * 130, 0);	
	Vector3 mission1Position = Vector3(menuPosition.x + (menuChange.width * 16), menuPosition.y + (menuChange.height * 69), 0);

	//	Main Menu
	menu[0]->position(menuPosition);
	menu[0]->size(menuSize);

	//	Mission Page
	menu[1]->position(menuPosition);
	menu[1]->size(menuSize);

	//	Mission Briefing
	menu[2]->position(menuPosition);
	menu[2]->size(menuSize);

	//	Options Page
	menu[3]->position(menuPosition);
	menu[3]->size(menuSize);
		
	//	Select Mission
	menuStrings[0]->scaleMag = menuChange.width / 1.1;
	menuStrings[0]->color(255, 255, 255);
	menuStrings[0]->setCharacerSpacing(18 * menuChange.width);
	menuStrings[0]->position(middle.x - (menuStrings[0]->getCalculatedSize().width / 1.9), middle.y);
	
	//	Options
	menuStrings[1]->scaleMag = menuChange.width / 1.1;
	menuStrings[1]->color(255, 255, 255);
	menuStrings[1]->setCharacerSpacing(18 * menuChange.width);
	menuStrings[1]->position(middle.x - (menuStrings[1]->getCalculatedSize().width / 1.9), middle.y + (menuChange.height * 50));

	//	Exit Game
	menuStrings[2]->scaleMag = menuChange.width / 1.1;
	menuStrings[2]->color(255, 255, 255);
	menuStrings[2]->setCharacerSpacing(18 * menuChange.width);
	menuStrings[2]->position(middle.x - (menuStrings[2]->getCalculatedSize().width / 1.9), middle.y + (menuChange.height * 100));

	//	Screen Resolution
	menuStrings[3]->scaleMag = menuChange.width / 1.5;
	menuStrings[3]->color(255, 255, 255);
	menuStrings[3]->setCharacerSpacing(14 * menuChange.width);
	menuStrings[3]->position(menuPosition.x + (menuChange.width * 50), menuPosition.y + (menuChange.height * 70));	

	//	Current Screen Resolution
	currentScreenResolution->scaleMag = menuChange.width / 1.5;
	currentScreenResolution->color(255, 255, 255);
	currentScreenResolution->setCharacerSpacing(14 * menuChange.width);
	currentScreenResolution->position(menuStrings[3]->getPosition().x + menuStrings[3]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[3]->getPosition().y);

	//	Game Volume
	menuStrings[4]->scaleMag = menuChange.width / 1.5;
	menuStrings[4]->color(255, 255, 255);
	menuStrings[4]->setCharacerSpacing(14 * menuChange.width);
	menuStrings[4]->position(menuStrings[3]->getPosition().x, menuStrings[3]->getPosition().y + (menuChange.height * 50));

	//	Current Volume Text
	currentGameVolume->scaleMag = menuChange.width / 1.5;
	currentGameVolume->color(255, 255, 255);
	currentGameVolume->setCharacerSpacing(14 * menuChange.width);
	currentGameVolume->position(menuStrings[4]->getPosition().x + menuStrings[4]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[4]->getPosition().y);

	//	Use Joystick
	menuStrings[5]->scaleMag = menuChange.width / 1.5;
	menuStrings[5]->color(255, 255, 255);
	menuStrings[5]->setCharacerSpacing(14 * menuChange.width);
	menuStrings[5]->position(menuStrings[4]->getPosition().x, menuStrings[4]->getPosition().y + (menuChange.height * 50));

	//	Joystick toggler text
	useJoystick->scaleMag = menuChange.width / 1.5;
	useJoystick->color(255, 255, 255);
	useJoystick->setCharacerSpacing(14 * menuChange.width);
	useJoystick->position(menuStrings[5]->getPosition().x + menuStrings[5]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[5]->getPosition().y);

	//	Joystick Sensitivity
	menuStrings[6]->scaleMag = menuChange.width / 1.5;
	menuStrings[6]->color(255, 255, 255);
	menuStrings[6]->setCharacerSpacing(14 * menuChange.width);
	menuStrings[6]->position(menuStrings[5]->getPosition().x, menuStrings[5]->getPosition().y + (menuChange.height * 50));

	joystickSensitivity->scaleMag = menuChange.width / 1.5;
	joystickSensitivity->color(255, 255, 255);
	joystickSensitivity->setCharacerSpacing(14 * menuChange.width);
	joystickSensitivity->position(menuStrings[6]->getPosition().x + menuStrings[6]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[6]->getPosition().y);
	joystickSensitivity->setText(toString((int)(joystick->sensitivity * 100.0f)) + "%");

	//	Key Mode
	menuStrings[7]->scaleMag = menuChange.width / 1.5;
	menuStrings[7]->color(255, 255, 255);
	menuStrings[7]->setCharacerSpacing(14 * menuChange.width);			
	
	if (joystick->isUsable())
	{
		useJoystick->setText("Yes");
		menuStrings[7]->position(menuStrings[6]->getPosition().x, menuStrings[6]->getPosition().y + (menuChange.height * 50));
	}
	else
	{
		useJoystick->setText("No");		
	}

	//	Key Mode
	currentKeyMode->scaleMag = menuChange.width / 1.5;
	currentKeyMode->color(255, 255, 255);
	currentKeyMode->setCharacerSpacing(14 * menuChange.width);
	currentKeyMode->position(menuStrings[7]->getPosition().x + menuStrings[7]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[7]->getPosition().y);

	//	Invert Y
	menuStrings[9]->scaleMag = menuChange.width / 1.5;
	menuStrings[9]->color(255, 255, 255);
	menuStrings[9]->setCharacerSpacing(14 * menuChange.width);
	menuStrings[9]->position(menuStrings[7]->getPosition().x, menuStrings[7]->getPosition().y + (menuChange.height * 50));

	invertYAxis->scaleMag = menuChange.width / 1.5;
	invertYAxis->color(255, 255, 255);
	invertYAxis->setCharacerSpacing(14 * menuChange.width);
	invertYAxis->position(menuStrings[9]->getPosition().x + menuStrings[9]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[9]->getPosition().y);

	//	Back to main menu
	buttons[0]->position(backPosition);
	buttons[0]->size(backSize);

	//	Back to mission menu
	buttons[1]->position(backPosition + backPosition2);
	buttons[1]->size(backSize);

	//	Start Mission
	buttons[2]->position(backPosition);
	buttons[2]->size(backSize);	

	//	Resume Game
	buttons[3]->position(backPosition);
	buttons[3]->size(backSize);

	//	Mission 1 button
	buttons[4]->vertex(Rect(0, 0, menuChange.width * 114, menuChange.height * 88));
	buttons[4]->position(mission1Position);

	menuBackground->size(SCREEN_WIDTH, SCREEN_HEIGHT);	
}

//////////////////////////////////////////////////////////////
//
//	This function loads the game textures into memory
//
//////////////////////////////////////////////////////////////
Matrix4 fpMatrix;
void loadGameTextures()
{			
	//	Load the map settings
	dam = new Map();
	dam->loadSettings("maps/dam/dam.ini");	

	//	Create the skybox
	skyBox = new Object();	
	skyBox->position(0, 0);
	skyBox->loadTexture("graphics/textures/skybox.tga");	

	//	Resume Game
	pausedMenu[0] = new TextString();
	pausedMenu[0]->setFontFace(courierNew32);
	pausedMenu[0]->setText("Resume Game");
	pausedMenu[0]->color(255, 255, 255, 255);	

	//	Settings
	pausedMenu[1] = new TextString();
	pausedMenu[1]->setFontFace(courierNew32);
	pausedMenu[1]->setText("Settings");
	pausedMenu[1]->color(255, 255, 255, 255);		

	//	Exit game 
	pausedMenu[2] = new TextString();
	pausedMenu[2]->setFontFace(courierNew32);
	pausedMenu[2]->setText("Exit Game");
	pausedMenu[2]->color(255, 255, 255, 255);		

	//	Create the first person matrix
	fpMatrix = Matrix4
	(
		1, 0, 0, 0,
		0, 1.0, 0, 0,
		0, 0, 1.0, 0,
		0, -0.005, -0.03, 1.0
	);		

	setGamePositions();
}

void setGamePositions()
{	
	menuSize = Size(SCREEN_HEIGHT - 50, SCREEN_HEIGHT - 50);
	menuChange = Size(menuSize.width / 544.0, menuSize.height / 534.0);

	skyBox->size(SCREEN_WIDTH, SCREEN_HEIGHT);

	//	Resume Game
	pausedMenu[0]->setCharacerSpacing(18 * menuChange.width);
	pausedMenu[0]->scaleMag = menuChange.width / 1.1;
	pausedMenu[0]->position(middle.x - ((pausedMenu[0]->getCalculatedSize().width / 2)), middle.y - (menuChange.height * 50));

	//	Settings
	pausedMenu[1]->setCharacerSpacing(18 * menuChange.width);
	pausedMenu[1]->scaleMag = menuChange.width / 1.1;
	pausedMenu[1]->position(middle.x - ((pausedMenu[1]->getCalculatedSize().width / 2)), pausedMenu[0]->getPosition().y + (menuChange.height * 50));

	//	Exit Game
	pausedMenu[2]->setCharacerSpacing(18 * menuChange.width);
	pausedMenu[2]->scaleMag = menuChange.width / 1.1;
	pausedMenu[2]->position(middle.x - ((pausedMenu[2]->getCalculatedSize().width / 2)), pausedMenu[1]->getPosition().y + (menuChange.height * 50));
}

//////////////////////////////////////////////////////////////
//
//	This function renders the game stage
//
//////////////////////////////////////////////////////////////
void renderGame()
{			
	glPushMatrix();
		CAMERA.drawCamera();			 	
		dam->drawMap();		
	glPopMatrix();
}

void renderPlayer()
{
	glPushMatrix();		
		//	This resets the view matrix basically making the gun
		//	follow the camera		
		glLoadMatrixf(fpMatrix);			
		PLAYER->drawCurrentWeapon();
	glPopMatrix();
}

void renderGame2D()
{		
	glPushMatrix();		
		PLAYER->drawHUD();				
	glPopMatrix();
}

//////////////////////////////////////////////////////////////
//
//	This function will determine on what is rendered depending
//	on the current game mode
//
//////////////////////////////////////////////////////////////
void renderScreen()
{
	//	Clear the screen with the default colour
	glClearColor(GL_COLOR_REDf[0], GL_COLOR_BLACKf[1], GL_COLOR_BLACKf[2], GL_COLOR_BLACKf[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	if (GAME_STATE == GAME_RUNNING)
	{
		++averageFPS;

		if (GAME_MODE == IN_GAME)
		{				
			//	Lets create a backdrop on the game
			loadFor2D();		
			{
				skyBox->drawObject();
			}
		}		
		
		loadFor3D();
		{			
			switch(GAME_MODE)
			{ 				
				case IN_GAME:
				{					
					CAMERA.moveByMouse = true;
					renderGame();
					renderPlayer();

					break;
				}
			}	

			//	Update the FPS
			if (abs(currentTime - time(0)) >= 2)
			{	
				currentTime = time(0);
				FPS = averageFPS;
				averageFPS = 0;
			}
		}
		
		loadFor2D();
		{	
			switch(GAME_MODE)
			{
				case IN_MENU:
				{
					switch (MENU_ID)
					{
						case MENU1:
						{							
							showMenu1();
							break;
						}

						case MENU2:
						{
							showMenu2();
							break;
						}

						case MENU3:
						{
							showMenu3();
							break;
						}
						
						case GAME_OPTIONS:
						case OPTIONS:
						{
							showOptions();
							break;
						}
					}

					drawCrosshair();
					CAMERA.moveByMouse = false;
					break;
				}

				case IN_GAME:
				{
					renderGame2D();
					break;
				}
			}

			glPushMatrix();
				string text;
				text = "FPS: ";
				text += toString(FPS);

				TextString *fps = new TextString();
				fps->setFontFace(courierNew);
				fps->setCharacerSpacing(12);
				fps->color(255, 255, 255);
				fps->setText(text);
				fps->position(10, 10, 0);
				fps->drawString();	

				delete fps;
			glPopMatrix();
		}
	}
	else if (GAME_STATE == GAME_PAUSED)
	{		
		loadFor2D();
		{
			menuBackground->drawObject();			

			pausedMenu[0]->drawString();
			pausedMenu[1]->drawString();
			pausedMenu[2]->drawString();

			drawCrosshair();	
		}
	}		
	else if (GAME_STATE == GAME_LOADING)
	{		
		loadFor2D();
		{
			menuBackground->drawObject();
			
			TextString *loading = new TextString();
			loading->scaleMag = 1.7f;
			loading->setCharacerSpacing(14);
			loading->setFontFace(courierNew);
			loading->color(255, 255, 255, 255);
			loading->setText("LOADING...");
			loading->position(middle.x - (loading->getCalculatedSize().width / 2), middle.y, 0);
			loading->drawString();	

			delete loading;
		}
	}

	SwapBuffers(g_hDC);	
}

//////////////////////////////////////////////////////////////
//
//	Golden Eye Revenge - 2011 Games design project by
//	Callum Taylor
//
//	Button Handlers
//
//////////////////////////////////////////////////////////////
void selectMissionPress()
{
	sounds[1]->playSound();
	MENU_ID = MENU2;
}

void optionsButtonPress()
{
	sounds[1]->playSound();
	MENU_ID = OPTIONS;
}

void mainMenu(int button)
{
	sounds[1]->playSound();
	MENU_ID = MENU1;
}

void missionBriefing(int button)
{
	sounds[1]->playSound();
	MENU_ID = MENU3;
}

void missionSelection(int button)
{
	sounds[1]->playSound();
	MENU_ID = MENU2;
}

void startGame(int button)
{
	sounds[1]->playSound();
	buttons[5]->setClickable(false);
		
	//	Load the main game
	GAME_STATE = GAME_LOADING;
	HANDLE threadHandle = 0;
	threadHandle = CreateThread(NULL, 0, loadGame, NULL, 0, NULL);
	
	//	Show the loading screen
	while (GAME_STATE == GAME_LOADING)
	{
		renderScreen();
	}

	//	The thread has finished so lets close it and finish
	//	loading the game
	WaitForSingleObject(threadHandle, INFINITE);
	CloseHandle(threadHandle);

	//	Load the player settings
	PLAYER = new Player();	
	Vector3 position = PLAYER->getPlayerPos();
	
	//	Set the position of the camera
	CAMERA.positionCamera(position.x, position.y, position.z, 0, 1.5, 5, 0, 1, 0);		

	//	Now all the textures have been coppied to RAM, we need
	//	to generate their ID's so we can actually use them
	generateGameTextures();

	sounds[0]->stopSound();
	sounds[2]->playSound();

	GAME_MODE = IN_GAME;
}

//////////////////////////////////////////////////////////////
//
//	This function changes the screen resolution
//
//////////////////////////////////////////////////////////////
void changeScreenRes()
{	
	if (currentScreenRes == ARRAY_LENGTH(screenRes) || maxScreenRes == screenRes[currentScreenRes])
	{				
		currentScreenRes = 0;	
		currentScreenResolution->setText(toString(screenRes[currentScreenRes].width) + "x" + toString(screenRes[currentScreenRes].height));
		resizeWindow(screenRes[currentScreenRes].width, screenRes[currentScreenRes].height);
	}
	else
	{		
		currentScreenRes++;
		currentScreenResolution->setText(toString(screenRes[currentScreenRes].width) + "x" + toString(screenRes[currentScreenRes].height));
		resizeWindow(screenRes[currentScreenRes].width, screenRes[currentScreenRes].height);
	}

	settings->writeSetting("main", "width", (char *)toString(screenRes[currentScreenRes].width).c_str(), true);
	settings->writeSetting("main", "height", (char *)toString(screenRes[currentScreenRes].height).c_str(), true);	
}

//////////////////////////////////////////////////////////////
//
//	This function changes game volume
//
//////////////////////////////////////////////////////////////
void changeGameVolume()
{
	if (soundVolume + 0.1 >= 1.1)
	{
		soundVolume = 0.0;
	}
	else
	{
		soundVolume += 0.1;		
	}

	for (int soundsLoop = 0; soundsLoop < ARRAY_LENGTH(sounds); soundsLoop++)
	{
		if (sounds[soundsLoop] != NULL)
		{
			sounds[soundsLoop]->updateVolume(soundVolume);
		}
	}

	settings->writeSetting("sound", "volume", (char *)toString(soundVolume).c_str(), true);
}

//////////////////////////////////////////////////////////////
//
//	This function updates the joystick sensitivity
//
//////////////////////////////////////////////////////////////
void changeKeyMode()
{
	if (joystick->isUsable())
	{
		useThumbPad = !useThumbPad;
		settings->writeSetting("joystick", "keymode", (useThumbPad ? "0" : "1"), true);
	}
	else
	{
		if (selectedMovementMode == 0)
		{
			selectedMovementMode = 4;
			settings->writeSetting("keyboard", "keymode", "1", true);
		}
		else
		{
			selectedMovementMode = 0;
			settings->writeSetting("keyboard", "keymode", "0", true);
		}
	}
}

//////////////////////////////////////////////////////////////
//
//	This function updates the invert y axis bool in the camera
//
//////////////////////////////////////////////////////////////
void changeInvert()
{
	CAMERA.invertYAxis = !CAMERA.invertYAxis;

	invertYAxis->setText(CAMERA.invertYAxis ? "Yes" : "No");
	settings->writeSetting("control", "inverty", (char *)toString(CAMERA.invertYAxis).c_str(), true);
}

//////////////////////////////////////////////////////////////
//
//	This function updates the joystick sensitivity
//
//////////////////////////////////////////////////////////////
void changeJoystickSensitivity()
{
	if (joystick->sensitivity + 0.1 >= 1.1)
	{
		joystick->sensitivity = 0.1;
	}
	else
	{
		joystick->sensitivity += 0.1;		
	}

	joystickSensitivity->setText(toString((int)(joystick->sensitivity * 100.0f)) + "%");
	settings->writeSetting("joystick", "sensitivity", (char *)toString(joystick->sensitivity).c_str(), true);
}

//////////////////////////////////////////////////////////////
//
//	This function turns the joystick on or off
//
//////////////////////////////////////////////////////////////
void toggleJoystick()
{	
	if (joystick->isUsable())
	{
		SetCursorPos(joystick->getCoodinates().x, joystick->getCoodinates().y);
		joystick->disconnect();
		useJoystick->setText("No");
	}
	else
	{
		POINT mousePoint;
		GetCursorPos(&mousePoint);
		ScreenToClient(g_hWnd, &mousePoint);

		joystick->updateCoordinates(mousePoint.x, mousePoint.y);
		joystick->connect();
		useJoystick->setText("Yes");
	}

	settings->writeSetting("joystick", "usejoystick", (char *)toString(joystick->isUsable()).c_str(), true);
}

//////////////////////////////////////////////////////////////
//
//	This function resumes the game
//
//////////////////////////////////////////////////////////////
void resumeGame(int button)
{	
	GAME_STATE = GAME_RUNNING;
	GAME_MODE = IN_GAME;
	MENU_ID = 0;
}

//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Menu Functions
//
//////////////////////////////////////////////////////////////
void showMenu1()
{	
	menuBackground->drawObject();
	menu[0]->drawObject();
	
	menuStrings[0]->drawString();
	menuStrings[1]->drawString();
	menuStrings[2]->drawString();
}

void showMenu2()
{
	menuBackground->drawObject();
	menu[1]->drawObject();

	//	Back to main menu
	buttons[0]->drawObject();

	//	Mission 1 button
	buttons[4]->drawObject();	
}

void showMenu3()
{
	menuBackground->drawObject();
	menu[2]->drawObject();

	//	Back to missions
	buttons[1]->drawObject();

	//	Start Game
	buttons[2]->drawObject();
}

//////////////////////////////////////////////////////////////
//
//	This is the options menu
//
//////////////////////////////////////////////////////////////
void showOptions()
{
	menuBackground->drawObject();
	menu[3]->drawObject();
	buttons[3]->drawObject();

	//	Draw the screen resolution text
	menuStrings[3]->drawString();
	currentScreenResolution->drawString();

	//	Draw the volume string
	menuStrings[4]->drawString();
	currentGameVolume->setText(toString((int)(soundVolume * 100)) + "%");
	currentGameVolume->drawString();

	if (joystick->isConnected())
	{
		//	Draw the use joystick string
		currentKeyMode->setText(useThumbPad ? "Thumb Pad" : "D PAD");
		menuStrings[5]->drawString();
		useJoystick->drawString();		

		if (joystick->isUsable())
		{		
			//	Draw the joystick sensitivity string
			currentKeyMode->setText(useThumbPad ? "Thumb Pad" : "D PAD");
			menuStrings[6]->drawString();
			joystickSensitivity->drawString();

			menuStrings[7]->position(menuStrings[6]->getPosition().x, menuStrings[6]->getPosition().y + (menuChange.height * 50));
		}
		else
		{
			currentKeyMode->setText(selectedMovementMode == 0 ? "WASD" : "Arrows");
			menuStrings[7]->position(menuStrings[5]->getPosition().x, menuStrings[5]->getPosition().y + (menuChange.height * 50));
		}
	}
	else
	{
		currentKeyMode->setText(selectedMovementMode == 0 ? "WASD" : "Arrows");
		menuStrings[7]->position(menuStrings[4]->getPosition().x, menuStrings[4]->getPosition().y + (menuChange.height * 50));
	}
	
	//	Draw the arrow option strings
	menuStrings[7]->drawString();	
	currentKeyMode->position(menuStrings[7]->getPosition().x + menuStrings[7]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[7]->getPosition().y);
	currentKeyMode->drawString();

	//	Draw the invert y axis option strings
	menuStrings[9]->position(menuStrings[7]->getPosition().x, menuStrings[7]->getPosition().y + (menuChange.height * 50));
	menuStrings[9]->drawString();
	invertYAxis->position(menuStrings[9]->getPosition().x + menuStrings[9]->getCalculatedSize().width + (menuChange.height * 20), menuStrings[9]->getPosition().y);
	invertYAxis->drawString();
}

void showOptionsInGame()
{
	GAME_STATE = GAME_RUNNING;
	GAME_MODE = IN_MENU;
	MENU_ID = GAME_OPTIONS;
}

void showPausedMenu()
{
	GAME_STATE = GAME_PAUSED;
	GAME_MODE = IN_GAME;
}