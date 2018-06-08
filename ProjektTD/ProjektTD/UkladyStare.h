#pragma once
#include "Baza.h"
#include "RysowaneObiekty.h"

using namespace std;

class Uklad {
protected:
	static const int rozmiar_pola_liczby = DLUGOSC_POLA_LICZBOWEGO;

	sf::Sprite el_ukladu;

	const int odstep_el = 20;
	const int przycisk_x = 250;
	const int przycisk_y = 50;

	const int m_przycisk_x = 50;
	const int m_przycisk_y = 50;

	const int strona_x = 540;
	const int strona_y = 285;
	const int sklep_x = 400;
	const int cyfra_y = 50;
	const int cyfra_x = 40;

	string id_tla = "1bg";
	string id_przycisku = "2b";
	string id_pola_tekstowego = "3p_text";
	string id_pola_liczby = "4num";

	// liczba na poczatku oznacza kolejnosc renderowania el
	map<string, sf::Sprite> elementy_w_ukladzie;
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
	void dodaj_pole_cyfry(RysowaneObiekty *&baza_obiektow, int index_cyfry) {
		el_ukladu = baza_obiektow->zwroc_sprite_cyfry();
		ustaw_sprite((sklep_x / 2) - (rozmiar_pola_liczby / 2 * cyfra_x) + (index_cyfry*cyfra_x),
			WYMIAR_EKRANU_Y - odstep_el - cyfra_y, el_ukladu);
		mapuj_sprite(cyfra_x, cyfra_y, 0, 1500, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_pola_liczby + to_string(index_cyfry), el_ukladu));
	}

public:
	map<string, sf::Sprite> zwroc_uklad() {
		return elementy_w_ukladzie;
	}
	void nacisnij_przywroc_przycisk(string nazwa_przycisku, int flaga_akcji) {
		string id_pelne = id_przycisku + nazwa_przycisku;
		auto tmp_el = elementy_w_ukladzie.find(id_pelne);
		if (tmp_el != elementy_w_ukladzie.end()) {
			int pos_y = tmp_el->second.getTextureRect().top;

			int wym_x = tmp_el->second.getTextureRect().width; // szerokosc aktualnie naciskanego przycisku
			int wym_y = tmp_el->second.getTextureRect().height; // wysokosc aktualnie naciskanego przycisku

			if (flaga_akcji == 0) {
				mapuj_sprite(wym_x, wym_y, wym_x, pos_y, tmp_el->second);
			}
			else {
				mapuj_sprite(wym_x, wym_y, 0, pos_y, tmp_el->second);
			}
		}
	}
	void przewin_strone(int kierunek) {
		auto tmp_el = elementy_w_ukladzie.find(id_pola_tekstowego);
		if (tmp_el != elementy_w_ukladzie.end()) {
			int pos_y = tmp_el->second.getTextureRect().top;
			if (kierunek > 0 && elementy_w_ukladzie.find(id_pola_tekstowego)->second.getTextureRect().top < strona_y * 3) {
				mapuj_sprite(strona_x, strona_y, 0, pos_y + strona_y, tmp_el->second);
			}
			else if (kierunek <= 0 && elementy_w_ukladzie.find(id_pola_tekstowego)->second.getTextureRect().top >= strona_y) {
				mapuj_sprite(strona_x, strona_y, 0, pos_y - strona_y, tmp_el->second);
			}
		}
	}
	virtual void ustaw_zestaw_przyciskow(int ilosc_przyciskow, vector<int> typy_przyciskow) {};
	virtual void ustaw_wartosc_pola_liczbowego(string liczba) {
		if (liczba.size() != rozmiar_pola_liczby) {
			return;
		}
		else {
			for (int i = 0; i < rozmiar_pola_liczby; i++) {
				//pole_liczbowe[i] = rys_ob->zwroc_sprite_cyfry();
				auto tmp_el = elementy_w_ukladzie.find(id_pola_liczby + to_string(i));
				if (tmp_el != elementy_w_ukladzie.end()) {
					mapuj_sprite(cyfra_x, cyfra_y, cyfra_x*(liczba[i] - '0'), 1500, tmp_el->second);
				}
			}
		}
	}
};

class UkladProsty : public Uklad {
protected:
public:

};

class UkladTypyWieze : public Uklad {
private:
	RysowaneObiekty *baza_obiektow;
	void buduj_uklad(RysowaneObiekty *&baza_ob) {
		baza_obiektow = baza_ob;
		// wczytywanie tla
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(sklep_x, WYMIAR_EKRANU_Y, 1280, 0, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 600, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x - odstep_el, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 300, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x * 2 - odstep_el * 2, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "back", el_ukladu));
		// pole_liczbowe
		// num0
		dodaj_pole_cyfry(baza_obiektow, 0);
		// num1
		dodaj_pole_cyfry(baza_obiektow, 1);
		// num2
		dodaj_pole_cyfry(baza_obiektow, 2);
		// num3
		dodaj_pole_cyfry(baza_obiektow, 3);
	}
public:
	UkladTypyWieze(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}
	// b_slot
	void ustaw_zestaw_przyciskow(int ilosc, vector<int>) {
		for (int i = 0; i < ilosc; i++) {
			el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
			mapuj_sprite(przycisk_x, przycisk_y, 0, 450 + i*przycisk_y, el_ukladu);
			ustaw_sprite((sklep_x - przycisk_x) / 2, odstep_el + i*(przycisk_y + odstep_el), el_ukladu);
			elementy_w_ukladzie.insert(make_pair(id_przycisku + "typ" + to_string(i), el_ukladu));
		}
	}
};

class UkladSlotyWieze : public Uklad {
private:
	RysowaneObiekty *baza_obiektow;
	void buduj_uklad(RysowaneObiekty *&baza_ob) {
		baza_obiektow = baza_ob;
		// wczytywanie tla
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(sklep_x, WYMIAR_EKRANU_Y, 1280, 0, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 600, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x - odstep_el, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 300, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x * 2 - odstep_el * 2, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "back", el_ukladu));
		// pole_liczbowe
		// num0
		dodaj_pole_cyfry(baza_obiektow, 0);
		// num1
		dodaj_pole_cyfry(baza_obiektow, 1);
		// num2
		dodaj_pole_cyfry(baza_obiektow, 2);
		// num3
		dodaj_pole_cyfry(baza_obiektow, 3);
	}

public:
	UkladSlotyWieze(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}

	// b_slot
	void ustaw_zestaw_przyciskow(int ilosc, vector<int>) {
		for (int i = 0; i < ilosc; i++) {
			el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
			mapuj_sprite(przycisk_x, przycisk_y, 0, 800 + i*przycisk_y, el_ukladu);
			ustaw_sprite((sklep_x - przycisk_x) / 2, odstep_el + i*(przycisk_y + odstep_el), el_ukladu);
			elementy_w_ukladzie.insert(make_pair(id_przycisku + "slt" + to_string(i), el_ukladu));
		}
	}
};

class UkladTypyPulapki :public Uklad {
private:
	RysowaneObiekty *baza_obiektow;
	void buduj_uklad(RysowaneObiekty *&baza_ob) {
		baza_obiektow = baza_ob;
		// wczytywanie tla
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(sklep_x, WYMIAR_EKRANU_Y, 1280, 0, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 600, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x - odstep_el, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 300, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x * 2 - odstep_el * 2, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "back", el_ukladu));
		// pole_liczbowe
		// num0
		dodaj_pole_cyfry(baza_obiektow, 0);
		// num1
		dodaj_pole_cyfry(baza_obiektow, 1);
		// num2
		dodaj_pole_cyfry(baza_obiektow, 2);
		// num3
		dodaj_pole_cyfry(baza_obiektow, 3);
	}
public:
	UkladTypyPulapki(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}
	// b_slot
	void ustaw_zestaw_przyciskow(int ilosc, vector<int>) {
		for (int i = 0; i < ilosc; i++) {
			el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
			mapuj_sprite(przycisk_x, przycisk_y, 0, 450 + i*przycisk_y, el_ukladu);
			ustaw_sprite((sklep_x - przycisk_x) / 2, odstep_el + i*(przycisk_y + odstep_el), el_ukladu);
			elementy_w_ukladzie.insert(make_pair(id_przycisku + "typ" + to_string(i), el_ukladu));
		}
	}
};

class UkladSlotyPulapki : public Uklad {
private:
	RysowaneObiekty *baza_obiektow;
	void buduj_uklad(RysowaneObiekty *& baza_ob) {
		baza_obiektow = baza_ob;
		// wczytywanie tla
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(sklep_x, WYMIAR_EKRANU_Y, 1280, 0, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 600, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x - odstep_el, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 300, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x * 2 - odstep_el * 2, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "back", el_ukladu));
		// pole_liczbowe
		// num0
		dodaj_pole_cyfry(baza_obiektow, 0);
		// num1
		dodaj_pole_cyfry(baza_obiektow, 1);
		// num2
		dodaj_pole_cyfry(baza_obiektow, 2);
		// num3
		dodaj_pole_cyfry(baza_obiektow, 3);
	}

public:
	UkladSlotyPulapki(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}
	void ustaw_zestaw_przyciskow(int ilosc, vector<int>) {
		for (int i = 0; i < ilosc; i++) {
			el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
			mapuj_sprite(przycisk_x, przycisk_y, 0, 800 + i*przycisk_y, el_ukladu);
			ustaw_sprite((sklep_x - przycisk_x) / 2, odstep_el + i*(przycisk_y + odstep_el), el_ukladu);
			elementy_w_ukladzie.insert(make_pair(id_przycisku + "slt" + to_string(i), el_ukladu));
		}
	}
};

class UkladSklep : public Uklad {
private:
	void buduj_uklad(RysowaneObiekty *&baza_obiektow) {
		// wczytywanie tla
		el_ukladu = baza_obiektow->zwroc_sprite_tlo();
		mapuj_sprite(sklep_x, WYMIAR_EKRANU_Y, 1280, 0, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 600, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x - odstep_el, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 300, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x * 2 - odstep_el * 2, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "back", el_ukladu));
		// b_turr
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 350, el_ukladu);
		ustaw_sprite((sklep_x - przycisk_x) / 2, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "turr", el_ukladu));
		// b_trap
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 400, el_ukladu);
		ustaw_sprite((sklep_x - przycisk_x) / 2, odstep_el * 2 + przycisk_y, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "trap", el_ukladu));
		// pole_liczbowe
		// num0
		dodaj_pole_cyfry(baza_obiektow, 0);
		// num1
		dodaj_pole_cyfry(baza_obiektow, 1);
		// num2
		dodaj_pole_cyfry(baza_obiektow, 2);
		// num3
		dodaj_pole_cyfry(baza_obiektow, 3);
	}
public:
	UkladSklep(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
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
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_next
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 250, el_ukladu);
		ustaw_sprite(0 + odstep_el, WYMIAR_EKRANU_Y - przycisk_y - odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "prev", el_ukladu));
		// b_prev
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 200, el_ukladu);
		ustaw_sprite(0 + odstep_el + przycisk_x + odstep_el, WYMIAR_EKRANU_Y - przycisk_y - odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "next", el_ukladu));
		// b_back
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 300, el_ukladu);
		ustaw_sprite(0 + odstep_el + przycisk_x + odstep_el + przycisk_x + odstep_el, WYMIAR_EKRANU_Y - przycisk_y - odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "back", el_ukladu));
		// p_text
		el_ukladu = baza_obiektow->zwroc_sprite_pole_tekstowe_1();
		mapuj_sprite(strona_x, strona_y, 0, 0, el_ukladu);
		el_ukladu.setScale(sf::Vector2f(2, 2));
		ustaw_sprite(100, 20, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_pola_tekstowego, el_ukladu));
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
		elementy_w_ukladzie.insert(make_pair(id_tla, el_ukladu));
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 0, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 170, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "strt", el_ukladu));
		// b_continue
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 50, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 270, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "cont", el_ukladu));
		// b_info
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 100, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 370, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "info", el_ukladu));
		// b_quit
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 150, el_ukladu);
		ustaw_sprite((WYMIAR_EKRANU_X - przycisk_x) / 2, 470, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "quit", el_ukladu));
	}

public:
	UkladMenu(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
	}
};

class UkladGra : public Uklad {
private:
	RysowaneObiekty *rysowane_obiekty;
	void buduj_uklad(RysowaneObiekty *&baza_obiektow) {
		// b_start
		el_ukladu = baza_obiektow->zwroc_sprite_przycisk();
		mapuj_sprite(przycisk_x, przycisk_y, 0, 650, el_ukladu);
		ustaw_sprite(WYMIAR_EKRANU_X - przycisk_x - odstep_el, odstep_el, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(id_przycisku + "endg", el_ukladu));
	}

public:
	UkladGra(RysowaneObiekty *&baza_obiektow) {
		buduj_uklad(baza_obiektow);
		rysowane_obiekty = baza_obiektow;
	}
	void ustaw_zestaw_przyciskow(int ilosc, vector<int> typy_przyciskow) {
		// ustawia male przyciski akcji
		for (int i = 0; i < ilosc; i++) {
			el_ukladu = rysowane_obiekty->zwroc_sprite_p_akcji();
			mapuj_sprite(m_przycisk_x, m_przycisk_y, 0, 0 + typy_przyciskow[i] * m_przycisk_x, el_ukladu);
			ustaw_sprite(870 + (m_przycisk_x + odstep_el)*i, 650, el_ukladu);
			elementy_w_ukladzie.insert(make_pair(id_przycisku + "act" + to_string(i), el_ukladu));
		}
	}
};

class ManagerUkladow {
private:
	RysowaneObiekty *rysowane_obiekty;
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
		uklad_tmp = new UkladSklep(baza_ob);
		zestaw_ukladow[1] = uklad_tmp;
		uklad_tmp = new UkladSlotyWieze(baza_ob);
		zestaw_ukladow[2] = uklad_tmp;
		uklad_tmp = new UkladSlotyPulapki(baza_ob);
		zestaw_ukladow[3] = uklad_tmp;
		uklad_tmp = new UkladTypyWieze(baza_ob);
		zestaw_ukladow[4] = uklad_tmp;
		uklad_tmp = new UkladTypyPulapki(baza_ob);
		zestaw_ukladow[5] = uklad_tmp;
		uklad_tmp = new UkladGra(baza_ob);
		zestaw_ukladow[6] = uklad_tmp;
		uklad_tmp = new UkladInfo(baza_ob);
		zestaw_ukladow[7] = uklad_tmp;
	}

public:
	~ManagerUkladow() {
		for (int i = 0; i < ILOSC_UKLADOW; i++) {
			if (zestaw_ukladow[i]) {
				delete zestaw_ukladow[i];
			}
		}
	}

	ManagerUkladow(RysowaneObiekty *&baza_obiektow) {
		rysowane_obiekty = baza_obiektow;
		wczytaj_uklady(baza_obiektow);
	}
	map<string, sf::Sprite> zwroc_uklad(int index_ukladu) {
		return zestaw_ukladow[index_ukladu]->zwroc_uklad();
	}
	void nacisnij_przycisk_w_ukladzie(string nazwa_przycisku, int index_ukladu) {
		zestaw_ukladow[index_ukladu]->nacisnij_przywroc_przycisk(nazwa_przycisku, 0);
	}
	void przywroc_przyciski_w_ukladzie(string nazwa_przycisku, int index_ukladu) {
		// ---- zapewne trzeba bedzie przywrocic wszystkie przyciski w ukladzie
		zestaw_ukladow[index_ukladu]->nacisnij_przywroc_przycisk(nazwa_przycisku, 1);
	}
	void przewin_strone_w_ukladzie(int index_ukladu, int kierunek) {
		zestaw_ukladow[index_ukladu]->przewin_strone(kierunek);
	}
	void wstaw_zestaw_przyciskow_do_ukladu(int index_ukladu, int ilosc, vector<int> typy_przyciskow = { 0 }) {
		zestaw_ukladow[index_ukladu]->ustaw_zestaw_przyciskow(ilosc, typy_przyciskow);
	}

	void ustaw_wartosc_pola_liczbowego(int index_ukladu, string wartosc) {
		zestaw_ukladow[index_ukladu]->ustaw_wartosc_pola_liczbowego(wartosc);
	}
};