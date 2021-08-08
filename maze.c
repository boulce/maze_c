#define LENGTH 10
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct _Room
{
	short visit;
}Room;

typedef struct _Path
{
	int x;
	int y;
}Path;

Room room[LENGTH][LENGTH] = { {FALSE}, };
Room bgrd[2 * LENGTH + 1][2 * LENGTH + 1] = { {FALSE}, };
Path path[LENGTH * LENGTH] = { {0, 0}, };

void Visit_Room(int x, int y);

void gotoxy(int x, int y) //gotoxy 함수
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			break;
		}
	}

	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < LENGTH; j++)
		{
			bgrd[2 * i + 1][2 * j + 1].visit = TRUE;
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
		gotoxy(0, 0);
		Sleep(50);

		for (int m = 0; m < 2*LENGTH+1; m++)
		{
			for (int n = 0; n < 2 * LENGTH + 1; n++)
			{
				if (bgrd[m][n].visit == TRUE)
				{
					if ((float)(m -1)/2 == current_y && (float)(n-1)/2 == current_x)
					{
						printf("★");
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
				bgrd[2 * previous_y + 1 + move_y][2 * previous_x + 1].visit = TRUE;
			}
			else
			{
				bgrd[2 * previous_y + 1][2 * previous_x + 1 + move_x].visit = TRUE;
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

	return 0;
}

void Visit_Room(int x, int y)
{
	room[y][x].visit = TRUE;
}