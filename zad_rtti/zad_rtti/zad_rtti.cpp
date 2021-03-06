// zad_rtti.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <iostream>
#include <typeinfo> // header konieczy do dzialania type id
#include <string>

using namespace std;

/*class baza {
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
};*/

class baza {
public:
	virtual void wypisz() {
		cout << "metoda klasy bazowej" << endl;
	}
};

class pochodna : public baza {
public:
	virtual void wypisz() {
		cout << "metoda klasy pochodnej" << endl;
	}
	virtual void wypisz2() {
		cout << "specyficzna metoda klasy wirtualnej" << endl;
	}
};


int main()
{

	
	/*baza * wsk1 = new baza(20, 30);
	dziedziczaca * wsk2 = new dziedziczaca(10, 15);

	baza * wsk3 = wsk2;
	wsk3->wypisz(); // mimo ze wskaznik typu bazy wypisuje metode dla pochodnej*/


	int * wsk1 = new int(20);

	baza *p1 = new pochodna;
	pochodna *p2 = new pochodna;
	baza *p3 = new baza;
	baza *p4 = new pochodna;

	pochodna *konw1 = dynamic_cast<pochodna*>(p4);

	if (konw1 != nullptr) {
		cout << "wskaznik konwertowany jest dynamicznego typu pochodnej" << endl;
		konw1->wypisz2(); // p4 jest typu pochodnej mozna wywolac wypisz2
		cout << endl;
	}
	else {
		cout << "wskaznik konwrtowany jest dynamicznego tyu bazowej" << endl;
		//konw1->wypisz2(); // nie mozna wywolac wypisz2
	}

	p1->wypisz();
	p2->wypisz();
	p3->wypisz();

	cout << "wypisuje nazwe klasy konw1: " << typeid(konw1).name() << endl;
	cout << "wypisuje nazwe klasy p3: " << typeid(p3).name() << endl;
	
	const type_info& info = typeid(*wsk1); // zmienna przechowujaca nazwe typu *p3 - bez symbolu wskaznika 
	cout << "wypisuje nazwe klasy p3 : " << info.name() << endl;

    return 0;
}

