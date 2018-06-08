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
#include <typeinfo>

#define PI 3.14159265359

#define NAZWA_OKNA "Projekt Tower Defence"

#define SCIEZKA_PLIKU_MAP "Maps/mapy_2.txt"
#define SCIEZKA_PLIK_TX_PRZYCISKI "Textures/buttons_2.png"
#define SCIEZKA_PLIK_TX_TLA  "Textures/bgs.png"
#define SCIEZKA_PLIK_TX_POLA  "Textures/fields.png"
#define SCIEZKA_PLIK_TX_OBIEKTY  "Textures/objs.png"
#define SCIEZKA_PLIK_TX_TEKST  "Textures/tekst_2.png"
#define SCIEZKA_KROJ_TEKSTU  "Fonts/joystix_monospace.ttf"
#define SCIEZKA_PLIK_TX_WIEZ  "Textures/turrets.png"
#define SCIEZKA_PLIK_TX_PULAPEK "Textures/traps.png"
#define SCIEZKA_PLIK_TX_P_AKCJI "Textures/action_buttons.png"
#define SCIEZKA_PLIK_TX_WROGOWIE "Textures/enemies.png"
#define SCIEZKA_PLIK_TX_EFEKTY "Textures/effects.png"
#define SCIEZKA_PLIK_TX_RAMKI "Textures/frames.png"
#define SCIEZKA_PLIK_TX_CYFRY "Textures/numbers.png"
#define SCIEZKA_PLIK_TX_OKIENKA "Textures/msgboxes.png"
#define SCIEZKA_PLIK_TX_CZCIONKA "Textures/font.png"
#define SCIEZKA_PLIK_TX_INTERFACE01 "Textures/trap_frame.png"

#define SCIEZKA_PLIKU_ZAPISU "Saves/save_file.txt"

#define ZDEFINIOWANA_ILOSC_FAL_WROGOW  5
#define MAX_ILOSC_WROGOW_W_FALI  10
#define ILOSC_TYPOW_WROGOW  10
#define MAX_ODSTEP_CZASOWY_FAL  20
#define ZDEFINIOWANA_ILOSC_PULAPEK 5

#define WYMIAR_EKRANU_X  1280
#define WYMIAR_EKRANU_Y  720
#define CZESTOTLIWOSC_ODSWIEZANIA_OBRAZU  60

#define ILOSC_UKLADOW 8
#define ILOSC_KLAWISZY 13 // ilosc przyciskow zmapowanych w grze

#define ILOSC_TYPOW_WIEZ 3
#define ILOSC_TYPOW_PULAPEK 3

#define WYMIAR_POLA_GRY 80
#define MAX_ROZMIAR_SCIEZKI 100

#define CZESTOTLIWOSC_ODSWIEZANIA_ANIM 6 // 60hz / ->6 = 10
#define CZESTOTLIWOSC_ODSWIEZANIA_RUCH 3 // 60hz / ->3 = 20  wrogowie przemieszczaja sie 20 razy na sekunde
#define SZYBKOSC_PRZEMIESZCZENIA_WROGOW 2 // wrogowie pokonuja 2 px na odswiezenie -> jedno pole w 2 sekundy
#define ODSTEP_MIEDZY_WROGAMI 40 // odstep w ilosci klatek miedzy wrogami

#define WIDZIALNOSC_RAMEK  0.8
#define JASNOSC_RAMEK  0.8
#define POZIOM_B_W_RAMCE 180.0

