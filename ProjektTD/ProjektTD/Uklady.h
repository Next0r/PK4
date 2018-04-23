#pragma once
#include "Baza.h"
#include "RysowaneObiekty.h"

using namespace std;

class Uklad {
protected:

	sf::Sprite el_ukladu;
	int odstep_el = 20;
	int przycisk_x = 320;
	int przycisk_y = 80;
	int strona_x = 540;
	int strona_y = 285;

	string id_tla = "0bg";
	string id_przycisku = "1b";
	string id_pola_tekstowego = "2p_text";
	// liczba na poczatku oznacza kolejnosc renderowania el
	map<string, sf::Sprite> zestaw_spriteow;
	// identyfikatory: bg, b_<nazwa>, p_text itp

	void mapuj_sprite(int wymiar_x, int wymiar_y, int pos_x, int pos_y, sf::Sprite &mapowany) {
		mapowany.setTextureRect(sf::IntRect(pos_x, pos_y, wymiar_x, wymiar_y));
	}
	void ustaw_sprite(int pos_x, int pos_y, sf::Sprite &ustawiany) {
		ustawiany.setPosition(sf::Vector2f(pos_x, pos_y));
	}
	void ustaw_przycisk(int ekran_x, int ekran_y, int nr_przycisku, int ilosc_przyciskow, sf::Sprite &przycisk, int przycisk_x, int przycisk_y) {
		int odstep = 20;
		int wyjsciowe_x = ekran_x / 2 - przycisk_x / 2;
		int wyjsciowe_y = ekran_y / 2 - przycisk_y / 2;
		wyjsciowe_y -= ((przycisk_y + odstep) / 2)*(ilosc_przyciskow - 1);
		wyjsciowe_y += (nr_przycisku - 1)*(przycisk_y + odstep);
		ustaw_sprite(wyjsciowe_x, wyjsciowe_y, przycisk);
	}

public:
	map<string, sf::Sprite> zwroc_uklad() {
		return zestaw_spriteow;
	}

	void nacisnij_przycisk(string nazwa_przycisku) {
		string id_pelne = id_przycisku + nazwa_przycisku;
		auto tmp_el = zestaw_spriteow.find(id_pelne);
		if (tmp_el != zestaw_spriteow.end()) {
			int wymiar_x = tmp_el->second.getTextureRect().width;
			int wymiar_y = tmp_el->second.getTextureRect().height;
			int pos_y = tmp_el->second.getTextureRect().top;
			mapuj_sprite(wymiar_x, wymiar_y, wymiar_x, pos_y, tmp_el->second);
		}
	}

	void przywroc_przycisk(string nazwa_przycisku) {
		string id_pelne = id_przycisku + nazwa_przycisku;
		auto tmp_el = zestaw_spriteow.find(id_pelne);
		if (tmp_el != zestaw_spriteow.end()) {
			int wymiar_x = tmp_el->second.getTextureRect().width;
			int wymiar_y = tmp_el->second.getTextureRect().height;
			int pos_y = tmp_el->second.getTextureRect().top;
			mapuj_sprite(wymiar_x, wymiar_y, 0, pos_y, tmp_el->second);
		}
	}

	void przewin_strone(int kierunek) {
		if (zestaw_spriteow.find(id_pola_tekstowego) != zestaw_spriteow.end()) {
			if (kierunek > 0) {
				if(zestaw_spriteow.find(id_pola_tekstowego)->second.getTextureRect().top < strona_y*3)
				mapuj_sprite(strona_x, strona_y, 0, zestaw_spriteow.find(id_pola_tekstowego)->second.getTextureRect().top + strona_y, zestaw_spriteow.find(id_pola_tekstowego)->second);
			}
			else {
				if(zestaw_spriteow.find(id_pola_tekstowego)->second.getTextureRect().top >= strona_y)
				mapuj_sprite(strona_x, strona_y, 0, zestaw_spriteow.find(id_pola_tekstowego)->second.getTextureRect().top - strona_y, zestaw_spriteow.find(id_pola_tekstowego)->second);
			}
		}
	}

};

class UkladInfo : public Uklad {
private:
	// tlo, next, prev, back, tekst
	void buduj_uklad(RysowaneObiekty *&baza_obiektow) {
		// bg
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y, 0, 0, el_ukladu);
		ustaw_sprite(0, 0, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_tla, el_ukladu));
		// b_next
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 400, el_ukladu);
		ustaw_sprite(0 + odstep_el, WYMIAR_EKRANU_Y - przycisk_y - odstep_el, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "prev", el_ukladu));
		// b_prev
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 320, el_ukladu);
		ustaw_sprite(0 + odstep_el + przycisk_x + odstep_el, WYMIAR_EKRANU_Y - przycisk_y - odstep_el, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "next", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 480, el_ukladu);
		ustaw_sprite(0 + odstep_el + przycisk_x + odstep_el + przycisk_x + odstep_el, WYMIAR_EKRANU_Y - przycisk_y - odstep_el, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "back", el_ukladu));
		// p_text
		el_ukladu = baza_obiektow->zwroc_sprite_pole_tekstowe_1();
		mapuj_sprite(strona_x, strona_y, 0, 0, el_ukladu);
		el_ukladu.setScale(sf::Vector2f(2, 2));
		ustaw_sprite(100, 20, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_pola_tekstowego, el_ukladu));
	}
public:
	UkladInfo(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}
};

class UkladMenu :public Uklad {
private:
	// tlo, start, continue, info, quit
	void buduj_uklad(RysowaneObiekty *&baza_obiektow) {
		// wczytywanie tla
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y, 0, 0, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 0, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 170, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_continue
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 80, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 270, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "cont", el_ukladu));
		// b_info
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 160, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 370, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "info", el_ukladu));
		// b_quit
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 240, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 470, el_ukladu);
		zestaw_spriteow.insert(make_pair(id_przycisku + "quit", el_ukladu));
	}

public:
	UkladMenu(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}
};

class ManagerUkladow {
private:

	int aktualny_uklad = 0;
	static const int ilosc_ukladow = 8;
	static const int ilosc_klawiszy = 13;
	Uklad *uklad_tmp;
	Uklad *zestaw_ukladow[ilosc_ukladow];
	
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

	//para <uklad docelowy/akcja (akcje oznaczone warosciami ujemnymi), nazwa>
	pair<int,string> mapa_ukladow[ilosc_klawiszy][ilosc_ukladow] =
	{
		{ { 1,"strt" },{ 2,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ -2,"prev" } },
		{ { 1,"cont" },{ 5,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ -1,"next" } },
		{ { 7,"info" },{ 0,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"quit" },{ 0,"null" },{ 4,"null" },{ 5,"null" },{ 0,"null" },{ 0,"null" },{ 0,"null" },{ 7,"null" } },
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

	void wczytaj_uklady(RysowaneObiekty *&baza_ob) {
		uklad_tmp = new UkladMenu(baza_ob);
		zestaw_ukladow[0] = uklad_tmp;
		uklad_tmp = new UkladInfo(baza_ob);
		zestaw_ukladow[7] = uklad_tmp;
	}

public:
	ManagerUkladow(RysowaneObiekty *&baza_obiektow) {
		wczytaj_uklady(baza_obiektow);
	}

	map<string, sf::Sprite> zwroc_uklad() {
		return zestaw_ukladow[aktualny_uklad]->zwroc_uklad();
	}
	void animuj_przycisk_ukladu(int kod_klawisza) {
		
		// klikniecie przycisku
		string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;
		zestaw_ukladow[aktualny_uklad]->nacisnij_przycisk(nazwa_przycisku);
	}

	void zmien_modyfikuj_uklad(int kod_klawisza) {
		// "odklikniecie" przycisku
		int nr_przycisku_akcji = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
		string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;

		// ---- zapewne trzeba bedzie przywrocic wszystkie przyciski w ukladzie
		zestaw_ukladow[aktualny_uklad]->przywroc_przycisk(nazwa_przycisku);
		
		if (nr_przycisku_akcji >= 0) {
			// zmiana ukladu
			aktualny_uklad = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
		}
		else {
			// wykonaj modyfikacje w ukladzie - np. przewin strone
			switch (nr_przycisku_akcji)
			{
			case -1:
			{
				zestaw_ukladow[aktualny_uklad]->przewin_strone(1);
				break;
			}
			case -2:
			{
				zestaw_ukladow[aktualny_uklad]->przewin_strone(-1);
				break;
			}
			default:
				break;
			}
		}
	}

};