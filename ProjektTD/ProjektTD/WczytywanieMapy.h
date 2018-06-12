#pragma once
#include "Baza.h"
#include "RysowaneObiekty.h"

using namespace std;

class Pole_mapy {
	friend class Wczytana_mapa;
private:
	int x;
	int y;
	string kod_pola;
	sf::Sprite sprite_pola;
	void mapuj_pole_wg_kodu(RysowaneObiekty *rysowane_obiekty) {
		sprite_pola = rysowane_obiekty->zwroc_sprite_pole();
		if (kod_pola == "QQ") {
			// mapuj pole wolne
			sprite_pola.setTextureRect(sf::IntRect(0, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
		}
		else if (kod_pola == "XX") {
			// mapuj pole zabronione
			sprite_pola.setTextureRect(sf::IntRect(160, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
		}
		else if (kod_pola == "@@") {
			// mapuj pole bariery
			sprite_pola.setTextureRect(sf::IntRect(400, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
		}
		else {
			// mapuj pola sciezki
			sprite_pola.setTextureRect(sf::IntRect(240, 0, WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
		}
	}
	void ustaw_pole(int x, int y) {
		sprite_pola.setPosition(sf::Vector2f(x*WYMIAR_POLA_GRY, y*WYMIAR_POLA_GRY));
	}
public:
	sf::Sprite zwroc_sprite_pola() {
		return sprite_pola;
	}
	void wypisz_pole() {
		cout << kod_pola;
	}

	Pole_mapy(string kod, RysowaneObiekty *&rysowane_obiekty, int pos_x, int pos_y){
		kod_pola = kod;
		x = pos_x;
		y = pos_y;
		mapuj_pole_wg_kodu(rysowane_obiekty);
		ustaw_pole(pos_x, pos_y);
	}
};

class Wczytana_mapa {
	friend class Plik_map;
private:
	string kodowanie_pola = "A pole"; // kodowanie pola okresla warstwe na ktorej pola beda renderowane
	string nazwa_mapy;
	Pole_mapy *mapa[9][16];
	vector <vector<int>> fale_wrogow;
	vector <int> pola_pulapek;
	int pula_pieniedzy;
	vector <pair<int,int>> sciezka;
	vector <pair<int,int>> lokacje_wiez;

	void wczytaj_nazwe(string nazwa) {
		nazwa_mapy = nazwa;
	}
	void wczytaj_pole(int x, int y, string kod_pola, RysowaneObiekty *&rys_ob) {
		mapa[y][x] = new Pole_mapy{kod_pola, rys_ob, x, y };
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
	void oznacz_pola_wiez_pulapek() {
		// modyfikuje maping dla pol wiez i pulapek 
		// tak by ich oznaczenia sie zgadzaly

		// przed modyfikowaniem mappingu nalezy posortowac wieze i pulapki od lewej do prawej na mapie
		sort(lokacje_wiez.begin(), lokacje_wiez.end());
		sort(pola_pulapek.begin(), pola_pulapek.end());

		int licznik = 0;
		for (auto i = pola_pulapek.begin(); i != pola_pulapek.end(); i++) {
			mapa[sciezka[*i].second][sciezka[*i].first]->sprite_pola.setTextureRect(sf::IntRect(0 + (licznik*WYMIAR_POLA_GRY), 160,
				WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
			licznik++;
		}
		licznik = 0;
		for (auto i = lokacje_wiez.begin(); i != lokacje_wiez.end(); i++) {
			mapa[i->second][i->first]->sprite_pola.setTextureRect(sf::IntRect(0 + (licznik*WYMIAR_POLA_GRY), 80,
				WYMIAR_POLA_GRY, WYMIAR_POLA_GRY));
			licznik++;
		}
	}
	
public:
	map<string, sf::Sprite> zwroc_pola_na_mapie() {
		oznacz_pola_wiez_pulapek();
		map<string, sf::Sprite> map_tmp;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				map_tmp.insert(make_pair(kodowanie_pola + to_string(j) + to_string(i), mapa[i][j]->zwroc_sprite_pola()));
			}	
		}
		return map_tmp;
	}

	~Wczytana_mapa() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				delete mapa[i][j];
			}
		}
	}
};

class Plik_map {
private:
	fstream plik_map;
	Wczytana_mapa *mapa_tmp;
	vector <Wczytana_mapa*> mapy_w_grze;
	RysowaneObiekty *rysowane_obiekty;

	bool otworz_plik(string sciezka) {
		plik_map.open(sciezka);
		if (!plik_map.good()) {
			return false;
		}
		return true;
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
						mapa_tmp->wczytaj_pole(j, i, slowo_tmp, rysowane_obiekty);
					}
					else if (slowo_tmp[0] == '$' && slowo_tmp[1] - '0' >= 0 && slowo_tmp[1] - '0' <= 9 && slowo_tmp.length() == 2) {
						mapa_tmp->wczytaj_pole(j, i, slowo_tmp, rysowane_obiekty);
						mapa_tmp->wczytaj_lokacje_wiezy(j, i);
					}
					else if (slowo_tmp[0] - '0' >= 0 && slowo_tmp[0] - '0' <= 9 && slowo_tmp[1] - '0' >= 0 && slowo_tmp[1] - '0' <= 9 && slowo_tmp.length() == 2) {
						mapa_tmp->wczytaj_pole(j, i, slowo_tmp, rysowane_obiekty);
						int liczba_tmp = stoi(slowo_tmp);
						if (liczba_tmp + 1 > mapa_tmp->sciezka.size()) {
							mapa_tmp->sciezka.resize(liczba_tmp + 1);
						}
						mapa_tmp->sciezka[liczba_tmp] = make_pair(j,i);
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

					// ---------- wczytywanie ilosci
					try {
						stoi(slowo_tmp);
					}
					catch (invalid_argument) {
						// nie mozna dokonac konwersji
						return false;
					}
					liczba_tmp = stoi(slowo_tmp);
					if (liczba_tmp > 0 && liczba_tmp <= MAX_ILOSC_WROGOW_W_FALI) {
						fala_tmp.push_back(liczba_tmp);
					}
					else {
						return false;
					}

					// ----------

					// ---------- wczytywanie typu

					slowo_tmp = czytaj_slowo();					
					try {
						stoi(slowo_tmp);
					}
					catch (invalid_argument) {
						// nie mozna dokonac konwersji
						return false;
					}
					liczba_tmp = stoi(slowo_tmp);
					if (liczba_tmp >= 0 && liczba_tmp < ILOSC_TYPOW_WROGOW) {
						fala_tmp.push_back(liczba_tmp);
					}
					else {
						return false;
					}
					// ----------

					// ---------- wczytywanie czasu

					slowo_tmp = czytaj_slowo();
					try {
						stoi(slowo_tmp);
					}
					catch (invalid_argument) {
						// nie mozna dokonac konwersji
						return false;
					}
					liczba_tmp = stoi(slowo_tmp);
					if (liczba_tmp >= 0 && liczba_tmp <= MAX_ODSTEP_CZASOWY_FAL) {
						fala_tmp.push_back(liczba_tmp);
					}
					else {
						return false;
					}

					// ----------

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
						if (mapa_tmp->sciezka[liczba_tmp].first && mapa_tmp->sciezka[liczba_tmp].second) {
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
		return true;
	}

public:
	bool blad_pliku_map = false;
	
	Wczytana_mapa* zwroc_mape(int index_mapy) {
		if (mapy_w_grze.size() > index_mapy) {
			return mapy_w_grze[index_mapy];
		}
		else {
			return nullptr;
		}
	}
	vector<pair<int,int>> zwroc_wieze_na_mapie(int index_mapy) {
		return mapy_w_grze[index_mapy]->lokacje_wiez;
	}
	vector<pair<int, int>> zwroc_pulapki_na_mapie(int index_mapy) {
		vector<pair<int, int>> tmp;
		for (auto i = mapy_w_grze[index_mapy]->pola_pulapek.begin(); i != mapy_w_grze[index_mapy]->pola_pulapek.end(); i++) {
			tmp.push_back(mapy_w_grze[index_mapy]->sciezka[*i]);
		}
		return tmp;
	}
	vector<pair<int, int>> zwroc_sciezke(int index_mapy) {
		return mapy_w_grze[index_mapy]->sciezka;
	}
	int zwroc_pule_pieniedzy(int index_mapy) {
		return mapy_w_grze[index_mapy]->pula_pieniedzy;
	}
	vector<vector<int>> zwroc_fale_wrogow(int index_mapy) {
		return mapy_w_grze[index_mapy]->fale_wrogow;
	}
	string zwroc_nazwe_mapy(int index_mapy) {
		return mapy_w_grze[index_mapy]->nazwa_mapy;
	}
	
	Plik_map(RysowaneObiekty *&rys_ob) {
		rysowane_obiekty = rys_ob;
		otworz_plik(SCIEZKA_PLIKU_MAP);
		if (!czytaj_plik()) {
			blad_pliku_map = true;
		}

	}
	~Plik_map() {
		for (auto i = mapy_w_grze.begin(); i != mapy_w_grze.end(); i++) {
			if ((*i) != nullptr) {
				delete (*i);
			}
		}
	}
};
