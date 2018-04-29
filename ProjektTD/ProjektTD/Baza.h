#pragma once
//biblioteki
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>
#include <vector>
#include <map>
#include <math.h>

#define NAZWA_OKNA "Projekt Tower Defence"

#define SCIEZKA_PLIKU_MAP "Maps/mapy_2.txt"
#define SCIEZKA_PLIK_TX_PRZYCISKI "Textures/buttons.png"
#define SCIEZKA_PLIK_TX_TLA  "Textures/bgs.png"
#define SCIEZKA_PLIK_TX_POLA  "Textures/fields.png"
#define SCIEZKA_PLIK_TX_OBIEKTY  "Textures/objs.png"
#define SCIEZKA_PLIK_TX_TEKST  "Textures/tekst.png"

#define ZDEFINIOWANA_ILOSC_FAL_WROGOW  5
#define MAX_ILOSC_WROGOW_W_FALI  10
#define ILOSC_TYPOW_WROGOW  10
#define MAX_ODSTEP_CZASOWY_FAL  20
#define ZDEFINIOWANA_ILOSC_PULAPEK 5

#define WYMIAR_EKRANU_X  1280
#define WYMIAR_EKRANU_Y  720

#define ILOSC_UKLADOW 8
#define ILOSC_KLAWISZY 13 // ilosc przyciskow zmapowanych w grze



using namespace std;

//ponizsze wartosci beda wczytywane z pliku ustawien

//stale
/*
const string sciezka_plik_map = "Maps/mapy_2.txt";
const string sciezka_plik_tx_przyciski = "Textures/buttons.png";
const string sciezka_plik_tx_tla = "Textures/bgs.png";
const string sciezka_plik_tx_pola = "Textures/fields.png";
const string sciezka_plik_tx_obiekty = "Textures/objs.png";
const string sciezka_plik_tx_tekst = "Textures/tekst.png";


const int zdefiniowana_ilosc_fal_wrogow = 5;
const int max_ilosc_wrogow_w_fali = 10;
const int ilosc_typow_wrogow = 10;
const int max_odstep_czasowy_fal = 20;
const int zdefiniowana_ilosc_pulapek = 5;

const int wymiar_ekranu_x = 1280;
const int wymiar_ekranu_y = 720;
*/
