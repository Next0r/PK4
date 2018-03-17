// ProjektTD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//----------------------------------------------------TODO

//class interface
//	klasa zarzadzajaca interface'm uzytkownika 
//	interakcja z poziomu klawiatury - byc moze osobna klasa do obslugi eventow zwracajaca cos w zaleznosci od typu?
//	w zaleznosci od "pozycji" w programie zwraca spritey - wczesniej zdefiniowane - ktore nalezy rysowac
//	wie gdzie sie znajduje - rozne dzialanie buttonow w zaleznosci od zaglebienia w grze
//		wszystkie pochodne interfaceu typu:
//		parametry w trakcie gry i buttony np. powrotu i pulapek
//		ramki main menu
//		ramki pod menu
//		ramki sklepu
//		buttony - byc moze stale pozycjonowane niezaleznie od poszczegolnych ramek na odpowiedniej warstwie renderowania?

//class przeciwnicy
// korzysta z wczytanych informacji dotyczacych przeciwnikow
// kontroluje czestotliwosc spawnu wzgledem odstepow czasowych
// generuje odpowiednich przeciwnikow
// zarzadza przemieszczeniem przeciwnikow (lerp miedzy pozycjami na mapie w danym czasie)
// kontrolouje paramtery przeciwnikow - zdrowie itp.
//		wszytskie klasy pochodne:
//		przeciwnicy (zdefiniowane 5 typow w debuggerze)
//		przeciwnicy typu A, B, C, D i E - konstruktory z roznymi paramterami zdrowia/pancerza itd.
//		szybkosc przeciwnikow prawdopodobnie bedzie stala - zeby nie zaburzac ukladu spriteow na mapie i nie ukrywac jednych pod drugimi
//		predkoscia/iloscia mozna zarzadzac z poziomu parametru czasu dla danej fali

//class pulapki
// dziala w trakcie gry 
// realizuje dzialanie pulapek na przeciwnikach
// wspolpracuje z interface'm - korzysta z eventow typu keypressed
//		wszystkie pochodne:
//		typy pulapek
//		opoznienia w ich dzialaniu
//		koszta itp.

//class wieze
// dziala w trakcie gry
// zarzadza wiezami
// wieze wyszukuja cel w ich zasiegu, najdalej na sciezce
// kontrola pociskow, ktore przemieszczaja sie od wiezy do wroga
//		klasy pochodne jak w przypadku pulapek - dla roznych parametow

//class sklep
// uruchamiana w odpowiednim momencie przed gra
// wspolpracuje z interface'm
// ilosc pozycji do wykupienia stala (ale nie wszystkie sa zdefiniowane) np. 5/10 slotow wiez pulapek itp.
// trzeba dorobic slowa kluczowe definiujace ilosc pieniedzy przeznaczonych na mape

//class zarzadzanie
// dziala na wszystkich powyzszych obiektach
// w odpowiednim momencie wywoluje interface, sklep, generacje przeciwnikow
// nadzoruje warunki zwyciestwa i porazki - wspolpraca z generowanym interface'm
// zapis/odczyt/kontynuacja gry? byc moze zapisywane i odczytywane z pliku tekstowego

// inne
// generacja mapowania dla elemetow pola zabronionego i pol pustych - trawa/kamienie itp
// mapowanie tekstury zalezne od numeru pulapki,
// mapownaie tekstury zalezne od numeru wiezy,
// pulapki i wierze z odpowiedim kanalem alfa zostana nalozone na wspomniane pola - pulapki na warstwie pod potworami
// tak aby widoczyny byl numer slotu (na mapie), co ulatwi korzystanie ze sklepu
// kanal alfa dla sciezki, powiekszenie spritea, renderowanie warstwe wyzej - tak aby utworzyc efekt przejscia miedzy innymi polami

//----------------------------------------------------WCZYTYWANIE MAPY/KREATOR

class baza_spriteow {
private:
	sf::Texture tekstura_spritea_pola_planszy;


public:
	sf::Sprite sprite_pola_planszy;

	

	baza_spriteow(string s_tex_pol_m) {
		tekstura_spritea_pola_planszy.loadFromFile(s_tex_pol_m); // wczytywanie tekstury za pomoca podanej sciezki
		sprite_pola_planszy.setTexture(tekstura_spritea_pola_planszy); // przypisaywanie tekstury do spritea
	}

	
};

class map_info_tekstowe { // przechowuje dane dotyczace mapy
public:
	string nazwa = "";
	string pola_pulapek[5] = { "" };
	string zestaw_pol[9][16];
	string fale_wrogow[10][3] = { "","" }; // maksymalnie 10 fal - 1 odstep czasowy (od startu/poprzedniej fali)
	//                                                           - 2 typ wroga
	//                                                           - 3 ilosc wrogow danego typu
	int sciezka_na_mapie[100] = { 100 }; // dla testu poprawnosci konstukcji sciekzi

	void nadaj_nazwe(string nazwa1) { // przypisuje nazwe do obieku
		nazwa = nazwa1;
	}

	void wpisz_kod_pola(int x, int y, string kod) { // przypisuje kod pola do obiektu
		zestaw_pol[y][x] = kod;
	}

	void wpisz_pulapke(string numer_p, int nr_p) { // przypisuje dane pulapek do obiektu
		pola_pulapek[nr_p] = numer_p;
	}

	void wypisz_dane() { // wypisuje zgrowadzone dane na temat mapy
		cout << "nazwa mapy: " << nazwa << endl << endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				cout << zestaw_pol[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "pulapki zdefiniowano na polach sciezki: " << endl;
		int i = 0;
		while (!pola_pulapek[i].empty()) {
			cout << pola_pulapek[i] << " ";
			i++;
		}
		cout << endl << endl;
	}
}; 

class mapy_wczytywanie_z_pliku { // klasa zarzadzajaca danymi dot. mapy
private:
	int licznik_map = 0;
	ifstream plik_mapy; // plik z zestawem map
	string sciezka_czytania;
	string slowo_tmp;
	int max = 0;

	bool otworz_plik() {
		// plik z mapami
		plik_mapy.open(sciezka_czytania);
		if (!plik_mapy.good()) { // test otwarcia pliku
			cout << "blad wczytywania pliku map" << endl;
			return false;
		}
		else {
			return true;
		}
	}

	void czytaj_slowo() { // wczytywanie jednego slowa (do bilaego znaku)
		plik_mapy >> slowo_tmp;
	}

	bool instrukcja_level() {
		czytaj_slowo();
		if (slowo_tmp == "LEVEL") {
			zestaw_map[licznik_map] = new map_info_tekstowe(); // instukcja level generuje obiekt info mapy
			czytaj_slowo();
			while (slowo_tmp != "LEVELEND") { // wczytywanie nazwy poziomu z pliku
				zestaw_map[licznik_map]->nazwa += (slowo_tmp + " ");
				czytaj_slowo();
				if (zestaw_map[licznik_map]->nazwa.length() > 50) { // zabiezpieczenie co do dlugosci nazwy mapy
					cout << "nazwa wczytywanej mapy jest zbyt dluga (pamietaj o instukcji LEVELEND)" << endl;
					return false;
				}
			}
			cout << zestaw_map[licznik_map]->nazwa << endl << endl;
			return true;
		}
		else {
			cout << "brak slowa kluczowego LEVEL" << endl;
			return false;
		}

	}

	bool test_poprawnosci_pola(string pole) { // sprawdza poprawnosc skladni wpisanego pola
		if (pole.length() != 2) { // pole musi miec dwa znaki
			return false;
		}
		if (pole != "XX" && pole != "QQ" && pole != "@@") { // jesli nie jest to pole zabronione, puste lub mur
			int tmp;
			if (pole[0] == '#' && pole[1] > 47 && pole[1] < 58) { // jesli jes to pole wiezy - #<0-9>
				return true;
			}
			else if (istringstream(pole) >> tmp) { // jesli nie jest to pole wiezy to musi byc to pole sciezki
												   // udalo sie skonwertowac slowo do inta (czyli jest to pole sciezki)
				zestaw_map[licznik_map]->sciezka_na_mapie[tmp] = tmp; // zapamietaj istnienie tego pola (dla testu poprawnosci wytyczenia sciezki)
				return true;
			}
			else {
				return false; // dwuliterowe slowo nie jest prawidlowe 
			}
		}
		else { // jest to pole zabronione, mur lub pole puste
			return true;
		}
	}

	bool instukcja_map() {
		czytaj_slowo();
		if (slowo_tmp == "MAP") {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 16; j++) {
					czytaj_slowo(); // wczytywanie pola mapy (z pliku)
					if (test_poprawnosci_pola(slowo_tmp)) { // jesli wczytane pole ma poprawna skladnie
						zestaw_map[licznik_map]->zestaw_pol[i][j] = slowo_tmp;
						cout << slowo_tmp << " ";
					}
					else {
						cout << "blad w skaldni mapy" << endl;
						return false;
					}
				}
				cout << endl;
			}
			czytaj_slowo(); // wczytuje slowo kluczowe MAPEND
			if (slowo_tmp != "MAPEND") {
				cout << "brak slowa kluczowego MAPEND" << endl;
				return false;
			}
			return true;
		}
		else {
			cout << "blad - brak slowa kluczowego MAP po instrukcji LEVELEND" << endl;
			return false;
		}
	}

	bool test_ciaglosci_sciezki() {
		
		
		if (zestaw_map[licznik_map]->sciezka_na_mapie[0] != 0) {
			cout << "sciezka powinna byc numerowana od 0" << endl;
			return false;
		}
		// wyszukaj najwiekszy element w tablicy sciezki - potencjalnie ostatni
		for (int i = 0; i < 100; i++) {
			if (zestaw_map[licznik_map]->sciezka_na_mapie[i] > max && zestaw_map[licznik_map]->sciezka_na_mapie[i] < 100) {
				max = zestaw_map[licznik_map]->sciezka_na_mapie[i];
			}	
		}
		// max przechowuje ostatnie pole sciezki (mniejsze od 100)
		for (int i = 0; i < max; i++) {
			if (zestaw_map[licznik_map]->sciezka_na_mapie[i + 1] != (i + 1)) {
				// jesli element mapy (lacznie z max) nie jest zgodny z indeksem w tablicy (tablica badana po kolei)
				cout << "blad w numeracji sciezki" << endl;
				return false;
			}
		}
		return true;
	}

	bool instukcja_foe() {
		czytaj_slowo();
		if (slowo_tmp != "FOE") {
			cout << "oczekiwana instrukcja FOE" << endl;
			return false;
		}
		for (int i = 0; i < 10; i++) { // maksymalnie 10 fal wrogow
			czytaj_slowo();
			if (slowo_tmp == "FOEEND") {
				break;
			}
			if (slowo_tmp != "WAVE:") {
				cout << "konstukcje fali wrogow nalezy rozpoczac slowem kluczowym WAVE:" << endl;
				return false;
			}
			// wczytano slowo wave
			czytaj_slowo();
			if (slowo_tmp != "TIME") {
				cout << "Po slowie kluczowym WAVE: nalezy uzyc TIME by okreslic czas odstepu fali" << endl;
				return false;
			}
			czytaj_slowo();
			int tmp;
			if (slowo_tmp.length() == 2 && istringstream(slowo_tmp) >> tmp && tmp < 21 && tmp > 0) {
				zestaw_map[licznik_map]->fale_wrogow[i][0] = slowo_tmp;
				cout << endl << "fala:" << i << " czas odstepu: " << slowo_tmp << "s ";
			}
			else {
				cout << "czas odstepu powinien byc dwucyfrowa liczba od 01 do 20 wlacznie" << endl;
				return false;
			}
			czytaj_slowo();
			if (slowo_tmp != "ENEMY") {
				cout << "Po slowie kluczowym TIME i liczbie nalezy podac typ wroga po slowie kluczowym ENEMY" << endl;
				return false;
			}
			czytaj_slowo();
			if (slowo_tmp[0] > 64 && slowo_tmp[0] < 70) {
				zestaw_map[licznik_map]->fale_wrogow[i][1] = slowo_tmp;
				cout << "typ wroga: " << slowo_tmp;
			}
			else {
				cout << "pamietaj aby podac typ wroga: A, B, C, D lub E" << endl;
				return false;
			}
			czytaj_slowo();
			if (slowo_tmp != "NUMBER") {
				cout << "Po typie wroga naleyz okreslic jego ilosc za pomoca slowa kkluczowego NUMER i liczby (dwucyfrowej)" << endl;
				return false;
			}
			czytaj_slowo();
			if (slowo_tmp.length() == 2 && istringstream(slowo_tmp) >> tmp && tmp > 0 && tmp < 11) { // jesli podano liczbe miedzy 0-10
				zestaw_map[licznik_map]->fale_wrogow[i][2] = slowo_tmp;
				cout << "  ilosc wroga: " << slowo_tmp;
			}
			else {
				cout << "liczba wrogów powinna miesc nastepujacy format 01-10" << endl;
				return false;
			}
			// wszystkie warunk skladni spelnione
		}
		// test - czy podano chociaz jedna fale wrogow
		if (zestaw_map[licznik_map]->fale_wrogow[0][0] == "") {
			cout << "nalezy podac chociaz jedna fale wrogow" << endl;
			return false;
		}
		if (slowo_tmp != "FOEEND") { // pomijane jesli fal bylo mniej niz 10
									 // for sie zakonczyl ostatnie wczytane slowo to numer wrogow, wyjscie prawidlowe
			czytaj_slowo();
			if (slowo_tmp != "FOEEND") { // test zakonczenia definicji foe
				cout << "brak slowa kluczowego FOEEND na koncu definicji fal wrogow" << endl;
				return false;
			}
		}
		cout << endl << endl;
		return true;
	}

	bool instrukcja_traps() {
		czytaj_slowo();
		if (slowo_tmp != "TRAPS") {
			cout << "brak definicj traps - jesli nie chcesz ustawiac pualpek zastosuje TRAPS TRAPSEND" << endl;
			return false;
		}
		for (int i = 0; i < 5; i++) {
			czytaj_slowo();
			int tmp;
			if (slowo_tmp.length() == 2 && istringstream(slowo_tmp) >> tmp && tmp > 4 && tmp <= max) {
				for (int j = 0; j < 5; j++) {
					if (slowo_tmp == zestaw_map[licznik_map]->pola_pulapek[j]) {
						cout << "pola pulapek nie powinny sie powtarzac" << endl;
						return false;
					}
				}
				zestaw_map[licznik_map]->pola_pulapek[i] = slowo_tmp;
				cout << "pulapka ustawiona na polu: " << slowo_tmp << endl;
			}
			else if (slowo_tmp == "TRAPSEND") {
				break;
			}
			else{
				cout << "niepoprawnie wpisany numer pola pulapki, numery powinny znajdowac sie na polach gracza (w ramach sciezki)" << endl;
				return false;
			}
		}
		// wczytano piec kolejnych liczb
		if (slowo_tmp != "TRAPSEND") {
			czytaj_slowo();
			if (slowo_tmp != "TRAPSEND") {
				cout << "nie napotkano slowa kluczowego TRAPSEND, pamietaj, ze pulapek nie moze byc wiecej niz 5" << endl;
				return false;
			}
		}
		return true;
	}

	bool instrukcja_end() {
		czytaj_slowo();
		if (slowo_tmp == "END") {
			cout << endl << "tworzenie mapy \"" << zestaw_map[licznik_map]->nazwa << "\" zostalo ukonczone" << endl;
			return true;
		}
		else {
			cout << "oczekiwane slowo kluczowe END po zakonczeniu definicji mapy" << endl;
			return false;
		}
	}

	bool test_ilosc_map() {
		if (licznik_map > 20) {
			cout << "Przekroczono ilosc mozliwych do zdefiniowania map" << endl;
			return false;
		}
		else {
			return true;
		}
	}

public:

	bool odczyt_pliku() { // odczytywanie pliku tekstowego mapy
		otworz_plik();
		while (!plik_mapy.eof()) { // puki plik sie nie skonczy
			if (!test_ilosc_map()) { // uznanu za blad by nie oszukwiac gracza co do map, ktore nie istnieja - nie zostaly wczytane
				return false;
			}
			if (!instrukcja_level()) { // sprawdzaj instrukcje level (wczytuje nazwe i tworzy obiekt mapy)
				return false;
			}
			if (!instukcja_map()) {
				return false;
			}
			if (!test_ciaglosci_sciezki()) {
				return false;
			}
			if (!instukcja_foe()) {
				return false;
			}
			if (!instrukcja_traps()) {
				return false;
			}
			if (!instrukcja_end()) {
				return false;
			}
			licznik_map++; // po sprawdzeniu wszystkich instrukcji wczytaj kolejna mape (o ile istnieje)
		}
		// skladnia poprawna dla wszystkich map
		cout << "poprawnie wczytano plik mapy" << endl; 
		return true; 
	}

	map_info_tekstowe *zestaw_map[20] = { nullptr }; // przechowuje mapy zdefiniowane w pliku mapy.txt masymalna ilosc map to 20

	mapy_wczytywanie_z_pliku(string sciezka_czytania1) {
		sciezka_czytania = sciezka_czytania1;
	}

	~mapy_wczytywanie_z_pliku() { // destruktor - zwalnia pamiec zarezerwowana na informacje o mapach
		int i = 0;
		while (zestaw_map[i] != nullptr) {
			delete zestaw_map[i];
		}
	}

};

//----------------------------------------------------POLA W GRZE

class pole {
public:
	// pozycja x i y w tablicy
	int wymiary = 80; // wymiary pola x i y
	int pos_x_tex; // pozycja x na teksturze
	int pos_y_tex; // pozycja y na teksturze
	int x;
	int y;
	sf::Sprite sprite_pola; // sprite pojedynczego pola

	void mapuj_teksture_dla_pola() { // przypisanie odpowiedniego prostokata dla spritea
		sprite_pola.setTextureRect(sf::IntRect(pos_x_tex, pos_y_tex, wymiary, wymiary));
	}

	void przesun_pole() { // przesuwa pole zaleznie od pozycji w tablicy
		sprite_pola.setPosition(sf::Vector2f(x*wymiary, y*wymiary));
	}

	pole(int x1, int y1, sf::Sprite s1) {
		// przypisanie pozycji pola przy konstrukcji
		sprite_pola = s1;
		x = x1;
		y = y1;
	}
};

class trawa :public pole {
public:
	trawa(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		pos_x_tex = 0; // pozycja x na teksturze
		pos_y_tex = 0; // pozycja y na teksturze
		mapuj_teksture_dla_pola(); // nadawanie tekstury przy kostrukcji pola
		przesun_pole();
	}
};

class droga :public pole {
public:
	droga(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		pos_x_tex = 240; // pozycja x na teksturze
		pos_y_tex = 0; // pozycja y na teksturze
		mapuj_teksture_dla_pola(); // nadawanie tekstury przy kostrukcji pola
		przesun_pole();
	}
};

class pole_wroga:public pole {
public:
	pole_wroga(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		pos_x_tex = 160; // pozycja x na teksturze
		pos_y_tex = 0; // pozycja y na teksturze
		mapuj_teksture_dla_pola(); // nadawanie tekstury przy kostrukcji pola
		przesun_pole();
	}
};

class pole_pulapki :public pole {
public:
	pole_pulapki(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		pos_x_tex = 320; // pozycja x na teksturze
		pos_y_tex = 0; // pozycja y na teksturze
		mapuj_teksture_dla_pola(); // nadawanie tekstury przy kostrukcji pola
		przesun_pole();
	}
};

class mur :public pole {
public:
	mur(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		pos_x_tex = 400; // pozycja x na teksturze
		pos_y_tex = 0; // pozycja y na teksturze
		mapuj_teksture_dla_pola(); // nadawanie tekstury przy kostrukcji pola
		przesun_pole();
	}
};

class pole_wiezy :public pole {
public:
	pole_wiezy(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		pos_x_tex = 480; // pozycja x na teksturze
		pos_y_tex = 0; // pozycja y na teksturze
		mapuj_teksture_dla_pola(); // nadawanie tekstury przy kostrukcji pola
		przesun_pole();
	}
};

//----------------------------------------------------KONWERSJA DO PAMIECI

class mapa_w_pamieci { // przechowuje informacje dotyczace jednej mapy
private:
	string nazwa_mapy;
	pole *pola_mapy[9][16];
	pole *pola_drogi[99];
	pole *pola_pulapek[5];
	pole *pola_wiez[10];
	sf::Sprite sprite_pola;
public:
	mapa_w_pamieci(string s1, sf::Sprite sp1) {
		nazwa_mapy = s1;
		sprite_pola = sp1;

	}

	void dodaj_pole_do_mapy(int x, int y, string kodowanie_pola) {
		int tmp;
		if (kodowanie_pola == "QQ") { // pole trawy
			pola_mapy[y][x] = new trawa(x, y, sprite_pola);
		}
		else if (kodowanie_pola == "XX"){ // pole wroga
			pola_mapy[y][x] = new pole_wroga(x, y, sprite_pola);
		}
		else if (kodowanie_pola == "@@"){ // mur
			pola_mapy[y][x] = new mur(x, y, sprite_pola);
		}
		else if (istringstream(kodowanie_pola) >> tmp) { // droga
			// tmp przechowuje numer pola
			pola_mapy[y][x] = new droga(x, y, sprite_pola);
			pola_drogi[tmp] = pola_mapy[y][x];
		}
		else if (kodowanie_pola[0] == '#' && (kodowanie_pola[1] > 47 && kodowanie_pola[1] < 58)) { // wieza
			// tmp przechowuje numer wiezy
			tmp = (int)kodowanie_pola[1];
			pola_mapy[y][x] = new pole_wiezy(x, y, sprite_pola);
			pola_wiez[tmp] = pola_mapy[y][x];

		}
	}

	void dodaj_pole_pulapki(int nr_pola_drogi, int nr_pulapki) {
		// dodawanie do tablicy pulapek na mapie
		pola_pulapek[nr_pulapki] = new pole_pulapki(pola_drogi[nr_pola_drogi]->x, pola_drogi[nr_pola_drogi]->y, sprite_pola);
		// zastepowanie pola na mapie polem pulapki
		pola_mapy[pola_drogi[nr_pola_drogi]->y][pola_drogi[nr_pola_drogi]->x] = pola_pulapek[nr_pulapki];
	}

	sf::Sprite zwroc_sprite_pola(int x_sprite, int y_sprite) {
		return pola_mapy[y_sprite][x_sprite]->sprite_pola;
	}

};

class mapy_wczytywanie_do_pamieci {
private:
	// generuje obiekt przechowujacy wczytane mapy w formie tekstowej z pliku mpay.txt
	mapy_wczytywanie_z_pliku *wczytane_mapy; 
	mapa_w_pamieci *glowny_zestaw_map[20]; // ilosc map jest ograniczona 
	int licznik_map = 0;
	sf::Sprite sprite_pola;

public:
	mapy_wczytywanie_do_pamieci(string s1, sf::Sprite spr1) {
		wczytane_mapy = new mapy_wczytywanie_z_pliku(s1);
		sprite_pola = spr1;
	}

	bool przenies_mapy_do_pamieci() {

		if (!wczytane_mapy->odczyt_pliku()) { // wykonanie odczytu z pliku - pelne debuggowanie zawartosci
			return false;
		}

		while (wczytane_mapy->zestaw_map[licznik_map] != nullptr) { // tak dlugo jak istnieja "tekstowe" mapy
			// znaleziono pierwsza mapie
			glowny_zestaw_map[licznik_map] = new mapa_w_pamieci(wczytane_mapy->zestaw_map[licznik_map]->nazwa, sprite_pola);
			//wczytwanie pol
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 16; j++) {
					glowny_zestaw_map[licznik_map]->dodaj_pole_do_mapy(j, i, wczytane_mapy->zestaw_map[licznik_map]->zestaw_pol[i][j]);
				}
			}

			// oznacznie pol pulapek
			int licznik_pulapek = 0;

			while (wczytane_mapy->zestaw_map[licznik_map]->pola_pulapek[licznik_pulapek] != "") {
				// tak dlugo jak w mapie tekstowej istnieja zdefiniowane pola pulapek
				int tmp;
				// konwertuj numery do intow
				istringstream(wczytane_mapy->zestaw_map[licznik_map]->pola_pulapek[licznik_pulapek]) >> tmp;
				// przypisz pulapke do danego pola - kolejnosc taka jak w pliku tekstowym
				glowny_zestaw_map[licznik_map]->dodaj_pole_pulapki(tmp, licznik_pulapek); 
				licznik_pulapek++;
			}
			licznik_map++;
		}
		return true;
	}

	sf::Sprite zwroc_wskazane_pole_mapy(int pos_x_pola, int pos_y_pola, int nr_mapy) {
		return glowny_zestaw_map[nr_mapy]->zwroc_sprite_pola(pos_x_pola, pos_y_pola);
	}

};

//----------------------------------------------------MAIN

int main()
{
	baza_spriteow *wczytane_spritey = new baza_spriteow("Textures/slot_mapping.png");
	mapy_wczytywanie_do_pamieci *wczytane_mapy = new mapy_wczytywanie_do_pamieci("Maps/mapy.txt", wczytane_spritey->sprite_pola_planszy);
	if (!wczytane_mapy->przenies_mapy_do_pamieci()) { // wyjscie w przypadku niepoprawnego wczytywania
		return 0;
	}

	
	sf::RenderWindow window(sf::VideoMode(1280, 720), "TD project");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				window.draw(wczytane_mapy->zwroc_wskazane_pole_mapy(j, i, 0));
			}
		}
		window.display();
	}
	return 0;
}

