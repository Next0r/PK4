// Watki.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

using namespace std;

mutex m;

int zmienna1 = 0;
atomic_int zmienna2 = 0;

void wypisz(int ile, char znak) {
	for (int i = 0; i < ile; i++) {
		cout << znak << " ";
	}
}

void praca1() {
	cout << endl << "Funkcja praca1 rozpoczyna dzialanie, " << "id watku " << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 1 << " ";
	}
	cout << endl << "Funkcja praca1 konczy dzialanie" << endl;
}

void praca2() {
	cout << endl << "Funkcja praca2 rozpoczyna dzialanie, " << "id watku " << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 2 << " ";
	}
	cout << endl << "Funkcja praca2 konczy dzialanie" << endl;
}

void praca3() {
	m.lock();
	cout << endl << "Funkcja praca3 rozpoczyna dzialanie, " << "id watku " << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 3 << " ";
	}
	cout << endl << "Funkcja praca3 konczy dzialanie" << endl;
	m.unlock();
}

void praca4() {
	m.lock();
	cout << endl << "Funkcja praca4 rozpoczyna dzialanie, " << "id watku " << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 4 << " ";
	}
	cout << endl << "Funkcja praca4 konczy dzialanie" << endl;
	m.unlock();
}

void odliczanie() {
	cout << "Rozpoczynam odliczanie" << endl;
	for (int i = 5; i >= 0; i--) {
		cout << i << endl;
		this_thread::sleep_for(1s);
	}
}

void trudne_obliczenia(int &liczba) {
	for (int i = 0; i < 30000; i++) {
		liczba += 1;
	}
}

void trudne_obliczenia2(atomic_int& a) {
	for (int i = 0; i < 30000; i++) {
		a++;
	}
}

int main()
{
	// zadanie 1
	cout << "Zadanie 1" << endl << endl;
	cout << "Ilosc watkow: " << thread::hardware_concurrency() << endl;
	cout << endl;
	// zadanie 2
	cout << "Zadanie 2" << endl << endl;
	thread t1(wypisz, 10, 'A');
	thread t2(wypisz, 10, 'B');
	t1.join();
	t2.join();
	cout << endl << endl;
	// zadanie 3
	cout << "Zadanie 3" << endl << endl;
	cout << endl;
	thread t3(praca1);
	thread t4(praca2);
	t3.join();
	t4.join();
	thread t5(praca3);
	thread t6(praca4);
	t5.join();
	t6.join();
	cout << endl;
	// zadanie 4
	cout << "Zadanie 4" << endl << endl;
	int liczba = 0;
	thread t7(odliczanie);
	thread t8(trudne_obliczenia, ref(liczba));
	t7.join();
	t8.join();
	cout << "Wartosc po obliczeniach" << endl;
	cout << liczba << endl;

	cout << endl;
	//zadanie 4
	cout << "Zadanie 5" << endl << endl;
	cout << "Zmienna 1: " << zmienna1 << endl;
	cout << "Zmienna 2: " << zmienna2 << endl;

	vector <thread> wektor_watkow;

	for (int i = 0; i < 10; i++) {
		wektor_watkow.push_back(thread(trudne_obliczenia, ref(zmienna1)));
	}
	for (int i = 0; i < 10; i++) {
		wektor_watkow.push_back(thread(trudne_obliczenia2, ref(zmienna2)));
	}

	for (auto& th : wektor_watkow) {
		th.join();
	}

	cout << "Zmienna 1: " << zmienna1 << endl;
	cout << "Zmienna 2: " << zmienna2 << endl;

    return 0;
}

