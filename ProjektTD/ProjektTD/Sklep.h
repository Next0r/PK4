#pragma once
#include "Baza.h"
#include "ObiektyAktywne.h"

using namespace std;

class Sprzedawany {
private:
	string kod;
	int cena;
public:
	int zwroc_cene_towaru() {
		return cena;
	}

	Sprzedawany(string k, int c) {
		kod = k;
		cena = c;
	}
};

class ManagerSklepu {
private:
	ManagerObiektowAktywnych *manager_ob_akt;
	Plik_map* mapy;
	int pula_pieniedzy;
	vector<Sprzedawany*> zestaw_sprzedawanych_wiez;
	vector<Sprzedawany*> zestaw_sprzedawanych_pulapek;
	void utworz_sprzedawane_pulapki() {
		zestaw_sprzedawanych_pulapek.push_back(new Sprzedawany("Pulapka1", 100));
		zestaw_sprzedawanych_pulapek.push_back(new Sprzedawany("Pulapka2", 150));
		zestaw_sprzedawanych_pulapek.push_back(new Sprzedawany("Pulapka3", 300));
	}
	void utworz_sprzedawane_wieze() {
		zestaw_sprzedawanych_wiez.push_back(new Sprzedawany("Wieza1", 200));
		zestaw_sprzedawanych_wiez.push_back(new Sprzedawany("Wieza2", 400));
		zestaw_sprzedawanych_wiez.push_back(new Sprzedawany("Wieza3", 700));
	}

public:
	void wykonaj_zakup() {
		int typ_kupowany = manager_ob_akt->zwroc_typ_obiektu();
		int typ_ob_na_pozycji = manager_ob_akt->czy_slot_jest_zajety();
		bool kupowana_wieza = manager_ob_akt->czy_wybrano_wieze();
		int cena_zakupu;
		int cena_sprzedarzy;

		// wyznacznie ceny zakupu
		if (kupowana_wieza == true) {
			cena_zakupu = zestaw_sprzedawanych_wiez[typ_kupowany]->zwroc_cene_towaru();
		}
		else {
			cena_zakupu = zestaw_sprzedawanych_pulapek[typ_kupowany]->zwroc_cene_towaru();
		}

		if (typ_ob_na_pozycji == -1) {
			// pole jest puste - mozna dalej testowac
			if (pula_pieniedzy - cena_zakupu >= 0) {
				// jesli mozna wykonac zakup
				pula_pieniedzy -= cena_zakupu;
				manager_ob_akt->dodaj_obiekt();
			}
		}
		else {
			// pole jest zajete - nalezy sprzedac obiekt
			// ustalanie ceny sprzedarzy
			if (kupowana_wieza == true) {
				cena_sprzedarzy = zestaw_sprzedawanych_wiez[typ_ob_na_pozycji]->zwroc_cene_towaru();
			}
			else {
				cena_sprzedarzy = zestaw_sprzedawanych_pulapek[typ_ob_na_pozycji]->zwroc_cene_towaru();
			}

			pula_pieniedzy += cena_sprzedarzy;
			manager_ob_akt->usun_obiekt();
			// po sprzedarzy sprawdz czy mozna kupic wybrana wieze o ile wybrano inna
			if (typ_kupowany != typ_ob_na_pozycji) {
				wykonaj_zakup();
			}
		}
	}
	int zwroc_pule_pieniedzy() {
		return pula_pieniedzy;
	}
	
	ManagerSklepu(ManagerObiektowAktywnych *&mgr, Plik_map *&wczytane_mapy, int aktulana_mapa) {
		manager_ob_akt = mgr;
		mapy = wczytane_mapy;
		pula_pieniedzy = mapy->zwroc_pule_pieniedzy(aktulana_mapa);
		utworz_sprzedawane_wieze();
		utworz_sprzedawane_pulapki();
	}
	~ManagerSklepu() {
		for (int i = 0; i < zestaw_sprzedawanych_wiez.size(); i++) {
			if (zestaw_sprzedawanych_wiez[i] != nullptr) {
				delete zestaw_sprzedawanych_wiez[i];
			}
		}
		for (int i = 0; i < zestaw_sprzedawanych_pulapek.size(); i++) {
			if (zestaw_sprzedawanych_pulapek[i] != nullptr) {
				delete zestaw_sprzedawanych_pulapek[i];
			}
		}
	}
};
