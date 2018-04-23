#pragma once
#include "Baza.h"

using namespace std;

class Pole_mapy {
private:
	string kod_pola;
public:
	Pole_mapy(string kod){
		kod_pola = kod;
	}

	void wypisz_pole() {
		cout << kod_pola;
	}
};

class Wczytana_mapa {
	friend class Plik_map;
private:
	string nazwa_mapy;
	Pole_mapy *mapa[9][16];
	vector <vector<int>> fale_wrogow;
	vector <int> pola_pulapek;
	int pula_pieniedzy;
	vector <pair<int,int>> sciezka[100];
	vector <pair<int,int>> lokacje_wiez;

	void wczytaj_nazwe(string nazwa) {
		nazwa_mapy = nazwa;
	}

	void wczytaj_pole(int x, int y, string kod_pola) {
		mapa[y][x] = new Pole_mapy{kod_pola};
	}

	void wczytaj_lokacje_wiezy(int x, int y) {
		lokacje_wiez.push_back(make_pair(x,y));
	}

	void wczytaj_pieniadze(int kasa) {
		pula_pieniedzy = kasa;
	}

	void wypisz_mape() {
		cout << endl << nazwa_mapy << endl << endl;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				mapa[i][j]->wypisz_pole(); 
				cout << " ";
			}
			cout << endl;
		}
		cout << endl << "Fale wrogow na tej mapie:" << endl;
		for (auto i = fale_wrogow.begin(); i < fale_wrogow.end(); i++) {
			cout << "Ilosc wrogow: " << i->at(0) << "   Typ wrogow: " << i->at(1) << "   Odstep czasowy: " << i->at(2) << endl;
		}
		cout << endl << "Pulapki na polach:" << endl;
		for (auto i = pola_pulapek.begin(); i < pola_pulapek.end(); i++) {
			cout << *i << " ";
		}
		cout << endl << endl;
		cout << "Zdefiniowana ilosc pieniedzy dla tej mapy: " << pula_pieniedzy << endl;

	}

};

class Plik_map {
private:
	fstream plik_map;
	Wczytana_mapa *mapa_tmp;
	vector <Wczytana_mapa*> mapy_w_grze;

	bool otworz_plik(string sciezka) {
		plik_map.open(sciezka);
		if (!plik_map.good()) {
			return false;
		}
	}

	string czytaj_slowo() {
		string slowo_tmp;
		plik_map >> slowo_tmp;
		plik_map.ignore();
		return slowo_tmp;
	}

	bool inst_level() {
		if (czytaj_slowo() == "level:") {
			mapa_tmp = new Wczytana_mapa;
			string nazwa_tmp;
			getline(plik_map, nazwa_tmp);
			mapa_tmp->wczytaj_nazwe(nazwa_tmp);
			return true;
		}
		return false;
	}

	bool inst_map() {
		if (czytaj_slowo() == "map:") {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 16; j++) {
					string slowo_tmp;
					slowo_tmp = czytaj_slowo();
					if (slowo_tmp == "XX" || slowo_tmp == "QQ" || slowo_tmp == "@@") {
						mapa_tmp->wczytaj_pole(j, i, slowo_tmp);
					}
					else if (slowo_tmp[0] == '$' && slowo_tmp[1] - 48 >= 0 && slowo_tmp[1] - 48 <= 9 && slowo_tmp.length() == 2) {
						mapa_tmp->wczytaj_pole(j, i, slowo_tmp);
						mapa_tmp->wczytaj_lokacje_wiezy(j, i);
					}
					else if (slowo_tmp[0] - 48 >= 0 && slowo_tmp[0] - 48 <= 9 && slowo_tmp[1] - 48 >= 0 && slowo_tmp[1] - 48 <= 9 && slowo_tmp.length() == 2) {
						mapa_tmp->wczytaj_pole(j, i, slowo_tmp);
						int liczba_tmp = stoi(slowo_tmp);
						mapa_tmp->sciezka[liczba_tmp] = { make_pair(j,i) };
					}
					else {
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	bool inst_foe() {
		string slowo_tmp;
		int liczba_tmp;		
		int licznik = 0;
		if (czytaj_slowo() == "foe:") {
			for (int i = 0; i < ZDEFINIOWANA_ILOSC_FAL_WROGOW; i++) {
				slowo_tmp = czytaj_slowo();
				if (slowo_tmp == "empty") {
					continue;
				}
				else {
					vector<int> fala_tmp;
					if (stoi(slowo_tmp)) {
						liczba_tmp = stoi(slowo_tmp);
						if (liczba_tmp > 0 && liczba_tmp <= MAX_ILOSC_WROGOW_W_FALI) {
							fala_tmp.push_back(liczba_tmp);
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
					slowo_tmp = czytaj_slowo();
					if (stoi(slowo_tmp)) {
						liczba_tmp = stoi(slowo_tmp);
						if (liczba_tmp >= 0 && liczba_tmp < ILOSC_TYPOW_WROGOW) {
							fala_tmp.push_back(liczba_tmp);
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
					slowo_tmp = czytaj_slowo();
					if (stoi(slowo_tmp)) {
						liczba_tmp = stoi(slowo_tmp);
						if (liczba_tmp >= 0 && liczba_tmp <= MAX_ODSTEP_CZASOWY_FAL) {
							fala_tmp.push_back(liczba_tmp);
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
					mapa_tmp->fale_wrogow.push_back(fala_tmp);

				}
			}
			return true;
		}
		return false;
	}

	bool inst_traps() {
		string slowo_tmp;
		int liczba_tmp;
		if (czytaj_slowo() == "traps:") {
			for (int i = 0; i < ZDEFINIOWANA_ILOSC_PULAPEK; i++) {
				slowo_tmp = czytaj_slowo();
				if (slowo_tmp == "empty") {
					continue;
				}
				else {
					if (stoi(slowo_tmp)) {
						liczba_tmp = stoi(slowo_tmp);
						if (!mapa_tmp->sciezka[liczba_tmp].empty()) {
							mapa_tmp->pola_pulapek.push_back(liczba_tmp);
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
			}
			return true;
		}
		return false;
	
	}

	bool inst_money() {
		if (czytaj_slowo() == "money:") {
			string slowo_tmp = czytaj_slowo();
			int liczba_tmp;
			if (stoi(slowo_tmp)) {
				liczba_tmp = stoi(slowo_tmp);
				mapa_tmp->pula_pieniedzy = liczba_tmp;
			}
			else {
				return false;
			}
			return true;
		}
		return false;
	}

	bool czytaj_plik() {
		while (!plik_map.eof()) {
			if (!inst_level()) {
				return false;
			}
			if (!inst_map()) {
				return false;
			}
			if (!inst_foe()) {
				return false;
			}
			if (!inst_traps()) {
				return false;
			}
			if (!inst_money()) {
				return false;
			}
			mapy_w_grze.push_back(mapa_tmp);
			mapa_tmp->wypisz_mape();
		}
	}

public:
	bool blad_pliku_map = false;

	Plik_map() {
		otworz_plik(SCIEZKA_PLIKU_MAP);
		if (!czytaj_plik()) {
			blad_pliku_map = true;
		}

	}

	~Plik_map() {
		delete(mapa_tmp);
	}
};
