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

int main() {
	RysowaneObiekty *baza_obiektow = new RysowaneObiekty();
	Plik_map *mapy = new Plik_map();
	ManagerUkladow *manager_ukladow = new ManagerUkladow(baza_obiektow);
	TranslatorWejscia *translator_wejscia = new TranslatorWejscia();

	map<string, sf::Sprite> warstwa_renderowana = manager_ukladow->zwroc_uklad();

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
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
				int wejscie_tmp = translator_wejscia->tlumacz_wejscie(event.key.code);
				if (wejscie_tmp != -1) {
					if (event.type == sf::Event::KeyPressed) {
						// nacisnieto przycisk
						manager_ukladow->animuj_przycisk_ukladu(wejscie_tmp);
						warstwa_renderowana = manager_ukladow->zwroc_uklad();
					}
					else {
						// puszczono przycisk
						manager_ukladow->zmien_modyfikuj_uklad(wejscie_tmp);
						warstwa_renderowana = manager_ukladow->zwroc_uklad();
					}
				}
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
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