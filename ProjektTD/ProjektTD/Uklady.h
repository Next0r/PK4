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
	void nacisnij_przywroc_przycisk(string nazwa_przycisku, int flaga_akcji) {
		string id_pelne = id_przycisku + nazwa_przycisku;
		auto tmp_el = zestaw_spriteow.find(id_pelne);
		if (tmp_el != zestaw_spriteow.end()) {
			int pos_y = tmp_el->second.getTextureRect().top;
			if (flaga_akcji == 0) {
				mapuj_sprite(przycisk_x, przycisk_y, przycisk_x, pos_y, tmp_el->second);
			}
			else {
				mapuj_sprite(przycisk_x, przycisk_y, 0, pos_y, tmp_el->second);
			}			
		}
	}
	void przewin_strone(int kierunek) {
		auto tmp_el = zestaw_spriteow.find(id_pola_tekstowego);
		if (tmp_el != zestaw_spriteow.end()) {
			int pos_y = tmp_el->second.getTextureRect().top;
			if (kierunek > 0 && zestaw_spriteow.find(id_pola_tekstowego)->second.getTextureRect().top < strona_y * 3) {
				mapuj_sprite(strona_x, strona_y, 0, pos_y + strona_y, tmp_el->second);
			}
			else if (kierunek <= 0 && zestaw_spriteow.find(id_pola_tekstowego)->second.getTextureRect().top >= strona_y) {
				mapuj_sprite(strona_x, strona_y, 0, pos_y - strona_y, tmp_el->second);
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

	Uklad *uklad_tmp;
	Uklad *zestaw_ukladow[ILOSC_UKLADOW];

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

	// wczytywanie wszystkich mozliwych ukladow interfaceu
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
	map<string, sf::Sprite> zwroc_uklad(int index_ukladu) {
		return zestaw_ukladow[index_ukladu]->zwroc_uklad();
	}
	void nacisnij_przycisk_w_ukladzie(string nazwa_przycisku, int index_ukladu) {		
		zestaw_ukladow[index_ukladu]->nacisnij_przywroc_przycisk(nazwa_przycisku, 0);
	}
	void przywroc_przycisk_w_ukladzie(string nazwa_przycisku, int index_ukladu) {
		// ---- zapewne trzeba bedzie przywrocic wszystkie przyciski w ukladzie
		zestaw_ukladow[index_ukladu]->nacisnij_przywroc_przycisk(nazwa_przycisku, 1);
	}
	void przewin_strone_w_ukladzie(int index_ukladu, int kierunek) {
		zestaw_ukladow[index_ukladu]->przewin_strone(kierunek);
	}
};