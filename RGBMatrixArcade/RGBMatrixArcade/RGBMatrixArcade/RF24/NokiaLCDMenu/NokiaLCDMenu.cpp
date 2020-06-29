#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "NokiaLCDMenu.h"

String mainMenu[] = {"Games", "Options", "About"};
String gamesMenu[] = {"Pong", "Frogger"};
Blocks nextBlocks[] = {Z, T, I};

long Score = 0;
unsigned int Level = 0;
unsigned int Lives = 3;

int currentIndex = 0;

Screens activeMenu = MAIN;

static const unsigned char PROGMEM FullHeart[] =
{
	B01000100,
	B11101110,
	B11111110,
	B11111110,
	B01111100,
	B00111000,
	B00010000
};

static const unsigned char PROGMEM EmptyHeart[] =
{
	B01000100,
	B10101010,
	B10010010,
	B10000010,
	B01000100,
	B00101000,
	B00010000,
};

static const unsigned char PROGMEM TetrisL[] =
{
	B00000000,
	B01110000,
	B01110000,
	B01110000,
	B01110000,
	B01110000,
	B01110000,
	B01110000,
	B01111111,
	B01111111,
	B00000000,
	B00000000
};

static const unsigned char PROGMEM TetrisRL[] =
{
	B00000000,
	B00001110,
	B00001110,
	B00001110,
	B00001110,
	B00001110,
	B00001110,
	B00001110,
	B01111110,
	B01111110,
	B00000000,
	B00000000
};

static const unsigned char PROGMEM TetrisT[] =
{
	B00000000,
	B00001110,
	B00001110,
	B00001110,
	B01111110,
	B01111110,
	B01111110,
	B00001110,
	B00001110,
	B00001110,
	B00000000,
	B00000000
};

static const unsigned char PROGMEM TetrisSq[] =
{
	B00000000,
	B00000000,
	B00000000,
	B01111111,
	B01111111,
	B01111111,
	B01111111,
	B01111111,
	B01111111,
	B00000000,
	B00000000,
	B00000000
};

static const unsigned char PROGMEM TetrisRZ[] =
{
	B00000000,
	B01110000,
	B01110000,
	B01110000,
	B01111110,
	B01111110,
	B01111110,
	B00001110,
	B00001110,
	B00001110,
	B00000000,
	B00000000
};

static const unsigned char PROGMEM TetrisZ[] =
{
	B00000000,
	B00001110,
	B00001110,
	B00001110,
	B01111110,
	B01111110,
	B01111110,
	B01110000,
	B01110000,
	B01110000,
	B00000000,
	B00000000
};

static const unsigned char PROGMEM TetrisI[] =
{
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100,
	B00011100
};

NokiaLCDMenu::NokiaLCDMenu(Adafruit_PCD8544* _display, CallBack menuChangeCallback)
{
	Display = _display;
	_menuChange = menuChangeCallback;
}


void NokiaLCDMenu::Init(int contrast)
{
	Display->begin();
	Display->setContrast(contrast);
	drawMainMenu();
	selectFirstMenuItem();
	Display->display();
}

void NokiaLCDMenu::drawMenu(Screens screen)
{
	switch(screen)
	{
		case MAIN:
			drawMainMenu();
			break;
		case GAMES:
			drawGamesMenu();
			break;
		case TETRIS:
			drawTetrisScreen();
			break;
		case ABOUT:
			drawAboutScreen();
			break;
	}
}

void NokiaLCDMenu::drawMainMenu()
{
	clear();

	activeMenu = MAIN;
	int posY = getPosY(MAINMENULENGTH);

	for (int i = 0; i < MAINMENULENGTH; i++) 
	{
		Display->WriteString(centerTekst(mainMenu[i].length()), posY + (i * CHARHEIGHT), mainMenu[i], 0);
	}
}

void NokiaLCDMenu::drawGamesMenu()
{
	clear();

	activeMenu = GAMES;
	int posY = getPosY(GAMESMENULENGTH);

	for (int i = 0; i < GAMESMENULENGTH; i++) 
	{
		Display->WriteString(centerTekst(gamesMenu[i].length()), posY + (i * CHARHEIGHT), gamesMenu[i], 0);
	}
}

int NokiaLCDMenu::centerTekst(int length)
{
	return (int)((LCDWIDTH - (length * CHARWIDTH)) * 0.5);
}

void NokiaLCDMenu::selectFirstMenuItem()
{
	int posY = 0;

	switch(activeMenu)
	{
		case MAIN:
			posY = getPosY(MAINMENULENGTH);
			Display->WriteString(centerTekst(mainMenu[0].length() + 2), posY + (0 * CHARHEIGHT), ">", 0);
			Display->WriteString(centerTekst(mainMenu[0].length()) + (mainMenu[0].length() * CHARWIDTH), posY + (0 * CHARHEIGHT), "<", 0);
			break;

		case GAMES:
			posY = getPosY(GAMESMENULENGTH);
			Display->WriteString(centerTekst(gamesMenu[0].length() + 2), posY + (0 * CHARHEIGHT), ">", 0);
			Display->WriteString(centerTekst(gamesMenu[0].length()) + (gamesMenu[0].length() * CHARWIDTH), posY + (0 * CHARHEIGHT), "<", 0);
			break;
	}
}

void NokiaLCDMenu::selectMenuItem(bool upOrDown)
{
	int posY = 0;

	if (upOrDown)
	{
		currentIndex--;
	}
	else
	{
		currentIndex++;
	}

	switch(activeMenu)
	{
		case MAIN:
			clear();
			if (currentIndex < 0)
			{
				currentIndex = MAINMENULENGTH - 1;
			}
			else if (currentIndex >= MAINMENULENGTH)
			{
				currentIndex = 0;
			}

			drawMainMenu();
			posY = getPosY(MAINMENULENGTH);
			Display->WriteString(centerTekst(mainMenu[currentIndex].length() + 2), posY + (currentIndex * CHARHEIGHT), ">", 0);
			Display->WriteString(centerTekst(mainMenu[currentIndex].length()) + (mainMenu[currentIndex].length() * CHARWIDTH), posY + (currentIndex * CHARHEIGHT), "<", 0);
			break;

		case GAMES:
			clear();
			if (currentIndex < 0)
			{
				currentIndex = GAMESMENULENGTH - 1;
			}
			else if (currentIndex >= GAMESMENULENGTH)
			{
				currentIndex = 0;
			}
			drawGamesMenu();
			posY = getPosY(GAMESMENULENGTH);
			Display->WriteString(centerTekst(gamesMenu[currentIndex].length() + 2), posY + (currentIndex * CHARHEIGHT), ">", 0);
			Display->WriteString(centerTekst(gamesMenu[currentIndex].length()) + (gamesMenu[currentIndex].length() * CHARWIDTH), posY + (currentIndex * CHARHEIGHT), "<", 0);
			break;

		case TETRIS:
			currentIndex = 0;
			break;

		case BREAKOUT:
			Lives += (upOrDown) ? 1 : -1;
			if (Lives < 0)
			{
				Lives = 0;
			}
			else if (Lives > 3)
			{
				Lives = 3;
			}
			drawBreakoutScreen();
			break;
	}

	Display->display();
}


int NokiaLCDMenu::getPosY(int xLength)
{
	return (int)(((LCDHEIGHT - (xLength * CHARHEIGHT)) * 0.5));
}

void NokiaLCDMenu::clear()
{
	Display->clearDisplay();
}

void NokiaLCDMenu::ToggleActive(bool state)
{
	if (state)
	{
		Display->drawRect(0,0,84,48,1);
	}
	else
	{
		Display->drawRect(0,0,84,48,0);
	}
	Display->display();
}

void NokiaLCDMenu::drawTetrisScreen()
{
	activeMenu = TETRIS;

	clear();
	Display->WriteString(centerTekst(6), 0, "Pong", 0);
	Display->drawLine(0,8,84,8,1);
	Display->drawLine(72,8,72,48,1);
	Display->drawLine(72,23,84,23,1);

	Display->WriteString(0, 10, "Score:", 0);
	Display->WriteString(0, 19, String(Score), 0);

	Display->WriteString(0, 28, "Level:", 0);
	Display->WriteString(0, 37, String(Level), 0);

	//drawNextBlocks();

	Display->display();
}

void NokiaLCDMenu::drawNextBlocks()
{
	for (int i = 0; i < 3; ++i)
	{
		int posY = (i != 0 ? 12 : 10);
		//int posY = 10;
		switch(nextBlocks[i])
		{
			case L:
				Display->drawBitmap(74, posY + 13 * i,  TetrisL, 8, 12, 1);
				break;

			case RL:
				Display->drawBitmap(74, posY + 13 * i,  TetrisRL, 8, 12, 1);
				break;

			case Z:
				Display->drawBitmap(74, posY + 13 * i,  TetrisZ, 8, 12, 1);
				break;

			case RZ:
				Display->drawBitmap(74, posY + 13 * i,  TetrisRZ, 8, 12, 1);
				break;

			case Sq:
				Display->drawBitmap(74, posY + 13 * i,  TetrisSq, 8, 12, 1);
				break;

			case I:
				Display->drawBitmap(74, posY + 13 * i,  TetrisI, 8, 12, 1);
				break;

			case T:
				Display->drawBitmap(74, posY + 13 * i,  TetrisT, 8, 12, 1);
				break;
		}
	}
}

void NokiaLCDMenu::setNextBlocks(Blocks b)
{
	for (int i = 1; i < 3; ++i)
	{
		nextBlocks[i-1] = nextBlocks[i];
	}

	nextBlocks[2] = b;

	drawTetrisScreen();
}

void NokiaLCDMenu::setNewScore(long score)
{
	Score = score;
	drawTetrisScreen();
}

void NokiaLCDMenu::setLevel(int level)
{
	Level = level;
	drawTetrisScreen();
}

void NokiaLCDMenu::drawAboutScreen()
{
	activeMenu = ABOUT;

	clear();
	Display->WriteString(centerTekst(6), 0, "ABOUT", 0);
	Display->drawLine(0,8,84,8,1);
	
	Display->WriteString(0, 10, "RGB-LED Matrix", 0);
	Display->WriteString(0, 21, "Emil Bahnsen", 0);
	Display->WriteString(0, 30, "Martin Nyholm", 0);
	Display->WriteString(0, 40, "El, 2015", 0);
	Display->display();

}

bool NokiaLCDMenu::enterMenu()
{
	bool temp = false;

	switch(activeMenu)
	{
		case MAIN:
			switch(currentIndex)
			{
				case 0:
					drawGamesMenu();
					selectFirstMenuItem();
					temp = true;
					break;
				case 1:
					Display->WriteString(0,0,"Options",0);
					temp = false;
					break;
				case 2:
					drawAboutScreen();
					selectFirstMenuItem();
					temp = true;
					break;
			}
			break;

		case GAMES:
			switch(currentIndex)
			{
				case 0:
					//Display->WriteString(0,0,"Tetris",0);
					_menuChange(Change2Pong);
					drawTetrisScreen();
					temp = true;
					break;
				case 1:
					//Display->WriteString(0,0,"Pong",0);
					_menuChange(Change2Frogger);
					drawBreakoutScreen();
					temp = false;
					break;
				case 2:
					Display->WriteString(0,0,"Snake",0);
					temp = false;
					break;
			}
			break;
	}

	Display->display();
	return temp;
}

void NokiaLCDMenu::goBack()
{
	switch(activeMenu)
	{
		//case MAIN:
		//	drawMainMenu();
		//	break;
		default:
			drawMainMenu();
			selectFirstMenuItem();
			Display->display();
			break;
	}
}

void NokiaLCDMenu::drawBreakoutScreen()
{
	clear();

	activeMenu = BREAKOUT;

	Display->WriteString(0, 0, "Frogger", 0);
	Display->drawLine(0,8,84,8,1);

	Display->WriteString(0, 10, "Score:", 0);
	Display->WriteString(0, 19, String(Score), 0);

	Display->WriteString(0, 28, "Level:", 0);
	Display->WriteString(0, 37, String(Level), 0);

	for (int i = 0; i < 3 - Lives; ++i)
	{
		Display->drawBitmap(60 + i * 8,0,EmptyHeart,8, 7, 1);			
	}

	for (int i = 0; i < Lives; ++i)
	{
		Display->drawBitmap(76 - i*8,0,FullHeart,8, 7, 1);
	}

	//Display->drawBitmap(60,0,FullHeart,8, 7, 1);
	//Display->drawBitmap(64,0,EmptyHeart,8, 7, 1);
}

void NokiaLCDMenu::e_gameChange(int gameEvent) {
	switch (gameEvent) {
		case LifeGained:
			selectMenuItem(true);
			break;

		case LifeLost:
			selectMenuItem(false);
			break;

		case GameLost:
			goBack();
			break;
	}
}
