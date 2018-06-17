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
	ObiektNaMapie(RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
	}

	virtual ~ObiektNaMapie() {};
};

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

	Efekt(int pos_x, int pos_y, int typ, RysowaneObiekty *&rys_o): ObiektNaMapie(rys_o) {
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

class Wrog : public ObiektNaMapie {
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
	vector<vector<float>> parametry_wrogow{
		// pancerz | zdrowie
		{  5.0, 150.0},
		{ 15.0, 250.0},
		{ 10.0, 150.0},
	};

public:
	vector<sf::Sprite> zwroc_ramki() {
		return ramki;
	}
	float zwroc_zdrowie() {
		return zdrowie;
	}
	void odejmij_pacerz(float wartosc) {
		pancerz -= wartosc;
		if (pancerz < 0.0) {
			pancerz = 0.0;
		}
	}
	void odejmij_zdrowie(float wartosc, bool nie_omijaj_pancerza = true) {
		zdrowie = zdrowie - (wartosc - pancerz*nie_omijaj_pancerza);
	}
	void ustaw_zdrowie_na_1() {
		zdrowie = 1;
	}
	void dodaj_ramke(int numer_ramki) {
		sf::Sprite ramka_tmp;
		ramka_tmp = rys_ob->zwroc_sprite_ramki();
		ramka_tmp.setTextureRect(sf::IntRect(0, WYMIAR_POLA_GRY*numer_ramki, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
		ramki.push_back(ramka_tmp);
		synchronizuj_ramki();
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

	Wrog(int pos_x, int pos_y, int typ, RysowaneObiekty *&rys_o) : ObiektNaMapie(rys_o) {
		sprite = rys_ob->zwroc_sprite_wrogowie();
		pancerz = parametry_wrogow[typ][0];
		zdrowie_max = parametry_wrogow[typ][1];
		zdrowie = zdrowie_max;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = typ;
		ustaw_na_mapie();
		dodaj_ramke(0);
		mapuj(0, WYMIAR_POLA_GRY * 3 * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
		synchronizuj_ramki();
	}
};

class Pulapka :public ObiektNaMapie {
private:
	vector<vector<int>> parametry_pulpek{
		// zasieg wykrycia | obrazenia | obrazenia pacerza
		{ 60,  0,20},
		{ 60,100, 0},
		{ 60,  0, 0},
	};

	int obrazenia_zdrowia;
	int obrazenia_pacerza;
	int zasieg_wykrycia_wroga;
public:
	int zwroc_zasieg_wykrycia() {
		return zasieg_wykrycia_wroga;
	}
	int zwroc_obrazania_pacerza() {
		return obrazenia_pacerza;
	}
	int zwroc_obrazania_zdrowia() {
		return obrazenia_zdrowia;
	}
	
	Pulapka(int pos_x, int pos_y, int typ, RysowaneObiekty *&rys_o): ObiektNaMapie(rys_o) {
		sprite = rys_ob->zwroc_sprite_pulapki();
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = typ;
		zasieg_wykrycia_wroga = parametry_pulpek[typ][0];
		obrazenia_pacerza = parametry_pulpek[typ][2];
		obrazenia_zdrowia = parametry_pulpek[typ][1];
		ustaw_na_mapie();
		mapuj(WYMIAR_POLA_GRY*kodowanie_typu, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class Wieza : public ObiektNaMapie {
protected:
	int czas_przeladowania; // jak dlugo trwa przeladowanie
	int przeladowanie = 0; // aktualny stopien przeladowania
	int zasieg;
	int obrazenia;
	int zasieg_obszarowy;
	float rotacja;

	vector<vector<int>> parametry_wiez{
		// zasieg | czas_ladowania | obrazenia | zasieg obszarowy
		{ 140,40,50,  0},
		{ 200,30,25,100},
		{ 100,60,30,200},
	};

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
	void laduj_wieze() {
		if (przeladowanie < czas_przeladowania) {
			przeladowanie++;
		}
	}
	int zwroc_zasieg() {
		return zasieg;
	}
	int zwroc_obrazenia() {
		return obrazenia;
	}
	int zwroc_zasieg_obszarowy() {
		return zasieg_obszarowy;
	}
	void mapuj_obrot_wiezy(int obrot) {
		rotacja = obrot * 45.0 + 45.0;
		sprite.setTextureRect(sf::IntRect(WYMIAR_POLA_GRY*obrot, sprite.getTextureRect().top, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
	}
	
	Wieza(int pos_x, int pos_y, int typ, RysowaneObiekty *&rys_o) : ObiektNaMapie(rys_o) {
		sprite = rys_ob->zwroc_sprite_wiezy();	
		zasieg = parametry_wiez[typ][0];
		czas_przeladowania = parametry_wiez[typ][1];
		obrazenia = parametry_wiez[typ][2];
		zasieg_obszarowy = parametry_wiez[typ][3];
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = typ;
		ustaw_na_mapie();
		mapuj(WYMIAR_POLA_GRY * 4, WYMIAR_POLA_GRY * kodowanie_typu, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class Atak {
private:
	RysowaneObiekty *rys_ob;
	vector<Efekt*> efekty_specjalne;

	float oblicz_dlugosc_wektora(pair<int, int> A, pair<int, int> B) {
		double dlugosc;
		double r1 = (B.first - A.first)*(B.first - A.first);
		double r2 = (B.second - A.second)*(B.second - A.second);
		dlugosc = sqrt(r1 + r2);
		return dlugosc;
	}
	void zniszcz_wroga(ObiektNaMapie *wrog, vector<ObiektNaMapie*> &wrogowie) {
		if (dynamic_cast<Wrog*>(wrog)->zwroc_zdrowie() <= 0) {
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 9, 0.0);
			for (int i = 0; i < wrogowie.size(); i++) {
				if (wrogowie[i] == wrog) {
					wrogowie.erase(wrogowie.begin() + i);
					break;
				}
			}
			auto *wsk = wrog;
			delete wsk;
		}
	}
	void dodaj_efekt_specjalny(int srodek_x, int srodek_y, int pos_x, int pos_y, int typ, float rotacja) {
		efekty_specjalne.push_back(new Efekt(pos_x, pos_x, typ, rys_ob));
		efekty_specjalne.back()->ustaw_srodek(srodek_x, srodek_y);
		efekty_specjalne.back()->ustaw_pozycje(pos_x, pos_y);
		efekty_specjalne.back()->ustaw_rotacje(rotacja);
	}
	void zadaj_obrazenia(ObiektNaMapie *&atakujacy, ObiektNaMapie *&wrog, vector<ObiektNaMapie*> &wrogowie, float mnoznik_obr = 1.0, bool nie_omijaj_pacerza = true) {	
		if (dynamic_cast<Wieza*>(atakujacy)) {
			// atakujacy jest wieza
			dynamic_cast<Wieza*>(atakujacy)->resetuj_ladowanie();
			dynamic_cast<Wrog*>(wrog)->odejmij_zdrowie(dynamic_cast<Wieza*>(atakujacy)->zwroc_obrazenia(), nie_omijaj_pacerza);
			zniszcz_wroga(wrog, wrogowie);
		}
		else {
			// jesli atakujacego nie da sie skonwertowac do typu wiezy
			dynamic_cast<Wrog*>(wrog)->odejmij_zdrowie(dynamic_cast<Pulapka*>(atakujacy)->zwroc_obrazania_zdrowia(), nie_omijaj_pacerza);
			zniszcz_wroga(wrog, wrogowie);
		}
	}
	vector<ObiektNaMapie*> zwroc_wrogow_w_zasiegu(int ilosc, ObiektNaMapie *&wieza, ObiektNaMapie *&wrog, vector<ObiektNaMapie*> &wrogowie) {
		int zasieg_obs_tmp = dynamic_cast<Wieza*>(wieza)->zwroc_zasieg_obszarowy();
		vector<ObiektNaMapie*> wrogowie_w_zasiegu;
		for (auto i = wrogowie.begin(); i != wrogowie.end(); i++) {
			if ((*i) != nullptr && (*i)!= wrog) {
				if (oblicz_dlugosc_wektora(wrog->zwroc_pozycje(), (*i)->zwroc_pozycje()) < zasieg_obs_tmp) {
					wrogowie_w_zasiegu.push_back((*i));
					if (wrogowie_w_zasiegu.size() > ilosc) {
						wrogowie_w_zasiegu.pop_back();
					}
				}
			}
		}
		return wrogowie_w_zasiegu;
	}
	void obnizenie_pacerza(ObiektNaMapie *&pulapka, ObiektNaMapie *&wrog) {
		dynamic_cast<Wrog*>(wrog)->odejmij_pacerz(dynamic_cast<Pulapka*>(pulapka)->zwroc_obrazania_pacerza());
		dynamic_cast<Wrog*>(wrog)->dodaj_ramke(1); // dodaje do wroga ramke obnizonego pancerza
	}

public:
	vector<Efekt*> zwroc_efekty_specjalne() {
		return efekty_specjalne;
	}
	void wykonaj_atak_wiezy_zniszcz_wroga(ObiektNaMapie *&wieza, ObiektNaMapie *&wrog, vector<ObiektNaMapie*> &wrogowie) {
		if (wieza->zwroc_typ() == 0) {
			// ---------- efekt strzalu
			dodaj_efekt_specjalny(40, -27, wieza->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2,
				wieza->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2, 1, 270.0 - dynamic_cast<Wieza*>(wieza)->zwroc_rotacje());
			// ---------- efekt wybuchu na wrogu
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 0, 0.0);
			// jesli jest to wieza typu zerowego
			zadaj_obrazenia(wieza, wrog, wrogowie);
		}
		else if (wieza->zwroc_typ() == 1) {
			// jesli jest to wieza typu pierwszego
			vector<ObiektNaMapie*> wrogowie_w_zasiegu = zwroc_wrogow_w_zasiegu(2, wieza, wrog, wrogowie);
			dodaj_efekt_specjalny(WYMIAR_POLA_GRY / 2, WYMIAR_POLA_GRY / 2, wieza->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2,
				wieza->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2, 2, 270.0 - dynamic_cast<Wieza*>(wieza)->zwroc_rotacje());
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 3, 0.0);
			zadaj_obrazenia(wieza, wrog, wrogowie);
			for (auto i = wrogowie_w_zasiegu.begin(); i != wrogowie_w_zasiegu.end(); i++) {
				dodaj_efekt_specjalny(WYMIAR_POLA_GRY / 2, WYMIAR_POLA_GRY / 2, wieza->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2,
					wieza->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2, 2, 270.0 - dynamic_cast<Wieza*>(wieza)->zwroc_rotacje());
				dodaj_efekt_specjalny(0, 0, (*i)->zwroc_pozycje().first, (*i)->zwroc_pozycje().second, 3, 0.0);
				zadaj_obrazenia(wieza, (*i), wrogowie, 0.75);
			}
		}
		else if (wieza->zwroc_typ() == 2) {
			// jesli jest to wieza typu drugiego
			vector<ObiektNaMapie*> wrogowie_w_zasiegu = zwroc_wrogow_w_zasiegu(4, wieza, wrog, wrogowie);
			dodaj_efekt_specjalny(WYMIAR_POLA_GRY / 2, WYMIAR_POLA_GRY / 2, wieza->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2,
				wieza->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2, 7, 0.0);
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 8, 0.0);
			zadaj_obrazenia(wieza, wrog, wrogowie, false);
			for (auto i = wrogowie_w_zasiegu.begin(); i != wrogowie_w_zasiegu.end(); i++) {
				dodaj_efekt_specjalny(WYMIAR_POLA_GRY / 2, WYMIAR_POLA_GRY / 2, wieza->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2,
					wieza->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2, 7, 0.0);
				dodaj_efekt_specjalny(0, 0, (*i)->zwroc_pozycje().first, (*i)->zwroc_pozycje().second, 8, 0.0);
				zadaj_obrazenia(wieza, (*i), wrogowie, false);
			}
		}
	}
	void wykonaj_atak_pulapki_zniszcz_wroga(ObiektNaMapie *&pulapka, ObiektNaMapie *&wrog, vector<ObiektNaMapie*> &wrogowie) {
		if (pulapka->zwroc_typ() == 0) {
			// jesli jest to pulpaka typu 0
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 4, 0.0);
			obnizenie_pacerza(pulapka, wrog);
		}
		else if (pulapka->zwroc_typ() == 1) {		
			// jesli jest to pulpaka typu 1
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 5, 0.0);
			zadaj_obrazenia(pulapka, wrog, wrogowie);
		}
		else if (pulapka->zwroc_typ() == 2) {
			// jesli jest to pulpaka typu 2
			dodaj_efekt_specjalny(0, 0, wrog->zwroc_pozycje().first, wrog->zwroc_pozycje().second, 6, 0.0);
			dynamic_cast<Wrog*>(wrog)->ustaw_zdrowie_na_1();
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

	Atak(RysowaneObiekty *&r_ob) {
		rys_ob = r_ob;
	}
	~Atak() {
		for (auto i = efekty_specjalne.begin(); i != efekty_specjalne.end(); i++) {
			if ((*i) != nullptr) {
				delete (*i);
			}
		}
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
	Atak *atak;

	vector<ObiektNaMapie*> wrogowie_na_mapie;
	vector<ObiektNaMapie*> wieze_na_mapie;
	vector<ObiektNaMapie*> pulapki_na_mapie;

	string kodowanie_wroga = "F emy";
	string kodowanie_pulapki = "E trp";
	string kodowanie_wiezy = "E turr";
	string kodowanie_ramki = "G frm";
	string kodowanie_efektu = "H efx";


	vector <pair<int, int>> sciezka_danej_mapy; // wielokrotnie uzywana - dlatego kopiowana do pamieci

	int nr_slotu; // slot na ktorych chcemy postawic obiekt
	int nr_typu; // typ obiektu ktory chcemy postawic
	int nr_uzywanej_pulapki; 

	int dekoduj_zwroc_nr_slotu_typu(string kod_przycisku) {
		// klawisze maja kodowanie np. slt0
		return kod_przycisku[3] - '0';
	}
	void ustaw_wroga(int typ_wroga) {
		// oblicz pozycje startowa
		int pos_start_x = mapy->zwroc_sciezke(index_mapy)[0].first * WYMIAR_POLA_GRY;
		int pos_start_y = mapy->zwroc_sciezke(index_mapy)[0].second * WYMIAR_POLA_GRY;
		// ustaw danego wroga na starcie
		wrogowie_na_mapie.push_back(new Wrog(pos_start_x, pos_start_y, typ_wroga, rys_ob));
	}
	float oblicz_dlugosc_wektora(pair<int, int> A, pair<int, int> B) {
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
	ObiektNaMapie *zwroc_wroga_najdalej_na_sciezce(vector<ObiektNaMapie*> wrogowie) {
		ObiektNaMapie *wrog_najdlalej_na_sciezce = nullptr;
		for (auto j = wrogowie.begin(); j != wrogowie.end(); j++) {
			if ((*j) != nullptr) {
				wrog_najdlalej_na_sciezce = (*j);
				break;
			}
		}

		// wyszukiwanie wroga, ktory jest najdalej na sciezce
		for (auto j = wrogowie.begin(); j != wrogowie.end(); j++) {
			if ((*j) != nullptr) {
				if (dynamic_cast<Wrog*> (wrog_najdlalej_na_sciezce)->zwroc_pozycje_na_sciezce() < dynamic_cast<Wrog*>(*j)->zwroc_pozycje_na_sciezce()) {
					wrog_najdlalej_na_sciezce = (*j);
				}
			}
		}
		return wrog_najdlalej_na_sciezce;
	}
	vector<ObiektNaMapie*> zwroc_wrogow_w_zasiegu_wiezy(Wieza* wieza) {
		vector<ObiektNaMapie*> wrogowie = wrogowie_na_mapie; // skopiuj wrogow na mapie
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
		for (int i = 0; i < atak->zwroc_efekty_specjalne().size(); i++) {
			if (atak->zwroc_efekty_specjalne()[i] != nullptr) {
				atak->zwroc_efekty_specjalne()[i]->animuj_sprite();
				if (atak->zwroc_efekty_specjalne()[i]->zwroc_ukonczony() == true) {
					atak->usun_efekt_specjalny(i);
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
				int pozycja_na_sciezce_s = dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->zwroc_pozycje_na_sciezce();
				pair<int, int> pozycja_abs = sciezka_danej_mapy[pozycja_na_sciezce_s];
				pair<int, int> pozycja_docelowa_abs;
				if (sciezka_danej_mapy.back() != sciezka_danej_mapy[pozycja_na_sciezce_s]) { // jesli aktualna pozycja abs nie jest ostatnim polem sciezki
					pozycja_docelowa_abs = sciezka_danej_mapy[pozycja_na_sciezce_s + 1];
				}
				int x = pozycja_docelowa_abs.first - pozycja_abs.first;
				int y = pozycja_docelowa_abs.second - pozycja_abs.second;

				dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->przelicz_rotacje();
				dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->aktualizuj_wektor_przemieszczenia(x, y);
			}
		}
	}
	void odswiez_pozycje_na_sciezce_wrogow() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				int pozycja_na_sciezce = dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->zwroc_pozycje_na_sciezce();
				pair<int, int> pozycja_abs = sciezka_danej_mapy[pozycja_na_sciezce]; // pozycja bazowa 16:9
				pair<int, int> pozycja_obiektu = wrogowie_na_mapie[i]->zwroc_pozycje(); // wyrazona w px
				if (pozycja_obiektu.first%WYMIAR_POLA_GRY == 0 && pozycja_obiektu.second%WYMIAR_POLA_GRY == 0) {
					// jesli aktualna pozycja pozwala obliczyc pozycje abs - 16:9
					int pos_x_abs = pozycja_obiektu.first / WYMIAR_POLA_GRY;
					int pos_y_abs = pozycja_obiektu.second / WYMIAR_POLA_GRY;

					if (pozycja_abs.first != pos_x_abs || pozycja_abs.second != pos_y_abs) {
						// i nie jest to ostatnia zapamietana pozycja abs
						// oznacza to, ze wrog osiagna kolejne pole sciezki
						dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->ustaw_pozycje_na_sciezce(pozycja_na_sciezce += 1);
					}

				}
			}
		}
	}
	void przemiesc_wrogow_synchronizuj_ramki() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->synchronizuj_ramki();
				dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->przemiesc_wroga();
			}
		}
	}
	bool wrogowie_na_koncu() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				if (dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->zwroc_pozycje_na_sciezce() == sciezka_danej_mapy.size() - 1) {
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
				dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->animuj_sprite();
			}
		}

	}
	bool nie_ma_wrogow_na_mapie() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				return false;
			}
		}
		return true;
	}
	// ---------- obiekty statyczne (wieze, pulapki)
	void animuj_wyceluj_wieze() {
		for (auto i = wieze_na_mapie.begin(); i != wieze_na_mapie.end(); i++) {
			if ((*i) != nullptr) {
				// dla kazdej istniejacej wiezy
				vector<ObiektNaMapie*> wrogowie = zwroc_wrogow_w_zasiegu_wiezy(dynamic_cast<Wieza*>(*i));
				ObiektNaMapie *wrog_cel = zwroc_wroga_najdalej_na_sciezce(wrogowie);
				if (wrog_cel != nullptr) {
					// konwersja wspolrzednych na srodek obiektu 
					int wieza_x = (*i)->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2;
					int wieza_y = (*i)->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2;
					int wrog_x = wrog_cel->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2;
					int wrog_y = wrog_cel->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2;

					double rotacja = zwroc_rotacje_wiezy(make_pair(wieza_x, wieza_y), make_pair(wrog_x, wrog_y));
					if (rotacja > 22.5 && rotacja <= 67.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(0);

					}
					else if (rotacja > 67.5 && rotacja <= 112.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(1);
					}
					else if (rotacja > 112.5 && rotacja <= 157.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(2);
					}
					else if (rotacja > 157.5 && rotacja <= 202.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(3);
					}
					else if (rotacja > 202.5 && rotacja <= 247.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(4);
					}
					else if (rotacja > 247.5 && rotacja <= 292.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(5);
					}
					else if (rotacja > 292.5 && rotacja <= 337.5) {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(6);
					}
					else {
						dynamic_cast<Wieza*>(*i)->mapuj_obrot_wiezy(7);
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
	void pobierz_kod_przycisku(string kod) {
		// dla kodowania slt0
		if (kod[0] == 's') {
			nr_slotu = dekoduj_zwroc_nr_slotu_typu(kod);
		}
		// dla kodowania typ0
		else if (kod[0] == 't') {
			nr_typu = dekoduj_zwroc_nr_slotu_typu(kod);
		}
		// dla kodowania act0
		else {
			nr_uzywanej_pulapki = dekoduj_zwroc_nr_slotu_typu(kod);
		}
	}
	int zwroc_typ_obiektu() {
		return nr_typu;
	}
	int czy_slot_jest_zajety() {
		if (ustawiam_wieze == true) { // sprawdzam czy zajety slot wiezy
			if (wieze_na_mapie[nr_slotu] != nullptr) {
				// jesli pozycja jest zajeta zwroc typ wiezy na tym slocie
				return wieze_na_mapie[nr_slotu]->zwroc_typ();
			}
			else {
				// jesli pozycja jest pusta zwraca -1
				return -1;
			}
		}
		else {  // sprawdzam czy zajety slot pulapki
			if (pulapki_na_mapie[nr_slotu] != nullptr) {
				// jesli pozycja jest zajeta zwroc typ pulpaki na tym slocie
				return pulapki_na_mapie[nr_slotu]->zwroc_typ();
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
			wieze_na_mapie[nr_slotu] = new Wieza(pos_x, pos_y, nr_typu, rys_ob);
		}
		else {
			int pos_x;
			int pos_y;
			pos_x = mapy->zwroc_pulapki_na_mapie(index_mapy).at(nr_slotu).first * WYMIAR_POLA_GRY;
			pos_y = mapy->zwroc_pulapki_na_mapie(index_mapy).at(nr_slotu).second * WYMIAR_POLA_GRY;
			pulapki_na_mapie[nr_slotu] = new Pulapka(pos_x, pos_y, nr_typu, rys_ob);
		}
	}
	void usun_obiekt() {
		if (ustawiam_wieze == true) {
			auto *wsk_tmp = wieze_na_mapie[nr_slotu];
			wieze_na_mapie[nr_slotu] = nullptr;
			delete wsk_tmp;
		}
		else {
			auto *wsk_tmp = pulapki_na_mapie[nr_slotu];
			pulapki_na_mapie[nr_slotu] = nullptr;
			delete wsk_tmp;
		}
	}
	// ---------- mechanika walki
	void przeladuj_wieze() {
		for (auto i = wieze_na_mapie.begin(); i != wieze_na_mapie.end(); i++) {
			if ((*i) != nullptr) {
				// dla kazdej istniejacej wiezy
				dynamic_cast<Wieza*>(*i)->laduj_wieze();
			}
		}
	}
	void wykonaj_atak_wiezami() {
		for (auto i = wieze_na_mapie.begin(); i != wieze_na_mapie.end(); i++) {
			if ((*i) != nullptr) {
				// dla kazdej istniejacej wiezy
				if (dynamic_cast<Wieza*>(*i)->wieza_gotowa_do_strzalu() == true) {
					// jesli jest ona gotowa do strzalu
					vector<ObiektNaMapie*> wrogowie = zwroc_wrogow_w_zasiegu_wiezy(dynamic_cast<Wieza*>(*i));
					ObiektNaMapie *wrog_cel = zwroc_wroga_najdalej_na_sciezce(wrogowie);
					if (wrog_cel != nullptr) {
						// oraz jest do kogo strzelac
						// ---------- wykonuje strzal
						atak->wykonaj_atak_wiezy_zniszcz_wroga((*i), wrog_cel, wrogowie_na_mapie);
					}
				}
			}
		}
	}
	bool uzyj_pulapki() {
		// jesli uzywana pulapka istnieje
		if (pulapki_na_mapie[nr_uzywanej_pulapki] != nullptr) {
			// skopiuj pozycje wrogow na mapie
			// przelicz punkty uchytu dla poszczeglonych spriteow
			vector <pair<int, int>> wrogowie;
			wrogowie.resize(wrogowie_na_mapie.size());
			for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
				if (wrogowie_na_mapie[i] != nullptr) {
					wrogowie[i] = wrogowie_na_mapie[i]->zwroc_pozycje();
					wrogowie[i].first += WYMIAR_POLA_GRY / 2;
					wrogowie[i].second += WYMIAR_POLA_GRY / 2;
				}
			}
			// przelicz pozycje uzywanej pulapki
			int pos_x_pulapki = pulapki_na_mapie[nr_uzywanej_pulapki]->zwroc_pozycje().first + WYMIAR_POLA_GRY / 2;
			int pos_y_pulapki = pulapki_na_mapie[nr_uzywanej_pulapki]->zwroc_pozycje().second + WYMIAR_POLA_GRY / 2;

			// poszukaj najblizszego wroga
			if (wrogowie.size() != 0) {
				float dl_najkrotszego_wektora = oblicz_dlugosc_wektora(make_pair(pos_x_pulapki, pos_y_pulapki), wrogowie[0]);
				int index_najblizszego_wroga = 0;

				for (int i = 1; i < wrogowie.size(); i++) {
					float dl_wektora = oblicz_dlugosc_wektora(make_pair(pos_x_pulapki, pos_y_pulapki), wrogowie[i]);
					if (dl_wektora < dl_najkrotszego_wektora) {
						index_najblizszego_wroga = i;
						dl_najkrotszego_wektora = dl_wektora;
					}
				}

				// jesli odszukany wrog jest w zasiegu pulapki
				if (dl_najkrotszego_wektora <= dynamic_cast<Pulapka*>(pulapki_na_mapie[nr_uzywanej_pulapki])->zwroc_zasieg_wykrycia()) {
					atak->wykonaj_atak_pulapki_zniszcz_wroga(pulapki_na_mapie[nr_uzywanej_pulapki], wrogowie_na_mapie[index_najblizszego_wroga], wrogowie_na_mapie);
					// pulpaka zaatakowala nalezy ja usunac
					auto *wsk_tmp = pulapki_na_mapie[nr_uzywanej_pulapki];
					pulapki_na_mapie[nr_uzywanej_pulapki] = nullptr;
					delete wsk_tmp;
					// wykonano poprawnie atak pulapka nalezy usunac przycisk interfaceu
					return true;
				}
			}
			// nie udalo sie uzyc poprawnie pulpaki		
		}
		return false;
	}
	
	vector<int> zwroc_typy_ustawionych_pulapek() {
		vector<int> tmp;
		for (int i = 0; i < pulapki_na_mapie.size(); i++) {
			if (pulapki_na_mapie[i] != nullptr) {
				tmp.push_back(pulapki_na_mapie[i]->zwroc_typ());
			}
		}
		return tmp;
	}
	map<string, sf::Sprite> zwroc_spritey_obiektow() {
		map<string, sf::Sprite> mapa_tmp;

		// ---------- zwracam wieze
		for (int i = 0; i < wieze_na_mapie.size(); i++) {
			if (wieze_na_mapie[i]) {
				mapa_tmp.insert(make_pair(kodowanie_wiezy + to_string(i), wieze_na_mapie[i]->zwroc_sprite()));
			}
		}
		// ---------- zwracam pulapki
		for (int i = 0; i < pulapki_na_mapie.size(); i++) {
			if (pulapki_na_mapie[i]) {
				mapa_tmp.insert(make_pair(kodowanie_pulapki + to_string(i), pulapki_na_mapie[i]->zwroc_sprite()));
			}
		}
		// ---------- zwracam wrogow
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i]) {
				mapa_tmp.insert(make_pair(kodowanie_wroga + to_string(i), wrogowie_na_mapie[i]->zwroc_sprite()));
				for (int j = 0; j < dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->zwroc_ramki().size(); j++) {
					// zwracanie ramek spriteow wrogow
					mapa_tmp.insert(make_pair(kodowanie_ramki + to_string(i) + "." + to_string(j), dynamic_cast<Wrog*>(wrogowie_na_mapie[i])->zwroc_ramki()[j]));
				}
			}
		}
		// ---------- zwracam efekty

		for (int i = 0; i < atak->zwroc_efekty_specjalne().size(); i++) {
			if (atak->zwroc_efekty_specjalne()[i]) {
				mapa_tmp.insert(make_pair(kodowanie_efektu + to_string(i), atak->zwroc_efekty_specjalne()[i]->zwroc_sprite()));
			}
		}
		return mapa_tmp;
	}

	ManagerObiektowAktywnych(Plik_map *&wczytane_mapy, int wybrana_mapa, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		mapy = wczytane_mapy;
		index_mapy = wybrana_mapa;
		wieze_na_mapie.resize(mapy->zwroc_wieze_na_mapie(index_mapy).size());
		pulapki_na_mapie.resize(mapy->zwroc_pulapki_na_mapie(index_mapy).size());
		sciezka_danej_mapy = mapy->zwroc_sciezke(index_mapy);
		atak = new Atak(rys_ob);
	}
	~ManagerObiektowAktywnych() {
		delete atak;
		for (auto i = wieze_na_mapie.begin(); i != wieze_na_mapie.end(); i++) {
			if ((*i) != nullptr) {
				delete (*i);
			}
		}
		for (auto i = pulapki_na_mapie.begin(); i != pulapki_na_mapie.end(); i++) {
			if ((*i) != nullptr) {
				delete (*i);
			}
		}
		for (auto i = wrogowie_na_mapie.begin(); i != wrogowie_na_mapie.end(); i++) {
			if ((*i) != nullptr) {
				delete (*i);
			}
		}
	}
};

