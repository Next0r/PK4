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
		sprite.setPosition(sf::Vector2f(pozycja_x*WYMIAR_POLA_GRY, pozycja_y*WYMIAR_POLA_GRY));
	}
	virtual void ustaw_sprite() = 0;

public:
	sf::Sprite zwroc_sprite() {
		return sprite;
	}
	int zwroc_typ() {
		return kodowanie_typu;
	}
};

class Wrogowie : public ObiektNaMapie {
protected:
	pair<int, int> wektor_przemieszczenia = make_pair(0, 0); // startowy wektor przemieszczenia = 0,0
	int pozycja_na_sciezce = 0; // domyslnie wrogowie pojawiaja sie na pierwszym polu sciezki
	int rotacja = 0;
public:
	// test ***************************************
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

	pair<int,int> zwroc_pozycje() { // zwraca pozycje wyrazona w px
		return make_pair(pozycja_x, pozycja_y);
	}

	void animuj_sprite() {
		if (rotacja == 0) { // animacja nr 1 - przemieszczneie w prawo
			int left = sprite.getTextureRect().left;
			if (left != 720) { // animacja nie jest na ostatnim obrazku
				mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, WYMIAR_POLA_GRY*6, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
			else { // animacja jest na ostatnim obrazku
				mapuj(0, WYMIAR_POLA_GRY * 6, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
		}
		else if (rotacja == 90) {  // animacja nr 2 - przemieszczenie w dol
			int left = sprite.getTextureRect().left;
			if (left != 720) { // animacja nie jest na ostatnim obrazku
				mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, WYMIAR_POLA_GRY*7, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
			else { // animacja jest na ostatnim obrazku
				mapuj(0, WYMIAR_POLA_GRY * 7, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
		}
		else if (rotacja == -90) { // animacja nr 3 - przemieszczenie w gore
			int left = sprite.getTextureRect().left;
			if (left != 720) { // animacja nie jest na ostatnim obrazku
				mapuj(sprite.getTextureRect().left + WYMIAR_POLA_GRY, WYMIAR_POLA_GRY*8, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
			else { // animacja jest na ostatnim obrazku
				mapuj(0, WYMIAR_POLA_GRY * 8, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
			}
		}
	}

	// test ***************************************
};

class WrogTypu0 : public Wrogowie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wrogowie();
	}
public:
	WrogTypu0(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 0;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(0, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
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

class WiezaTypu0 :public ObiektNaMapie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wiezy();
	}
public:
	WiezaTypu0(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 0;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(0, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class WiezaTypu1 :public ObiektNaMapie {
private:
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wiezy();
	}
public:
	WiezaTypu1(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 1;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(80, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};

class WiezaTypu2 :public ObiektNaMapie {
private:	
	void ustaw_sprite() {
		sprite = rys_ob->zwroc_sprite_wiezy();
	}
public:
	WiezaTypu2(int pos_x, int pos_y, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		pozycja_x = pos_x;
		pozycja_y = pos_y;
		kodowanie_typu = 2;
		ustaw_sprite();
		ustaw_na_mapie();
		mapuj(160, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY);
	}
};


class ManagerObiektowAktywnych {
private:
	bool ustawiam_wieze; 
	RysowaneObiekty *rys_ob;
	int index_mapy;
	Plik_map *mapy;
	vector<ObiektNaMapie*> ustawione_wieze;
	vector<ObiektNaMapie*> ustawione_pulapki;
	vector<Wrogowie*> wrogowie_na_mapie;

	vector <pair<int, int>> sciezka_danej_mapy; // wielokrotnie uzywana - dlatego kopiowana do pamieci

	int nr_slotu; // slot na ktorych chcemy postawic obiekt
	int nr_typu; // typ obiektu ktory chcemy postawic

	int dekoduj_zwroc_nr_slotu_typu(string kod_przycisku) {
		// klawisze maja kodowanie np. slt0
		return kod_przycisku[3] - '0';
	}



public:


	// -- testowanie wrogow ***************************************
	void test_ustaw_wroga() {

		// oblicz pozycje startowa
		int pos_start_x = mapy->zwroc_sciezke(index_mapy)[0].first;
		int pos_start_y = mapy->zwroc_sciezke(index_mapy)[0].second;
		
		// ustaw na starcie
		wrogowie_na_mapie.push_back(new WrogTypu0(pos_start_x*WYMIAR_POLA_GRY, pos_start_y*WYMIAR_POLA_GRY, rys_ob));

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

	void przemiesc_wrogow() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
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

	void test_animuj_wrogow() {
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i] != nullptr) {
				wrogowie_na_mapie[i]->animuj_sprite();
			}
		}
	}
	// -- testowanie wrogow ***************************************


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
			pos_x = mapy->zwroc_wieze_na_mapie(index_mapy).at(nr_slotu).first;
			pos_y = mapy->zwroc_wieze_na_mapie(index_mapy).at(nr_slotu).second;
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
			pos_x = mapy->zwroc_pulapki_na_mapie(index_mapy).at(nr_slotu).first;
			pos_y = mapy->zwroc_pulapki_na_mapie(index_mapy).at(nr_slotu).second;
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
		for (int i = 0; i < ustawione_wieze.size(); i++) {
			if (ustawione_wieze[i]) {
				mapa_tmp.insert(make_pair(kodowanie_wiezy + to_string(i), ustawione_wieze[i]->zwroc_sprite()));
			}
		}
		for (int i = 0; i < ustawione_pulapki.size(); i++) {
			if (ustawione_pulapki[i]) {
				mapa_tmp.insert(make_pair(kodowanie_pulapki + to_string(i), ustawione_pulapki[i]->zwroc_sprite()));
			}
		}
		// zwracanie spriteow wrogow ***************************************
		for (int i = 0; i < wrogowie_na_mapie.size(); i++) {
			if (wrogowie_na_mapie[i]) {
				mapa_tmp.insert(make_pair("6emy" + to_string(i), wrogowie_na_mapie[i]->zwroc_sprite()));
			}
		}
		// zwracanie spriteow wrogow ***************************************
		return mapa_tmp;
	}

	ManagerObiektowAktywnych(Plik_map *&wczytane_mapy, int wybrana_mapa, RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
		mapy = wczytane_mapy;
		index_mapy = wybrana_mapa;
		ustawione_wieze.resize(mapy->zwroc_wieze_na_mapie(index_mapy).size());
		ustawione_pulapki.resize(mapy->zwroc_pulapki_na_mapie(index_mapy).size());
		sciezka_danej_mapy = mapy->zwroc_sciezke(index_mapy);
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

