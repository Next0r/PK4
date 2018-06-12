#include "stdafx.h"
#include "Baza.h"
#include "WczytywanieMapy.h"
#include "RysowaneObiekty.h"
#include "Uklady.h"
#include "Sklep.h"
#include "ObiektyAktywne.h"

// testowanie wyciekow pamieci
#include <vld.h>


using namespace std;

class TranslatorWejscia {
private:
	vector<int> blokowane_wejscia;
	map<int, int> mapa_wejsc =
	{
		// para - wejscie event.key.code i kod_klawisza z mapy ukladow	
		{ 27,0 },	// 1
		{ 28,1 },	// 2
		{ 29,2 },	// 3
		{ 30,3 },	// 4
		{ 31,4 },	// 5
		{ 32,5 },	// 6
		{ 33,6 },	// 7
		{ 34,7 },	// 8
		{ 35,8 },	// 9
		{ 26,9 },	// 0
		{ 18,10 },	// S
		{ 4,11 },	// E
		{ 1,12 },	// B
	};

public:
	int tlumacz_wejscie(int klucz) {
		if (mapa_wejsc.find(klucz) != mapa_wejsc.end()) {
			for (int i = 0; i < blokowane_wejscia.size(); i++) {
				if (mapa_wejsc.find(klucz)->second == blokowane_wejscia[i]) {
					return -1;
				}
			}
			return mapa_wejsc.find(klucz)->second;
		}
		else {
			return -1;
		}
	}
	void blokuj_wejscia(vector<int> zablokowane) {
		blokowane_wejscia = zablokowane;
	}
	void odblokuj_wejscia() {
		blokowane_wejscia.clear();
	}
};

class WejscieWyjscie {
private:
	ifstream plik_odczytu;
	ofstream plik_zapisu;
	string kodowanie_zapisu = "LastSavedLevel: ";
	char znak_limitujacy = ':';

	string nazwa_obslugiwanego_przycisku;

	int aktualna_mapa = 0;
	int aktualny_uklad = 0;
	bool okno_startu = false;
	bool okno_kontynuacji = false;
	bool okno_wygranej = false;
	bool okno_przegranej = false;

	map<string, sf::Sprite> mapa_gry;

	TranslatorWejscia *translator_wejscia;
	ManagerUkladow *manager_ukladow;
	Plik_map *mapy;
	ManagerObiektowAktywnych *mgr_ob_akt;
	ManagerSklepu *manager_sklepu;
	RysowaneObiekty *rys_ob;

	pair<int, string> mapa_ukladow[ILOSC_KLAWISZY][ILOSC_UKLADOW] =
	{
		//    menu        wybor ob       slot wiezy     slot trap      typ wiezy       typ trap       rozgrywka        info
		{ {  1,"strt" }, { 5,"turr" }, {  13,"slt0" }, { 14,"slt0" }, { 15,"typ0" }, { 15,"typ0" }, { 16,"act0" }, {  9,"prev" } },
		{ {  2,"cont" }, { 6,"trap" }, {  13,"slt1" }, { 14,"slt1" }, { 15,"typ1" }, { 15,"typ1" }, { 16,"act1" }, { 10,"next" } },
		{ {  3,"info" }, { 0,"null" }, {  13,"slt2" }, { 14,"slt2" }, { 15,"typ2" }, { 15,"typ2" }, { 16,"act2" }, {  0,"null" } },
		{ {  4,"quit" }, { 0,"null" }, {  13,"slt3" }, { 14,"slt3" }, {  0,"null" }, {  0,"null" }, { 16,"act3" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {  13,"slt4" }, { 14,"slt4" }, {  0,"null" }, {  0,"null" }, { 16,"act4" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {  13,"slt5" }, { 14,"slt5" }, {  0,"null" }, {  0,"null" }, {  0,"null" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {  13,"slt6" }, { 14,"slt6" }, {  0,"null" }, {  0,"null" }, {  0,"null" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {  13,"slt7" }, { 14,"slt7" }, {  0,"null" }, {  0,"null" }, {  0,"null" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {  13,"slt8" }, { 14,"slt8" }, {  0,"null" }, {  0,"null" }, {  0,"null" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {  13,"slt9" }, { 14,"slt9" }, {  0,"null" }, {  0,"null" }, {  0,"null" }, {  0,"null" } },
		{ { -1,"conf" }, { 7,"strt" }, {   7,"strt" }, {  7,"strt" }, {  7,"strt" }, {  7,"strt" }, { 17,"conf" }, {  0,"null" } },
		{ {  0,"null" }, { 0,"null" }, {   0,"null" }, {  0,"null" }, {  0,"null" }, {  0,"null" }, {  8,"endg" }, {  0,"null" } },
		{ { -1,"rect" }, { 8,"back" }, {  12,"back" }, { 12,"back" }, {  5,"back" }, {  6,"back" }, { 17,"rect" }, { 11,"back" } },
	};

	int wykonaj_odczyt_poziomu() {
		string poziom;
		plik_odczytu.open(SCIEZKA_PLIKU_ZAPISU);
		if (plik_odczytu.good()) {
			getline(plik_odczytu, poziom, znak_limitujacy);
			plik_odczytu >> poziom;
			plik_odczytu.close();
			return stoi(poziom);
		}
		plik_odczytu.close();
		return 0;
	}
	void wykonaj_akcje(int nr_akcji, string nazwa_przycisku, int &flaga_zd_spec) {
		if (nr_akcji == -1) {
			if (okno_startu == true) {
				nr_akcji = 1;
			}
			else if (okno_kontynuacji == true) {
				nr_akcji = 2;
			}	
		}
		switch (nr_akcji){
		case 1:
			if (okno_startu == false && nazwa_przycisku != "conf" && nazwa_przycisku != "rect") {
				translator_wejscia->blokuj_wejscia(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11});
				okno_startu = true;
				manager_ukladow->wywolaj_okno_startu(true);
			}
			else if(okno_startu == true){
				if (nazwa_przycisku == "conf") {
					aktualny_uklad = 1;	// przniesienie do ukladu sklepu
					aktualna_mapa = 0;	// wczytanie mapy 0 - rozpoczecie nowej gry
					flaga_zd_spec = 2;	// reset obiektow rozgrywki	
					wykonaj_zapis_poziomu(aktualna_mapa);
					okno_startu = false;
					manager_ukladow->wywolaj_okno_startu(false);
					translator_wejscia->odblokuj_wejscia();
				}
				else {
					okno_startu = false;
					manager_ukladow->wywolaj_okno_startu(false);
					translator_wejscia->odblokuj_wejscia();
				}
			}
			break;
		case 2:		
			aktualna_mapa = wykonaj_odczyt_poziomu();
			if (aktualna_mapa == 0) {
				if (okno_kontynuacji == false) {
					okno_kontynuacji = true;
					translator_wejscia->blokuj_wejscia(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
					manager_ukladow->wywolaj_okno_kontynuacji(true);
				}
				else {
					okno_kontynuacji = false;
					translator_wejscia->odblokuj_wejscia();
					manager_ukladow->wywolaj_okno_kontynuacji(false);
				}
			}
			else {
				aktualny_uklad = 1;	// przniesienie do ukladu sklepu
				flaga_zd_spec = 2;	// reset obiektow rozgrywki
			}
			break;
		case 3:
			aktualny_uklad = 7; // przniesienie do ukladu info
			break;
		case 4:
			flaga_zd_spec = 1;	// wyjscie z gry
			break;
		case 5:
			aktualny_uklad = 2;	// przniesienie do ukladu wyboru slotu na wieze
			mgr_ob_akt->wybrano_wieze(true); // wybrano wieze
			manager_ukladow->wstaw_zestaw_przyciskow_do_ukladu(aktualny_uklad, mapy->zwroc_wieze_na_mapie(aktualna_mapa).size());
			break;
		case 6: 
			aktualny_uklad = 3;	// przeniesienie do ukladu wyboru slotu na pulapke
			mgr_ob_akt->wybrano_wieze(false); // wybrano pulapke
			manager_ukladow->wstaw_zestaw_przyciskow_do_ukladu(aktualny_uklad, mapy->zwroc_pulapki_na_mapie(aktualna_mapa).size());
			break;
		case 7:
			aktualny_uklad = 6; // przniesienie do ukladu rozgrywki
			manager_ukladow->wstaw_przyciski_pulapek(mgr_ob_akt->zwroc_typy_ustawionych_pulapek());
			manager_ukladow->dodaj_el_interfaceu_rozgrywki(mapy->zwroc_nazwe_mapy(aktualna_mapa));
			flaga_zd_spec = 3; // rozpoczecie generowania wrogow jest zdarzeniem specjalnym
			break;
		case 8: 
			aktualny_uklad = 0;	// przeniesienie do ukladu menu glownego
			break;
		case 9:
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, false); 
			break;
		case 10:
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, true);
			break;
		case 11:
			aktualny_uklad = 0; // przeniesienie do ukladu menu glownego
			break;
		case 12:
			aktualny_uklad = 1; // przniesienie do ukladu bazowego sklepu
			break;
		case 13:
			aktualny_uklad = 4;
			manager_ukladow->wstaw_zestaw_przyciskow_do_ukladu(aktualny_uklad, ILOSC_TYPOW_WIEZ);
			mgr_ob_akt->pobierz_kod_przycisku(nazwa_przycisku);	
			break;
		case 14:
			aktualny_uklad = 5;
			manager_ukladow->wstaw_zestaw_przyciskow_do_ukladu(aktualny_uklad, ILOSC_TYPOW_PULAPEK);
			mgr_ob_akt->pobierz_kod_przycisku(nazwa_przycisku);
			break;
		case 15:
			manager_ukladow->ustaw_opis_wiezy_pulapki(aktualny_uklad, mgr_ob_akt->czy_wybrano_wieze(), nazwa_przycisku);
			mgr_ob_akt->pobierz_kod_przycisku(nazwa_przycisku);
			manager_sklepu->wykonaj_zakup();
			break;
		case 16:
			mgr_ob_akt->pobierz_kod_przycisku(nazwa_przycisku);
			if (mgr_ob_akt->uzyj_pulapki() == true) {
				// proba wykorzystania pulapki powiodla sie
				manager_ukladow->usun_wybrany_przycisk_akcji(nazwa_przycisku);
			}
			break;
		case 17:
			if (okno_wygranej == true) {
				if (nazwa_przycisku == "conf") {
					aktualny_uklad = 1;	// przniesienie do ukladu sklepu
					aktualna_mapa = wykonaj_odczyt_poziomu();
				}
				else {
					aktualny_uklad = 0;	// przeniesienie do ukladu menu glownego
				}
				// zamykanie okna
				okno_wygranej = false;
				manager_ukladow->wywolaj_okno_wygranej(false);
				translator_wejscia->odblokuj_wejscia();
			}
			else if (okno_przegranej == true) {
				if (nazwa_przycisku == "conf") {
					aktualny_uklad = 1;	// przniesienie do ukladu sklepu
				}
				else {
					aktualny_uklad = 0;	// przeniesienie do ukladu menu glownego
				}
				// zamykanie okna
				okno_przegranej = false;
				manager_ukladow->wywolaj_okno_przegranej(false);
				translator_wejscia->odblokuj_wejscia();
			}
			flaga_zd_spec = 2;	// reset obiektow rozgrywki	
		default:
			break;
		}
	}
	map<string, sf::Sprite> zbuduj_warstwe_renderowania() {
		if (aktualny_uklad != 0 && aktualny_uklad != 7) {
			manager_ukladow->ustaw_wartosc_pola_liczbowego(aktualny_uklad, manager_sklepu->zwroc_pule_pieniedzy());
			map<string, sf::Sprite> map_tmp = mapa_gry;
			map<string, sf::Sprite> map_tmp2 = manager_ukladow->zwroc_uklad(aktualny_uklad);
			map<string, sf::Sprite> map_tmp3 = mgr_ob_akt->zwroc_spritey_obiektow();
			map_tmp.insert(map_tmp2.begin(), map_tmp2.end());
			map_tmp.insert(map_tmp3.begin(), map_tmp3.end());
			return map_tmp;
		}
		else {
			// uklad nie zawiera mapy jako tla
			return manager_ukladow->zwroc_uklad(aktualny_uklad);
		}
	
	}
public:
	WejscieWyjscie(ManagerUkladow *&mgr_ukladow, Plik_map *&plik_map, RysowaneObiekty *&rys_o) {
		manager_ukladow = mgr_ukladow;
		mapy = plik_map;
		rys_ob = rys_o;
		mgr_ob_akt = new ManagerObiektowAktywnych(mapy, aktualna_mapa, rys_ob);
		manager_sklepu = new ManagerSklepu(mgr_ob_akt, mapy, aktualna_mapa);
		translator_wejscia = new TranslatorWejscia();
		mapa_gry = mapy->zwroc_mape(aktualna_mapa)->zwroc_pola_na_mapie();
	}
	~WejscieWyjscie() {
		delete translator_wejscia;
		delete manager_ukladow;
		delete mapy;
		delete mgr_ob_akt;
		delete manager_sklepu;
		delete rys_ob;
	}
	void odswiez_mape_gry() {
		mapa_gry = mapy->zwroc_mape(aktualna_mapa)->zwroc_pola_na_mapie();
	}
	void odswiez_aniamcje(int &timer, map<string,sf::Sprite> &warstwa_renderowania) {
		if (timer < CZESTOTLIWOSC_ODSWIEZANIA_ANIM) {
			timer++;
		}
		else {
			// odswiez
			// --------- animacja wrogow
			mgr_ob_akt->animuj_wrogow();
			mgr_ob_akt->animuj_usuwaj_efekty();
			warstwa_renderowania = zbuduj_warstwe_renderowania();
			// resetuj timer
			timer = 0;
		}	
	}
	void generuj_wrogow(bool &flaga_generacji, int &licznik1, int &licznik2) {
		mgr_ob_akt->tworz_fale_wrogow(flaga_generacji, licznik1, licznik2);
	}
	void odswiez_przemieszczenie_atak(int &timer, map<string, sf::Sprite> &warstwa_renderowania, bool &flaga_zatrzymania_gry) {
		if (timer < CZESTOTLIWOSC_ODSWIEZANIA_RUCH) {
			timer++;
		}
		else {
			// odswiez
			mgr_ob_akt->odswiez_pozycje_na_sciezce_wrogow();
			flaga_zatrzymania_gry = mgr_ob_akt->wrogowie_na_koncu();	// decyduje o zakonczeniu gry
			mgr_ob_akt->ustal_wektory_przemieszczenia_i_rotacje_wrogow();
			mgr_ob_akt->przemiesc_wrogow_synchronizuj_ramki();	

			// ---------- realizowanie ataku

			mgr_ob_akt->przeladuj_wieze();

			// --------- animacja wiezy (obrot)
			mgr_ob_akt->animuj_wyceluj_wieze();
			mgr_ob_akt->wykonaj_atak_wiezami();

			warstwa_renderowania = zbuduj_warstwe_renderowania();
			// resetuj timer
			timer = 0;
		}
	}
	void reset_odswiez(ManagerUkladow *&mgr_ukladow, Plik_map *&plik_map, RysowaneObiekty *&rys_ob, map<string,sf::Sprite> &warstwa_renderowania) {
		delete mgr_ob_akt;
		delete manager_sklepu;
		manager_ukladow = mgr_ukladow;
		mgr_ob_akt = new ManagerObiektowAktywnych(mapy, aktualna_mapa, rys_ob);
		manager_sklepu = new ManagerSklepu(mgr_ob_akt, mapy, aktualna_mapa);
		odswiez_mape_gry();
		warstwa_renderowania = zbuduj_warstwe_renderowania();
	}
	void przyjmij_wejscie(sf::Event ev1, map<string, sf::Sprite> &warstwa_renderowania, int &flaga_zd_spec) {
		if (ev1.type == sf::Event::KeyPressed || ev1.type == sf::Event::KeyReleased) {
			int kod_klawisza = translator_wejscia->tlumacz_wejscie(ev1.key.code);
			if (kod_klawisza != -1) {
				if (ev1.type == sf::Event::KeyPressed) {
					// nacisnieto przycisk
					string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;
					// zapamietaj nacisniety przycisk
					nazwa_obslugiwanego_przycisku = nazwa_przycisku;
					manager_ukladow->nacisnij_przycisk_w_ukladzie(nazwa_przycisku, aktualny_uklad);
					// zwraca uklad
					warstwa_renderowania = zbuduj_warstwe_renderowania();
				}
				else {
					// puszczono przycisk	
					string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;
					manager_ukladow->przywroc_przyciski_w_ukladzie(nazwa_przycisku, aktualny_uklad);
					if (nazwa_obslugiwanego_przycisku == nazwa_przycisku) {
						// zapobiega wykonywania nadmiernych operacji przy 2 przyciskach nacisnietych
						// podajemowanie akcji
						int nr_akcji = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
						wykonaj_akcje(nr_akcji, nazwa_przycisku, flaga_zd_spec);				
					}
					warstwa_renderowania = zbuduj_warstwe_renderowania();
				}
			}	
		}
	}
	bool uklad_rozgrywki() {
		if (aktualny_uklad == 6) {
			return true;
		}
		else {
			return false;
		}
	}
	bool nie_ma_wrogow_na_mapie() {
		return mgr_ob_akt->nie_ma_wrogow_na_mapie();
	}
	void wywolaj_okno_wygranej_przegranej(bool wygrana, map<string, sf::Sprite> &warstwa_renderowania) {
		translator_wejscia->blokuj_wejscia(vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11});
		if (wygrana == true) {
			okno_wygranej = true;
			manager_ukladow->wywolaj_okno_wygranej(true);
			warstwa_renderowania = zbuduj_warstwe_renderowania();

		}
		else {
			okno_przegranej = true;
			manager_ukladow->wywolaj_okno_przegranej(true);	
			warstwa_renderowania = zbuduj_warstwe_renderowania();
		}
	}
	int zwroc_kolejna_mape() { // zwaraca aktualna jesli nastepna nie istneieje
		if (mapy->zwroc_mape(aktualna_mapa + 1) != nullptr) {
			return aktualna_mapa + 1;
		}
		else {
			return aktualna_mapa;
		}
	}
	void wykonaj_zapis_poziomu(int nr_poziomu) {
		plik_zapisu.open(SCIEZKA_PLIKU_ZAPISU);
		if (plik_zapisu.good()) {
			plik_zapisu << kodowanie_zapisu + to_string(nr_poziomu);
		}
		plik_zapisu.close();
	}
};

int main() {
	int flaga_zdarzenia_specjalnego = 0; // 1 - zamyka okno 2 - resetuje gre i buduje nowy render
	int licznik_klatek1 = 0;
	int licznik_klatek2 = 0;
	int licznik_klatek3 = 0; 
	int licznik_klatek4 = 0;

	bool flaga_zatrzymania_gry = false; // dla wartosci true zatrzymuje gre
	bool flaga_generacji_wrogow = true; // false oznacza, ze wrogowie w fali sie skonczyli
										// jesli wrogowie sie skonczyli i zaden nie znajduje sie na mapie gracz wygral

	// obiekty pasywne
	RysowaneObiekty *baza_obiektow = new RysowaneObiekty();
	Plik_map *mapy = new Plik_map(baza_obiektow);

	// test zwracanych bledow
	if (mapy->blad_pliku_map) {
		cout << "Blad wczytywania pliku map." << endl;
		delete mapy;
		delete baza_obiektow;
		return 0;
	}

	// obiekty aktywne
	ManagerUkladow *manager_ukladow = new ManagerUkladow(baza_obiektow);
	WejscieWyjscie *wejscie_wyjscie = new WejscieWyjscie(manager_ukladow, mapy, baza_obiektow);
	map<string, sf::Sprite> warstwa_renderowana = manager_ukladow->zwroc_uklad(0);

	sf::RenderWindow window(sf::VideoMode(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y), NAZWA_OKNA, sf::Style::Default);
	window.setFramerateLimit(CZESTOTLIWOSC_ODSWIEZANIA_OBRAZU);

	while (window.isOpen())
	{
		// ---------- obsluga wejscia
		sf::Event event;
		while (window.pollEvent(event))
		{
			// ---------- wyjscie awaryjne
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// ---------- przyjmowanie wejscia
			wejscie_wyjscie->przyjmij_wejscie(event, warstwa_renderowana, flaga_zdarzenia_specjalnego);
			// ---------- zdarzenia sepcjalne regulowane flagami
			if (flaga_zdarzenia_specjalnego == 1) {
				// ---------- zdarzenie: zamkniecie okna
				window.close();
			}
			else if (flaga_zdarzenia_specjalnego == 2) {
				// ---------- zdarzenie: rozgrywka nowego poziomu 
				delete manager_ukladow;
				manager_ukladow = new ManagerUkladow(baza_obiektow);
				wejscie_wyjscie->reset_odswiez(manager_ukladow, mapy, baza_obiektow, warstwa_renderowana);
				flaga_zdarzenia_specjalnego = 0;
				flaga_zatrzymania_gry = false;
				flaga_generacji_wrogow = true;
			}			
		}
		// ---------- renderowanie
		window.clear();
		// ---------- gdy trwa gra	
		if (wejscie_wyjscie->uklad_rozgrywki() == true) {
			if (flaga_zatrzymania_gry == false) {
				// ---------- generowanie wrogow
				wejscie_wyjscie->generuj_wrogow(flaga_generacji_wrogow, licznik_klatek3, licznik_klatek4);
				// ---------- sprawdzanie czy gracz wygral
				if (flaga_generacji_wrogow == false && wejscie_wyjscie->nie_ma_wrogow_na_mapie() == true) {
					wejscie_wyjscie->wykonaj_zapis_poziomu(wejscie_wyjscie->zwroc_kolejna_mape());
					wejscie_wyjscie->wywolaj_okno_wygranej_przegranej(true, warstwa_renderowana);
				}
				// ---------- odswiezanie animacji
				wejscie_wyjscie->odswiez_aniamcje(licznik_klatek1, warstwa_renderowana);
				wejscie_wyjscie->odswiez_przemieszczenie_atak(licznik_klatek2, warstwa_renderowana, flaga_zatrzymania_gry);		
			}
			else {
				// ---------- gra zostala zatrzymana, garcz przegral
				wejscie_wyjscie->wywolaj_okno_wygranej_przegranej(false, warstwa_renderowana);
			}
		}
		// ---------- rysowanie
		for (auto i = warstwa_renderowana.begin(); i != warstwa_renderowana.end(); i++) {
			window.draw(i->second);
		}
		window.display();
	}

	delete wejscie_wyjscie;

	return 0;
}