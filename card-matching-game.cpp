#include<iostream>
#include<Windows.h>
#include<conio.h>
using namespace std;
#define INIT_POS 1
//-----------------------------------------
// A function that moves the cursor to a specific location inside the console
void GotoXY(int x, int y)
{
	COORD pos;
	pos.X = 2 * x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// A function that manages the console window size and title.
void SetConsoleSize()
{
	system("mode con:cols=65 lines=16");
	system("title Card Matching Game ");
}

// A function detects keaboard input and returns the input keyboard
int GetKeyDown()
{
	if (_kbhit() != 0)
	{
		return _getch();
	}
	return 0;
}
//-----------------------------------------
// A map consisting of two dimensional array
const int originMap[4][4] = { {1, 6, 1, 8},
								{6, 2, 5, 4},
								{7, 2, 5, 4},
								{8, 3, 3, 7} };

// Position structure
struct Pos
{
	int x;
	int y;
};

// enum storing keyboard input

enum KEYBOARD
{
	IS_ARROW = 224,
	UP = 72,
	LEFT = 75,
	RIGHT = 77,
	DOWN = 80,
	SPACE = 32,
};

//enum with colour saved
enum COLOR
{
	GREEN = 10,
	MINT,
	RED,
	PINK,
	YELLOW,
	WHITE,
};

//game manager class
class GameManager
{
private:
	char questionMap[4][4];	// The map we are matching.
	Pos player;				// Player cursor position.
	Pos flipPos[2];			// save the flipped card possition.
	int flipCount;			// Number of cards turned over.
	int spaceCount;			// Number of turns
	int matchedCardCount;	// number of cards matched.
public:
	void StartGame()
	{
		Init();		// Game initialization
		DrawBack();	// Draw background

		// starts the game loop
		int key = 0;
		while (true)
		{
			
			key = GetKeyDown();	// Function to receive key input
			if (key == IS_ARROW)
			{
				//Arrows
				MovePos();
			}
			if (key == SPACE)
			{
				//Sspace
				FlipCard();
			}
			//draw
			DrawMain();
			Sleep(100);
			CheckCard();
		}
	}

	// Map initialization
	void Init()
	{
		for (int y = 0; y < 4; ++y)
		{
			for (int x = 0; x < 4; ++x)
			{
				questionMap[y][x] = '?';	// Set everything with a question mark
			}
		}
		player.x = 0;
		player.y = 0;
		flipCount = 0;
		spaceCount = 0;
		matchedCardCount = 0;
	}

	// Card flip Function
	void FlipCard()
	{
		if (questionMap[player.y][player.x] != '?') return;
		if (flipCount > 2) return;
		flipPos[flipCount].x = player.x;
		flipPos[flipCount].y = player.y;
		questionMap[player.y][player.x] = originMap[player.y][player.x] + '0';
		++flipCount;
	}

	// Move the cursor position
	void MovePos()
	{
		GotoXY(INIT_POS + (player.x * 3), INIT_POS + (player.y * 2) + 1);
		printf("  ");

		// Receives keyboard input and moves to the corresponding location
		switch (_getch())
		{
		case LEFT:
			--player.x;
			if (player.x <= 0)
			{
				player.x = 0;
			}
			break;
		case RIGHT:
			++player.x;
			if (player.x >= 3)
			{
				player.x = 3;
			}
			break;
		case UP:
			--player.y;
			if (player.y <= 0)
			{
				player.y = 0;
			}
			break;
		case DOWN:
			++player.y;
			if (player.y >= 3)
			{
				player.y = 3;
			}
			break;
		}
	}

	void DrawMain()
	{
		// Map draw
		SetTextColor(WHITE);
		char c = '\0';
		for (int y = 0; y < 4; ++y)
		{
			GotoXY(INIT_POS, INIT_POS + (2 * y));
			for (int x = 0; x < 4; ++x)
			{
				c = questionMap[y][x];
				if (c != '?')
				{
					SetTextColor(GREEN);
				}
				printf("[%c]   ", c);
				SetTextColor(WHITE);
			}
		}

		// Player draw
		GotoXY(INIT_POS + (player.x * 3), INIT_POS + (player.y * 2) + 1);
		printf(" ^");

		GotoXY(14, 1);
		printf("Count : %d", spaceCount);
	}

	void CheckCard()
	{
		if (flipCount == 2)
		{
			char c = '?';
			if (originMap[flipPos[0].y][flipPos[0].x] == originMap[flipPos[1].y][flipPos[1].x])
			{
				// When equal
				c = originMap[flipPos[0].y][flipPos[0].x] + '0';
				++matchedCardCount;
			}
			else
			{
				// When different
				Sleep(500);
			}
			questionMap[flipPos[0].y][flipPos[0].x] = c;
			questionMap[flipPos[1].y][flipPos[1].x] = c;
			flipCount = 0;
			++spaceCount;

			if (matchedCardCount >= 8)
			{
				DrawGameClear();
				Sleep(2000);
				system("cls");
				Init();
				DrawBack();
			}
		}
	}

	void DrawBack()
	{
		SetTextColor(MINT);
		GotoXY(14, 2);
		cout << " Number Card Matching Game";
		GotoXY(14, 3);
		SetTextColor(GREEN);
		cout << "by: ";
		SetTextColor(PINK);
		GotoXY(14, 4);
		cout << " kavindu";
		SetTextColor(WHITE);
	}

	void DrawGameClear()
	{
		SetTextColor(YELLOW);
		GotoXY(10, 3);
		cout << "=========================";
		GotoXY(10, 4);
		cout << "=======  YOU WON  =======";
		GotoXY(10, 5);
		cout << "=========================";
		SetTextColor(WHITE);
	}

	void SetTextColor(int color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
};

// main function
int main(void)
{

	SetConsoleSize();
	GameManager* pGameMgr = new GameManager();
	pGameMgr->StartGame();
	delete(pGameMgr);
	return 0;
}
