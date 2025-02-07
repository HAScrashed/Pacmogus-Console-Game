#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <conio.h>
#include <iomanip>
#include <time.h>
#include "LEFunctions.h"
using namespace std;


void ShowConsoleCursor(bool showFlag) //wskaźnik konsolowy
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}

int NumberOfWays(char GameZone[][50], int posX, int posY) { //ile jest możliwych dróg do wybrania z danego pola
	int NumberOfWays = 1;
	if (GameZone[posX - 1][posY] != '#' && GameZone[posX - 1][posY] != '=') {
		NumberOfWays++;
	}
	if (GameZone[posX + 1][posY] != '#' && GameZone[posX + 1][posY] != '=') {
		NumberOfWays++;
	}
	if (GameZone[posX][posY - 1] != '#' && GameZone[posX][posY - 1] != '=') {
		NumberOfWays++;
	}
	if (GameZone[posX][posY + 1] != '#' && GameZone[posX][posY + 1] != '=') {
		NumberOfWays++;
	}
	NumberOfWays--;
	return NumberOfWays;
}

void ClearArena(COORD Refresh) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Refresh);
}

bool CorrectPathWidth(int size, char tab[][50]) //czy wielkość ścieżek jest prawidłowa
{
	//sprawdzaj planszę poprzez elementy sąsiednie
	/*
	|------------|--------------|    |-|-|
	| tab[n][m]  | tab[n][m+1]  |    | | |
	|------------|--------------| -> |-|-|
	|tan[n+1][m] | tab[n+1][m+1]|    | | |
	|------------|--------------|    |-|-|
	
	jeśli napotka się takie ustawienie pól, że wszystkie elementy tych pól będą równe spacji, to znaczy, że szerokość jest nieprawidłowa	
	*/
	
	
	for (int i = 0; i < size - 1; i++) {

		for (int j = 0; j < size - 1; j++)
		{
			int licznik = 0;
			if (tab[i][j] == '-' || tab[i][j] == 'A' || tab[i][j] == 'V') {
				licznik++;
			}
			if (tab[i + 1][j] == '-' || tab[i + 1][j] == 'A' || tab[i + 1][j] == 'V') {
				licznik++;
			}
			if (tab[i][j + 1] == '-' || tab[i][j + 1] == 'A' || tab[i][j + 1] == 'V')
			{
				licznik++;
			}
			if (tab[i + 1][j + 1] == '-' || tab[i + 1][j + 1] == 'A' || tab[i + 1][j + 1] == 'V') {
				licznik++;
			}
			if (licznik == 4) {
				return false;
			}
		}
	}
	return true;
}

bool NoDeadEnds(int size, char tab[][50]) //sprawdź czy plansza ma ślepe ścieżki
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (tab[i][j] == '-' || tab[i][j] == 'V' || tab[i][j] == 'A')
			{
				//numberofways<2 -> return false
				if (NumberOfWays(tab, i, j) < 2) { //dla każdego sprawdzanego elementu -, v, A, pozycja na którego stoją musi mieć co najmniej 2 drogi wyjścia
					return false;
				}
			}
		}
	}
	return true;

}

bool CorrectNumbers(int size, char tab[][50]) //sprawdź liczby drzwi, wrogów, graczy
{
	int doors = 0;
	int spawns = 0;
	int enemies = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (tab[i][j] == 'A')
			{
				spawns++;
			}

			if (tab[i][j] == 'V')
			{
				enemies++;
			}

			if (tab[i][j] == '=')
			{
				doors++;
			}

			if (enemies > 4 || spawns > 1 || doors > 1)
			{
				return false;
			}
		}

	}
	if (spawns == 0 || doors == 0)
	{
		return false;
	}
	return true;
}

void ShowArena(char Arena[50][50], int size, Coords Selected, int Insert, char CharSet[]) { //pokaż planszę 
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (Selected.x == i && Selected.y == j) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); //wyróżnij aktualnie edytowaną pozycję
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			cout << Arena[i][j];
			if (i == 0 && j == size - 1) { //wyróżnij aktualnie wybrany element 
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "   " << CharSet[Insert];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
		cout << endl;
	}

}

int SelectSize() { //wprowadzenie rozmiaru planszy

	bool Choose = true;
	vector<int> size;

	char pressedkey;
	int realsize = 0;

	while (true) {
		system("cls");
		cout << "Press ESC to leave." << endl;
		cout << "Max Arena size is 46 x 46. Minimum is 3x3. Enter Arena size.\n";
		cout << "!Some higher values would require to reduce console font size to prevent arena flickering!\n";
		cout << "Arena size: ";

		for (int i = 0; i < size.size(); i++) {
			cout << size[i];
		}


		pressedkey = _getch();
		if (pressedkey > 47 && pressedkey < 58 && size.size() < 2) { //mogą zostać pobrane z klawiatury tylko liczby
			size.push_back(pressedkey - 48); //wprowadź liczbę do wektora
		}

		else if (pressedkey == 8) { //jeśli backspace, usuń liczbę z wektora
			if (!size.empty()) {
				size.pop_back();
			}
		}

		if (pressedkey == 27) {
			return 0;
		}

		else if (pressedkey == 13) { //jeśli enter, przekonwertuj liczby z wektora na pełną liczbę 
			realsize = 0;
			for (int i = 0; i < size.size(); i++) { //...10^1 + 10^0 
				realsize += pow(10, size.size() - 1 - i) * size[i];
			}
			if (realsize > 2 && realsize < 47) {
				realsize += 3;
				size.clear();
				system("cls");
				break;
			}
		}







	}

	return realsize;


}

void Edit(Coords Pos, char key, char Arena[50][50], int Insert, char CharSet[]) { //edytuj pole



	switch (key) {

	case 13:
		Arena[Pos.x][Pos.y] = CharSet[Insert];
		break;
	case 8:
		Arena[Pos.x][Pos.y] = '#';
		break;

	}
}

Coords Select(Coords Pos, int size, char key, char Arena[][50], int &Insert, char CharSet[]) { //wybierz konkretne pole do edycji na planszy
	if (key == 's' && Pos.x + 1 != size) {
		return { Pos.x + 1, Pos.y };
	}
	else if (key == 'w' && Pos.x - 1 != 0) {
		return { Pos.x - 1, Pos.y };
	}
	else if (key == 'd' && Pos.y + 1 != size - 1) {
		return { Pos.x, Pos.y + 1 };
	}
	else if (key == 'a' && Pos.y - 1 != 0) {
		return { Pos.x , Pos.y - 1 };
	}
	else if (key == 'q') {
		Insert--;
		if (Insert < 0) {
			Insert = 3;
		}
		return Pos;
	}
	else if (key == 'e') {
		Insert++;
		if (Insert > 3) {
			Insert = 0;
		}
		return Pos;
	}
	else {

		Edit(Pos, key, Arena, Insert, CharSet);
		return Pos;
	}
}
