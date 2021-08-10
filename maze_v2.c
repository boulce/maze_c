#define LENGTH 20
#define TRUE 1
#define FALSE 0

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define RED 4 
#define YELLOW 14
#define GREEN 10
#define WHITE 15

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

typedef struct _Room
{
	short visit;
}Room;

typedef struct _Map
{
	short empty;
	short visit;
}Map;

typedef struct _Path
{
	int x;
	int y;
}Path;

Room room[LENGTH][LENGTH] = { {FALSE}, };
Map bgrd[2 * LENGTH + 1][2 * LENGTH + 1] = { {FALSE, FALSE}, };
Path path[LENGTH * LENGTH] = { {0, 0}, };

void Make_Map();
void Visit_Room(int x, int y);
void RemoveCursor();
int getKeyDown();
void Display(int player_x, int player_y, int goal_x, int goal_y);
void Player();

void FontColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main()
{
	system("mode con cols=82lines=42 | title 미로찾기");
	RemoveCursor();
	Make_Map();
	Player();

	return 0;
}


void Visit_Room(int x, int y)
{
	room[y][x].visit = TRUE;
}

void Make_Map()
{
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH; j++)
		{
			bgrd[2 * i + 1][2 * j + 1].empty = TRUE;
		}
	}

	int path_count = 0;

	srand((long)time(NULL));

	int current_x = rand() % LENGTH;
	int current_y = rand() % LENGTH;
	int previous_x;
	int previous_y;
	
	Visit_Room(current_x, current_y);
	path[path_count].x = current_x;
	path[path_count].y = current_y;
	path_count++;

	// 반복
	// 방 랜덤 선택

	while(path_count != 0)
	{ 	
		short available_room = 0;
		int i = 0;
		Path rand_room_list[4] = { {0,0}, };
		previous_x = current_x, previous_y = current_y;

		if (current_x > 0 && room[current_y][current_x - 1].visit == FALSE) //왼쪽 검사
		{
			rand_room_list[i].x = current_x - 1, rand_room_list[i].y = current_y;
			available_room++;
			i++;
		}
		if (current_x < LENGTH - 1 && room[current_y][current_x + 1].visit == FALSE) //오른쪽 검사
		{
			rand_room_list[i].x = current_x + 1, rand_room_list[i].y = current_y;
			available_room++;
			i++;
		}
		if (current_y > 0 && room[current_y - 1][current_x].visit == FALSE) //위쪽 검사
		{
			rand_room_list[i].x = current_x, rand_room_list[i].y = current_y - 1;
			available_room++;
			i++;
		}
		if (current_y < LENGTH - 1 && room[current_y + 1][current_x].visit == FALSE) //아래쪽 검사
		{
			rand_room_list[i].x = current_x, rand_room_list[i].y = current_y + 1;
			available_room++;
			i++;
		}

		if (available_room != 0)
		{
			short rand_num = rand() % available_room;
			current_x = rand_room_list[rand_num].x, current_y = rand_room_list[rand_num].y;

			Visit_Room(current_x, current_y);
			path[path_count].x = current_x;
			path[path_count].y = current_y;
			path_count++;

			int move_x = current_x - previous_x, move_y = current_y - previous_y;

			if (move_x == 0)
			{
				bgrd[2 * previous_y + 1 + move_y][2 * previous_x + 1].empty = TRUE;
			}
			else
			{
				bgrd[2 * previous_y + 1][2 * previous_x + 1 + move_x].empty = TRUE;
			}
		}
		else 
		{
			path[path_count].x = 0;
			path[path_count].y = 0;
			path_count--;
			current_x = path[path_count].x, current_y = path[path_count].y;
		}
	}
}
void RemoveCursor() //커서깜빡이 제거
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; 
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

}

int getKeyDown()
{
	if (_kbhit())
	{
		return _getch();
	}
	else return -1;
}

void Display(int player_x, int player_y, int goal_x, int goal_y)
{
	gotoxy(0, 0);
	for (int m = 0; m < 2 * LENGTH + 1; m++)
	{
		for (int n = 0; n < 2 * LENGTH + 1; n++)
		{
			if (bgrd[m][n].empty == TRUE)
			{
				if (m == player_y && n == player_x)
				{
					FontColor(GREEN);
					printf("＠");
					FontColor(WHITE);
				}
				else if (m == goal_y && n == goal_x)
				{
					FontColor(YELLOW);
					printf("※");
					FontColor(WHITE);
				}
				else if (bgrd[m][n].visit == TRUE)
				{
					printf("·");
				}
				else
				{
					printf("  ");
				}
			}
			else
			{
				printf("■");
			}
		}
		printf("\n");
	}
}

void Player()
{
	int player_x = 1, player_y = 1;
	int goal_x = 2 * LENGTH - 1, goal_y = 2 * LENGTH - 1;
	bgrd[player_y][player_x].visit = TRUE;
	Display(player_x, player_y, goal_x, goal_y);

	while (player_x != goal_x || player_y != goal_y)
	{
		int key = getKeyDown();

		if (key == UP)
		{
			if (bgrd[player_y - 1][player_x].empty == TRUE)
			{
				player_y--;
				bgrd[player_y][player_x].visit = TRUE;
				Display(player_x, player_y, goal_x, goal_y);
			}
		}
		else if (key == DOWN)
		{
			if (bgrd[player_y + 1][player_x].empty == TRUE)
			{
				player_y++;
				bgrd[player_y][player_x].visit = TRUE;
				Display(player_x, player_y, goal_x, goal_y);
			}
		}
		else if (key == LEFT)
		{
			if (bgrd[player_y][player_x - 1].empty == TRUE)
			{
				player_x--;
				bgrd[player_y][player_x].visit = TRUE;
				Display(player_x, player_y, goal_x, goal_y);
			}
		}
		else if (key == RIGHT)
		{
			if (bgrd[player_y][player_x + 1].empty == TRUE)
			{
				player_x++;
				bgrd[player_y][player_x].visit = TRUE;
				Display(player_x, player_y, goal_x, goal_y);
			}
		}
	}
	printf("도착!");
}