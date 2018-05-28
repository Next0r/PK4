#include "stdafx.h"
#include "Baza.h"
#include "WczytywanieMapy.h"
#include "RysowaneObiekty.h"
#include "Uklady.h"
#include "Sklep.h"
#include "ObiektyAktywne.h"

using namespace std;

class TranslatorWejscia {
private:
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
			return mapa_wejsc.find(klucz)->second;
		}
		else {
			return -1;
		}
	}
};

class WejscieWyjscie {
private:

	int aktualna_mapa = 0;
	int aktualny_uklad = 0;

	map<string, sf::Sprite> mapa_gry;	// mapa pol jest przechowoywana w pamieci - dla wiekszej wydajnosci
										// tlo mozna odswiezac "na zlecenie" - nie w trybie ciaglym

	TranslatorWejscia *translator_wejscia = new TranslatorWejscia();
	ManagerUkladow *manager_ukladow;
	Plik_map *mapy;

	ManagerObiektowAktywnych *mgr_ob_akt;
	ManagerSklepu *manager_sklepu;	

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
	pair<int, string> mapa_ukladow[ILOSC_KLAWISZY][ILOSC_UKLADOW] =
	{
		// menu         wybor ob     slot wiezy     slot trap   typ wiezy     typ trap      rozgrywka   info
		{ { -4,"strt" }, { -5,"turr" }, {-6,"slt0" },  { -9,"slt0" },  { -7,"typ0" },{ -10,"typ0" },{ -12,"act0" },{ -1,"prev" } },
		{ { -99,"cont" },{ -8,"trap" }, {-6,"slt1" },  { -9,"slt1" },  { -7,"typ1" },{ -10,"typ1" },{ -12,"act1" },{ -2,"next" } },
		{ { 7,"info" },  { 1,"null" },  {  -6,"slt2" },{ -9,"slt2" },  { -7,"typ2" },{ -10,"typ2" },{ -12,"act2" },{ 7,"null" } },
		{ { -3,"quit" }, { 1,"null" },  { -6,"slt3" }, { -9,"slt3" },  { 4,"null" }, { 0,"null" },  { -12,"act3" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {  -6,"slt4" },{ -9,"slt4" },  { 4,"null" }, { 0,"null" },  { -12,"act4" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {  -6,"slt5" },{ -9,"slt5" },  { 4,"null" }, { 0,"null" },  { 0,"null" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {  -6,"slt6" },{ -9,"slt6" },  { 4,"null" }, { 0,"null" },  { 0,"null" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {  -6,"slt7" },{ -9,"slt7" },  { 4,"null" }, { 0,"null" },  { 0,"null" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {  -6,"slt8" },{ -9,"slt8" },  { 4,"null" }, { 0,"null" },  { 0,"null" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {  -6,"slt9" },{ -9,"slt9" },  { 4,"null" }, { 0,"null" },  { 0,"null" },{ 7,"null" } },
		{ { 0,"null" },  { -11,"strt" },{ -11,"strt" },{ -11,"strt" }, { -11,"strt" },{ -11,"strt" },{ 0,"null" },{ 7,"null" } },
		{ { 0,"null" },  { 1,"null" },  {   2,"null" },{ 3,"null" },   { 4,"null" }, { 5,"null" },  { -13,"endg" },{ 7,"null" } },
		{ { 0,"null" },  { 0,"back" },  {   1,"back" },{ 1,"back" },   { 2,"back" }, { 3,"back" },  { 0,"null" },{ 0,"back" } },
	};

	void wykonaj_akcje(int nr_akcji, string nazwa_przycisku, int &flaga_zd_spec) {
		switch (nr_akcji) {
		case -99: // przycisk continue - TEST DRUGIEJ MAPY
			aktualny_uklad = 1;
			aktualna_mapa = 1;
			flaga_zd_spec = 2;
			break;
		case -1: // przycisk prev w ukladzie info
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, -1);
			break;
		case -2: // przycisk next w ukladzie info
			manager_ukladow->przewin_strone_w_ukladzie(aktualny_uklad, 1);
			break;
		case -3: // przycisk quit w ukladzie menu 
			flaga_zd_spec = 1;
			break;
		case -4: // przycisk start w ukladzie menu
			aktualny_uklad = 1;
			aktualna_mapa = 0;
			flaga_zd_spec = 2;
			break;
		case -5: // przycisk turr w ukladzie sklep
			aktualny_uklad = 2;
			mgr_ob_akt->wybrano_wieze(true);
			// w momencie przjescia do interfejsu wyboru wierzy generowana jest ilosc potrzebnych przyciskow
			manager_ukladow->ustaw_ilosc_zestawu_przyciskow(aktualny_uklad, mapy->zwroc_wieze_na_mapie(aktualna_mapa).size());
			break;
		case -6: // przycisk slot w ukladzie wieze_sklep
			aktualny_uklad = 4;
			manager_ukladow->ustaw_ilosc_zestawu_przyciskow(aktualny_uklad, ILOSC_TYPOW_WIEZ);
			mgr_ob_akt->pobierz_kod_slotu_typu(nazwa_przycisku);		
			break;
		case -7: // przycisk typu w ukladzie wieze_typ
			aktualny_uklad = 4;
			mgr_ob_akt->pobierz_kod_slotu_typu(nazwa_przycisku);
			manager_sklepu->wykonaj_zakup();
			break;
		case -8: // przycisk trap w ukladzie sklep
			aktualny_uklad = 3;
			mgr_ob_akt->wybrano_wieze(false);
			// generowanie odpowiedniej ilosci przyciskow
			manager_ukladow->ustaw_ilosc_zestawu_przyciskow(aktualny_uklad, mapy->zwroc_pulapki_na_mapie(aktualna_mapa).size());	
			break;
		case - 9: // przycisk slot w ukladzie pulapki_sklep
			aktualny_uklad = 5;
			// generowanie ilosci przyciskow typu
			manager_ukladow->ustaw_ilosc_zestawu_przyciskow(aktualny_uklad, ILOSC_TYPOW_PULAPEK);
			mgr_ob_akt->pobierz_kod_slotu_typu(nazwa_przycisku);
			break;
		case -10: // przycisk typ w ukldzie pulapki_typ
			aktualny_uklad = 5; 
			mgr_ob_akt->pobierz_kod_slotu_typu(nazwa_przycisku);
			manager_sklepu->wykonaj_zakup();
			break;
		case -11: // przycisk start w dowolnym menu gry
			aktualny_uklad = 6;
			// budowanie zestawu przyciskow akcji
			manager_ukladow->ustaw_ilosc_zestawu_przyciskow(aktualny_uklad,
			mgr_ob_akt->zwroc_typy_ustawionych_pulapek().size(), mgr_ob_akt->zwroc_typy_ustawionych_pulapek());
			flaga_zd_spec = 3; // rozpoczecie generowania wrogow jest zdarzeniem specjalnym

			break;
		case -12: // przycisk akcji w grze
			aktualny_uklad = 6;
			break;
		case -13: // przycisk end w rozgrywce
			aktualny_uklad = 0;
			break;
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
	WejscieWyjscie(ManagerUkladow *&mgr_ukladow, Plik_map *&plik_map, RysowaneObiekty *&rys_ob) {
		manager_ukladow = mgr_ukladow;
		mapy = plik_map;
		mgr_ob_akt = new ManagerObiektowAktywnych(mapy, aktualna_mapa, rys_ob);
		manager_sklepu = new ManagerSklepu(mgr_ob_akt, mapy, aktualna_mapa);
		mapa_gry = mapy->zwroc_mape(aktualna_mapa)->zwroc_pola_na_mapie();
	}

	~WejscieWyjscie() {
		delete translator_wejscia;
		delete mgr_ob_akt;
		delete manager_sklepu;
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

	void odswiez_przemieszczenie_wrogow_ramki_atak(int &timer, map<string, sf::Sprite> &warstwa_renderowania) {
		if (timer < CZESTOTLIWOSC_ODSWIEZANIA_RUCH) {
			timer++;
		}
		else {
			// odswiez
			mgr_ob_akt->odswiez_pozycje_na_sciezce_wrogow();
			mgr_ob_akt->wrogowie_na_koncu();	// decyduje o zakonczeniu gry
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
					manager_ukladow->nacisnij_przycisk_w_ukladzie(nazwa_przycisku, aktualny_uklad);

					// zwraca uklad
					warstwa_renderowania = zbuduj_warstwe_renderowania();
				}
				else {
					// puszczono przycisk		
					string nazwa_przycisku = mapa_ukladow[kod_klawisza][aktualny_uklad].second;
					manager_ukladow->przywroc_przycisk_w_ukladzie(nazwa_przycisku, aktualny_uklad);

					// podajemowanie akcji
					int nr_akcji = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
					
					if (nr_akcji >= 0) { // gdy nalezy przejsc do innego ukladu
						aktualny_uklad = mapa_ukladow[kod_klawisza][aktualny_uklad].first;
					}
					else { // gdy nalezy wykonac modyfikacje w aktualnym ukladzie
						wykonaj_akcje(nr_akcji, nazwa_przycisku, flaga_zd_spec);
					}
					// zwraca uklad
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
};

int main() {
	int flaga_zdarzenia_specjalnego = 0; // 1 - zamyka okno 2 - resetuje gre i buduje nowy render
	int licznik_klatek1 = 0;
	int licznik_klatek2 = 0;
	int licznik_klatek3 = 0; 
	int licznik_klatek4 = 0;

	bool flaga_generacji_wrogow; // false oznacza, ze wrogowie w fali sie skonczyli
										// jesli wrogowie sie skonczyli i zaden nie znajduje sie na mapie gracz wygral

	// obiekty pasywne
	RysowaneObiekty *baza_obiektow = new RysowaneObiekty();
	Plik_map *mapy = new Plik_map(baza_obiektow);

	// test zwracanych bledow
	if (mapy->blad_pliku_map) {
		cout << "Blad wczytywania pliku map." << endl;
		return 0;
	}

	// obiekty aktywne
	ManagerUkladow *manager_ukladow = new ManagerUkladow(baza_obiektow);
	WejscieWyjscie *wejscie_wyjscie = new WejscieWyjscie(manager_ukladow, mapy, baza_obiektow);
	map<string, sf::Sprite> warstwa_renderowana = manager_ukladow->zwroc_uklad(0);


	sf::RenderWindow window(sf::VideoMode(WYMIAR_EKRANU_X, WYMIAR_EKRANU_Y), NAZWA_OKNA);
	window.setFramerateLimit(CZESTOTLIWOSC_ODSWIEZANIA_OBRAZU);

	while (window.isOpen())
	{
		// ---------- obsluga wejscia
		sf::Event event;
		while (window.pollEvent(event))
		{
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
			}
			
		}

		// ---------- renderowanie
		window.clear();
		// ---------- gdy trwa gra
		if (wejscie_wyjscie->uklad_rozgrywki() == true) {
			// ---------- generowanie wrogow
			wejscie_wyjscie->generuj_wrogow(flaga_generacji_wrogow, licznik_klatek3, licznik_klatek4);
			// ---------- odswiezanie animacji
			wejscie_wyjscie->odswiez_aniamcje(licznik_klatek1, warstwa_renderowana);
			wejscie_wyjscie->odswiez_przemieszczenie_wrogow_ramki_atak(licznik_klatek2, warstwa_renderowana);
		}
		// ---------- rysowanie
		for (auto i = warstwa_renderowana.begin(); i != warstwa_renderowana.end(); i++) {
			window.draw(i->second);
		}
		window.display();
	}

	return 0;
}