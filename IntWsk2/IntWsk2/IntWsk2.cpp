// IntWsk2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

class Kierunek {
private:
	string nazwa_kierunku;
	int liczba_studentow;
public:
	string get_nazwa_kierunku() {
		return nazwa_kierunku;
	}

	void wypisz_dane() {
		cout << "Nazwa kierunku: " << nazwa_kierunku << endl;
		cout << "Ilosc studentow: " << liczba_studentow << endl;
	}

	Kierunek(string s1, int l1) {
		liczba_studentow = l1;
		nazwa_kierunku = s1;
	}


};

class TestWyjatku {
private:
public:
	TestWyjatku() {
		throw exception();
	}
};


int main()
{
	// zadanie 1

	cout << "zadanie 1" << endl << endl;

	vector<unique_ptr<Kierunek>> kierunki;
	kierunki.push_back(unique_ptr<Kierunek>( new Kierunek("Informaktyka", 128)));
	kierunki.push_back(unique_ptr<Kierunek>(new Kierunek("Robotyka", 256)));
	kierunki.push_back(unique_ptr<Kierunek>(new Kierunek("Automatyka", 64)));


	for (auto i = kierunki.begin(); i != kierunki.end(); i++) {
		i->get()->wypisz_dane();
	}
	// zadanie 2
	cout << endl << endl << "zadanie 2" << endl << endl;

	kierunki[0] = move(kierunki.back());

	for (auto i = kierunki.begin(); i != kierunki.end(); i++) {
		if (i->get() != nullptr) {
			i->get()->wypisz_dane();
		}
		else{
			cout << "Element nie istnieje" << endl;
		}
		
	}

	// zadanie 3
	cout << endl << endl << "zadanie 3" << endl << endl;

	try {
		unique_ptr<TestWyjatku> wsk1 = unique_ptr<TestWyjatku>(new TestWyjatku());
	}
	catch (const exception& e) {
		cout << "Zlapano wyjatek testowy" << endl;
	}
	catch (...) {
		cout << "Zlapano inny wyjatek" << endl;
	}

	// zadanie 4
	cout << endl << endl << "zadanie 4" << endl << endl;

	auto wsk2 = make_shared<Kierunek>("Fizyka", 120);
	wsk2->wypisz_dane();
	auto wsk3(wsk2);
	wsk3->wypisz_dane();
	auto wsk4 = shared_ptr<Kierunek>(new Kierunek("Macro", 150));
	wsk4->wypisz_dane();
	cout << "Ilosc referencji do pierwszego wskaznika: " << wsk2.use_count() << endl;

	// zadanie 5
	cout << endl << endl << "zadanie 5" << endl << endl;

	auto wsk5 = shared_ptr<Kierunek>(new Kierunek("Informatyka Przemsylowa", 125));
	cout << "Ilosc referencji: " << wsk5.use_count() << endl;
	auto wsk6(wsk5);
	cout << "Ilosc referencji: " << wsk5.use_count() << endl;
	auto wsk7(wsk5);
	cout << "Ilosc referencji: " << wsk5.use_count() << endl;

	// zadanie 6
	cout << endl << endl << "zadanie 6" << endl << endl;

	if (wsk5.unique()) {
		cout << "Wskaznik z zadania 5 jest jedynym wlascicielem obieku." << endl;
	}
	else {
		cout << "Wskaznik z zadania 5 nie jest jedynym wlascicielem obieku." << endl;
	}

	// zadanie 7
	cout << endl << endl << "zadanie 7" << endl << endl;
	weak_ptr<Kierunek> wsk8(wsk2);
	cout << "Ilosc referencji: " << wsk8.use_count() << endl;

	if (wsk8.expired()) {
		cout << "Zarzadzana pamiec jest juz zwolniona." << endl;	
	}
	else {
		cout << "Zarzadzana pamiec nie jest jeszcze zwolniona." << endl;
		wsk8.lock();
		cout << "Adres wskaznika po konwersji do shared_ptr: " << &wsk8 << endl;
	}
	cout << endl;

    return 0;
}

