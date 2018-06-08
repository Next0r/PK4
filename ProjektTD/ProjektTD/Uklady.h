#pragma once
#include "Baza.h"
#include "RysowaneObiekty.h"

using namespace std;

class Uklad {
protected:
	const int rozmiar_pola_liczbowego = 4;
	const float skala_cyfry = 0.71;
	const int cyfra_x = 50;
	const int cyfra_y = 70;
	const int wymiar_przycisku_x = 370;
	const int wymiar_przycisku_y = 100;
	const int wymiar_p_akcji_x = 80;
	const int wymiar_p_akcji_y = 80;
	const int wymiar_okna_x = 450;
	const int wymiar_okna_y = 300;
	const float skala_okna = 1.5;
	
	const int poziom_przyciemnienia = 160;

	string kodowanie_przycisku = "C_bttn";
	string kodowanie_przycisku_okna = "Z_bttn";
	string kodowanie_przycisku_akcji = "U_bttn";
	string kodowanie_tla = "B_bg";
	string kodowanie_pola_tekstowego = "D_pt";
	string kodowanie_pola_liczbowego = "D_nm";
	string kodowanie_potwierdzenia = "conf";
	string kodowanie_odrzucenia = "rect";
	string kodowanie_liter = "T_chr";
	string kodowanie_w_przyciemnienia = "X_efx";
	string kodowanie_okna = "Y_win";
	string kodowanie_interfaceu = "U_int";

	sf::Sprite el_ukladu;
	RysowaneObiekty *rys_ob;
	map<string, sf::Sprite> elementy_w_ukladzie;
	vector<map<string, sf::Sprite>::iterator> pole_liczbowe;
	map<string, sf::Sprite>::iterator it_okna;
	map<string, sf::Sprite>::iterator it_przyciemnienia;

	void przyciemnienie_tla() {
		sf::Sprite efekt = rys_ob->zwroc_sprite_tlo();
		mapuj_sprite(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y, 0, 0, efekt);
		ustaw_sprite(0, 0, efekt);
		efekt.setColor(sf::Color(0, 0, 0, poziom_przyciemnienia));
		it_przyciemnienia = elementy_w_ukladzie.insert(make_pair(kodowanie_w_przyciemnienia, efekt)).first;
	}
	void mapuj_sprite(int wymiar_x, int wymiar_y, int tex_pos_x, int tex_pos_y, sf::Sprite &mapowany) {
		mapowany.setTextureRect(sf::IntRect(tex_pos_x, tex_pos_y, wymiar_x, wymiar_y));
	}
	void ustaw_sprite(int pos_x, int pos_y, sf::Sprite &ustawiany) {
		ustawiany.setPosition(sf::Vector2f(pos_x, pos_y));
	}
	void skaluj_sprite(float skala_x, float skala_y, sf::Sprite &skalowany) {
		skalowany.setScale(sf::Vector2f(skala_x, skala_y));
	}
	void wstaw_pole_liczbowe() {
		float pos_x = (400 - rozmiar_pola_liczbowego * skala_cyfry * cyfra_x) / 2 + (cyfra_x*skala_cyfry) / 2;
		float pos_y = WYMIAR_EKRANU_Y - 20 - (skala_cyfry * cyfra_y) / 2;
		for (int i = 0; i < rozmiar_pola_liczbowego; i++) {
			sf::Sprite cyfra = rys_ob->zwroc_sprite_cyfry();
			mapuj_sprite(cyfra_x, cyfra_y, 0, 0, cyfra);
			cyfra.setOrigin(sf::Vector2f(cyfra_x / 2, cyfra_y / 2));
			skaluj_sprite(skala_cyfry, skala_cyfry, cyfra);
			ustaw_sprite(pos_x + skala_cyfry*cyfra_x*i, pos_y, cyfra);
			auto wstawiony_el = elementy_w_ukladzie.insert(make_pair(kodowanie_pola_liczbowego + to_string(i), cyfra));
			pole_liczbowe.push_back(wstawiony_el.first);
		}
	}
	
public:
	Uklad(RysowaneObiekty *&rys_o) {
		rys_ob = rys_o;
	}
	virtual ~Uklad() {};
	void wstaw_tlo(int wymiar_x, int wymiar_y, int  pos_x, int pos_y, int map_x) {
		el_ukladu = rys_ob->zwroc_sprite_tlo();
		mapuj_sprite(wymiar_x, wymiar_y, map_x, 0, el_ukladu);
		ustaw_sprite(pos_x, pos_y, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(kodowanie_tla, el_ukladu));
	}
	void wstaw_standardowy_przycisk(int pos_x, int pos_y, int nr_przycisku_na_texturze, string nazwa_przycisku, float skala_x = 1.0, float skala_y = 1.0, bool przycisk_okna = false) {
		el_ukladu = rys_ob->zwroc_sprite_przycisk();
		// ustaw origin przycisku (jako srodek spritea)
		el_ukladu.setOrigin(sf::Vector2f(wymiar_przycisku_x / 2, wymiar_przycisku_y / 2));
		mapuj_sprite(wymiar_przycisku_x, wymiar_przycisku_y, 0, nr_przycisku_na_texturze*wymiar_przycisku_y, el_ukladu);
		skaluj_sprite(skala_x, skala_y, el_ukladu);
		ustaw_sprite(pos_x, pos_y, el_ukladu);
		if (przycisk_okna == false) {
			elementy_w_ukladzie.insert(make_pair(kodowanie_przycisku + nazwa_przycisku, el_ukladu));
		}
		else {
			elementy_w_ukladzie.insert(make_pair(kodowanie_przycisku_okna + nazwa_przycisku, el_ukladu));
		}
	}
	void wstaw_przycisk_akcji(int pos_x, int pos_y, int nr_przycisku_na_texturze, string nazwa_przycisku, float skala_x = 1.0, float skala_y = 1.0) {
		el_ukladu = rys_ob->zwroc_sprite_p_akcji();
		// ustaw origin przycisku (jako srodek spritea)
		el_ukladu.setOrigin(sf::Vector2f(wymiar_p_akcji_x / 2, wymiar_p_akcji_y / 2));
		mapuj_sprite(wymiar_p_akcji_x, wymiar_p_akcji_y, 0, nr_przycisku_na_texturze*wymiar_p_akcji_y, el_ukladu);
		skaluj_sprite(skala_x, skala_y, el_ukladu);
		ustaw_sprite(pos_x, pos_y, el_ukladu);
		elementy_w_ukladzie.insert(make_pair(kodowanie_przycisku_akcji + nazwa_przycisku, el_ukladu));
	}
	void zmien_stan_przycisku(string nazwa_przycisku, bool nacisniety) {
		auto znaleziony_element = elementy_w_ukladzie.find(kodowanie_przycisku + nazwa_przycisku);
		if (znaleziony_element == elementy_w_ukladzie.end()) {
			znaleziony_element = elementy_w_ukladzie.find(kodowanie_przycisku_okna + nazwa_przycisku);
		}
		if (znaleziony_element == elementy_w_ukladzie.end()) {
			znaleziony_element = elementy_w_ukladzie.find(kodowanie_przycisku_akcji + nazwa_przycisku);
		}
		if (znaleziony_element != elementy_w_ukladzie.end()) {
			// jesli poszukiwany przycisk zostal znaleziony
			int x = (*znaleziony_element).second.getTextureRect().width;
			int y = (*znaleziony_element).second.getTextureRect().height;
			int map_y = (*znaleziony_element).second.getTextureRect().top;
			// zerowanie mapingu dla false lub przesuniecie dla true
			mapuj_sprite(x, y, nacisniety* x, map_y, (*znaleziony_element).second);
		}


	}
	map<string, sf::Sprite> zwroc_uklad() {
		return elementy_w_ukladzie;
	}
	void ustaw_wartosc_pola_liczbowego(int wartosc) {
		string wartosc_tekst = to_string(wartosc);
		while (wartosc_tekst.size() != rozmiar_pola_liczbowego) {
			wartosc_tekst = '0' + wartosc_tekst;
		}
		for (int i = 0; i < rozmiar_pola_liczbowego; i++) {
			int wartosc_pola = wartosc_tekst[i] - '0';
			mapuj_sprite(cyfra_x, cyfra_y, cyfra_x*wartosc_pola, 0, pole_liczbowe[i]->second);
		}
	}
	void wstaw_okno(int numer_okna, int nr_przycisku1 = 4, int nr_przycisku2 = 5, bool jeden_przycisk = false) {
		sf::Sprite okno = rys_ob->zwroc_sprite_okienka();
		okno.setOrigin(sf::Vector2f(wymiar_okna_x / 2, wymiar_okna_y / 2));
		mapuj_sprite(wymiar_okna_x, wymiar_okna_y, 0, wymiar_okna_y * numer_okna, okno);
		ustaw_sprite(WYMIAR_EKRANU_X / 2, WYMIAR_EKRANU_Y / 2, okno);
		skaluj_sprite(skala_okna, skala_okna, okno);	
		it_okna = elementy_w_ukladzie.insert(make_pair(kodowanie_okna, okno)).first;
		przyciemnienie_tla();
		if (jeden_przycisk == false) {
			wstaw_standardowy_przycisk(493, 518, nr_przycisku1, kodowanie_potwierdzenia, 0.74, 0.74, true);
			wstaw_standardowy_przycisk(787, 518, nr_przycisku2, kodowanie_odrzucenia, 0.74, 0.74, true);
		}
		else {
			wstaw_standardowy_przycisk(WYMIAR_EKRANU_X / 2, 518, nr_przycisku2, kodowanie_odrzucenia, 0.74, 0.74, true);
		}
	}
	void usun_okno() {
		elementy_w_ukladzie.erase(it_przyciemnienia);
		elementy_w_ukladzie.erase(it_okna);
		elementy_w_ukladzie.erase(kodowanie_przycisku_okna + kodowanie_potwierdzenia);
		elementy_w_ukladzie.erase(kodowanie_przycisku_okna + kodowanie_odrzucenia);
	}
};

class UkladMenu :public Uklad {
public:
	UkladMenu(RysowaneObiekty *&rys_o) :Uklad(rys_o) {
		wstaw_tlo(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y, 0, 0, 0);
		wstaw_standardowy_przycisk(WYMIAR_EKRANU_X / 2, WYMIAR_EKRANU_Y / 5, 0, "strt");
		wstaw_standardowy_przycisk(WYMIAR_EKRANU_X / 2, WYMIAR_EKRANU_Y / 5 * 2, 1, "cont");
		wstaw_standardowy_przycisk(WYMIAR_EKRANU_X / 2, WYMIAR_EKRANU_Y / 5 * 3, 2, "info");
		wstaw_standardowy_przycisk(WYMIAR_EKRANU_X / 2, WYMIAR_EKRANU_Y / 5 * 4, 3, "quit");
	}
};

class UkladInfo :public Uklad {
private:
	int ilosc_stron = 4;
	int pole_x = 400;
	int pole_y = 270;
	int pos_y = 305;
	void dodaj_pole_tekstowe() {
		sf::Sprite pole_tekstowe = rys_ob->zwroc_sprite_pole_tekstowe_1();
		mapuj_sprite(pole_x, pole_y, 0, 0, pole_tekstowe);
		pole_tekstowe.setOrigin(sf::Vector2f(pole_x / 2, pole_y / 2));
		ustaw_sprite(WYMIAR_EKRANU_X / 2, pos_y, pole_tekstowe);
		skaluj_sprite(2.0, 2.0, pole_tekstowe);
		elementy_w_ukladzie.insert(make_pair(kodowanie_pola_tekstowego, pole_tekstowe));
	}
public:
	UkladInfo(RysowaneObiekty *&rys_o) :Uklad(rys_o) {
		wstaw_tlo(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y, 0, 0, 0);
		dodaj_pole_tekstowe();
		wstaw_standardowy_przycisk(159, 675, 6, "prev", 0.74, 0.74);
		wstaw_standardowy_przycisk(457, 675, 7, "next", 0.74, 0.74);
		wstaw_standardowy_przycisk(1121, 675, 5, "back", 0.74, 0.74);
	}
	void przewin_strone(bool do_przodu) {
		auto znaleziony_element = elementy_w_ukladzie.find(kodowanie_pola_tekstowego);
		if (znaleziony_element != elementy_w_ukladzie.end()) {
			if (do_przodu == true) {
				int mapowanie_y = (*znaleziony_element).second.getTextureRect().top;
				if (mapowanie_y != (ilosc_stron - 1)*pole_y) {
					mapuj_sprite(pole_x, pole_y, 0, mapowanie_y + pole_y, (*znaleziony_element).second);;
				}
				else {
					return;
				}
			}
			else {
				int mapowanie_y = (*znaleziony_element).second.getTextureRect().top;
				if (mapowanie_y != 0) {
					mapuj_sprite(pole_x, pole_y, 0, mapowanie_y - pole_y, (*znaleziony_element).second);;
				}
				else {
					return;
				}
			}
		}
	}
};

class UkladSklep : public Uklad {
public:
	UkladSklep(RysowaneObiekty *&rys_o) : Uklad(rys_o) {
		wstaw_tlo(400, WYMIAR_EKRANU_Y, 0, 0, WYMIAR_EKRANU_X);
		wstaw_standardowy_przycisk(200, 57, 9, "turr", 0.74, 0.74);
		wstaw_standardowy_przycisk(200, 151, 8, "trap", 0.74, 0.74);
		wstaw_standardowy_przycisk(1123, 57, 4, "strt", 0.74, 0.74);
		wstaw_standardowy_przycisk(829, 57, 5, "back", 0.74, 0.74);
		wstaw_pole_liczbowe();
	}
};

class UkladSklepWybor : public Uklad {
private:
	map<string, sf::Sprite>::iterator it_pole_tekstowe;
	int pole_x = 275;
	int pole_y = 170;
	int pos_x = 200;
	int pos_y = 475;
	int map_x = 400;
public:
	UkladSklepWybor(RysowaneObiekty *&rys_o) :Uklad(rys_o) {
		wstaw_tlo(400, WYMIAR_EKRANU_Y, 0, 0, WYMIAR_EKRANU_X);
		wstaw_standardowy_przycisk(1123, 57, 4, "strt", 0.74, 0.74);
		wstaw_standardowy_przycisk(829, 57, 5, "back", 0.74, 0.74);
		wstaw_pole_liczbowe();
	}
	void wstaw_zestaw_przyciskow(int ilosc_przyciskow, string nazwa_przycisku) {
		int przesuniecie = 0;
		if (nazwa_przycisku != "slt") {
			przesuniecie = -5;
		}

		if (ilosc_przyciskow <= 5) {
			for (int i = 0; i < ilosc_przyciskow; i++) {
				wstaw_standardowy_przycisk(pos_x, 45 + 70 * i, 15 + i + przesuniecie, nazwa_przycisku + to_string(i), 0.5, 0.5);
			}
		}
		else {
			for (int i = 0; i < 5; i++) {
				wstaw_standardowy_przycisk(pos_x / 2 + 3, 45 + 70 * i, 15 + i + przesuniecie, nazwa_przycisku + to_string(i), 0.5, 0.5);
			}
			for (int i = 5; i < ilosc_przyciskow; i++) {
				wstaw_standardowy_przycisk(pos_x / 2 + pos_x - 3, 45 + 70 * (i-5), 15 + i + przesuniecie, nazwa_przycisku + to_string(i), 0.5, 0.5);
			}
		}
	}
	void dodaj_pole_tekstowe() {
		sf::Sprite pole_tekstowe = rys_ob->zwroc_sprite_pole_tekstowe_1();
		mapuj_sprite(0, 0, 400, 0, pole_tekstowe);
		pole_tekstowe.setOrigin(sf::Vector2f(pole_x / 2, pole_y / 2));
		ustaw_sprite(pos_x, pos_y, pole_tekstowe);
		it_pole_tekstowe = elementy_w_ukladzie.insert(make_pair(kodowanie_pola_tekstowego, pole_tekstowe)).first;		
	}
	void ustaw_strone_pola_tekstowego(int numer_strony) {
		mapuj_sprite(pole_x, pole_y, 400, pole_y*numer_strony, it_pole_tekstowe->second);
	}
};

class UkladRozgrywka : public Uklad {
	int odstep_lit = 2;
	int wymiar_litery_x = 40;
	int wymiar_litery_y = 40;
	float skala = 0.85;
	vector<map<string, sf::Sprite>::iterator> nazwa_mapy;
	sf::Sprite mapuj_litere(char litera) {	
		sf::Sprite tmp = rys_ob->zwroc_sprite_czcionka();
		int pozycja_na_texturze;
		if (litera != ' ') {
			if (litera - 'a' < 0) {
				pozycja_na_texturze = (litera - 'A')*wymiar_litery_x;
			}
			else {
				pozycja_na_texturze = (litera - 'a')*wymiar_litery_x;
			}
		}
		else {
			// mapowanie dla spacji
			pozycja_na_texturze = 26 * wymiar_litery_x;
		}
		mapuj_sprite(wymiar_litery_x, wymiar_litery_y, pozycja_na_texturze, 0, tmp);
		return tmp;
	}
public:
	void utworz_nazwe_mapy(string nazwa) {
		sf::Sprite tmp;
		for (int i = 0; i < nazwa.size(); i++) {
			tmp = mapuj_litere(nazwa[i]);
			tmp.setOrigin(wymiar_litery_x / 2, wymiar_litery_y / 2);
			skaluj_sprite(skala, skala, tmp);
			if (i == 0) {
				ustaw_sprite((20 + wymiar_litery_x / 2)*skala, (20 + wymiar_litery_y / 2)*skala, tmp);
			}
			else {
				ustaw_sprite(((20 + wymiar_litery_x / 2)*skala)+(wymiar_litery_x + odstep_lit)*i*skala, wymiar_litery_y*skala, tmp);
			}
			auto it_litery = elementy_w_ukladzie.insert(make_pair(kodowanie_liter+to_string(i), tmp)).first;
			nazwa_mapy.push_back(it_litery);
		}
	}
	void dodaj_ramke_interfaceu() {
		sf::Sprite ramka = rys_ob->zwroc_sprite_interface01();
		mapuj_sprite(589, 132, 0, 0, ramka);
		ustaw_sprite(WYMIAR_EKRANU_X - 589, WYMIAR_EKRANU_Y - 132, ramka);
		elementy_w_ukladzie.insert(make_pair(kodowanie_interfaceu + "ramka", ramka));
	}
	void wstaw_przyciski_pulapek(vector<int> typy_pulapek) {
		for (int i = 0; i < typy_pulapek.size(); i++) {
			wstaw_przycisk_akcji(780 + wymiar_p_akcji_x / 2 + 100 * i, 700 - wymiar_p_akcji_y / 2, typy_pulapek[i], "act" + to_string(i));
		}
	}
	void usun_przycisk_pulapki(string nazwa_przycisku) {
		auto znaleziony_element = elementy_w_ukladzie.find(kodowanie_przycisku_akcji + nazwa_przycisku);
		if (znaleziony_element != elementy_w_ukladzie.end()) {
			elementy_w_ukladzie.erase(znaleziony_element);
		}
	}
	UkladRozgrywka(RysowaneObiekty *&rys_o) :Uklad(rys_o) {
		wstaw_standardowy_przycisk(1123, 57, 25, "endg", 0.74, 0.74);
	}
};

class ManagerUkladow {
private:
	vector<Uklad*> zestaw_ukladow;
	void wczytaj_uklady(RysowaneObiekty *&rys_ob) {
		zestaw_ukladow[0] = new UkladMenu(rys_ob);
		zestaw_ukladow[1] = new UkladSklep(rys_ob);
		zestaw_ukladow[2] = new UkladSklepWybor(rys_ob);
		zestaw_ukladow[3] = new UkladSklepWybor(rys_ob);
		zestaw_ukladow[4] = new UkladSklepWybor(rys_ob);
		dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[4])->dodaj_pole_tekstowe();
		zestaw_ukladow[5] = new UkladSklepWybor(rys_ob);
		dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[5])->dodaj_pole_tekstowe();
		zestaw_ukladow[6] = new UkladRozgrywka(rys_ob);
		zestaw_ukladow[7] = new UkladInfo(rys_ob);
	}

public:
	~ManagerUkladow() {
		for (int i = 0; i < ILOSC_UKLADOW; i++) {
			if (zestaw_ukladow[i]) {
				delete zestaw_ukladow[i];
			}
		}
	}
	ManagerUkladow(RysowaneObiekty *&rys_ob) {
		zestaw_ukladow.resize(ILOSC_UKLADOW);
		wczytaj_uklady(rys_ob);
	}
	map<string, sf::Sprite> zwroc_uklad(int index_ukladu) {
		return zestaw_ukladow[index_ukladu]->zwroc_uklad();
	}
	void nacisnij_przycisk_w_ukladzie(string nazwa_przycisku, int index_ukladu) {
		zestaw_ukladow[index_ukladu]->zmien_stan_przycisku(nazwa_przycisku, true);
	}
	void przywroc_przyciski_w_ukladzie(string nazwa_przycisku, int index_ukladu) {
		map<string, sf::Sprite> kopia_elemntow_w_ukladzie = zwroc_uklad(index_ukladu);
		for (auto i = kopia_elemntow_w_ukladzie.begin(); i != kopia_elemntow_w_ukladzie.end(); i++) {
			if ((*i).first.substr(2,4) == "bttn") {
				zestaw_ukladow[index_ukladu]->zmien_stan_przycisku((*i).first.substr(6,(*i).first.size()-6), false);
			}
		}
	}
	void przewin_strone_w_ukladzie(int index_ukladu, bool do_przodu) {
		dynamic_cast<UkladInfo*>(zestaw_ukladow[index_ukladu])->przewin_strone(do_przodu);
	}
	void ustaw_wartosc_pola_liczbowego(int index_ukladu, int wartosc) {
		if (typeid(*zestaw_ukladow[index_ukladu]) == typeid(UkladSklep)) {
			dynamic_cast<UkladSklep*>(zestaw_ukladow[index_ukladu])->ustaw_wartosc_pola_liczbowego(wartosc);
		}
		else if (typeid(*zestaw_ukladow[index_ukladu]) == typeid(UkladSklepWybor)){
			dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[index_ukladu])->ustaw_wartosc_pola_liczbowego(wartosc);
		}
	}
	void wstaw_zestaw_przyciskow_do_ukladu(int index_ukladu, int ilosc) {
		if (index_ukladu == 2 || index_ukladu == 3) {
			dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[index_ukladu])->wstaw_zestaw_przyciskow(ilosc, "slt");
		}
		else{
			dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[index_ukladu])->wstaw_zestaw_przyciskow(ilosc, "typ");
		}
	}
	void wstaw_przyciski_pulapek(vector<int> typy_pulapek) {
		dynamic_cast<UkladRozgrywka*>(zestaw_ukladow[6])->wstaw_przyciski_pulapek(typy_pulapek);
	}
	void ustaw_opis_wiezy_pulapki(int index_ukladu, bool wybrano_wieze, string nazwa_przycisku) {
		if (wybrano_wieze == true) {
			dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[index_ukladu])->ustaw_strone_pola_tekstowego(nazwa_przycisku[3] - '0');
		}
		else {
			dynamic_cast<UkladSklepWybor*>(zestaw_ukladow[index_ukladu])->ustaw_strone_pola_tekstowego(nazwa_przycisku[3] - '0' + 3);
		}
	}
	void wywolaj_okno_startu(bool wywolaj) {
		if (wywolaj == true) {
			zestaw_ukladow[0]->wstaw_okno(2);
		}
		else {
			zestaw_ukladow[0]->usun_okno();
		}
	}
	void wywolaj_okno_wygranej(bool wywolaj) {
		if (wywolaj == true) {
			zestaw_ukladow[6]->wstaw_okno(0);
		}
		else {
			zestaw_ukladow[6]->usun_okno();
		}
	}
	void wywolaj_okno_przegranej(bool wywolaj) {
		if (wywolaj == true) {
			zestaw_ukladow[6]->wstaw_okno(1);
		}
		else {
			zestaw_ukladow[6]->usun_okno();
		}
	}
	void wywolaj_okno_kontynuacji(bool wywolaj) {
		if (wywolaj == true) {
			zestaw_ukladow[0]->wstaw_okno(3,4,5,true);
		}
		else {
			zestaw_ukladow[0]->usun_okno();
		}
	}
	void usun_wybrany_przycisk_akcji(string nazwa_przycisku) {
		dynamic_cast<UkladRozgrywka*>(zestaw_ukladow[6])->usun_przycisk_pulapki(nazwa_przycisku);
	}
	void dodaj_el_interfaceu_rozgrywki(string nazwa_rozgrywki) {
		dynamic_cast<UkladRozgrywka*>(zestaw_ukladow[6])->utworz_nazwe_mapy(nazwa_rozgrywki);
		dynamic_cast<UkladRozgrywka*>(zestaw_ukladow[6])->dodaj_ramke_interfaceu();
	}
};