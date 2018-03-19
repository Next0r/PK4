// stl_zadanie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <time.h>
#include <string>
#include <queue>
#include <map>


using namespace std;

default_random_engine eng(time(0));
uniform_real_distribution<float> dist(1, 5);
uniform_real_distribution<float> dist2(0, 999);
class Macierz {
private:
	vector<vector <float>> macierz;
public:
	Macierz(int x, int y) {
		macierz.resize(y);
		for (int i = 0; i < y; i++) {
			macierz[i].resize(x);
		}
	}
	void wyswietl() {
		for (int i = 0; i < macierz.size(); i++) {
			cout << "|";
			for (int j = 0; j < macierz[i].size(); j++) {
				cout << fixed << setprecision(2) << macierz[i][j] << " ";
			}
			cout << "|";
			cout << endl;
		}
	}

	void wypelnij() {
		for (int i = 0; i < macierz.size(); i++) {
			for (int j = 0; j < macierz[i].size(); j++) {
				macierz[i][j] = dist(eng);
			}
		}
	}

	Macierz operator *(int mnoznik) {
		for (int i = 0; i < this->macierz.size(); i++) {
			for (int j = 0; j < this->macierz[i].size(); j++) {
				macierz[i][j] = macierz[i][j] * mnoznik;
			}
		}
		return *this;
	}

	void dodaj_wiersz() {
		vector<float> wiersz;
		wiersz.resize(macierz[0].size());
		for (int i = 0; i < wiersz.size(); i++) {
			wiersz[i] = 0;
		}
		macierz.push_back(wiersz);
	}

	void dodaj_kolumne() {
		for (int i = 0; i < macierz.size(); i++) {
			macierz[i].push_back(0);
		}
	}

};

//struktura Zadanie zawierajaca czas i nazwe zadania wraz z konstruktorem i metoda wyswietl
struct Zadanie {
	int czas = 0;
	string nazwa = "empty";
	Zadanie(int t, string n) {
		czas = t;
		nazwa = n;
	}
	void  wyswietl() {
		cout << nazwa << endl;
	}
};


//struktura punkt - uklad kartezjanski wraz z konstruktorem i metoda zwracajaca odleglosc miedzy
//dwoma punktami
struct punkt {
	int x;
	int y;
	punkt(int x_, int y_) {
		x = x_;
		y = y_;
	}
	punkt() {
		x = 0;
		y = 0;
	}
	double odl(punkt p) {
		double o = sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
		return o;
	}
};

struct Porownanie {
	bool operator ()(const Zadanie &zad1, const Zadanie &zad2) {
		if (zad1.czas > zad2.czas) {
			return true;
		}
		if (zad2.czas > zad1.czas) {
			return false;
		}
	}
};

class Dzien {
private:
	priority_queue<Zadanie, vector<Zadanie>, Porownanie> kol_p;
public:
	void dodaj_zadanie(int liczba, string slowo) {
		Zadanie zadanie(liczba, slowo);
		kol_p.push(zadanie); // dopisuje obiekt do kolejki
	}

	void wyswietl() {
		if (!kol_p.empty()) {
			cout << "Wyswietlam zadanie:" << endl;
			cout << kol_p.top().nazwa << endl; // wypisuje obiekt na szczycie
		}
		else {
			cout << "Kolejka jest pusta" << endl;
		}
	}

	void wykonaj() {
		if (!kol_p.empty()) {
			cout << "Wykonuje zadanie:" << endl;
			cout << kol_p.top().nazwa << endl;
			kol_p.pop();	// usuwa obiekt ze szczytu
		}
		else {
			cout << "Kolejka jest pusta" << endl;
		}
	}

	void oproznij() {
		cout << "Czyszcze zawartosc kolejki" << endl;
		while (!kol_p.empty()) {
			kol_p.pop();
		}
	}
};


class Miasta {
private:
	map<string, punkt> mapa_miasta;	
public:
	void dodaj_miasto(string nazwa_miasta, int x, int y) {
		punkt p1(x, y);
		mapa_miasta.insert(make_pair(nazwa_miasta, p1));
	}

	void odleglosc() {
		string nazwa1;
		string nazwa2;
		punkt pos_miasta1;
		punkt pos_miasta2;
		
		try {
			cout << "Nazwa pierwszego miasta: " << endl;
			cin >> nazwa1;
			cout << "Nazwa drugiego miasta: " << endl;
			cin >> nazwa2;
			pos_miasta1 = mapa_miasta.at(nazwa1);
			pos_miasta2 = mapa_miasta.at(nazwa2);
			cout << "Z miasta " << nazwa1 << " do miasta " << nazwa2 << " jest " << pos_miasta1.odl(pos_miasta2) << "km." << endl;
		}
		catch (out_of_range) {
			cout << "Podane miasta nie znajduja sie w bazie" << endl;
		}	
	}
};

int main()
{
	Macierz matrix1(4, 4);
	matrix1.wypelnij();
	matrix1.wyswietl();
	matrix1 = matrix1 * 2;
	cout << "----------------------" << endl;
	matrix1.wyswietl();
	cout << "----------------------" << endl;
	matrix1.dodaj_wiersz();
	matrix1.dodaj_wiersz();
	matrix1.dodaj_kolumne();
	matrix1.wyswietl();
	cout << "----------------------" << endl;
	matrix1.wypelnij();
	matrix1.wyswietl();

	Dzien d1;
	cout << "----------------------" << endl;
	d1.dodaj_zadanie(20, "zadanie nr 1");
	d1.dodaj_zadanie(30, "zadanie nr 2");
	d1.dodaj_zadanie(10, "zadanie nr 3");
	d1.dodaj_zadanie(15, "zadanie nr 4");
	d1.wyswietl();
	d1.wykonaj();
	d1.wyswietl();
	d1.oproznij();
	d1.wyswietl();
	cout << "----------------------" << endl;

	Miasta spis_miast;
	spis_miast.dodaj_miasto("Gliwice", dist2(eng), dist2(eng));
	spis_miast.dodaj_miasto("Rybnik", dist2(eng), dist2(eng));
	spis_miast.dodaj_miasto("Zabrze", dist2(eng), dist2(eng));
	spis_miast.dodaj_miasto("Katowice", dist2(eng), dist2(eng));
	spis_miast.dodaj_miasto("Tristram", dist2(eng), dist2(eng));

	spis_miast.odleglosc();

    return 0;
}

