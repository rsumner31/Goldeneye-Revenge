//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Game.h - this is the header file for game.cpp and will
//	include all prototypes and externs for that class
//
//	Initialize all the needed constants, definitions and 
//	global variables for the game
//
//////////////////////////////////////////////////////////////
#define IN_GAME 1
#define IN_MENU 0
#define GAME_PAUSED 0
#define GAME_RUNNING 1
#define GAME_LOADING 2
#define MENU1 0
#define MENU2 1
#define MENU3 2
#define OPTIONS 3
#define GAME_OPTIONS 4

//	Game mode will handle what the game is currently rendering
extern int GAME_MODE;
//	Game state will handle if the game is paused or not
extern int GAME_STATE;
extern int MENU_ID;

//////////////////////////////////////////////////////////////
//
//	Initialize all of the functions a protocols
//
//////////////////////////////////////////////////////////////
void checkMouse(int button, int x, int y);
void toggleFire();
void checkInputs(WPARAM wParam);
void checkJoystickInput();
void checkMovement();
void drawCrosshair();
void generateMenuTextures();
void generateGameTextures();
void initializeARB();
void initializeAL();
void initializeGL(int width, int height);
void initializeLighting();
void loadFor2D();
void loadFor3D();
void loadMenuSounds();
void loadMenuTextures();
void setMenuPositions();
void loadGameSounds();
void loadGameTextures();
void setGamePositions();
void renderGame();
void renderPlayer();
void renderGame2D();
void renderScreen();

void selectMissionPress();
void optionsButtonPress();
void mainMenu(int button);
void missionBriefing(int button);
void missionSelection(int button);
void startGame(int button);
void changeScreenRes();
void changeGameVolume();
void changeJoystickSensitivity();
void toggleJoystick();
void changeInvert();
void changeKeyMode();
void resumeGame(int button);

void showMenu1();
void showMenu2();
void showMenu3();
void showOptions();
void showOptionsInGame();
void showPausedMenu();