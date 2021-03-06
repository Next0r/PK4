// zad_rtti2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

class gamer {
public:
	int ranking;
	virtual void getRanking() {
		cout << "wypisuje ranking gamera: " << ranking << endl;
	}
	gamer(int rank) {
		ranking = rank;
	}
	virtual ~gamer() {};
};

class gracz : public gamer {
private:
	string refleks;
public:
	void getRanking() {
		cout << "wypisuje ranking gracza: " << ranking << endl;
	}

	virtual void rozegrajMecz() {
		cout << "gracz rozegral mecz!" << endl;
		ranking++;
	}
	gracz(int liczba, string refleks1) :gamer(liczba) {
		refleks = refleks1;
	}
	virtual ~gracz() {};
};

class zawodowy_gracz : public gracz {
public:
	int ilosc_gier_r;
	void getRanking() {
		cout << "wypisuje ranking zawodowego gracza: " << ranking << endl;
	}

	void rozegrajMecz() {
		cout << "zawodowy gracz rozegral mecz!" << endl;
		ranking++;
	}

	zawodowy_gracz(int liczba2, string refleks2, int liczba) : gracz(liczba2, refleks2) {
		ilosc_gier_r = liczba;
	}
	virtual ~zawodowy_gracz() {};
};

int main()
{
	const int size = 6;
	gamer *tablica[size];
	tablica[0] = new gamer(10);
	tablica[1] = new zawodowy_gracz(13, "dobry", 12);
	tablica[2] = new gracz(30, "slaby");
	tablica[3] = new gracz(42, "bardzo dobry");
	tablica[4] = new zawodowy_gracz(3, "bardzo dobry", 51);
	tablica[5] = new gamer(22);
	
	cout << "Zadanie 1" << endl << endl;

	for (int i = 0; i < size; i++) {	
		cout << "typ klasy na pozycji: " << i << " w tablicy to: " << typeid(*tablica[i]).name() << endl;
	}
	
	cout << endl << "Zadanie 2 " << endl << endl;

	for (int i = 0; i < size; i++) {
		tablica[i]->getRanking();
	}

	cout << endl << "Zadanie 3 " << endl << endl;

	for (int i = 0; i < size; i++) {
		gracz *wsk1 = dynamic_cast<gracz*>(tablica[i]);
		zawodowy_gracz *wsk2 = dynamic_cast <zawodowy_gracz*>(tablica[i]);
		if (wsk1 != nullptr) {
			wsk1->rozegrajMecz();
		}
		else if (wsk2 != nullptr) {
			wsk2->rozegrajMecz();
		}
	}

	cout << endl << "Zadanie 4 " << endl << endl;

	int ilosc_graczy = 0;
	int ilosc_pkt = 0;
	gracz *tmp1 = new gracz(1, "asd");

	for (int i = 0; i < size; i++) {	
		if (typeid(*tablica[i]) == typeid(*tmp1)) {
			ilosc_pkt += tablica[i]->ranking;
			ilosc_graczy++;
		}
	}
	
	float tmp = ilosc_pkt / ilosc_graczy;
	cout << "srednia ilosc pkt: " << tmp << endl;

    return 0;
}

