#include "stdafx.h"
#include "Baza.h"
#include "WczytywanieMapy.h"
#include "RysowaneObiekty.h"
#include "Uklady.h"

using namespace std;

class TranslatorWejscia {
private:
	map<int, int> mapa_wejsc =
	{
		// para - wejscie event.key.code i kod_klawisza z mapy ukladow	
		{ 27,0 },	// 1
		{ 28,1 },	// 2
		{ 29,2 },	// 3
		{ 30,3 },	// 4
		{ 31,4 },	// 5
		{ 32,5 },	// 6
		{ 33,6 },	// 7
		{ 34,7 },	// 8
		{ 35,8 },	// 9
		{ 26,9 },	// 0
		{ 18,10 },	// S
		{ 4,11 },	// E
		{ 1,12 },	// B
	};

public:
	int tlumacz_wejscie(int klucz) {
		if (mapa_wejsc.find(klucz) != mapa_wejsc.end()) {
			return mapa_wejsc.find(klucz)->second;
		}
		else {
			return -1;
		}
	}
};

class WejscieWyjscie {
private:

	int poziom_gry = 0;
	int aktualny_uklad = 0;
	TranslatorWejscia *translator_wejscia = new TranslatorWejscia();
	ManagerUkladow *manager_ukladow;
	Plik_map *mapy;
	// mapa ukladow:
	// nr      |   0  |   1  |    2     |     3    |     4      |     5      |  6   |   7  |
	// klawisz | menu | shop | buy turr | buy trap | chose turr | chose trap | game | info |
	// 1    0  |   1  |   2  |    4     |    5     |     ts     |     as     |  u1  |  np  |
	// 2    1  |   1  |   5  |    4     |    5     |     ts     |     as     |  u2  |  pp  |
	// 3    2  |   7  |      |    4     |    5     |     ts     |     as     |  u3  |      |
	// 4    3  | quit |      |    4     |    5     |            |            |  u4  |      |
	// 5    4  |      |      |    4     |    5     |            |            |  u5  |      |
	// 6    5  |      |      |    4     |          |            |            |      |      |
	// 7    6  |      |      |    4     |          |            |            |      |      |
	// 8    7  |      |      |    4     |          |            |            |      |      |
	// 9    8  |      |      |    4     |          |            |            |      |      |
	// 0    9  |      |      |    4     |          |            |            |      |      |
	// S    10 |      |   6  |    6     |    6     |      6     |      6     |      |      |
	// E    11 |      |      |          |          |            |            |   0  |      |
	// B    12 |      |   0  |    1     |    1     |      2     |      3     |      |   0  |
	pair<int, string> mapa_ukladow[ILOSC_KLAWISZY][ILOSC_UKLADOW] =
	{
		{ { -4,"strt" },{ -5,"turr" },{ -6,"slt0" },{ 5,"null" },{ 4,"typ0" },{ 0,"null" },{ 0,"null" },{ -1,"prev" } },
		{ { 1,"cont" },{ 5,"trap" },{ -6,"slt1" },{ 5,"null" },{ 4,"typ1" },{ 0,"null" },{ 0,"null" },{ -2,"next" } },
		{ { 7,"info" },{ 1,"null" },{ -6,"slt2" },{ 5,"null" },{ 4,"typ2" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { -3,"quit" },{ 1,"null" },{ -6,"slt3" },{ 5,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ -6,"slt4" },{ 5,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ -6,"slt5" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ -6,"slt6" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ -6,"slt7" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ -6,"slt8" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ -6,"slt9" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 6,"strt" },{ 6,"strt" },{ 6,"null" },{ 6,"strt" },{ 6,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 1,"null" },{ 2,"null" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"back" },{ 1,"back" },{ 1,"null" },{ 2,"back" },{ 3,"null" },{ 0,"null" },{ 0,"back" } },
	};

	void wykonaj_akcje(int nr_akcji, int &flaga_wyjscia) {
		switch (nr_akcji) {
		case -1: // przycisk prev w ukladzie info
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, -1);
			break;
		case -2: // przycisk next w ukladzie info
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, 1);
			break;
		case -3: // przycisk quit w ukladzie menu 
			flaga_wyjscia = 1;
			break;
		case -4: // przycisk start w ukladzie menu
			aktualny_uklad = 1;
			poziom_gry = 0;
			break;
		case -5: // przycisk turr w ukladzie sklep
			aktualny_uklad = 2;
			manager_ukladow->ustaw_ilosc_przyciskow_sklepu(aktualny_uklad, mapy->zwroc_ilosc_wiez_na_mapie(poziom_gry));			
			break;
		case -6: // przycisk slot w ukladzie wieze_sklep
			aktualny_uklad = 4;
			manager_ukladow->ustaw_ilosc_przyciskow_sklepu(aktualny_uklad, ILOSC_TYPOW_WIEZ);
			break;
		default:
			break;
		}	
	}
	map<string, sf::Sprite> zbuduj_warstwe_renderowania() {
		// test funkcji pola liczbowego
		manager_ukladow->ustaw_wartosc_pola_liczbowego(aktualny_uklad, "1234");
		// test funkcji pola liczbowego
		if (aktualny_uklad > 0 && aktualny_uklad < 7) {
			// jesli uklady sa ukladami z tlem w postaci mapy
			map<string, sf::Sprite> map_tmp = mapy->zwroc_mape(poziom_gry)->zwroc_pola_na_mapie();
			map<string, sf::Sprite> map_tmp2 = manager_ukladow->zwroc_uklad(aktualny_uklad);
			map_tmp.insert(map_tmp2.begin(), map_tmp2.end());
			return map_tmp;
		}
		else {
			// uklad nie zawiera mapy jako tla
			return manager_ukladow->zwroc_uklad(aktualny_uklad);
		}
	
	}
public:

	WejscieWyjscie(ManagerUkladow *&mgr_ukladow, Plik_map *&plik_map) {
		manager_ukladow = mgr_ukladow;
		mapy = plik_map;
	}

	void przyjmij_wejscie(sf::Event ev1, map<string, sf::Sprite> &warstwa_renderowania, int &flaga_wyjscia) {
		if (ev1.type == sf::Event::KeyPressed || ev1.type == sf::Event::KeyReleased) {
			int kod_klawisza = translator_wejscia->tlumacz_wejscie(ev1.key.code);
			if (kod_klawisza != -1) {
				if (ev1.type == sf::Event::KeyPressed) {
					// nacisnieto przycisk
					string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;
					manager_ukladow->nacisnij_przycisk_w_ukladzie(nazwa_przycisku, aktualny_uklad);

					// zwraca uklad
					warstwa_renderowania = zbuduj_warstwe_renderowania();
				}
				else {
					// puszczono przycisk		
					string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;
					manager_ukladow->przywroc_przycisk_w_ukladzie(nazwa_przycisku, aktualny_uklad);

					// podajemowanie akcji
					int nr_akcji = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
					
					if (nr_akcji >= 0) { // gdy nalezy przejsc do innego ukladu
						aktualny_uklad = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
					}
					else { // gdy nalezy wykonac modyfikacje w aktualnym ukladzie
						wykonaj_akcje(nr_akcji, flaga_wyjscia);
					}
					// zwraca uklad
					warstwa_renderowania = zbuduj_warstwe_renderowania();
				}
			}	
		}
	}

};

int main() {
	int flaga_wyjscia = 0; // okresla czy mozna zamknac okno

	RysowaneObiekty *baza_obiektow = new RysowaneObiekty();
	Plik_map *mapy = new Plik_map(baza_obiektow);
	ManagerUkladow *manager_ukladow = new ManagerUkladow(baza_obiektow);
	WejscieWyjscie *wejscie_wyjscie = new WejscieWyjscie(manager_ukladow, mapy);

	//map<string, sf::Sprite> warstwa_renderowana2 = mapy->zwroc_mape(0)->zwroc_pola_na_mapie();

	//TranslatorWejscia *translator_wejscia = new TranslatorWejscia();
	map<string, sf::Sprite> warstwa_renderowana = manager_ukladow->zwroc_uklad(0);

	if (mapy->blad_pliku_map) {
		cout << "Blad wczytywania pliku map." << endl;
		return 0;
	}

	sf::RenderWindow window(sf::VideoMode(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y), NAZWA_OKNA);
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		//----------obsluga wejscia
		sf::Event event;
		while (window.pollEvent(event))
		{
			wejscie_wyjscie->przyjmij_wejscie(event, warstwa_renderowana, flaga_wyjscia);
			if (flaga_wyjscia == 1) {
				window.close();
			}
		}

		// ---------- renderowanie
		window.clear();
		for (auto i = warstwa_renderowana.begin(); i != warstwa_renderowana.end(); i++) {
			window.draw(i->second);
		}
		window.display();
	}

	return 0;
}