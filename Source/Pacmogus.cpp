#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <iomanip>
#include "PFunctions.h"

using namespace std;



int main()
{
	/////////
	//stałe//
	/////////

	const int size = 50; //maksymalny rozmiar
	const char Player = 'o';
	const char Enemy = 'x';

	///////////////////////////
	//obsługa wyglądu konsoli//
	///////////////////////////


	COORD Refresh; //ustawienie wskaźnika na pozycję 0,0 by odświeżać planszę
	Refresh.X = 0; 
	Refresh.Y = 0; 
	int lastH = 0; //ostatnie rozmiary okna konsoli
	int lastV = 0;
	int *LastHorizontal = &lastH;
	int *LastVertical = &lastV;
	ShowConsoleCursor(false); //schowaj kursor
	SetConsoleTitleA("Pacmogus"); //zmiana nazwy konsoli
	
	bool doitlater = false; 
	bool StillPlaying = true;
	int SmartAI = 0; //bazowe wartości SI przeciwników
	float EnemySpeed = 1.5; //bazowe wartości SI przeciwników

	////////////////
	//główna pętla//
	////////////////

	while (StillPlaying) //póki gracz nie wyjdzie z gry
	{
		bool loop = true;
		cout << endl;
		fstream Arena;
		if (doitlater) {
			ClearArena(Refresh);
			
		}
		else {
			
		}
		

		

		doitlater = true;
		
		

		bool choosing = true;
		char option;
		
		
		MainMenu(); //wyświetl menu 
		SettingsHelpMenu(EnemySpeed, SmartAI); //ustawienia rozgrywki
		if (EnemySpeed == -1) { //umowna wartość, jeśli ustawiona na -1, to zakończ program.
			loop = false;
			break;
		}
		bool backtoOptions = false;
		system("CLS");
		cout << "Choose The map 1 - 4 or ESC to return:" << endl;
		cout << " _____ " << endl;
		cout << "|| 1 ||" << endl;
		cout << "||___|| \t1 - Map 1" << endl;
		cout << "|/___\\|" << endl;
		cout << " _____ " << endl;
		cout << "|| 2 ||" << endl;
		cout << "||___|| \t2 - Map 2" << endl;
		cout << "|/___\\|" << endl;
		cout << " _____ " << endl;
		cout << "|| 3 ||" << endl;
		cout << "||___|| \t3 - Map 3" << endl;
		cout << "|/___\\|" << endl;
		cout << " _____ " << endl;
		cout << "|| 4 ||" << endl;
		cout << "||___|| \t4 -  Map from Editor" << endl;
		cout << "|/___\\|" << endl;

		/////////////////////////////
		//wybieranie planszy do gry//
		/////////////////////////////

		while (choosing) { 
			option = tolower(_getch());
			switch (option) {
			case '1':
				Arena.open("GameArena.txt", ios::in);
				choosing = false;
				break;
			case '2':
				Arena.open("GameArena2.txt", ios::in);
				choosing = false;
				break;
			case '3':
				Arena.open("GameArena3.txt", ios::in);
				choosing = false;
				break;
			case '4':
				Arena.open("CustomLevel.txt", ios::in);
				choosing = false;
				break;
			case 27: //jeśli ESC -> wróć do ustawień
				backtoOptions = true;
				loop = false;
				choosing = false;
				break;
			default:
				break;
			}
		}
		system("CLS");


		if (backtoOptions) { //powróć do ustawień
			system("CLS");
			continue;
		}

		
		
		char GameArena[size * size]{}; //do wczytania
		
		char character; 
		int i = 0;
		if (!Arena.good()) { //czy plik został poprawnie otwarty?
			cout << "\n BŁĄD OTWARCIA PLIKU PLANSZY " << endl;
			break; //jeśli nie, zakończ program
		}
		if (Arena.is_open()) { //zamień '-' na '.' (punkty)
			while (!Arena.eof()) {
				Arena.get(character);
				if (character != '\n' && character != '\0') {
					if (character == '-') {
						character = '.';
					}
					GameArena[i] = character;
					i++;
				}
			}
		}
		Arena.close();

		//////////////////////////////////////////////
		//wczytanie planszy do tablicy dwuwymiarowej//
		//////////////////////////////////////////////
		
		int PointsArena[size][size]{};
		char GameZone[size][size];
		int x = 0;
		int y = 0;


		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				GameZone[i][j] = '?';
			}
		}
		int realWidth{ 0 }, realHeight{ 0 };
		for (int i = 0; i < size * size; i++) {
			if (GameArena[i] == 'F') { //koniec wiersza
				realHeight = y;
				GameZone[x][y] = GameArena[i];
				break;
			}
			if (GameArena[i] == 'E') { //koniec planszy
				realWidth = x;
				GameZone[x][y] = GameArena[i];
				x = 0;
				y++;
			}
			else {
				GameZone[x][y] = GameArena[i];
				if (GameArena[i] == '.') {
					PointsArena[x][y] = 1; //ustaw punkt do zebrania na planszy z punktami
				}
				x++;
			}
		}

		

		//Gracz
		int Points = 0;
		
		char LastPosPlayer = ' '; //usuń kropkę (punkt), jeśli gracz już był na tej pozycji
		int PlayerPosX; //pozycja gracza x
		int PlayerPosY; //pozycja gracza y


		//Wróg
		char LastPosEnemy = '.';
		int EnemyPosX[4]{}; //maksymalnie 4 przeciwników
		int EnemyPosY[4]{};
		int DifficultySkip = EnemySpeed * (2 + (200 / (realWidth + 3)) - 5); //funkcja hiperboliczna, która dostosowuje prędkość przeciwników do rozmiaru planszy
		int DScount = 0; 

		//Drzwi
		const char Door = '=';
		int DoorOpenScore = 0;
		bool OpenDoorOnce = true;
		int DoorX;
		int DoorY;

		
		int EnemyNumber = 0;


		int a = 0;
		int b = 0;
		//Ustawenie przeciwników, graczy, liczby punktów do wygrania, pozycji drzwi
		while (GameZone[a][b] != 'F') {
			if (GameZone[a][b] == 'E') {
				b++;
				a = 0;
				continue;
			}
			char element = GameZone[a][b];
			if (element == 'V') {
				GameZone[a][b] = Enemy;
				EnemyPosX[EnemyNumber] = a;
				EnemyPosY[EnemyNumber] = b;
				EnemyNumber++;

			}
			if (element == 'A') {
				GameZone[a][b] = Player;
				PlayerPosX = a;
				PlayerPosY = b;
			}
			if (element == '.') {
				DoorOpenScore++;
			}

			if (element == '=') {
				GameZone[a][b] = Door;
				DoorX = a;
				DoorY = b;
			}
			a++;
		}

		int EnemyLastPosX[4];
		int EnemyLastPosY[4];
		for (int i = 0; i < EnemyNumber; i++) {
			EnemyLastPosX[i] = EnemyPosX[i];
			EnemyLastPosY[i] = EnemyPosY[i];

		}
		

		int PlayerLastPosX = PlayerPosX;
		int PlayerLastPosY = PlayerPosY;
		//wstępne ustawienie, użyte do SmartAI+
		reachDest RDest[4] = { {false,0,0,0,0},{false,0,0,0,0},{false,0,0,0,0},{false,0,0,0,0} };
		
		int lastWidth{}, lastHeight{};

		bool emptyPole = false;
		bool wasEmptyPole[] = {true,true,true,true};
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		
		//////////////////
		//gra na planszy//
		//////////////////
		
		while (loop) { 

			
			int columns, rows;

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			columns = csbi.srWindow.Right - csbi.srWindow.Left + 1; //pobierz informację o liczbie kolumn
			rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; //pobierz informacjęo liczbie wierszy
			


			ShowConsoleCursor(false);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Refresh);
			if (lastWidth != columns || lastHeight != rows) { //jeśli użytkownik zmienił rozmiar okna
				
				ClearArena(Refresh);
				InproperConsoleSize(realWidth, realHeight); //sprawdź czy rozmiar pozwala na poprawne wyświetlenie planszy
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
				columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
				rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
				lastWidth = columns;
				lastHeight = rows;
			}
			
			
			if (OpenDoorOnce && Points == DoorOpenScore) { //otwórz drzwi jeśli gracz zdobył wystarczającą ilość punktów
				GameZone[DoorX][DoorY] = '.';
				PointsArena[DoorX][DoorY] = 2;
				OpenDoorOnce = false;
			}

			if (PlayerPosX == DoorX && PlayerPosY == DoorY) { //jesli gracz stanął na pozycji drzwi
				GameOver(Refresh, size, Points, realWidth, realHeight, LastVertical, LastHorizontal, true); //zakończ grę z wynikiem pozytywnym
				loop = false;
				break;
			}

			for (int i = 0; i < EnemyNumber; i++) {
				if (PlayerPosX == EnemyPosX[i] && PlayerPosY == EnemyPosY[i]) { // jeżeli pozycja gracza == pozycja przeciwnika
					GameOver(Refresh, size, Points, realWidth, realHeight, LastVertical, LastHorizontal); //zakończ grę z wynikiem negatywnym
					loop = false;
					break;
				}
			}

			GameZone[PlayerLastPosX][PlayerLastPosY] = LastPosPlayer; 
			if (PointsArena[PlayerPosX][PlayerPosY] == 1) { //dodaj punkt za kropkę
				Points++;
				PointsArena[PlayerPosX][PlayerPosY] = 0;
			}
			GameZone[PlayerPosX][PlayerPosY] = Player; //ustaw gracza na x, y

			
			a = 0;
			b = 0;

			
			while (GameZone[a][b] != 'F') { // pokaż planszę
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				if (GameZone[a][b] == 'E') {
					cout << "\n";
					
					b++;
					a = 0;
					continue;
				}
				char element = GameZone[a][b];
				switch (element) 
				{
					case '#':
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 17);
						cout << element;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						break;
					case '?':
						cout << " ";
						break;
				
					case Enemy:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
						cout << element;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						break;

					case Player:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
						cout << element;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						break;

					case Door:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
						cout << element;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
						break;

					default:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						cout << element;
					
				}
				
				a++;
			}
			
			DScount++;
			if (DScount > DifficultySkip) { //SI może wykonać ruch co konkretną liczbę obiegów pętli
				MoveEnemy(GameZone, EnemyLastPosX, EnemyLastPosY, EnemyPosX, EnemyPosY, EnemyNumber, Enemy, wasEmptyPole, PointsArena, SmartAI, RDest, PlayerPosX, PlayerPosY);
				DScount = 0;
			}

			PlayerLastPosX = PlayerPosX;
			PlayerLastPosY = PlayerPosY;




			char pressedkey;

			bool didPressKey = true;
			
			int timer = 0;
			
			if (!_kbhit()) { //jeśli gracz w danym obiegu pętli nie nacisnął klawisza
				
					pressedkey = 'x'; //umowna wartość, oznacza, by przejść dalej
					didPressKey = false;
					break;
			}


			if (didPressKey) { //jeśli gracz nacisnął klawisz
				pressedkey = tolower(_getch()); 
			}
			bool b = true; //jeśli gra została wstrzymana, i wznowiona, nie wyświetlaj dwa razy liczby punktów
			switch (pressedkey) { //sterowanie 
			case 'a':
				if (GameZone[PlayerPosX - 1][PlayerPosY] != '#' && GameZone[PlayerPosX - 1][PlayerPosY] != '=') {
					PlayerPosX -= 1;
				}
				break;
			case 'd':
				if (GameZone[PlayerPosX + 1][PlayerPosY] != '#' && GameZone[PlayerPosX + 1][PlayerPosY] != '=') {
					PlayerPosX += 1;
				}
				break;
			case 'w':
				if (GameZone[PlayerPosX][PlayerPosY - 1] != '#' && GameZone[PlayerPosX][PlayerPosY - 1] != '=') {
					PlayerPosY -= 1;
				}
				break;
			case 's':
				if (GameZone[PlayerPosX][PlayerPosY + 1] != '#' && GameZone[PlayerPosX][PlayerPosY + 1] != '=') {
					PlayerPosY += 1;
				}
				break;
			case 27: 
				cout  << "\nGame Paused. Press Enter to leave."; 
				
				char k = _getch();
				if (k == 13) { //gracz wyszedł z planszy
					loop = false;
				}
				cout << "\r                                  ";
				b = false;
				break;
				
			}

			if (b) {
				cout << "\n";
				cout << "SCORE: " << Points << " / " << DoorOpenScore << " DOOR UNLOCK";
			}
				
			
			


		}
	}
	system("cls");
	
	/////////////////////////////
	//koniec działania programu//
	/////////////////////////////
	
	cout << "Thank you for playing!\n\n\n\n"; 

	return 0;
}