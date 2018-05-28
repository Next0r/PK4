#pragma once

#include "Baza.h"
#include "WczytywanieMapy.h"
#include "RysowaneObiekty.h"

using namespace std;


class ObiektNaMapie {
protected:
	RysowaneObiekty *rys_ob;
	int kodowanie_typu;
	int pozycja_x;
	int pozycja_y;
	sf::Sprite sprite;

	void mapuj(int left, int top, int wym_x, int wym_y) {
		sprite.setTextureRect(sf::IntRect(left, top, wym_x, wym_y));
	}
	void ustaw_na_mapie() {
		sprite.setPosition(sf::Vector2f(pozycja_x, pozycja_y));
	}
	virtual void ustaw_sprite() = 0;

public:
	sf::Sprite zwroc_sprite() {
		return sprite;
	}
	int zwroc_typ() {
		return kodowanie_typu;
	}
	pair<int, int> zwroc_pozycje() { // zwraca pozycje wyrazona w px
		return make_pair(pozycja_x, pozycja_y);
	}
};

//************************************************** EFEKTY SPECJALNE

class Efekt : public ObiektNaMapie {
private:
	bool ukonczony = false; // infrmuje czy efekt skonczyl sie i powinien byc zniszczony
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_efekty();
	}

public:
	void ustaw_rotacje(float kat) {
		sprite.setRotation(kat);
	}

	void ustaw_srodek(int x, int y) {
		sprite.setOrigin(sf::Vector2f(x, y));
	}

	void ustaw_pozycje(int x, int y) {
		sprite.setPosition(sf::Vector2f(x, y));
		pozycja_x = x;
		pozycja_y = y;
	}

	bool zwroc_ukonczony() {
		return ukonczony;
	}

	Efekt(int pos_x, int pos_y, int typ, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = typ;
		ustaw_sprite();
		mapuj(0, WYMIAR_POLA_GRY*kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
		ustaw_na_mapie();
	}

	void animuj_sprite() {
		int left = sprite.getTextureRect().left;
		if (left != 880) { // animacja nie jest na ostatnim obrazku
			mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, WYMIAR_POLA_GRY * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
		}
		else { // animacja jest na ostatnim obrazku
			ukonczony = true;
		}
	}
};

//************************************************** EFEKTY SPECJALNE

class Wrogowie : public ObiektNaMapie {
protected:
	double zdrowie;
	double zdrowie_max;
	double pancerz;
	vector<sf::Sprite> ramki; //ramki okreslajace dzialajace efekty i poziom zdrowia
	pair<int, int> wektor_przemieszczenia = make_pair(0, 0); // startowy wektor przemieszczenia = 0,0
	int pozycja_na_sciezce = 0; // domyslnie wrogowie pojawiaja sie na pierwszym polu sciezki
	int rotacja = 0;

	void koloruj_ramke_zdrowia() {
		// obliczanie wartosci kolorow
		double procent_zdrowia = zdrowie / zdrowie_max;
		double zielony = 255.0 * procent_zdrowia;
		double czerwony = 255.0 - zielony;
		// normalizacja kolorow
		if (zielony < 255.0) {
			if (zielony > czerwony) {
				double param = 255.0 - zielony;
				zielony = 255.0;
				czerwony += param;
			}
			else {
				double param = 255.0 - czerwony;
				czerwony = 255.0;
				zielony += param;
			}
		}
		ramki[0].setColor(sf::Color(czerwony * JASNOSC_RAMEK, zielony * JASNOSC_RAMEK, (POZIOM_B_W_RAMCE*procent_zdrowia*0.5 + 0.5)*JASNOSC_RAMEK, 255.0*WIDZIALNOSC_RAMEK));
	}

public:
	vector<sf::Sprite> zwroc_ramki() {
		return ramki;
	}

	int zwroc_zdrowie() {
		return zdrowie;
	}

	void odejmij_zdrowie(int wartosc) {
		zdrowie = zdrowie - (wartosc - pancerz);
	}

	void dodaj_ramke(int numer_ramki) {
		sf::Sprite ramka_tmp;
		ramka_tmp = rys_ob->zwroc_sprite_ramki();
		ramka_tmp.setTextureRect(sf::IntRect(0, WYMIAR_POLA_GRY*numer_ramki, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
		ramki.push_back(ramka_tmp);
	}

	void synchronizuj_ramki() {
		// kolorowanie ramki zalezne od zdrowia
		koloruj_ramke_zdrowia();
		for (auto i = ramki.begin(); i != ramki.end(); i++) {
			// aktualizacja pozycji ramki
			i->setPosition(sf::Vector2f(pozycja_x, pozycja_y));
		}
	}

	void przemiesc_wroga() {
		int przesuniecie_x = wektor_przemieszczenia.first * SZYBKOSC_PRZEMIESZCZENIA_WROGOW;
		int przesuniecie_y = wektor_przemieszczenia.second * SZYBKOSC_PRZEMIESZCZENIA_WROGOW;
		sprite.setPosition(sf::Vector2f(pozycja_x + przesuniecie_x, pozycja_y + przesuniecie_y));
		pozycja_x += przesuniecie_x;
		pozycja_y += przesuniecie_y;
	}

	void przelicz_rotacje() {
		if (wektor_przemieszczenia.first == 1) {
			rotacja = 0;
		}
		else if (wektor_przemieszczenia.second == -1) {
			rotacja = -90;
		}
		else if (wektor_przemieszczenia.second == 1) {
			rotacja = 90;
		}
	}

	int zwroc_pozycje_na_sciezce() {
		return pozycja_na_sciezce;
	}

	void aktualizuj_wektor_przemieszczenia(int x, int y) {
		wektor_przemieszczenia = make_pair(x, y);
	}

	void ustaw_pozycje_na_sciezce(int pos) {
		pozycja_na_sciezce = pos;
	}

	void animuj_sprite() {
		if (rotacja == 0) { // animacja nr 1 - przemieszczneie w prawo
			int left = sprite.getTextureRect().left;
			if (left != 720) { // animacja nie jest na ostatnim obrazku
				mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, WYMIAR_POLA_GRY * 3 * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
			else { // animacja jest na ostatnim obrazku
				mapuj(0, WYMIAR_POLA_GRY * 3 * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
		}
		else if (rotacja == 90) {  // animacja nr 2 - przemieszczenie w dol
			int left = sprite.getTextureRect().left;
			if (left != 720) { // animacja nie jest na ostatnim obrazku
				mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, (WYMIAR_POLA_GRY * 3 * kodowanie_typu) + (WYMIAR_POLA_GRY), WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
			else { // animacja jest na ostatnim obrazku
				mapuj(0, (WYMIAR_POLA_GRY * 3 * kodowanie_typu) + (WYMIAR_POLA_GRY), WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
		}
		else if (rotacja == -90) { // animacja nr 3 - przemieszczenie w gore
			int left = sprite.getTextureRect().left;
			if (left != 720) { // animacja nie jest na ostatnim obrazku
				mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, (WYMIAR_POLA_GRY * 3 * kodowanie_typu) + (WYMIAR_POLA_GRY * 2), WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
			else { // animacja jest na ostatnim obrazku
				mapuj(0, (WYMIAR_POLA_GRY * 3 * kodowanie_typu) + (WYMIAR_POLA_GRY * 2), WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
		}
	}
};

class WrogTypu0 : public Wrogowie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wrogowie();
	}
public:
	WrogTypu0(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		pancerz = 10.0;
		zdrowie_max = 150.0;
		zdrowie = zdrowie_max;		
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 0;
		ustaw_sprite();
		ustaw_na_mapie();
		dodaj_ramke(0); // ramka 0 jest ramka zrowia
		mapuj(0, WYMIAR_POLA_GRY * 3 * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
		synchronizuj_ramki();
	}
};

class WrogTypu1 : public Wrogowie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wrogowie();
	}
public:
	WrogTypu1(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		pancerz = 5.0;
		zdrowie_max = 180.0;
		zdrowie = zdrowie_max;
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 1;
		ustaw_sprite();
		ustaw_na_mapie();
		dodaj_ramke(0); // ramka 0 jest ramka zrowia
		mapuj(0, WYMIAR_POLA_GRY * 3 * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
		synchronizuj_ramki();
	}
};

class WrogTypu2 : public Wrogowie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wrogowie();
	}
public:
	WrogTypu2(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		pancerz = 15.0;
		zdrowie_max = 200.0;
		zdrowie = zdrowie_max;
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 2;
		ustaw_sprite();
		ustaw_na_mapie();
		dodaj_ramke(0); // ramka 0 jest ramka zrowia
		mapuj(0, WYMIAR_POLA_GRY * 3 * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
		synchronizuj_ramki();
	}
};

class PulapkaTypu0 :public ObiektNaMapie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_pulapki();
	}
public:
	PulapkaTypu0(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 0;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(0, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class PulapkaTypu1 :public ObiektNaMapie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_pulapki();
	}
public:
	PulapkaTypu1(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 1;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(80, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class PulapkaTypu2 :public ObiektNaMapie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_pulapki();
	}
public:
	PulapkaTypu2(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 2;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(160, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class Wieza : public ObiektNaMapie {
protected:
	int czas_przeladowania; // jak dlugo trwa przeladowanie
	int przeladowanie = 0; // aktualny stopien przeladowania
	int rodzaj_ataku;	// 0 - atak pojedynczego celu
						// 1 - obszarowy atak pojedynczego celu
						// 2 - atak obszarowy
	int zasieg;
	float rotacja;
public:
	float zwroc_rotacje() {
		return rotacja;
	}
	void resetuj_ladowanie() {
		przeladowanie = 0;
	}
	bool wieza_gotowa_do_strzalu() {
		if (czas_przeladowania <= przeladowanie) {
			return true;
		}
		else {
			return false;
		}
	}
	void ustaw_czas_przeladowania(int wartosc) {
		czas_przeladowania = wartosc;
	}
	void laduj_wieze() {
		if (przeladowanie < czas_przeladowania) {
			przeladowanie++;
		}
	}
	int zwroc_zasieg() {
		return zasieg;
	}
	void mapuj_obrot_wiezy(int obrot) {
		rotacja = obrot * 45.0 + 45.0;
		sprite.setTextureRect(sf::IntRect(WYMIAR_POLA_GRY*obrot, sprite.getTextureRect().top, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
	}
};

class WiezaTypu0 :public Wieza {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wiezy();
	}
public:
	WiezaTypu0(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		ustaw_czas_przeladowania(20); 
		zasieg = 120;
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 0;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(WYMIAR_POLA_GRY*4, WYMIAR_POLA_GRY * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class WiezaTypu1 :public Wieza {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wiezy();
	}
public:
	WiezaTypu1(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		ustaw_czas_przeladowania(40);
		zasieg = 120;
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 1;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(WYMIAR_POLA_GRY * 4, WYMIAR_POLA_GRY * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class WiezaTypu2 :public Wieza {
private:	
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wiezy();
	}
public:
	WiezaTypu2(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		ustaw_czas_przeladowania(60);
		zasieg = 120;
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 2;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(WYMIAR_POLA_GRY * 4, WYMIAR_POLA_GRY * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class Strzal {
private:
	RysowaneObiekty *rys_ob;
	vector<Efekt*> efekty_specjalne;

	void dodaj_efekt_specjalny(int srodek_x, int srodek_y, int pos_x, int pos_y, int typ, float rotacja) {
		efekty_specjalne.push_back(new Efekt(pos_x, pos_x, typ, rys_ob));
		efekty_specjalne.back()->ustaw_srodek(srodek_x, srodek_y);
		efekty_specjalne.back()->ustaw_pozycje(pos_x, pos_y);
		efekty_specjalne.back()->ustaw_rotacje(rotacja);
	}

	void pojedynczy_strzal(Wieza *&wieza, Wrogowie *&wrog, vector<Wrogowie*> &wrogowie) {
		wieza->resetuj_ladowanie();
		wrog->odejmij_zdrowie(OBRAZENIA_STRZAL);

		if (wrog->zwroc_zdrowie() <= 0) {
			for (int i = 0; i < wrogowie.size(); i++) {
				if (wrogowie[i] == wrog) {
					wrogowie[i] = nullptr;
					break;
				}
			}
			auto *wsk = wrog;
			wrog = nullptr;
			delete wsk;		
		}
	}

public:
	Strzal(RysowaneObiekty *&r_ob) {
		rys_ob = r_ob;
	}

	vector<Efekt*> zwroc_efekty_specjalne() {
		return efekty_specjalne;
	}

	void wykonaj_atak_zniszcz_wroga(Wieza *&wieza, Wrogowie *&wrog, vector<Wrogowie*> &wrogowie) {
		if (wieza->zwroc_typ() == 0) {
			// ---------- efekt strzalu
			dodaj_efekt_specjalny(40, -27, wieza->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2,
				wieza->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2, 1, 270.0 - wieza->zwroc_rotacje());
			// ---------- efekt wybuchu na wrogu
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 0, 0.0);
			// jesli jest to wieza typu zerowego
			pojedynczy_strzal(wieza, wrog, wrogowie);
		}
		else if (wieza->zwroc_typ() == 1) {
			// jesli jest to wieza typu pierwszego
		}
		else if (wieza->zwroc_typ() == 2) {
			// jesli jest to wieza typu drugiego
		}
	}

	void usun_efekt_specjalny(int index_w_tablicy) {
		auto *wsk_tmp = efekty_specjalne[index_w_tablicy]; // zapamietaj usuwany	
		auto koniec_vektora = efekty_specjalne.back(); // zapamietaj ostatni el
		efekty_specjalne[efekty_specjalne.size() - 1] = nullptr; // wyzeryj ostatni el
		efekty_specjalne[index_w_tablicy] = koniec_vektora; // przypisz ostatni el w miejsce usuwanego
		delete wsk_tmp;
		efekty_specjalne.pop_back();
	}
};

class ManagerObiektowAktywnych {
private:
	int limit_generowania_wrogow = -1; // limit odstepu czasowego
	int limit_odstepu_fali = -1; // limit odstepu czasowego
	int aktualna_fala = 0; // bierzaco generowana afala wrogow
	int ilosc_wrogow_w_fali = -1;

	bool ustawiam_wieze; 
	RysowaneObiekty *rys_ob;
	int index_mapy;
	Plik_map *mapy;
	Strzal *strzal;
	vector<Wieza*> ustawione_wieze;
	vector<ObiektNaMapie*> ustawione_pulapki;
	vector<Wrogowie*> wrogowie_na_mapie;
	

	vector <pair<int, int>> sciezka_danej_mapy; // wielokrotnie uzywana - dlatego kopiowana do pamieci

	int nr_slotu; // slot na ktorych chcemy postawic obiekt
	int nr_typu; // typ obiektu ktory chcemy postawic

	int dekoduj_zwroc_nr_slotu_typu(string kod_przycisku) {
		// klawisze maja kodowanie np. slt0
		return kod_przycisku[3] - '0';
	}

	void ustaw_wroga(int typ_wroga) {

		// oblicz pozycje startowa
		int pos_start_x = mapy->zwroc_sciezke(index_mapy)[0].first;
		int pos_start_y = mapy->zwroc_sciezke(index_mapy)[0].second;

		// ustaw danego wroga na starcie
		switch (typ_wroga)
		{
		case 0:
			wrogowie_na_mapie.push_back(new WrogTypu0(pos_start_x*WYMIAR_POLA_GRY, pos_start_y*WYMIAR_POLA_GRY, rys_ob));
			break;
		case 1:
			wrogowie_na_mapie.push_back(new WrogTypu1(pos_start_x*WYMIAR_POLA_GRY, pos_start_y*WYMIAR_POLA_GRY, rys_ob));
			break;
		case 2:
			wrogowie_na_mapie.push_back(new WrogTypu2(pos_start_x*WYMIAR_POLA_GRY, pos_start_y*WYMIAR_POLA_GRY, rys_ob));
			break;
		default:
			wrogowie_na_mapie.push_back(new WrogTypu0(pos_start_x*WYMIAR_POLA_GRY, pos_start_y*WYMIAR_POLA_GRY, rys_ob));
			break;
		}


	}

	double oblicz_dlugosc_wektora(pair<int, int> A, pair<int, int> B) {
		double dlugosc;
		double r1 = (B.first - A.first)*(B.first - A.first);
		double r2 = (B.second - A.second)*(B.second - A.second);
		dlugosc = sqrt(r1 + r2);
		return dlugosc;
	}

	double zwroc_rotacje_wiezy(pair<int, int> wieza, pair<int, int> wrog) {

		double p_prost = oblicz_dlugosc_wektora(wieza, wrog);		
		double r2 = wrog.second - wieza.second;
		double w_sin = r2 / p_prost;
		
		if (wieza.second <= wrog.second) {
			// wrog w dolnej polowie
			if (wieza.first >= wrog.first) {
				// wrog w 3 cwiertce
				return 180 + fabs(asin(w_sin) * 180 / PI);
			}
			else {
				// wrog w 4 cwiartce
				return 360 - fabs(asin(w_sin) * 180 / PI);
			}
		}
		else {
			// wrog w gornej polowie
			if (wieza.first >= wrog.first) {
				// wrog w 2 cwiertce
				return 180 - fabs(asin(w_sin) * 180 / PI);
			}
			else {
				// wrog w 1 cwiartce
				return fabs(asin(w_sin) * 180 / PI);
			}
		}
	}

	Wrogowie *zwroc_wroga_najdalej_na_sciezce(vector<Wrogowie*> wrogowie) {
		Wrogowie *wrog_najdlalej_na_sciezce = nullptr;
		for (auto j = wrogowie.begin(); j != wrogowie.end(); j++) {
			if ((*j) != nullptr) {
				wrog_najdlalej_na_sciezce = (*j);
				break;
			}
		}

		// wyszukiwanie wroga, ktory jest najdalej na sciezce
		for (auto j = wrogowie.begin(); j != wrogowie.end(); j++) {
			if ((*j) != nullptr) {
				if (wrog_najdlalej_na_sciezce->zwroc_pozycje_na_sciezce() < (*j)->zwroc_pozycje_na_sciezce()) {
					wrog_najdlalej_na_sciezce = (*j);
				}
			}
		}
		return wrog_najdlalej_na_sciezce;
	}

	vector<Wrogowie*> zwroc_wrogow_w_zasiegu_wiezy(Wieza* wieza) {
		vector<Wrogowie*> wrogowie = wrogowie_na_mapie; // skopiuj wrogow na mapie
		for (auto j = wrogowie.begin(); j != wrogowie.end(); j++) {
			if ((*j) != nullptr) {
				// dla kazdego wroga 
				// sprawdz czy jest w zasiegu
				// obliczanie dlugosci wektora
				int dlugosc_w = oblicz_dlugosc_wektora(wieza->zwroc_pozycje(), (*j)->zwroc_pozycje());
				if (dlugosc_w > wieza->zwroc_zasieg()) {
					(*j) = nullptr; // jesli wrog jest poza zasiegiem ustaw go jako nullptr
				}

			}
		}
		return wrogowie;
	}

public:


	// ---------- efekty specjalne

	void animuj_usuwaj_efekty() {
		for (int i = 0; i < strzal->zwroc_efekty_specjalne().size(); i++) {
			if (strzal->zwroc_efekty_specjalne()[i] != nullptr) {
				strzal->zwroc_efekty_specjalne()[i]->animuj_sprite();
				if (strzal->zwroc_efekty_specjalne()[i]->zwroc_ukonczony() == true) {
					strzal->usun_efekt_specjalny(i);
				}
			}
		}
	}

	// ---------- obiekty dynamiczne (wrogowie)
	void tworz_fale_wrogow(bool &flaga_generacji_wrogow, int &licznik1, int &licznik2) {
		// ********** obliczanie timera
		// zmienna odstepu czasowego znajduje sie na pozycji 2 w tablicy
		if (ilosc_wrogow_w_fali == -1 || limit_odstepu_fali == -1) {
			// przelicz dane jesli nie zostaly wyznaczone
			if (aktualna_fala < mapy->zwroc_fale_wrogow(index_mapy).size()) {
				// oblicz dane jesli jest to mozliwe
				ilosc_wrogow_w_fali = mapy->zwroc_fale_wrogow(index_mapy)[aktualna_fala][0];
				limit_odstepu_fali = mapy->zwroc_fale_wrogow(index_mapy)[aktualna_fala][2] * CZESTOTLIWOSC_ODSWIEZANIA_OBRAZU;
			}
			else {
				// jesli kolejna fala nie istnieje
				// zablokuj wywolywanie funkcji generowania wrogow
				flaga_generacji_wrogow = false;
			}
		}

		if (licznik1 < limit_odstepu_fali) {
			// jeszcze nie uplynal zdefiniowany dla fali czas odstepu
			licznik1++;
		}
		else {
			// licznik odstepu zablokowany
			// nalezy wygenerowac wrogow co przemieszczenie wroga po polu
			limit_generowania_wrogow = WYMIAR_POLA_GRY / (CZESTOTLIWOSC_ODSWIEZANIA_OBRAZU / CZESTOTLIWOSC_ODSWIEZANIA_RUCH * SZYBKOSC_PRZEMIESZCZENIA_WROGOW) * CZESTOTLIWOSC_ODSWIEZANIA_OBRAZU + ODSTEP_MIEDZY_WROGAMI;
			if (licznik2 < limit_generowania_wrogow) {
				//wrog jeszcze nie przemiescil sie o pole
				licznik2++;
			}
			else {
				// jesli sa jeszcze wrogowie do generowania
				if (ilosc_wrogow_w_fali > 0) {
					ustaw_wroga(mapy->zwroc_fale_wrogow(index_mapy)[aktualna_fala][1]); // ustaw wroga zdefiniowanageo typu
					ilosc_wrogow_w_fali--;
					licznik2 = 0; // zresetuj licznik
				}
				else {
					// wrogowie w danej fali skonczyli sie 
					// nalezy przejsci do nastepnej
					ilosc_wrogow_w_fali = -1; // zresetuj dane fali
					limit_odstepu_fali = -1; // zresetuj dane fali
					licznik1 = 0; // odblokuj licznik odstepu
					aktualna_fala++;
				}
			}
		}
	}

	void ustal_wektory_przemieszczenia_i_rotacje_wrogow() { // ustala wektory przemieszczenia dla wszystkich wrogow na mapie
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				int pozycja_na_sciezce_s = wrogowie_na_mapie[i]->zwroc_pozycje_na_sciezce();
				pair<int, int> pozycja_abs = sciezka_danej_mapy[pozycja_na_sciezce_s];
				pair<int, int> pozycja_docelowa_abs;
				if (sciezka_danej_mapy.back() != sciezka_danej_mapy[pozycja_na_sciezce_s]) { // jesli aktualna pozycja abs nie jest ostatnim polem sciezki
					pozycja_docelowa_abs = sciezka_danej_mapy[pozycja_na_sciezce_s + 1];
				}
				int x = pozycja_docelowa_abs.first - pozycja_abs.first;
				int y = pozycja_docelowa_abs.second - pozycja_abs.second;

				wrogowie_na_mapie[i]->przelicz_rotacje();
				wrogowie_na_mapie[i]->aktualizuj_wektor_przemieszczenia(x, y);
			}
		}
	}

	void odswiez_pozycje_na_sciezce_wrogow() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				int pozycja_na_sciezce = wrogowie_na_mapie[i]->zwroc_pozycje_na_sciezce();
				pair<int, int> pozycja_abs = sciezka_danej_mapy[pozycja_na_sciezce]; // pozycja bazowa 16:9
				pair<int, int> pozycja_obiektu = wrogowie_na_mapie[i]->zwroc_pozycje(); // wyrazona w px
				if (pozycja_obiektu.first%WYMIAR_POLA_GRY == 0 && pozycja_obiektu.second%WYMIAR_POLA_GRY == 0) {
					// jesli aktualna pozycja pozwala obliczyc pozycje abs - 16:9
					int pos_x_abs = pozycja_obiektu.first / WYMIAR_POLA_GRY;
					int pos_y_abs = pozycja_obiektu.second / WYMIAR_POLA_GRY;

					if (pozycja_abs.first != pos_x_abs || pozycja_abs.second != pos_y_abs) {
						// i nie jest to ostatnia zapamietana pozycja abs
						// oznacza to, ze wrog osiagna kolejne pole sciezki
						wrogowie_na_mapie[i]->ustaw_pozycje_na_sciezce(pozycja_na_sciezce += 1);
					}

				}
			}
		}
	}

	void przemiesc_wrogow_synchronizuj_ramki() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				wrogowie_na_mapie[i]->synchronizuj_ramki();
				wrogowie_na_mapie[i]->przemiesc_wroga();
			}
		}
	}

	bool wrogowie_na_koncu() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				if (wrogowie_na_mapie[i]->zwroc_pozycje_na_sciezce() == sciezka_danej_mapy.size() - 1) {
					// jesli pozycja obiektu na sciezce jest pozycja koncowa sciezki
					auto *wsk_tmp = wrogowie_na_mapie[i];
					wrogowie_na_mapie[i] = nullptr;
					delete wsk_tmp;
					return true; // gracz przegral
				}
			}
		}
		return false;
	}

	void animuj_wrogow() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				wrogowie_na_mapie[i]->animuj_sprite();			
			}
		}

	}
	
	// ---------- obiekty statyczne (wieze, pulapki)

	void animuj_wyceluj_wieze() {
		for (auto i = ustawione_wieze.begin(); i != ustawione_wieze.end(); i++) {
			if ((*i) != nullptr) {
				// dla kazdej istniejacej wiezy
				vector<Wrogowie*> wrogowie = zwroc_wrogow_w_zasiegu_wiezy((*i));
				Wrogowie *wrog_cel = zwroc_wroga_najdalej_na_sciezce(wrogowie);
				if (wrog_cel != nullptr) {
					// konwersja wspolrzednych na srodek obiektu 
					int wieza_x = (*i)->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2;
					int wieza_y = (*i)->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2;
					int wrog_x = wrog_cel->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2;
					int wrog_y = wrog_cel->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2;

					double rotacja = zwroc_rotacje_wiezy(make_pair(wieza_x, wieza_y), make_pair(wrog_x, wrog_y));
					if (rotacja > 22.5 && rotacja <= 67.5) {
						(*i)->mapuj_obrot_wiezy(0); 

					}
					else if (rotacja > 67.5 && rotacja <= 112.5) {
						(*i)->mapuj_obrot_wiezy(1); 
					}
					else if (rotacja > 112.5 && rotacja <= 157.5) {
						(*i)->mapuj_obrot_wiezy(2); 
					}
					else if (rotacja > 157.5 && rotacja <= 202.5) {
						(*i)->mapuj_obrot_wiezy(3); 
					}
					else if (rotacja > 202.5 && rotacja <= 247.5) {
						(*i)->mapuj_obrot_wiezy(4);
					}
					else if (rotacja > 247.5 && rotacja <= 292.5) {
						(*i)->mapuj_obrot_wiezy(5); 
					}
					else if (rotacja > 292.5 && rotacja <= 337.5) {
						(*i)->mapuj_obrot_wiezy(6); 
					}
					else {
						(*i)->mapuj_obrot_wiezy(7); 
					}
				}
			}
		}
	}

	void wybrano_wieze(bool ust_w) {
		ustawiam_wieze = ust_w;
	}

	bool czy_wybrano_wieze() {
		return ustawiam_wieze;
	}

	void pobierz_kod_slotu_typu(string kod) {
		// dla kodowania slt0
		if (kod[0] == 's') {
			nr_slotu = dekoduj_zwroc_nr_slotu_typu(kod);
		}
		// dla kodowania typ0
		else {
			nr_typu = dekoduj_zwroc_nr_slotu_typu(kod);
		}	
	}

	int zwroc_typ_obiektu() {
		return nr_typu;
	}

	int czy_slot_jest_zajety() {
		if (ustawiam_wieze == true) { // sprawdzam czy zajety slot wiezy
			if (ustawione_wieze[nr_slotu] != nullptr) {
				// jesli pozycja jest zajeta zwroc typ wiezy na tym slocie
				return ustawione_wieze[nr_slotu]->zwroc_typ();
			}
			else {
				// jesli pozycja jest pusta zwraca -1
				return -1;
			}
		}
		else {  // sprawdzam czy zajety slot pulapki
			if (ustawione_pulapki[nr_slotu] != nullptr) {
				// jesli pozycja jest zajeta zwroc typ pulpaki na tym slocie
				return ustawione_pulapki[nr_slotu]->zwroc_typ();
			}
			else {
				// jesli pozycja jest pusta zwraca -1
				return -1;
			}
		}
	}

	void dodaj_obiekt() {
		if (ustawiam_wieze == true) {
			int pos_x;
			int pos_y;
			pos_x = mapy->zwroc_wieze_na_mapie(index_mapy).at(nr_slotu).first * WYMIAR_POLA_GRY;
			pos_y = mapy->zwroc_wieze_na_mapie(index_mapy).at(nr_slotu).second * WYMIAR_POLA_GRY;
			if (nr_typu == 0) {
				ustawione_wieze[nr_slotu] = new WiezaTypu0(pos_x, pos_y, rys_ob);
			}
			else if (nr_typu == 1) {
				ustawione_wieze[nr_slotu] = new WiezaTypu1(pos_x, pos_y, rys_ob);
			}
			else if (nr_typu == 2) {
				ustawione_wieze[nr_slotu] = new WiezaTypu2(pos_x, pos_y, rys_ob);
			}
		}
		else {
			int pos_x;
			int pos_y;
			pos_x = mapy->zwroc_pulapki_na_mapie(index_mapy).at(nr_slotu).first * WYMIAR_POLA_GRY;
			pos_y = mapy->zwroc_pulapki_na_mapie(index_mapy).at(nr_slotu).second * WYMIAR_POLA_GRY;
			if (nr_typu == 0) {
				ustawione_pulapki[nr_slotu] = new PulapkaTypu0(pos_x, pos_y, rys_ob);
			}
			else if (nr_typu == 1) {
				ustawione_pulapki[nr_slotu] = new PulapkaTypu1(pos_x, pos_y, rys_ob);
			}
			else if (nr_typu == 2) {
				ustawione_pulapki[nr_slotu] = new PulapkaTypu2(pos_x, pos_y, rys_ob);
			}
		}
	}

	void usun_obiekt() {
		if (ustawiam_wieze == true) {
			ObiektNaMapie *wsk_tmp;
			wsk_tmp = ustawione_wieze[nr_slotu];
			ustawione_wieze[nr_slotu] = nullptr;
			delete(wsk_tmp);
		}
		else {
			ObiektNaMapie *wsk_tmp;
			wsk_tmp = ustawione_pulapki[nr_slotu];
			ustawione_pulapki[nr_slotu] = nullptr;
			delete(wsk_tmp);
		}
	}

	// ---------- mechanika walki

	void przeladuj_wieze() {
		for (auto i = ustawione_wieze.begin(); i != ustawione_wieze.end(); i++) {
			if ((*i) != nullptr) {
				// dla kazdej istniejacej wiezy
				(*i)->laduj_wieze();
			}
		}
	}

	void wykonaj_atak_wiezami() {
		for (auto i = ustawione_wieze.begin(); i != ustawione_wieze.end(); i++) {
			if ((*i) != nullptr) {
				// dla kazdej istniejacej wiezy
				if ((*i)->wieza_gotowa_do_strzalu() == true) {
					// jesli jest ona gotowa do strzalu
					vector<Wrogowie*> wrogowie = zwroc_wrogow_w_zasiegu_wiezy((*i));
					Wrogowie *wrog_cel = zwroc_wroga_najdalej_na_sciezce(wrogowie);
					if (wrog_cel != nullptr) {
						// oraz jest do kogo strzelac
						// ---------- wykonuje strzal
						strzal->wykonaj_atak_zniszcz_wroga((*i), wrog_cel, wrogowie_na_mapie);
					}
				}
			}
		}
	}

	vector<int> zwroc_typy_ustawionych_pulapek() {
		vector<int> tmp;
		for (int i = 0; i < ustawione_pulapki.size(); i++) {
			if (ustawione_pulapki[i] != nullptr) {
				tmp.push_back(ustawione_pulapki[i]->zwroc_typ());
			}
		}
		return tmp;
	}

	map<string, sf::Sprite> zwroc_spritey_obiektow() {
		map<string, sf::Sprite> mapa_tmp;
		string kodowanie_wiezy = "5tur"; // 5 okresla warstwe renderowania
		string kodowanie_pulapki = "5trp";
		string kodowanie_wroga = "6emy";
		string kodowanie_ramki = "7frm";
		string kodowanie_efekty = "8efx";
		// ---------- zwracam wieze
		for (int i = 0; i < ustawione_wieze.size(); i++) {
			if (ustawione_wieze[i]) {
				mapa_tmp.insert(make_pair(kodowanie_wiezy + to_string(i), ustawione_wieze[i]->zwroc_sprite()));
			}
		}
		// ---------- zwracam pulapki
		for (int i = 0; i < ustawione_pulapki.size(); i++) {
			if (ustawione_pulapki[i]) {
				mapa_tmp.insert(make_pair(kodowanie_pulapki + to_string(i), ustawione_pulapki[i]->zwroc_sprite()));
			}
		}
		// ---------- zwracam wrogow
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i]) {
				mapa_tmp.insert(make_pair(kodowanie_wroga + to_string(i), wrogowie_na_mapie[i]->zwroc_sprite()));
				for (int j = 0; j < wrogowie_na_mapie[i]->zwroc_ramki().size(); j++) {
					// zwracanie ramek spriteow wrogow
					mapa_tmp.insert(make_pair(kodowanie_ramki + to_string(i) + "." + to_string(j), wrogowie_na_mapie[i]->zwroc_ramki()[j]));
				}
			}
		}
		// ---------- zwracam efekty

		for (int i = 0; i < strzal->zwroc_efekty_specjalne().size(); i++) {
			if (strzal->zwroc_efekty_specjalne()[i]) {
				mapa_tmp.insert(make_pair(kodowanie_efekty + to_string(i), strzal->zwroc_efekty_specjalne()[i]->zwroc_sprite()));
			}
		}
		return mapa_tmp;
	}

	ManagerObiektowAktywnych(Plik_map *&wczytane_mapy, int wybrana_mapa, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		mapy = wczytane_mapy;
		index_mapy = wybrana_mapa;
		ustawione_wieze.resize(mapy->zwroc_wieze_na_mapie(index_mapy).size());
		ustawione_pulapki.resize(mapy->zwroc_pulapki_na_mapie(index_mapy).size());
		sciezka_danej_mapy = mapy->zwroc_sciezke(index_mapy);
		strzal = new Strzal(rys_ob);
	}

	~ManagerObiektowAktywnych() {
		for (int i = 0; i < ustawione_wieze.size(); i++) {
			if (ustawione_wieze[i] != nullptr) {
				delete ustawione_wieze[i];
			}
		}
		for (int i = 0; i < ustawione_pulapki.size(); i++) {
			if (ustawione_pulapki[i] != nullptr) {
				delete ustawione_pulapki[i];
			}
		}
	}

};

