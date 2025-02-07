#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <thread>
#include <iomanip>
#include "PFunctions.h"
using namespace std;
void ClearArena(COORD Refresh) { //wyczyść planszę
	system("CLS");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

}

int NumberOfWays(char GameZone[][50], int posX, int posY) { //ile jest możliwych kierunków, w które można się udać
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

int randomNum(int maxNum) { //losowy numer od 0 do maxNum
	srand((unsigned)time(NULL));
	int random = rand() % maxNum;
	return random;
}

//steruj przeciwnikami
void MoveEnemy(char GameZone[][50], int* lastPosX, int* lastPosY, int* x, int* y, int ArrSize, char enemy, bool wasEmptyPole[], int PointsArena[][50], int SmartAi, reachDest ReachDestination[], int PPosX, int PPosY) { 

	for (int i = 0; i < ArrSize; i++) { //dla każdego przeciwnika na mapie
		int x1 = *(x + i);
		int y1 = *(y + i);
		int lx = *(lastPosX + i);
		int ly = *(lastPosY + i);
		int iniPosX = *(x + i);
		int iniPosY = *(y + i);

		bool foundPlayer = false;

		//////////////////////////////////////////////////////////
		//dodatkowe sterowanie przeciwnikami (SmartAI, SmartAI+)//
		//////////////////////////////////////////////////////////

		if (SmartAi == 2) { //jeżeli jest włączone SmartAI+ 

			int tempx = *(x + i);
			int tempy = *(y + i);

			while (GameZone[tempx][tempy] != '#' && GameZone[tempx][tempy] != '=') { //przeciwnik nie może widzieć przez ściany
				if (GameZone[tempx][tempy] == 'o') { //szukaj gracza w danym kierunku
					//poruszaj się w stronę gracza
					ReachDestination[i].moveTwY = 0;
					ReachDestination[i].moveTwX = 1;
					x1 = iniPosX + 1;
					ReachDestination[i].x = PPosX; //jeśli przeciwnik stracił z oczu gracza, to zapisz pozycję,
					ReachDestination[i].y = PPosY; //w której ostatni raz gracz był widziany
					ReachDestination[i].shouldReach = true; //przeciwnik ma ustawiony cel, który ma osiągnąć
					foundPlayer = true;
					break;
				}
				tempx++;
			}
			tempx = *(x + i);
			tempy = *(y + i);
			while (GameZone[tempx][tempy] != '#' && GameZone[tempx][tempy] != '=') { //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') {
					ReachDestination[i].moveTwY = 0;
					ReachDestination[i].moveTwX = -1;
					x1 = iniPosX - 1;
					ReachDestination[i].x = PPosX;
					ReachDestination[i].y = PPosY;
					ReachDestination[i].shouldReach = true;
					foundPlayer = true;
					break;
				}
				tempx--;
			}
			tempx = *(x + i);
			tempy = *(y + i);
			while (GameZone[tempx][tempy] != '#' && GameZone[tempx][tempy] != '=') { //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') {
					ReachDestination[i].moveTwY = 1; 
					ReachDestination[i].moveTwX = 0;
					y1 = iniPosY + 1;
					ReachDestination[i].x = PPosX;
					ReachDestination[i].y = PPosY;
					ReachDestination[i].shouldReach = true;
					foundPlayer = true;
					break;
				}
				tempy++;
			}
			tempx = *(x + i);
			tempy = *(y + i);
			while (GameZone[tempx][tempy] != '#' && GameZone[tempx][tempy] != '=') { //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') {
					ReachDestination[i].moveTwY = -1;
					ReachDestination[i].moveTwX = 0;
					y1 = iniPosY - 1;
					ReachDestination[i].x = PPosX;
					ReachDestination[i].y = PPosY;
					ReachDestination[i].shouldReach = true;
					foundPlayer = true;
					break;
				}
				tempy--;
			}
			//---------------//
			if (foundPlayer == false) { //jeżeli nie znaleziono gracza...
				if (ReachDestination[i].shouldReach && (ReachDestination[i].x != x1 || ReachDestination[i].y != y1)) { //jeżeli przeciwnik ma ustawiony cel i do niego jeszcze nie dotarł
					foundPlayer = true; //nie wykonuj pozostałych instrukcji z poruszaniem przeciwnika
					x1 = iniPosX + ReachDestination[i].moveTwX;
					y1 = iniPosY + ReachDestination[i].moveTwY;

				}
				else if (ReachDestination[i].shouldReach && ReachDestination[i].x == x1 && ReachDestination[i].y == y1) { //jeżeli dotarł do celu
					ReachDestination[i].shouldReach = false; //usuń cel
				}
			}

			if (ReachDestination[i].shouldReach) { 
				foundPlayer = true; //nie wykonuj pozostałych instrukcji z poruszaniem przeciwnika
			}
		}

		else if (SmartAi == 1) { //jeżeli jest włączone SmartAI  
			int tempx = *(x + i);
			int tempy = *(y + i);

			while (GameZone[tempx][tempy] != '#' && GameZone[tempx][tempy] != '=') {  //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') { //jeżeli przeciwnik widzi gracza to przeciwnik porusza się w jego kierunku
					x1 = iniPosX + 1; 
					foundPlayer = true;
					break;
				}
				tempx++;
			}
			tempx = *(x + i);
			tempy = *(y + i);
			while (GameZone[tempx][tempy] != '#' && GameZone[tempx][tempy] != '=' && foundPlayer == false) {  //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') {
					x1 = iniPosX - 1;
					foundPlayer = true;
					break;
				}
				tempx--;
			}
			tempx = *(x + i);
			tempy = *(y + i);
			while (GameZone[tempx][tempy] != '#'  && GameZone[tempx][tempy] != '=' && foundPlayer == false) {  //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') {
					y1 = iniPosY + 1;
					foundPlayer = true;
					break;
				}
				tempy++;
			}
			tempx = *(x + i);
			tempy = *(y + i);
			while (GameZone[tempx][tempy] != '#'  && GameZone[tempx][tempy] != '=' && foundPlayer == false) { //szukaj gracza w danym kierunku
				if (GameZone[tempx][tempy] == 'o') { 
					y1 = iniPosY - 1;
					foundPlayer = true;
					break;
				}
				tempy--;
			}

		}
		
		///////////////////////////////////////
		//podstawowe sterowanie przeciwnikami//
		///////////////////////////////////////
		
		if (foundPlayer == false) {
			if (NumberOfWays(GameZone, *(x + i), *(y + i)) < 3) { //jeżeli możliwych ścieżek jest < 3 to wybierz różną od tej, z której przeciwnik przyszedł
				if (GameZone[*(x + i) - 1][*(y + i)] != '#' && GameZone[*(x + i) - 1][*(y + i)] != '=' && *(x + i) - 1 != lx && PointsArena[*(x + i) - 1][*(y + i)] != 2) { //can go left?
					x1 = iniPosX - 1;
				}
				else if (GameZone[*(x + i) + 1][*(y + i)] != '#' && GameZone[*(x + i) + 1][*(y + i)] != '=' && *(x + i) + 1 != lx && PointsArena[*(x + i) + 1][*(y + i)] != 2) { //can go right?
					x1 = iniPosX + 1;
				}
				else if (GameZone[*(x + i)][*(y + i) - 1] != '#' && GameZone[*(x + i)][*(y + i) - 1] != '=' && *(y + i) - 1 != ly && PointsArena[*(x + i)][*(y + i) - 1] != 2) { //can go up?
					y1 = iniPosY - 1;
				}
				else if (GameZone[*(x + i)][*(y + i) + 1] != '#' && GameZone[*(x + i)][*(y + i) + 1] != '=' && *(y + i) + 1 != ly && PointsArena[*(x + i)][*(y + i) + 1] != 2) { //can go down?
					y1 = iniPosY + 1;
				}
			}

			else { //wykryto skrzyżowanie 
				int possibleways[4] = { 8,8,8,8 }; //8 - w tą drogę nie da się udać, 9 - możliwe drogi do wybrania
				int posscount = 0;
				if (GameZone[*(x + i) - 1][*(y + i)] != '#' && GameZone[*(x + i) - 1][*(y + i)] != '=' && *(x + i) - 1 != lx) {
					possibleways[0] = 9;
					posscount++;
				}
				if (GameZone[*(x + i) + 1][*(y + i)] != '#' && GameZone[*(x + i) + 1][*(y + i)] != '=' && *(x + i) + 1 != lx) {
					possibleways[1] = 9;
					posscount++;
				}
				if (GameZone[*(x + i)][*(y + i) - 1] != '#' && GameZone[*(x + i)][*(y + i) - 1] != '=' && *(y + i) - 1 != ly) {
					possibleways[2] = 9;
					posscount++;
				}
				if (GameZone[*(x + i)][*(y + i) + 1] != '#' && GameZone[*(x + i)][*(y + i) + 1] != '=' && *(y + i) + 1 != ly) {
					possibleways[3] = 9;
					posscount++;
				}
				//cout << posscount;
				int PickPath = randomNum(posscount); //wylosuj drogę z przedziału [0 ; ilość możliwych dróg]
				int PickedPath; 
				int k = 0;
				for (int i = 0; i < 4; i++) {
					if (possibleways[i] == 9) { //jeżeli droga jest możliwa...
						possibleways[i] = k; //przydziel jej numer: 0,1,2 itd
						if (PickPath == possibleways[i]) { //jeżeli numer drogi == wylosowanemu numerowi 
							PickedPath = i; //wybrano daną drogę
							break;
						}
						k++;
					
					}
				}

				switch (PickedPath) { //porusz przeciwnika w wybraną drogę
				case 0:
					x1 = iniPosX - 1;
					break;
				case 1:
					x1 = iniPosX + 1;
					break;
				case 2:
					y1 = iniPosY - 1;
					break;
				case 3:
					y1 = iniPosY + 1;
					break;
				}
			}
		}


		if (wasEmptyPole[i]) { //jeżeli pole wcześniej było puste, zostaw je puste 
			GameZone[iniPosX][iniPosY] = ' ';
		}
		else
		{
			GameZone[iniPosX][iniPosY] = '.'; //w innym wypadku pozostaw punkt (.)
		}

		if (PointsArena[x1][y1] == 0) { //jeżeli na tym samym indeksie w tablicy PointsArena nie było punktu, to znaczy, że pole było puste
			wasEmptyPole[i] = true;
		}
		else {
			wasEmptyPole[i] = false;
		}
		*(x + i) = x1;
		*(y + i) = y1;
		GameZone[x1][y1] = enemy;
		*(lastPosX + i) = iniPosX;
		*(lastPosY + i) = iniPosY;

	}
	this_thread::sleep_for(5ms);
}

void ShowConsoleCursor(bool hideCursor) //schowaj kursor
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = hideCursor;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void GameOver(COORD Refresh, int size, int points, int realWidth, int realHeight, int *lastvertical, int *lasthorizontal, bool win) { //wygrana / przegrana
	ClearArena(Refresh);
	if (win) {

		cout << "Congratulations! You won! Press enter to continue." << endl;
		cout << "SCORE: " << points << "\n\n\n";
	}
	else {

		cout << "You lost :(. Press enter to continue." << endl;
		cout << "SCORE: " << points << "\n\n\n";

	}

	do {
		char k = _getch();
		if (k == 13) {
			ClearArena(Refresh);
			break;
		}
	} while (_getch() != 13);
}

void MainMenu() { //rysuj główne menu
	cout << " ______                                              " << endl;
	cout << "|   __ \\.---.-.----.--------.-----.-----.--.--.-----." << endl;
	cout << "|    __/|  _  |  __|        |  _  |  _  |  |  |__ --|" << endl;
	cout << "|___|   |___._|____|__|__|__|_____|___  |_____|_____|" << endl;
	cout << "                                  |_____|            \n" << endl;
	cout << "Click the key on your keyboard or ESC to exit:\n";
	cout << " _____ " << endl;
	cout << "|| 1 ||" << endl;
	cout << "||___|| \t1 - Play" << endl;
	cout << "|/___\\|" << endl;
	cout << " _____ " << endl;
	cout << "|| 2 ||" << endl;
	cout << "||___|| \t2 - Difficulty settings" << endl;
	cout << "|/___\\|" << endl;
	cout << " _____ " << endl;
	cout << "|| 3 ||" << endl;
	cout << "||___|| \t3 - Help" << endl;
	cout << "|/___\\|" << endl;
}

void SettingsHelpMenu(float &EnemySpeed, int &SmartAI) { //ustawienia i pomoc
	char k;
	do {
		k = tolower(_getch());
		switch (k) { //wyświetlenie pomocy
		case '3':
			system("CLS");
			cout << "Move as Player - o" << endl;
			cout << "Collect all points - ." << endl;
			cout << "Avoid Enemies - X" << endl;
			cout << "When all points are collected, door will open" << endl;
			cout << "Go into door to win" << endl;
			cout << "Controls:\n";
			cout << " _____ " << endl;
			cout << "|| W ||" << endl;
			cout << "||___|| \tW - up" << endl;
			cout << "|/___\\|" << endl;
			cout << " _____ " << endl;
			cout << "|| A ||" << endl;
			cout << "||___|| \tA - left" << endl;
			cout << "|/___\\|" << endl;
			cout << " _____ " << endl;
			cout << "|| S ||" << endl;
			cout << "||___|| \tS - down" << endl;
			cout << "|/___\\|" << endl;
			cout << " _____ " << endl;
			cout << "|| D ||" << endl;
			cout << "||___|| \tD - right" << endl;
			cout << "|/___\\|" << endl;

			system("pause");
			system("cls");


			break;
		case '2': //wyświetlenie ustawień
			system("CLS");
			cout << "Click the key on your keyboard or ESC to return:\n";
			cout << " _____ " << endl;
			cout << "|| 1 ||" << endl;
			cout << "||___|| \t1 - AI settings" << endl;
			cout << "|/___\\|" << endl;
			cout << " _____ " << endl;
			cout << "|| 2 ||" << endl;
			cout << "||___|| \t2 - Enemy speed" << endl;
			cout << "|/___\\|" << endl;

			char k;
			do { 
				k = tolower(_getch());
				if (k == '1') { //ustawienie AI
					system("CLS");
					cout << "Press Enter to switch SmartAI or ESC to return\n";
					cout << "AI supports 3 modes | No SmartAI is default |: No SmartAI, SmartAI, SmartAI+\n";
					cout << "No SmartAI - bots picks random path always.\nSmartAI - bots will follow the player as long as he is visible to them\nSmart AI+ - enemies will follow the player + go to his last position he was visible\n";
					cout << " ________ " << endl;
					cout << "|| ENTER ||" << endl;
					cout << "||_______|| \tENTER - Switch SmartAI" << endl;
					cout << "|/_______\\|\n" << endl;
					char k1;
					do {
						k1 = tolower(_getch());
						if (k1 == 13) {

							SmartAI += 1;
							if (SmartAI > 2) {
								SmartAI = 0;
							}

							if (SmartAI == 0) {
								cout << "SmartAI turned off \r";
							}
							else if (SmartAI == 1) {
								cout << "Turned ON  SmartAI \r";
							}
							else {
								cout << "Turned ON  SmartAI+\r";
							}
						}
					} while (k1 != 27);
					system("cls");
					cout << "Click the key on your keyboard or ESC to return:\n";
					cout << " _____ " << endl;
					cout << "|| 1 ||" << endl;
					cout << "||___|| \t1 - AI settings" << endl;
					cout << "|/___\\|" << endl;
					cout << " _____ " << endl;
					cout << "|| 2 ||" << endl;
					cout << "||___|| \t2 - Enemy speed" << endl;
					cout << "|/___\\|" << endl;
				}

				else if (k == '2') { //prędkość przeciwników
					system("CLS");
					cout << "Press Enter to switch Enemy Speed or ESC to return\n";
					cout << "There are 4 speeds | Medium is default |: very slow, slow, medium, fast, very fast\n";
					cout << " ________ " << endl;
					cout << "|| ENTER ||" << endl;
					cout << "||_______|| \tENTER - Switch Enemy Speed" << endl;
					cout << "|/_______\\|\n" << endl;
					int number = 2;
					char k1;
					do {
						k1 = tolower(_getch());
						if (k1 == 13)
						{
							number += 1;
							if (number > 4) {
								number = 0;
							}
							if (number == 0) {
								EnemySpeed = 3;
								cout << "Very Slow\r";
							}
							else if (number == 1) {
								EnemySpeed = 2;
								cout << "Slow     \r";
							}
							else if (number == 2) {
								EnemySpeed = 1.5;
								cout << "Medium   \r";
							}
							else if (number == 3) {
								EnemySpeed = 1;
								cout << "Fast     \r";
							}
							else if (number == 4) {
								EnemySpeed = 0.5;
								cout << "Very Fast\r";
							}
						}
					} while (k1 != 27);
					system("cls");
					cout << "Click the key on your keyboard or ESC to return:\n";
					cout << " _____ " << endl;
					cout << "|| 1 ||" << endl;
					cout << "||___|| \t1 - AI settings" << endl;
					cout << "|/___\\|" << endl;
					cout << " _____ " << endl;
					cout << "|| 2 ||" << endl;
					cout << "||___|| \t2 - Enemy speed" << endl;
					cout << "|/___\\|" << endl;
				}
			} while (k != 27);

			system("cls");
			break;

		case 27:
			EnemySpeed = -1;
			break;

		default:
			system("cls");
			break;


		}

		if (EnemySpeed == -1) {
			break;
		}
		MainMenu();



	} while (k != '1');
}

void InproperConsoleSize(int realwidth, int realheight) //czuwanie nad prawidłowym rozmiarem konsoli
{

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	bool changed = false;

	if (columns < realwidth || rows < realheight + 4) { 
		system("cls");
	}

	while (columns < realwidth || rows < realheight + 4) { //jeżeli konsola jest zbyt mała by wyświetlić prawidłowo planszę
		ShowConsoleCursor(false);
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
		changed = true;
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		cout << columns << " / " << realwidth << "    " << rows << " / " << realheight + 4 << endl; //wypisz obecne i wymagane rozmiary
		cout << "Please adjust console size or lower font size to continue."; //wypisz komunikat
	}

	if (changed) {
		system("cls");
	}
}