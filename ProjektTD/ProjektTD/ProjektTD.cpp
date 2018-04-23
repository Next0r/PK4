// ProjektTD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Baza.h"

using namespace std;

default_random_engine random_eng(time(0));
uniform_int_distribution<int> distr(0, 1);
uniform_int_distribution<int> distr2(0, 3);

//----------------------------------------------------TODO


// zmiana nazw klas
// zapisywanie informacji o wrogach do pamieci
// debugger dla slow kluczowych MONEY dla mapy
//sprawdzanie powtarzania sie numeracji sciezki w debuggerze

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

//----------------------------------------------------GRA

class Gra {
private:
public:

};

class TeksturySpritey {
private:
	sf::Texture tekstura_pola;
	sf::Texture tekstura_przyciskow;
	sf::Texture tekstura_tla;
	sf::Sprite pole_planszy;
	sf::Sprite przycisk;
	sf::Sprite tlo_duze;
	sf::Sprite tlo_male;

public:
	TeksturySpritey(string sciezka_do_tx_pola, string sciezka_do_tx_przyciksu, string sciezka_do_tx_tla) {
		tekstura_pola.loadFromFile(sciezka_do_tx_pola);
		pole_planszy.setTexture(tekstura_pola);
		tekstura_przyciskow.loadFromFile(sciezka_do_tx_przyciksu);
		przycisk.setTexture(tekstura_przyciskow);
		tekstura_tla.loadFromFile(sciezka_do_tx_tla);
		tlo_duze.setTexture(tekstura_tla);
		tlo_male.setTexture(tekstura_tla);
	}

	sf::Sprite zwroc_sprite(int nr_spritea) { // nr pola oznacza konkretny sprite domyslnie zwraca nr pola
		switch (nr_spritea) {
		case 1:
			return pole_planszy;
			break;
		case 2:
			return przycisk;
			break;
		case 3:
			return tlo_duze;
			break;
		case 4:
			return tlo_male;
			break;
		default:
			return pole_planszy;
			break;
		}
	}
};

//----------------------------------------------------INTERFACE

class UkladInterfaceu {
	
protected:
	vector<sf::Sprite> spritey_ukladu;

	void mapuj_sprite(int x, int y, int wymiary_x, int wymiary_y, sf::Sprite mapowany_sprite) {
		mapowany_sprite.setTextureRect(sf::IntRect(x, y, wymiary_x, wymiary_y));
	}

	void ustaw_sprite(int x, int y, sf::Sprite ustawiany_sprite) {
		ustawiany_sprite.setPosition(sf::Vector2f(x, y));
	}
	
public:
	vector<sf::Sprite> zwroc_uklad() {
		return spritey_ukladu;
	}
};

class UkladMenu : public UkladInterfaceu {
private:
	sf::Sprite tlo_menu;
	sf::Sprite przycisk;
	int ilosc_przyciskow = 4;
	int odstep = 20;
	int rozmiar_x_przycisku = 320;
	int rozmiar_y_przycisku = 80;
	void generuj_uklad() {
		mapuj_sprite(0, 0, 1280, 720, tlo_menu);
		ustaw_sprite(0, 0, tlo_menu);
		spritey_ukladu.push_back(tlo_menu);

		int tmp = (720 - ilosc_przyciskow*rozmiar_y_przycisku - (ilosc_przyciskow - 1)*odstep) / 2;

		for (int i = 0; i < ilosc_przyciskow; i++) {
			mapuj_sprite(0, i*80, rozmiar_x_przycisku, rozmiar_y_przycisku, przycisk);
			
			//ustaw_sprite((1280 - rozmiar_x_przycisku) / 2, tmp + i*100, przycisk);
			ustaw_sprite(200, 200, przycisk);
			spritey_ukladu.push_back(przycisk);
		}

	}
	

public:
	UkladMenu(TeksturySpritey *&wczytane_spritey) {
		tlo_menu = wczytane_spritey->zwroc_sprite(3);
		przycisk = wczytane_spritey->zwroc_sprite(2);
		generuj_uklad();
	}
};


//----------------------------------------------------WCZYTYWANIE MAPY/KREATOR

struct DaneFaliWrogow { // przechowuje informacje o pojedynczej fali wrogow na mapie
	friend class DaneMapy;
	friend class KontenerMapTekstowych;
private:
	int czas_odstepu;
	char typ_wroga;
	int ilosc_wrogow = 0;
public:
	void wczytaj_czas_dostepu(int czas) {
		czas_odstepu = czas;
	}

	void wczytaj_typ_wroga(char typ) {
		typ_wroga = typ;
	}

	void wczytaj_ilosc_wrogow(int ilosc) {
		ilosc_wrogow = ilosc;
	}

	string zwroc_dane_o_fali() {
		return "Czas odstepu " + to_string(czas_odstepu) + ", typ wroga: " + typ_wroga + ", ilosc wrogow: " + to_string(ilosc_wrogow);
	}
};



class DaneMapy { // przechowuje dane dotyczace mapy wczytanej z pliku
	friend class MapyPamiec;
	friend class KontenerMapTekstowych;
private:
	string nazwa_mapy;
	int nr_pola_pulapki[5] = { -1,-1,-1,-1,-1 };
	char plansza[9][16][3];
	int dostepne_pieniadze = -1;
	DaneFaliWrogow fale_wrogow[10];
	int sciezka_na_mapie[100] = { -1 };
	int ostatnie_pole_sciezki = 0;
public:
	void nadaj_nazwe(string nazwa) { 
		nazwa_mapy += (nazwa + " ");
	}

	void wpisz_kod_pola(int x, int y, char kod[3]) {
		strcpy_s(plansza[y][x], kod);
	}

	void wpisz_pulapke(int numer_p, int index) {
		nr_pola_pulapki[index] = numer_p;
	}

	void wypisz_dane() {
		cout << "nazwa mapy: " << nazwa_mapy << endl << endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				cout << plansza[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "pulapki zdefiniowano na polach sciezki: " << endl;
		int i = 0;
		while (nr_pola_pulapki[i] != -1) {
			cout << nr_pola_pulapki[i] << " ";
			i++;
		}

		cout << endl << endl;
		
		int iterator = 0;
		while (fale_wrogow[iterator].ilosc_wrogow != 0) {
			cout << fale_wrogow[iterator].zwroc_dane_o_fali() << endl;
			iterator++;
		}


		cout << endl << endl;

	}

	bool test_ciaglosci_sciezki() {

		if (sciezka_na_mapie[0] != 0) {
			return false;
		}
		// wyszukaj najwiekszy element w tablicy sciezki - potencjalnie ostatni
		for (int i = 0; i < 100; i++) {
			if (sciezka_na_mapie[i] > ostatnie_pole_sciezki && sciezka_na_mapie[i] < 100) {
				ostatnie_pole_sciezki = sciezka_na_mapie[i];
			}
		}
		// max przechowuje ostatnie pole sciezki (mniejsze od 100)
		for (int i = 0; i < ostatnie_pole_sciezki; i++) {
			if (sciezka_na_mapie[i + 1] != (i + 1)) {
				// jesli element mapy (lacznie z max) nie jest zgodny z indeksem w tablicy (tablica badana po kolei)
				return false;
			}
		}
		return true;
	}

	bool test_powtarzania_pulapki(int nr_pulapki) {
		for (int i = 0; i < 5; i++) {
			if (nr_pulapki == nr_pola_pulapki[i]) {
				return false;
			}
		}
		return true;
	}

}; 

class KontenerMapTekstowych { // klasa zarzadzajaca danymi dot. mapy
	friend class MapyPamiec;
	friend class DaneMapy;
private:
	//---------------
	int ograniczenie_pierwszego_pola_pulapki = 5;
	int max_ilosc_pulapek = 5;
	int max_ilosc_wrogow_w_fali = 10; 
	int max_czas_odstepu_fali = 20;
	int max_rozmiar_nazwy = 50;
	//---------------
	int licznik_map = 0;
	ifstream plik_mapy; // plik z zestawem map
	string sciezka_do_pliku_map;
	string wczytane_slowo;
	int liczba_tmp; // zmienna dla konwersji do int
	char kod_pola_tmp[3]; // zmienna tymczasowa dla kodu pola
	DaneMapy *zestaw_wczytanych_map[20] = { nullptr }; // przechowuje mapy zdefiniowane w pliku mapy.txt masymalna ilosc map to 20

	bool otworz_plik() {
		// plik z mapami
		plik_mapy.open(sciezka_do_pliku_map);
		if (!plik_mapy.good()) { // test otwarcia pliku
			cout << "blad wczytywania pliku map" << endl;
			return false;
		}
		else {
			return true;
		}
	}

	void czytaj_slowo() { // wczytywanie jednego slowa (do bilaego znaku)
		plik_mapy >> wczytane_slowo;
	}

	bool instrukcja_level() {
		czytaj_slowo();
		if (wczytane_slowo == "LEVEL") {
			zestaw_wczytanych_map[licznik_map] = new DaneMapy(); // instukcja level generuje obiekt info mapy
			czytaj_slowo();
			while (wczytane_slowo != "LEVELEND") { // wczytywanie nazwy poziomu z pliku
				zestaw_wczytanych_map[licznik_map]->nadaj_nazwe(wczytane_slowo);
				czytaj_slowo();
				if (zestaw_wczytanych_map[licznik_map]->nazwa_mapy.size() > max_rozmiar_nazwy) { // zabiezpieczenie co do dlugosci nazwy mapy
					cout << "nazwa wczytywanej mapy jest zbyt dluga (pamietaj o instukcji LEVELEND)" << endl;
					return false;
				}
			}
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
		else {
			kod_pola_tmp[0] = pole[0];
			kod_pola_tmp[1] = pole[1];
		}
		if (pole == "XX" || pole == "QQ" || pole == "@@") { // jesli nie jest to pole zabronione, puste lub mur
			return true;
		}
		else { // jest to pole zabronione, mur lub pole puste

			if (kod_pola_tmp[0] == '#' && kod_pola_tmp[1] > 47 && kod_pola_tmp[1] < 58) { // jesli jes to pole wiezy - #<0-9>
				return true;
			}
			else if (istringstream(kod_pola_tmp) >> liczba_tmp) { // jesli nie jest to pole wiezy to musi byc to pole sciezki		
				zestaw_wczytanych_map[licznik_map]->sciezka_na_mapie[liczba_tmp] = liczba_tmp; // zapamietaj istnienie tego pola (dla testu poprawnosci wytyczenia sciezki)
				return true;
			}
			else {
				return false; // dwuliterowe slowo nie jest prawidlowe 
			}

		}
	}

	bool instukcja_map() {
		czytaj_slowo();
		if (wczytane_slowo == "MAP") {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 16; j++) {
					czytaj_slowo(); // wczytywanie pola mapy (z pliku)
					if (test_poprawnosci_pola(wczytane_slowo)) { // jesli wczytane pole ma poprawna skladnie
						zestaw_wczytanych_map[licznik_map]->wpisz_kod_pola(j, i, kod_pola_tmp);
					}
					else {
						cout << "blad w skaldni mapy" << endl;
						return false;
					}
				}
			}
			czytaj_slowo(); // wczytuje slowo kluczowe MAPEND
			if (wczytane_slowo != "MAPEND") {
				cout << "brak slowa kluczowego MAPEND" << endl;
				return false;
			}
			if (zestaw_wczytanych_map[licznik_map]->test_ciaglosci_sciezki()) {
				return true;
			}
			else {
				cout << "blad w numeracji sciezki" << endl;
				return false;
			}
		}
		else {
			cout << "blad - brak slowa kluczowego MAP po instrukcji LEVELEND" << endl;
			return false;
		}
	}

	bool instukcja_foe() {
		czytaj_slowo();
		if (wczytane_slowo != "FOE") {
			cout << "oczekiwana instrukcja FOE" << endl;
			return false;
		}
		for (int i = 0; i < 10; i++) { // maksymalnie 10 fal wrogow
			czytaj_slowo();
			if (wczytane_slowo == "FOEEND") {
				break;
			}
			if (wczytane_slowo != "WAVE:") {
				cout << "konstukcje fali wrogow nalezy rozpoczac slowem kluczowym WAVE:" << endl;
				return false;
			}
			// wczytano slowo wave
			czytaj_slowo();
			if (wczytane_slowo != "TIME") {
				cout << "Po slowie kluczowym WAVE: nalezy uzyc TIME by okreslic czas odstepu fali" << endl;
				return false;
			}
			czytaj_slowo();
			if (wczytane_slowo.length() == 2 && istringstream(wczytane_slowo) >> liczba_tmp && liczba_tmp <= max_czas_odstepu_fali && liczba_tmp > 0) {
				zestaw_wczytanych_map[licznik_map]->fale_wrogow[i].wczytaj_czas_dostepu(liczba_tmp);
			}
			else {
				cout << "czas odstepu powinien byc dwucyfrowa liczba od 01 do 20 wlacznie" << endl;
				return false;
			}
			czytaj_slowo();
			if (wczytane_slowo != "ENEMY") {
				cout << "Po slowie kluczowym TIME i liczbie nalezy podac typ wroga po slowie kluczowym ENEMY" << endl;
				return false;
			}
			czytaj_slowo();
			if (wczytane_slowo[0] > 64 && wczytane_slowo[0] < 70) {
				zestaw_wczytanych_map[licznik_map]->fale_wrogow[i].wczytaj_typ_wroga(wczytane_slowo[0]);
			}
			else {
				cout << "pamietaj aby podac typ wroga: A, B, C, D lub E" << endl;
				return false;
			}
			czytaj_slowo();
			if (wczytane_slowo != "NUMBER") {
				cout << "Po typie wroga naleyz okreslic jego ilosc za pomoca slowa kkluczowego NUMER i liczby (dwucyfrowej)" << endl;
				return false;
			}
			czytaj_slowo();
			if (wczytane_slowo.length() == 2 && istringstream(wczytane_slowo) >> liczba_tmp && liczba_tmp > 0 && liczba_tmp <= max_ilosc_wrogow_w_fali) { // jesli podano liczbe miedzy 0-10
				zestaw_wczytanych_map[licznik_map]->fale_wrogow[i].wczytaj_ilosc_wrogow(liczba_tmp);
			}
			else {
				cout << "liczba wrogów powinna miesc nastepujacy format 01-10" << endl;
				return false;
			}
			// wszystkie warunk skladni spelnione
		}
		// test - czy podano chociaz jedna fale wrogow
		if (zestaw_wczytanych_map[licznik_map]->fale_wrogow[0].ilosc_wrogow == 0) {
			cout << "nalezy podac chociaz jedna fale wrogow" << endl;
			return false;
		}
		if (wczytane_slowo != "FOEEND") { // pomijane jesli fal bylo mniej niz 10
										 // for sie zakonczyl ostatnie wczytane slowo to numer wrogow, wyjscie prawidlowe
			czytaj_slowo();
			if (wczytane_slowo != "FOEEND") { // test zakonczenia definicji foe
				cout << "brak slowa kluczowego FOEEND na koncu definicji fal wrogow" << endl;
				return false;
			}
		}
		return true;
	}

	bool instrukcja_traps() {
		czytaj_slowo();
		if (wczytane_slowo != "TRAPS") {
			cout << "brak definicj traps - jesli nie chcesz ustawiac pualpek zastosuje TRAPS TRAPSEND" << endl;
			return false;
		}
		for (int i = 0; i < max_ilosc_pulapek; i++) {
			czytaj_slowo();
			liczba_tmp;
			if (wczytane_slowo.length() == 2 && istringstream(wczytane_slowo) >> liczba_tmp && liczba_tmp > ograniczenie_pierwszego_pola_pulapki && liczba_tmp <= zestaw_wczytanych_map[licznik_map]->ostatnie_pole_sciezki) {
				if (!zestaw_wczytanych_map[licznik_map]->test_powtarzania_pulapki(liczba_tmp)) {
					cout << "pola pulapek nie powinny sie powtarzac" << endl;
					return false;
				}
				zestaw_wczytanych_map[licznik_map]->wpisz_pulapke(liczba_tmp, i);
			}
			else if (wczytane_slowo == "TRAPSEND") {
				break;
			}
			else{
				cout << "niepoprawnie wpisany numer pola pulapki, numery powinny znajdowac sie na polach gracza (w ramach sciezki)" << endl;
				return false;
			}
		}
		// wczytano piec kolejnych liczb
		if (wczytane_slowo != "TRAPSEND") {
			czytaj_slowo();
			if (wczytane_slowo != "TRAPSEND") {
				cout << "nie napotkano slowa kluczowego TRAPSEND, pamietaj, ze pulapek nie moze byc wiecej niz 5" << endl;
				return false;
			}
		}
		return true;
	}

	bool instrukcja_end() {
		czytaj_slowo();
		if (wczytane_slowo == "END") {
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
			else if (!instrukcja_level()) { // sprawdzaj instrukcje level (wczytuje nazwe i tworzy obiekt mapy)
				return false;
			}
			else if (!instukcja_map()) {
				return false;
			}
			else if (!instukcja_foe()) {
				return false;
			}
			else if (!instrukcja_traps()) {
				return false;
			}
			else if (!instrukcja_end()) {
				return false;
			}
			zestaw_wczytanych_map[licznik_map]->wypisz_dane();
			licznik_map++; // po sprawdzeniu wszystkich instrukcji wczytaj kolejna mape (o ile istnieje)
		}
		// skladnia poprawna dla wszystkich map
		cout << "poprawnie wczytano plik mapy" << endl; 
		return true; 
	}

	KontenerMapTekstowych(string sciezka_do_pliku) {
		sciezka_do_pliku_map = sciezka_do_pliku;
	}

	~KontenerMapTekstowych() { // destruktor - zwalnia pamiec zarezerwowana na informacje o mapach
		int i = 0;
		while (zestaw_wczytanych_map[i] != nullptr) {
			delete zestaw_wczytanych_map[i];
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
private:
	void losuj_mapping() {
		int tmp = distr(random_eng);
		switch (tmp)
		{
		case 0:
			pos_x_tex = 0; // pozycja x na teksturze
			pos_y_tex = 0; // pozycja y na teksturze
			break;
		case 1:
			pos_x_tex = 80; // pozycja x na teksturze
			pos_y_tex = 0; // pozycja y na teksturze
			break;
		}	
	}
public:
	trawa(int x1, int y1, sf::Sprite sprite1) : pole(x1, y1, sprite1) { // nadawanie pozycji pola przy konstukcji
		losuj_mapping();
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

class DaneMapyPamiec { // przechowuje informacje dotyczace jednej mapy
private:
	string nazwa_mapy;
	pole *pola_mapy[9][16];
	pole *pola_drogi[99];
	pole *pola_pulapek[5];
	pole *pola_wiez[10];
	sf::Sprite sprite_pola;
public:
	DaneMapyPamiec(string s1, sf::Sprite sp1) {
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
			tmp = kodowanie_pola[1] - 48;
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

class MapyPamiec {
private:
	// generuje obiekt przechowujacy wczytane mapy w formie tekstowej z pliku mpay.txt
	KontenerMapTekstowych *wczytane_mapy; 
	DaneMapyPamiec *glowny_zestaw_map[20]; // ilosc map jest ograniczona 
	int licznik_map = 0;
	sf::Sprite sprite_pola;

public:
	MapyPamiec(string s1, TeksturySpritey *&zestaw_spriteow) {
		wczytane_mapy = new KontenerMapTekstowych(s1);
		sprite_pola = zestaw_spriteow->zwroc_sprite(1);
	}

	bool przenies_mapy_do_pamieci() {

		if (!wczytane_mapy->odczyt_pliku()) { // wykonanie odczytu z pliku - pelne debuggowanie zawartosci
			return false;
		}

		while (wczytane_mapy->zestaw_wczytanych_map[licznik_map] != nullptr) { // tak dlugo jak istnieja "tekstowe" mapy
			// znaleziono pierwsza mapie
			glowny_zestaw_map[licznik_map] = new DaneMapyPamiec(wczytane_mapy->zestaw_wczytanych_map[licznik_map]->nazwa_mapy, sprite_pola);
			//wczytwanie pol
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 16; j++) {
					glowny_zestaw_map[licznik_map]->dodaj_pole_do_mapy(j, i, wczytane_mapy->zestaw_wczytanych_map[licznik_map]->plansza[i][j]);
				}
			}

			// oznacznie pol pulapek
			int licznik_pulapek = 0;
			while (wczytane_mapy->zestaw_wczytanych_map[licznik_map]->nr_pola_pulapki[licznik_pulapek] != -1) {
				// tak dlugo jak w mapie tekstowej istnieja zdefiniowane pola pulapek
				glowny_zestaw_map[licznik_map]->dodaj_pole_pulapki(wczytane_mapy->zestaw_wczytanych_map[licznik_map]->nr_pola_pulapki[licznik_pulapek], licznik_pulapek);
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
	TeksturySpritey *wczytane_spritey = new TeksturySpritey("Textures/slot_mapping.png",
															"Textures/buttons_set.png",
															"Textures/menu_background.png"
															);


	MapyPamiec *wczytane_mapy = new MapyPamiec("Maps/mapy.txt", wczytane_spritey);
	if (!wczytane_mapy->przenies_mapy_do_pamieci()) { // wyjscie w przypadku niepoprawnego wczytywania
		return 0;
	}

	UkladMenu * menu = new UkladMenu(wczytane_spritey);
	vector <sf::Sprite> test_layout = menu->zwroc_uklad();
	
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

		for (auto i = test_layout.begin(); i < test_layout.end(); i++) {
			window.draw(*i);
		}



		//for (int i = 0; i < 9; i++) {
		//	for (int j = 0; j < 16; j++) {
		//		window.draw(wczytane_mapy->zwroc_wskazane_pole_mapy(j, i, 0));
		//	}
		//}
		window.display();
	}
	return 0;
}

