#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include <string.h>
#include"conio2.h"

//wspolrzedne legendy
#define X_LEGENDA 3
#define Y_LEGENDA 6
//wspolrzedne licznika rundy
#define X_RUNDA 50
#define Y_RUNDA 5
//wspolrzedne planszy i rozpoczecia kursora
#define X_PLANSZA 45
#define Y_PLANSZA 8
//przewaga drugiego gracz
#define KOMI 6.5
//kolory
#define COLOR_OF_BOARD 6
#define COLOR_OF_FIRST 4
#define COLOR_OF_SECOND 15
//rozmiar pomocnicznej tablicy
#define I_SIZE_OF_ARRAY 60
#define J_SIZE_OF_ARRAY 110
//wyglad kamienia
#define SIGN 'O'
//wartosc backendowa drugiego kamienia
#define SECOND_SIGN '1'
//rozmiar tablicy X i Y potrzebnych do KO
#define XY_SIZE 2

//przygotowanie planszy
void Przygotowania();
//wyglad legendy
void WygladLegendy();
//przypisuje tablicy ,,plansza" wartosci planszy poczatkowej
void PrzypisanieTablicy(int**& plansza, int rozmiar, int rozmiarX, int startPlansza[I_SIZE_OF_ARRAY][J_SIZE_OF_ARRAY]);
//drukowanie planszy
void Drukowanie(int**& plansza, int runda, int rozmiar, int rozmiarX, int startPlansza[I_SIZE_OF_ARRAY][J_SIZE_OF_ARRAY]);
//usuwa ewentualne zbicie
void CzyZbicie(int**& plansza, int x, int y, int runda, int rozmiar, int rozmiarX, float zawodnicy[2]);
//sprawdzanie czy wspolrzedna nie jest zajeta
bool CzyDostepny(int**& plansza, int x, int y);
//czy ruch nie jest samobojstwem
bool CzyNieSamobojstwo(int**& plansza, int x, int y, int runda, int rozmiar, int rozmiarX);
//jesli ruch jest samobojstwem, ale zbijasz preciwny kamien to zawraca true
bool CzyMozliwoscZbicia(int**& plansza, int x, int y, int runda, int rozmiar, int rozmiarX);
bool HandicapMode(int**& plansza, int zn);
bool Czy_Ko(int runda1[XY_SIZE], int runda2[XY_SIZE], int runda3[XY_SIZE], int runda4[XY_SIZE], int x, int y, int runda);
//wartosci wokol kamienia
bool WartoscWokol(int**& plansza, int x, int y, int signer, int rozmiar, int rozmiarX);
int charsToDecimal(char aLitera, char bLitera);
//funkcja zwraca rozmiar (9,13 lub 19) albo 1 jestli customSize
int WybieranieRozmiaru(int zn);
//sprawdza czy zabija; Potrzebne do KO
int CzyZabija(int**& plansza, int x, int y, int signer, int rozmiar, int rozmiarX);
//wybranie swojej wielkosci planszy
int RozmiarGracza();
//tworzenei dynamicznej tablicy
int** TworzenieTabeli(int rozmiar, int rozmiarX);

int main() {
	//zmienne z szablonu(x i y to pozycja poczatkowa znaku)
	int x = X_PLANSZA, y = Y_PLANSZA;//<--aktualne wspolrzedne kursora, zaczyna w lewy rog planszy
	int zn = 0, attr = 7, back = 0, zero = 0;
	//zmienne przechowywujace 3 ostatnie rundy
	int runda1[XY_SIZE];
	int runda2[XY_SIZE];
	int runda3[XY_SIZE];
	int runda4[XY_SIZE];
	for (int i = 0; i < 2; i++) {
		runda1[i] = -1;
		runda2[i] = -2;
		runda3[i] = -3;
		runda4[i] = -4;
	}
	//zmienna x liczona co jeden (plansza jest zrobiona aby x przesuwal sie co jeden)
	int wspolrzedneX;
	char txt[32];
	//licznik rundy; na podstawie tego sprawdzam ktorego gracza jest kolej
	int runda = 0;
	//tablica ktora przechowuje aktualne wspolrzedne x i y
	char pozycjaX[32];
	char pozycjaY[32];
	//tworzymy 2 graczy
	float zawodnicy[2] = { 0 };
	zawodnicy[1] += KOMI;

	//dynamiczna tablica
	int rozmiar;
	int** plansza;
	int startPlansza[I_SIZE_OF_ARRAY][J_SIZE_OF_ARRAY] = { 0 };
	//czy handicap
	bool handicap;
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("Mateusz, Fydrych, s193410");
	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
	//uzytkownik wybiera rozmiar tablicy
	rozmiar = WybieranieRozmiaru(zn);
	//jesli uzytkownik wybral opcje 4, to rozmiar=1, wiec wybiera swoj rozmiar
	if (rozmiar == 1) {
		rozmiar = RozmiarGracza();
	}
	//rozmiar wiersza na planszy
	const int rozmiarX = rozmiar * 2 - 1;
	plansza = TworzenieTabeli(rozmiar, rozmiarX);
	PrzypisanieTablicy(plansza, rozmiar, rozmiarX, startPlansza);
	handicap = HandicapMode(plansza, zn);
	do {
		Przygotowania();
		Drukowanie(plansza, runda, rozmiar, rozmiarX, startPlansza);
		WygladLegendy();

		//ustalamy kolor kursora
		runda % 2 == 0 ? attr = COLOR_OF_FIRST : attr = COLOR_OF_SECOND;

		// print out the code of the last key pressed
		textcolor(LIGHTGREEN);
		if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
		else sprintf(txt, "key code: 0x%02x", zn);
		gotoxy(X_LEGENDA, Y_LEGENDA + 16);
		cputs(txt);

		//wydrukowanie pozycji x i y w konsoli
		wspolrzedneX = (int)ceil((float)(x - X_PLANSZA + 1) / 2);
		gotoxy(X_LEGENDA, Y_LEGENDA + 17);
		sprintf(pozycjaX, "Your x position: %d", wspolrzedneX);
		cputs(pozycjaX);
		gotoxy(X_LEGENDA, Y_LEGENDA + 18);
		sprintf(pozycjaY, "Your y position: %d", y - Y_PLANSZA + 1);
		cputs(pozycjaY);

		//punkty graczy
		textcolor(7);
		gotoxy(X_LEGENDA, Y_LEGENDA + 20);
		sprintf(pozycjaX, "1 PLAYER points: %.1f", zawodnicy[0]);
		cputs(pozycjaX);
		gotoxy(X_LEGENDA, Y_LEGENDA + 21);
		sprintf(pozycjaY, "2 PLAYER points: %.1f", zawodnicy[1]);
		cputs(pozycjaY);

		//wyglad kursor
		gotoxy(x, y);
		textcolor(attr);
		putch(SIGN);
		zero = 0;
		zn = getch();

		//jesli literka m to konczymy handicapa
		if (zn == 0x6d && handicap) {
			handicap = false;
			runda++;
		}

		//jesli literka s to zapis gry
		if (zn == 0x73) {
			//SavingGame(plansza, rozmiar, rozmiarX, zn);
		}

		//jesli literka l to ladowanie gry
		if (zn == 0x6c) {
			//LoadingGame(plansza, rozmiar, rozmiarX, zn);
		}

		//sprawdzamy czy zostalo kliknieta literka --> i
		if (zn == 0x69 && CzyDostepny(plansza, x - X_PLANSZA, y - Y_PLANSZA)
			&& CzyNieSamobojstwo(plansza, x - X_PLANSZA, y - Y_PLANSZA, runda, rozmiar, rozmiarX)) {
			while ((zn = getch()) != 0x1b) {
				//sprawdzamy czy zostal nacisniety enter
				if (zn == 0x0d) {
					if (handicap) {
						plansza[y - Y_PLANSZA][x - X_PLANSZA] = (int)SIGN;
						break;
					}
					else {
						if (Czy_Ko(runda1,runda2,runda3,runda4,x,y,runda))
							break;
						else {
							runda % 2 == 0
								? plansza[y - Y_PLANSZA][x - X_PLANSZA] = (int)SIGN
								: plansza[y - Y_PLANSZA][x - X_PLANSZA] = (int)SECOND_SIGN;
							runda++;
							CzyZbicie(plansza, x - X_PLANSZA, y - Y_PLANSZA, runda, rozmiar, rozmiarX, zawodnicy);
							break;
						}
					}
				}
			}
		}

		//sprawdzamy czy zostala kliknieta literka -->n
		if (zn == 0x6e) {
			x = X_PLANSZA;
			y = Y_PLANSZA;
			PrzypisanieTablicy(plansza, rozmiar, rozmiarX, startPlansza);
			for (int i = 0; i < 2; i++)
				zawodnicy[i] = 0;
			zawodnicy[1] += KOMI;
			runda = 0;
		}

		// we do not want the key 'H' to play role of "up arrow"
		// so we check if the first code is zero
		if (zn == 0) {
			zero = 1;		// if this is the case then we read
			zn = getch();		// the next code knowing that this
			if (zn == 0x48) {
				if (y > Y_PLANSZA)
					y--;
			}// will be a special key
			else if (zn == 0x50) {
				if (y < Y_PLANSZA + rozmiar - 1)
					y++;
			}
			else if (zn == 0x4b) {
				if (x > X_PLANSZA)
					x -= 2;
			}
			else if (zn == 0x4d) {
				if (x < X_PLANSZA + rozmiarX - 1)
					x += 2;
			}
		}
	} while (zn != 'q');

	//zwolnienie pamieci
	for (int i = 0; i < rozmiar; i++)
		delete[] plansza[i];
	delete[] plansza;
	plansza = NULL;

	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be

	//przerzucam informacje o zakonczeniu programu na dol i zmieniam jej kolor na przystepniejszy
	gotoxy(20, 30);
	textcolor(1);
	return 0;
}
void Przygotowania() {
	textbackground(0);
	clrscr();
	textcolor(LIGHTBLUE);
}

void WygladLegendy() {
	textcolor(YELLOW);
	gotoxy(X_LEGENDA, Y_LEGENDA);
	cputs("Mateusz, Fydrych, s193410");
	gotoxy(X_LEGENDA, Y_LEGENDA + 1);
	cputs("q       = exit");
	gotoxy(X_LEGENDA, Y_LEGENDA + 2);
	cputs("cursors = moving");
	gotoxy(X_LEGENDA, Y_LEGENDA + 3);
	cputs("enter   = confirm your move");
	gotoxy(X_LEGENDA, Y_LEGENDA + 4);
	cputs("n       = start a new game");
	gotoxy(X_LEGENDA, Y_LEGENDA + 5);
	cputs("i       = place a stone on the board");
	gotoxy(X_LEGENDA, Y_LEGENDA + 6);
	cputs("esc     = cancel your move");
	gotoxy(X_LEGENDA, Y_LEGENDA + 7);
	cputs("s       = save the game state");
	gotoxy(X_LEGENDA, Y_LEGENDA + 8);
	cputs("l       = load the game state");
	gotoxy(X_LEGENDA, Y_LEGENDA + 9);
	cputs("m       = turn off handicap mode");


	textcolor(LIGHTBLUE);
	gotoxy(X_LEGENDA, Y_LEGENDA + 13);
	cputs("finished subpoints: a,b,c,d,e,g,h,k");
}

void PrzypisanieTablicy(int**& plansza, int rozmiar, int rozmiarX, int startPlansza[I_SIZE_OF_ARRAY][J_SIZE_OF_ARRAY]) {
	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiarX; j++) {
			gotoxy(X_PLANSZA + j, Y_PLANSZA + i);
			if (j == 0) {
				if (i == 0) {
					startPlansza[i][j] = (218);
					plansza[i][j] = (218);
				}
				else if (i == rozmiar - 1) {
					startPlansza[i][j] = (192);
					plansza[i][j] = (192);
				}
				else {
					startPlansza[i][j] = (195);
					plansza[i][j] = (195);
				}
			}
			//sprawdzamy prawa sciane
			else if (j == rozmiarX - 1) {
				if (i == 0) {
					startPlansza[i][j] = (191);
					plansza[i][j] = (191);
				}
				else if (i == rozmiar - 1) {
					startPlansza[i][j] = (217);
					plansza[i][j] = (217);
				}

				else {
					startPlansza[i][j] = (180);
					plansza[i][j] = (180);
				}
			}
			//gorny rzad
			else if (i == 0) {
				if (j % 2 == 0) {
					startPlansza[i][j] = (194);
					plansza[i][j] = (194);
				}
				else {
					startPlansza[i][j] = (196);
					plansza[i][j] = (196);
				}
			}
			//dolny rzad
			else if (i == rozmiar - 1) {
				if (j % 2 == 0) {
					startPlansza[i][j] = (193);
					plansza[i][j] = (193);
				}
				else {
					startPlansza[i][j] = (196);
					plansza[i][j] = (196);
				}
			}
			//srodek planszy
			else {
				//krzyzyk
				if (i % 2 != 1 && j % 2 != 1) {
					startPlansza[i][j] = (197);
					plansza[i][j] = (197);
				}
				else {
					if (i % 2 == 1) {
						//kolumna
						if (j % 2 == 0) {
							startPlansza[i][j] = (197);
							plansza[i][j] = (197);
						}
						else {
							startPlansza[i][j] = (196);
							plansza[i][j] = (196);
						}
					}
					else {
						startPlansza[i][j] = (196);
						plansza[i][j] = (196);//rzad
					}
				}
			}
		}
	}
}

void Drukowanie(int**& plansza, int runda, int rozmiar, int rozmiarX, int startPlansza[I_SIZE_OF_ARRAY][J_SIZE_OF_ARRAY]) {
	gotoxy(X_PLANSZA, Y_PLANSZA);
	for (int i = 0; i < rozmiar; i++) {
		for (int j = 0; j < rozmiarX; j++) {
			gotoxy(X_PLANSZA + j, Y_PLANSZA + i);
			if (plansza[i][j] == (int)SECOND_SIGN) {
				textcolor(COLOR_OF_SECOND);
				putch((int)SIGN);
			}
			else if (plansza[i][j] == (int)SIGN) {
				textcolor(COLOR_OF_FIRST);
				putch((int)SIGN);
			}
			else if (plansza[i][j] == (int)'9') {
				plansza[i][j] = startPlansza[i][j];
				putch(plansza[i][j]);
			}
			else {
				textcolor(LIGHTBLUE);
				putch(plansza[i][j]);
			}

		}
	}

}

void CzyZbicie(int**& plansza, int x, int y, int runda, int rozmiar, int rozmiarX, float zawodnicy[2]) {
	//tymczasowa zmienna przypisujaca aktualny znak
	int signer;
	runda % 2 == 0
		? signer = (int)SIGN
		: signer = (int)SECOND_SIGN;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int xRozmiar = x + 2 * j;
			int yRozmiar = y + i;

			if ((i != 0 && j != 0) || (i == 0 && j == 0))
				continue;
			else if (xRozmiar >= 0 && yRozmiar >= 0 && yRozmiar < rozmiar && xRozmiar < rozmiarX) {
				if (WartoscWokol(plansza, xRozmiar, yRozmiar, signer, rozmiar, rozmiarX) || (plansza[yRozmiar][xRozmiar] != (int)'O' && plansza[yRozmiar][xRozmiar] != (int)SECOND_SIGN)) {
				}
				else {
					if (runda % 2 != 0)
						zawodnicy[0] += 1;
					else
						zawodnicy[1] += 1;
					plansza[yRozmiar][xRozmiar] = (int)'9';
				}
			}
		}
	}

}

bool CzyDostepny(int**& plansza, int x, int y) {
	if (plansza[y][x] == (int)SIGN || plansza[y][x] == (int)SECOND_SIGN)
		return false;
	return true;
}

bool CzyNieSamobojstwo(int**& plansza, int x, int y, int runda, int rozmiar, int rozmiarX) {
	//tymczasowa zmienna przypisujaca aktualny znak
	int signer;
	runda % 2 == 0
		? signer = (int)SIGN
		: signer = (int)SECOND_SIGN;
	if (WartoscWokol(plansza, x, y, signer, rozmiar, rozmiarX)) {
		return true;
	}

	if (CzyMozliwoscZbicia(plansza, x, y, runda, rozmiar, rozmiarX))
		return true;
	return false;
}

bool HandicapMode(int**& plansza, int zn) {
	clrscr();
	while (true) {
		gotoxy(X_PLANSZA, Y_PLANSZA);
		cputs("Czy chcesz zagrac z handicapem: t)tak n)nie\n");
		zn = getch();
		//jesli zn==n
		if (zn == 0x6e)
			return false;
		//jesli zn==t
		else if (zn == 0x74) {
			return true;
		}
	}
}

bool WartoscWokol(int**& plansza, int x, int y, int signer, int rozmiar, int rozmiarX) {
	int licznikOddechow = 0;
	int licznikTegoZnaku = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int xRozmiar = x + 2 * j;
			int yRozmiar = y + i;


			if ((i != 0 && j != 0) || (i == 0 && j == 0))
				continue;
			else if (xRozmiar >= 0 && yRozmiar >= 0 && yRozmiar < rozmiar && xRozmiar < rozmiarX) {
				if (plansza[yRozmiar][xRozmiar] != (int)SIGN && plansza[yRozmiar][xRozmiar] != (int)SECOND_SIGN) {
					licznikOddechow++;
				}
				else if (plansza[yRozmiar][xRozmiar] == signer) {
					licznikTegoZnaku++;

				}
			}
		}
	}


	if (licznikOddechow >= 1 || licznikTegoZnaku >= 1)
		return true;
	return false;

}

bool CzyMozliwoscZbicia(int**& plansza, int x, int y, int runda, int rozmiar, int rozmiarX) {
	//tymczasowa zmienna przypisujaca aktualny znak
	int signer;
	runda % 2 != 0
		? signer = (int)SIGN
		: signer = (int)SECOND_SIGN;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int xRozmiar = x + 2 * j;
			int yRozmiar = y + i;

			if ((i != 0 && j != 0) || (i == 0 && j == 0))
				continue;
			else if (xRozmiar >= 0 && yRozmiar >= 0 && yRozmiar < rozmiar && xRozmiar < rozmiarX) {
				if (CzyZabija(plansza, xRozmiar, yRozmiar, signer, rozmiar, rozmiarX)==4 ||
					(plansza[yRozmiar][xRozmiar] != (int)SIGN && plansza[yRozmiar][xRozmiar] != (int)SECOND_SIGN)) {
					continue;
				}
				else {
					return true;
				}
			}
		}
	}
	return false;
}

bool Czy_Ko(int runda1[XY_SIZE],int runda2[XY_SIZE], int runda3[XY_SIZE], int runda4[XY_SIZE], int x, int y, int runda) {
	//zaapisywanie ostatnich dwoch ruchow w celu zachowania reguly KO
	if (runda % 2 == 0) {
		if (runda > 1) {
				runda3[0] = runda1[0];
				runda3[1] = runda1[1];
			
		}
		runda1[0] = y;
		runda1[1] = x;
		if (runda > 1) {
			if (runda1[0] == runda3[0] &&
				runda1[1] == runda3[1])
				return true;
		}
		
	}
	else {
		if (runda > 1) {
				runda4[0] = runda2[0];
				runda4[1] = runda2[1];
			
		}
		runda2[0] = y;
		runda2[1] = x;
		if (runda > 1) {
			if (runda2[0] == runda4[0] &&
				runda2[1] == runda4[1])
				return true;
		}
		
	}
	runda1[0] = runda1[0];
	runda2[0] = runda2[0];
	runda3[0] = runda3[0];
	runda4[0] = runda4[0];
	return false;
}

int RozmiarGracza() {
	int aLiczba, bLiczba;
	char aLitera, bLitera;
	while (true) {
		clrscr();
		cputs("Podaj rozmiar tablicy\n");
		aLitera = getch();
		while (aLitera >= 0x30 && aLitera <= 0x39) {
			aLiczba = (int)(aLitera - 65);
			putch((int)aLitera);
			bLitera = getch();
			putch((int)bLitera);
			//sprawdzamy czy enter
			if (bLitera == 0x0d)
				return aLitera - 48;
			//jesli druga cryfra jest od 1-9
			if (bLitera >= 0x30 && bLitera <= 0x39) {
				bLiczba = charsToDecimal(aLitera, bLitera);
				return bLiczba;
			}
		}
	}
}

int WybieranieRozmiaru(int zn) {
	gotoxy(X_PLANSZA, Y_PLANSZA);
	cputs("Wybierz rozmiar planszy wpisujac liczbe 1-4: 1)9x9 2)13x13 3)19x19 4)chce sam wybrac rozmiar\n");
	while (true) {
		gotoxy(X_PLANSZA, Y_PLANSZA + 1);
		zn = getch();
		switch (zn) {
			//case 1
		case 0x31:
			return 9;
			//case 2
		case 0x32:
			return 13;
			//case 3
		case 0x33:
			return 19;
			//case 4
		case 0x34:
			return 1;
		default:
			cputs("\nNie ma takiej odpowiedzi, sprobuj ponownie\n");
		}
	}
}

int charsToDecimal(char aLitera, char bLitera) {
	int dziesiatki = (aLitera - '0') * 10;
	int jednosci = (bLitera - '0') * 1;
	return dziesiatki + jednosci;
}

int CzyZabija(int**& plansza, int x, int y, int signer, int rozmiar, int rozmiarX) {
	int licznikOddechow = 0;
	int licznikTegoZnaku = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int xRozmiar = x + 2 * j;
			int yRozmiar = y + i;


			if ((i != 0 && j != 0) || (i == 0 && j == 0))
				continue;
			else if (xRozmiar >= 0 && yRozmiar >= 0 && yRozmiar < rozmiar && xRozmiar < rozmiarX) {
				if (plansza[yRozmiar][xRozmiar] != (int)SIGN && plansza[yRozmiar][xRozmiar] != (int)SECOND_SIGN) {
					licznikOddechow++;
				}
				else if (plansza[yRozmiar][xRozmiar] == signer) {
					licznikTegoZnaku++;
				}
			}
		}
	}


	if (licznikOddechow > 1 || licznikTegoZnaku >= 1)
		return 4;
	return 1;

}

int** TworzenieTabeli(int rozmiar, int rozmiarX) {
	int** tab2 = new int* [rozmiar];
	for (int i = 0; i < rozmiar; i++)
	{
		tab2[i] = new int[rozmiarX]; //alokacja pamieci
	}
	return tab2;
}

