#pragma once

void ClearArena(COORD Refresh);
int NumberOfWays(char GameZone[][50], int posX, int posY);
int randomNum(int maxNum);
struct reachDest {
	bool shouldReach;
	int x;
	int y;
	int moveTwX;
	int moveTwY;
};
void MoveEnemy(char GameZone[][50], int* lastPosX, int* lastPosY, int* x, int* y, int ArrSize, char enemy, bool wasEmptyPole[], int PointsArena[][50], int SmartAi, reachDest ReachDestination[], int PPosX, int PPosY);
void ShowConsoleCursor(bool showFlag);
void GameOver(COORD Refresh, int size, int points, int realWidth, int realHeight, int *lastvertical, int *lasthorizontal, bool win = false);
void MainMenu();
void SettingsHelpMenu(float &EnemySpeed, int &SmartAI);
void InproperConsoleSize(int realwidth, int realheight);

