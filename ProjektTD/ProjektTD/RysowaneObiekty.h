#pragma once
#include "Baza.h"

using namespace std;

class RysowaneObiekty {
private:
	sf::Sprite przycisk;
	sf::Sprite tlo;
	sf::Sprite pole;
	sf::Sprite obiekty; // wrogowie, wieze itp.
	sf::Sprite pole_tekstowe_1;

	sf::Texture t_przyciski;
	sf::Texture t_tlo;
	sf::Texture t_pole;
	sf::Texture t_obiekty;
	sf::Texture t_pole_tekstowe_1;
public:
	RysowaneObiekty() {
		t_przyciski.loadFromFile(SCIEZKA_PLIK_TX_PRZYCISKI);
		t_tlo.loadFromFile(SCIEZKA_PLIK_TX_TLA);
		t_pole.loadFromFile(SCIEZKA_PLIK_TX_POLA);
		t_obiekty.loadFromFile(SCIEZKA_PLIK_TX_OBIEKTY);
		t_pole_tekstowe_1.loadFromFile(SCIEZKA_PLIK_TX_TEKST);

		przycisk.setTexture(t_przyciski);
		tlo.setTexture(t_tlo);
		pole.setTexture(t_pole);
		obiekty.setTexture(t_obiekty);
		pole_tekstowe_1.setTexture(t_pole_tekstowe_1);
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
		return pole_tekstowe_1;
	}

};