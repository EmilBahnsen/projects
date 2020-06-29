#ifndef NokiaLCDMenu_h
#define NokiaLCDMenu_h

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define CHARWIDTH 6
#define CHARHEIGHT 9

#define MAINMENULENGTH 3
#define GAMESMENULENGTH 2

enum Screens {MAIN, GAMES, TETRIS, ABOUT, BREAKOUT};

enum Blocks {L, RL, Z, RZ, Sq, I, T};

enum {
	LifeLost,
	LifeGained,
	GameLost
};

typedef void (*CallBack)(int);
typedef void (*GameStageChangeCallback)(int);

enum CallBAckType {
	Change2Pong,
	Change2Frogger,
	Change2Menu,
	Change2ScrollBannerForPong
};

class NokiaLCDMenu
{
	public:
		NokiaLCDMenu(Adafruit_PCD8544* _display, CallBack menuChangeCallback);
		void Init(int contrast);
		void selectMenuItem(bool upOrDown);
		void clear();
		void ToggleActive(bool state);
		void setNextBlocks(Blocks b);
		void setNewScore(long score);
		void setLevel(int level);
		void drawMenu(Screens screen);
		bool enterMenu();
		void goBack();
		void selectFirstMenuItem();
		void drawBreakoutScreen();
		void e_gameChange(int gameEvent);
	private:
		int centerTekst(int length);
		int getPosY(int xLength);
		void drawTetrisScreen();
		void drawNextBlocks();
		void drawAboutScreen();
		void drawMainMenu();
		void drawGamesMenu();
		Adafruit_PCD8544* Display;
		CallBack _menuChange;
};

#endif