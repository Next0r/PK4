// IntWsk.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>


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

	Kierunek(string nazwa, int liczba) 	{
		nazwa_kierunku = nazwa;
		liczba_studentow = liczba;
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
	cout << endl << "zadanie1" << endl;
	vector <unique_ptr<Kierunek>> kierunki;
	kierunki.push_back(unique_ptr<Kierunek>(new Kierunek("informatyka", 150)));
	kierunki.push_back(unique_ptr<Kierunek>(new Kierunek("robotyka", 200)));
	kierunki.push_back(unique_ptr<Kierunek>(new Kierunek("automatyka", 130)));

	for (auto i = kierunki.begin(); i != kierunki.end(); i++) {
		i->get()->wypisz_dane();
	}

	
	// zadanie 2
	cout << endl << "zadanie2" << endl;

	kierunki[0] = move(kierunki[2]);

	for (auto i = kierunki.begin(); i != kierunki.end(); i++) {
		if (i->get() != nullptr) {
			i->get()->wypisz_dane();
		}
		else {
			cout << "Blad: brak elementu!" << endl;
		}
	}

	// zadanie 3

	cout << endl << "zadanie 3" << endl;

	try {
		unique_ptr<TestWyjatku> wsk = unique_ptr<TestWyjatku>(new TestWyjatku);
	}
	catch(const exception& e) {
		cout << "Zlapano wyjatek testu wskaznika" << endl;
	}
	catch(...){
		cout << "Zlapano inny wyjatek" << endl;
	}

	// zadanie 4
	cout << endl << "zadanie 4" << endl;
	auto wsk1 = make_shared<Kierunek>("fizyka", 180);
	wsk1->wypisz_dane();
	auto wsk2(wsk1);
	wsk2->wypisz_dane();
	auto wsk3 = shared_ptr<Kierunek>(new Kierunek("grafika", 128));
	wsk3->wypisz_dane();

	cout << "Ilosc referencji: " << wsk1.use_count() << endl;

	// zadanie 5
	cout << endl << "zadanie 5" << endl;
	auto wsk4 = shared_ptr<Kierunek>(new Kierunek("makro", 128));
	cout << "Ilosc referencji: " << wsk4.use_count() << endl;
	auto wsk5(wsk4);
	cout << "Ilosc referencji: " << wsk4.use_count() << endl;
	auto wsk6(wsk4);
	cout << "Ilosc referencji: " << wsk4.use_count() << endl;
	auto wsk7(wsk4);
	cout << "Ilosc referencji: " << wsk4.use_count() << endl;

	// zadanie 6
	cout << endl << "zadanie 6" << endl;

	if (wsk4.unique()) {
		cout << "Wskaznik jest unikalny" << endl;
	}
	else {
		cout << "Wskaznik ma wiele referencji" << endl;
	}

	// zadanie 7
	cout << endl << "zadanie 7" << endl;

	weak_ptr<Kierunek> wsk8(wsk1);
	cout << "Ilosc referencji: " << wsk1.use_count() << endl;
	if (wsk8.expired()) {
		cout << "Pamiec wskaznika jest zwolniona" << endl;
	}
	else{
		cout << "Pamiec wskaznika jest zajeta" << endl;
	}
	wsk8.lock();
	cout << "Adres wskaznika: " << &wsk8 << endl;
	cout << endl;

    return 0;
}

