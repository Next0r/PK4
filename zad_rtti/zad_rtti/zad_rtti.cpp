// zad_rtti.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>

using namespace std;

class baza {
public:
	int zmienna1;
	int zmienna2;
	virtual void wypisz() {
		cout << "wirtualna metoda klasy bazowej" << endl;
		cout << zmienna1 << endl;
		cout << zmienna2 << endl;
	}
	
	baza(int l1, int l2) {
		zmienna1 = l1;
		zmienna2 = l2;
	}

	virtual ~baza() {};
};

class dziedziczaca : public baza {
public:
	int zmienna1;
	int zmienna2;
	virtual void wypisz() {
		cout << "wirtualna metoda klasy pochodnej" << endl;
		cout << zmienna1 << endl;
		cout << zmienna2 << endl;
	}

	dziedziczaca(int l1, int l2): baza(l1, l2) {
		zmienna1 = l1;
		zmienna2 = l2;
	}

	virtual ~dziedziczaca() {};
};


int main()
{
	baza * wsk1 = new baza(20, 30);
	dziedziczaca * wsk2 = new dziedziczaca(10, 15);

	baza * wsk3 = wsk2;
	wsk3->wypisz(); // mimo ze wskaznik typu bazy wypisuje metode dla pochodnej

    return 0;
}

