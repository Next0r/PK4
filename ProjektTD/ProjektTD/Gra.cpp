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

	int aktualny_uklad = 0;
	TranslatorWejscia *translator_wejscia = new TranslatorWejscia();
	ManagerUkladow *manager_ukladow;

	pair<int, string> mapa_ukladow[ILOSC_KLAWISZY][ILOSC_UKLADOW] =
	{
		{ { 1,"strt" },{ 2,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ -1,"prev" } },
		{ { 1,"cont" },{ 5,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ -2,"next" } },
		{ { 7,"info" },{ 0,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { -3,"quit" },{ 0,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 4,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 6,"null" },{ 6,"null" },{ 6,"null" },{ 6,"null" },{ 6,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },{ 0,"null" },{ 1,"null" },{ 1,"null" },{ 2,"null" },{ 3,"null" },{ 0,"null" },{ 0,"back" } },
	};

	void wykonaj_akcje(int nr_akcji, int &flaga_wyjscia) {
		switch (nr_akcji) {
		case -1:
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, -1);
			break;
		case -2:
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, 1);
			break;
		case -3:
			flaga_wyjscia = 1;
			break;
		default:
			break;
		}	
	}

public:

	WejscieWyjscie(ManagerUkladow *mgr_ukladow) {
		manager_ukladow = mgr_ukladow;
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
					warstwa_renderowania = manager_ukladow->zwroc_uklad(aktualny_uklad);
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
					warstwa_renderowania = manager_ukladow->zwroc_uklad(aktualny_uklad);
				}
			}	
		}
	}

};




int main() {
	int flaga_wyjscia = 0; // okresla czy mozna zamknac okno

	RysowaneObiekty *baza_obiektow = new RysowaneObiekty();
	Plik_map *mapy = new Plik_map();
	ManagerUkladow *manager_ukladow = new ManagerUkladow(baza_obiektow);
	WejscieWyjscie *wejscie_wyjscie = new WejscieWyjscie(manager_ukladow);
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