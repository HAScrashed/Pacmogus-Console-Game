// LevelEditor.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include "LEFunctions.h"
using namespace std;


int main()
{
	while (true) {
		ShowConsoleCursor(false); //ukryj kursor
		COORD Refresh;
		Refresh.X = 0;
		Refresh.Y = 0;
		bool IsMapValidated = false; 
		int Insert = 0;

		int size = SelectSize(); //wybór rozmiaru planszy
		if (size == 0) { //umowna wartość, oznacza, że gracz opuścił edytor 
			break;
		}
		int playSize = size - 1;
		char CharSet[] = { '-','A','=','V' }; //z tych znaków można budować planszę

		char Arena[50][50]{};
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (j == size - 1) {
					Arena[i][j] = 'E'; //ustaw stałe zakończenia linii
				}
				else {
					Arena[i][j] = '#'; //bazowo ustaw #
				}
				if (i == size - 1 && j == size - 1) {
					Arena[i][j] = 'F'; // ustaw stałe zakończenia wierszy
				}
			}
		}

		Coords Sel = { 1,1 }; //bazowo ustaw selekcję
		
		////////////////
		//główna pętla//
		////////////////
		
		while (true) {
			ClearArena(Refresh);
			cout << "Press ESC if you want to return.\n";
			ShowConsoleCursor(false);
			ShowArena(Arena, size, Sel, Insert, CharSet); //pokaż planszę
			cout << "\nSelected: ";
			switch (CharSet[Insert]) { //pokaż aktualnie wybrany element do wstawiania
			case '-':
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "Corridor    ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				break;
			case 'A':
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "Player Spawn";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				break;
			case '=':
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "Door        ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				break;
			case 'V':
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
				cout << "Enemy Spawn ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				break;

			}
			cout << endl;
			cout << endl;
			cout << "P: validate (& save) the map | WASD: move | Q,E: change block | Enter: place block | Backspace: #\nRequirements: " << endl; //instrukcje
			cout << "Player Spawns:1\nDoors:1\nCorridors width: 1\nNo dead ends\nEnemy Spawns: 0-4\n"; //wymogi


			char pressedkey = tolower(_getch());
			Sel = Select(Sel, playSize, pressedkey, Arena, Insert, CharSet); //ustaw wskaźnik na danej pozycji
			ClearArena(Refresh);
			if (pressedkey == 'p') {
				system("cls");
				bool a = CorrectPathWidth(size, Arena); //czy korytarze mają prawidłową wielkość
				bool b = CorrectNumbers(size, Arena); //czy liczby elementów są prawidłowe
				bool c = NoDeadEnds(size, Arena); //czy nie ma ślepych zaułków
				cout << endl;
				cout << "Does arena have correct path width?: ";
				if (!a) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); }
				cout << boolalpha << a;
				if (!a) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }
				cout << endl;
				cout << "Does arena have correct number of Spawns/Enemies/Doors?: ";
				if (!b) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); }
				cout << boolalpha << b;
				if (!b) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }
				cout << endl;
				cout << "Is arena safe from dead ends?: ";
				if (!c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); }
				cout << boolalpha << c;
				if (!c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }
				cout << endl;
				if (a && b && c) { //jeśli wszystko jest w porządku 
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					cout << "\nPress P again to save the map";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					char key = tolower(_getch());
					if (key == 'p') { //jeśli gracz chce zapisać planszę
						fstream map;
						map.open("CustomLevel.txt", ios::out);
						if(map.good()) {
							for (int i = 0; i < size; i++) { //zapisz planszę do pliku
								for (int j = 0; j < size; j++) {
									map << Arena[i][j];
								}
								map << "\n";
							}
							map.close();
						} else {
							cout << "\nBLAD ZAPISU"<< endl;
						}
		
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						cout << "\nMap saved\n";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						system("pause");
						system("cls");
					}
					else {
						system("cls");
					}
				}
				else { //jeśli nie wszystkie wymogi są spełnione, zabroń zapisu
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					cout << "\nMap is not valid & can't be saved. ";
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					system("pause");
					system("cls");

				}
				
			}
			else if (pressedkey == 27) {
				break;
			}
		}
	}
	
}

