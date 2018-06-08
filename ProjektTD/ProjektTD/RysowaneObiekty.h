#pragma once
#include "Baza.h"

using namespace std;

class RysowaneObiekty {
private:
	sf::Sprite przycisk;
	sf::Sprite tlo;
	sf::Sprite pole;
	sf::Sprite obiekty; // wrogowie, wieze itp.
	sf::Sprite pole_tekstowe;
	sf::Sprite wieza;
	sf::Sprite pulapka;
	sf::Sprite p_akcji;
	sf::Sprite wrogowie;
	sf::Sprite efekty;
	sf::Sprite ramki;
	sf::Sprite cyfry;
	sf::Sprite okienka;
	sf::Sprite czcionka;
	sf::Sprite interface01;

	sf::Texture t_przyciski;
	sf::Texture t_tlo;
	sf::Texture t_pole;
	sf::Texture t_obiekty;
	sf::Texture t_pole_tekstowe;
	sf::Texture t_wieza;
	sf::Texture t_pulapka;
	sf::Texture t_p_akcji;
	sf::Texture t_wrogowie;
	sf::Texture t_efekty;
	sf::Texture t_ramki;
	sf::Texture t_cyfry;
	sf::Texture t_okienka;
	sf::Texture t_czcionka;
	sf::Texture t_interface01;


public:
	RysowaneObiekty() {
		t_przyciski.loadFromFile(SCIEZKA_PLIK_TX_PRZYCISKI);
		t_tlo.loadFromFile(SCIEZKA_PLIK_TX_TLA);
		t_pole.loadFromFile(SCIEZKA_PLIK_TX_POLA);
		t_obiekty.loadFromFile(SCIEZKA_PLIK_TX_OBIEKTY);
		t_pole_tekstowe.loadFromFile(SCIEZKA_PLIK_TX_TEKST);
		t_wieza.loadFromFile(SCIEZKA_PLIK_TX_WIEZ);
		t_pulapka.loadFromFile(SCIEZKA_PLIK_TX_PULAPEK);
		t_p_akcji.loadFromFile(SCIEZKA_PLIK_TX_P_AKCJI);
		t_wrogowie.loadFromFile(SCIEZKA_PLIK_TX_WROGOWIE);
		t_efekty.loadFromFile(SCIEZKA_PLIK_TX_EFEKTY);
		t_ramki.loadFromFile(SCIEZKA_PLIK_TX_RAMKI);
		t_cyfry.loadFromFile(SCIEZKA_PLIK_TX_CYFRY);
		t_okienka.loadFromFile(SCIEZKA_PLIK_TX_OKIENKA);
		t_czcionka.loadFromFile(SCIEZKA_PLIK_TX_CZCIONKA);
		t_interface01.loadFromFile(SCIEZKA_PLIK_TX_INTERFACE01);

		przycisk.setTexture(t_przyciski);
		tlo.setTexture(t_tlo);
		pole.setTexture(t_pole);
		obiekty.setTexture(t_obiekty);
		pole_tekstowe.setTexture(t_pole_tekstowe);
		wieza.setTexture(t_wieza);
		pulapka.setTexture(t_pulapka);
		p_akcji.setTexture(t_p_akcji);
		wrogowie.setTexture(t_wrogowie);
		efekty.setTexture(t_efekty);
		ramki.setTexture(t_ramki);
		cyfry.setTexture(t_cyfry);
		okienka.setTexture(t_okienka);
		czcionka.setTexture(t_czcionka);
		interface01.setTexture(t_interface01);
	}

	sf::Sprite zwroc_sprite_przycisk() {
		return przycisk;
	}
	sf::Sprite zwroc_sprite_tlo() {
		return tlo;
	}
	sf::Sprite zwroc_sprite_pole() {
		return pole;
	}
	sf::Sprite zwroc_sprite_obiekt() {
		return obiekty;
	}
	sf::Sprite zwroc_sprite_pole_tekstowe_1() {
		return pole_tekstowe;
	}
	sf::Sprite zwroc_sprite_cyfry() {
		return cyfry;
	}
	sf::Sprite zwroc_sprite_wiezy() {
		return wieza;
	}
	sf::Sprite zwroc_sprite_pulapki() {
		return pulapka;
	}
	sf::Sprite zwroc_sprite_p_akcji() {
		return p_akcji;
	}
	sf::Sprite zwroc_sprite_wrogowie() {
		return wrogowie;
	}
	sf::Sprite zwroc_sprite_efekty() {
		return efekty;
	}
	sf::Sprite zwroc_sprite_ramki() {
		return ramki;
	}
	sf::Sprite zwroc_sprite_okienka() {
		return okienka;
	}
	sf::Sprite zwroc_sprite_czcionka() {
		return czcionka;
	}
	sf::Sprite zwroc_sprite_interface01() {
		return interface01;
	}
};