#pragma once
struct Coords {
	int x;
	int y;
};

void ShowConsoleCursor(bool showFlag);
int NumberOfWays(char GameZone[][50], int posX, int posY);
void ClearArena(COORD Refresh);
bool CorrectPathWidth(int size, char tab[][50]);
bool NoDeadEnds(int size, char tab[][50]);
bool CorrectNumbers(int size, char tab[][50]);
void ShowArena(char Arena[50][50], int size, Coords Selected, int Insert, char CharSet[]);
int SelectSize();
void Edit(Coords Pos, char key, char Arena[50][50], int Insert, char CharSet[]);
Coords Select(Coords Pos, int size, char key, char Arena[][50], int &Insert, char CharSet[]);