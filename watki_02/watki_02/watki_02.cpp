// watki_02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

using namespace std;

mutex m1;

int liczba_g = 0;
atomic_int liczba_at_g = 0;

void wypisz(int liczba, char znak) {
	for (int i = 0; i < liczba; i++) {
		cout << znak << " ";
	}
}

void praca1() {
	cout << "Rozpoczecie 1" << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 1 << " ";
	}
	cout << "Zakonczenie 1" << this_thread::get_id() << endl;
}

void praca2() {
	cout << "Rozpoczecie 2" << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 2 << " ";
	}
	cout << "Zakonczenie 2" << this_thread::get_id() << endl;
}

void praca3() {
	m1.lock();
	cout << "Rozpoczecie 3" << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 3 << " ";
	}
	cout << "Zakonczenie 3" << this_thread::get_id() << endl;
	m1.unlock();
}

void praca4() {
	m1.lock();
	cout << "Rozpoczecie 4" << this_thread::get_id() << endl;
	for (int i = 0; i < 10; i++) {
		cout << 4 << " ";
	}
	cout << "Zakonczenie 4" << this_thread::get_id() << endl;
	m1.unlock();
}


void odliczanie() {
	cout << "Rozpoczecie odliczania" << endl;
	for (int i = 5; i >= 0; i--) {
		cout << i << endl;
		this_thread::sleep_for(1s);
	}
}

void trudne_obliczenia(int &a) {
	for (int i = 0; i < 30000000; i++) {
		a++;
	}
}

void trudne_obliczenia2(atomic_int &a) {
	for (int i = 0; i < 30000000; i++) {
		a++;
	}
}

int main()
{
	// Zadanie 1
	cout << "Zadanie 1" << endl << endl;
	cout << "Ilosc watkow: " << thread::hardware_concurrency() << endl;
	cout << endl;

	// Zadanie 2
	cout << "Zadanie 2" << endl << endl;
	thread t1(wypisz, 10, 'A');
	thread t2(wypisz, 15, 'B');
	t1.join();
	t2.join();
	cout << endl;

	// Zadanie 3
	cout << "Zadanie 3" << endl << endl;
	thread t3(praca1);
	thread t4(praca2);
	t3.join();
	t4.join();
	thread t3_2(praca3);
	thread t4_2(praca4);
	t3_2.join();
	t4_2.join();


	cout << endl;
	// Zadanie 4
	cout << "Zadanie 4" << endl << endl;
	int liczba = 0;
	thread t5(odliczanie);
	thread t6(trudne_obliczenia, ref(liczba));
	t5.join();
	t6.join();
	cout << "Wartosc zmiennej: " << liczba << endl;
	cout << endl;
	// Zadanie 5
	cout << "Zadanie 5" << endl << endl;
	cout << "Zmienna int: " << liczba_g << endl;
	cout << "Zmienna atomic_int: " << liczba_at_g << endl;

	vector <thread> wektor_watkow;

	for (int i = 0; i < 10; i++) {
		wektor_watkow.push_back(thread(trudne_obliczenia, ref(liczba_g)));
	}

	for (int i = 0; i < 10; i++) {
		wektor_watkow.push_back(thread(trudne_obliczenia2, ref(liczba_at_g)));
	}

	for (auto& watek : wektor_watkow) {
		watek.join();
	}

	cout << "Zmienna int: " << liczba_g << endl;
	cout << "Zmienna atomic_int: " << liczba_at_g << endl;

    return 0;
}

