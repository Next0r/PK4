// algorytmy_iteratory_stl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iterator>
#include <vector>
#include <algorithm>
#include <iostream>
#include <list>
#include <deque>
#include <random>
#include <time.h>
#include <string>
#include <iomanip>

using namespace std;

default_random_engine eng(time(0));
uniform_int_distribution<int> dist(1, 10);
uniform_real_distribution<float> dist2(3, 5);

class Student {
private:
	string nazwisko;
	float ocena;
public:
	Student() {};
	Student(string nazw) {
		nazwisko = nazw;
		ocena = dist2(eng);
	};

	void wypisz_dane() {
		cout << nazwisko << fixed << setprecision(2) << setw(15 - nazwisko.size()) << ocena << endl;	
	}

	float get_srednia() {
		return ocena;
	}

	string get_nazwisko() {
		return nazwisko;
	}

	~Student() {};
};

bool porownanie_naziwsko(Student a, Student b) {
	if (a.get_nazwisko() > b.get_nazwisko()) {
		return true;
	}
	else {
		return false;
	}
}

bool porownanie_oceny(Student a, Student b) {
	if (a.get_srednia() > b.get_srednia()) {
		return true;
	}
	else {
		return false;
	}
}

int oblicz_kwadrat(int liczba) {
	return liczba*liczba;
}


int main()
{
	cout << "zadanie1" << endl;
	vector<int> liczby_calkowite(15);
	for (int i = 0; i < 15; i++) {
		liczby_calkowite[i] = dist(eng);
	}
	cout << endl;
	vector<int>::iterator it1;
	for (it1 = liczby_calkowite.begin(); it1 < liczby_calkowite.end(); it1++) {
		cout << *it1 << ", ";
	}
	cout << endl << endl;
	vector<int>::reverse_iterator it2;
	for (it2 = liczby_calkowite.rbegin(); it2 < liczby_calkowite.rend(); it2++) {
		cout << *it2 << ", ";
	}
	cout << endl << endl;
	cout << "zadanie2" << endl << endl;

	deque<char> lista_znakow;
	front_insert_iterator<deque<char>> poczatek(lista_znakow);
	front_insert_iterator<deque<char>> koniec(lista_znakow);
	*poczatek = 'Q';
	*poczatek = 'W';
	*poczatek = 'E';
	*koniec = 'A';
	*koniec = 'S';
	*koniec = 'D';
	
	for (auto it3 = lista_znakow.begin(); it3 < lista_znakow.end(); it3++) {
		cout << *it3 << ", ";
	}
	cout << endl << endl;
	cout << "zadanie3" << endl;
	cout << endl;

	front_inserter(lista_znakow) = 'F';
	front_inserter(lista_znakow) = 'G';
	front_inserter(lista_znakow) = 'H';
	back_inserter(lista_znakow) = 'R';
	back_inserter(lista_znakow) = 'T';
	back_inserter(lista_znakow) = 'Y';

	for (auto it3 = lista_znakow.begin(); it3 < lista_znakow.end(); it3++) {
		cout << *it3 << ", ";
	}
	cout << endl << endl;
	cout << "zadanie4" << endl << endl;

	copy(lista_znakow.begin(), lista_znakow.end(), front_inserter(lista_znakow));
	for (auto it3 = lista_znakow.begin(); it3 < lista_znakow.end(); it3++) {
		cout << *it3 << ", ";
	}
	cout << endl << endl;
	cout << "zadanie5" << endl << endl;

	list<Student> lista_studentow;
	lista_studentow.push_back(Student("Bifur"));
	lista_studentow.push_back(Student("Bombur"));
	lista_studentow.push_back(Student("Balin"));
	lista_studentow.push_back(Student("Fili"));
	lista_studentow.push_back(Student("Kili"));
	lista_studentow.push_back(Student("Ori"));
	lista_studentow.push_back(Student("Nori"));
	lista_studentow.push_back(Student("Dori"));
	lista_studentow.push_back(Student("Oin"));
	lista_studentow.push_back(Student("Gloin"));
	lista_studentow.push_back(Student("Bofur"));
	lista_studentow.push_back(Student("Thorin"));

	vector<Student> wektor_studentow_1(12);
	vector<Student> wektor_studentow_2(12);

	copy(lista_studentow.begin(), lista_studentow.end(), wektor_studentow_1.begin());
	copy(lista_studentow.begin(), lista_studentow.end(), wektor_studentow_2.begin());

	sort(wektor_studentow_1.begin(), wektor_studentow_1.end(), porownanie_naziwsko);
	sort(wektor_studentow_2.begin(), wektor_studentow_2.end(), porownanie_oceny);
	cout << "Sortowanie po nazwiskach: " << endl;
	for (auto i = wektor_studentow_1.begin(); i < wektor_studentow_1.end(); i++) {
		i->wypisz_dane();
	}
	cout << endl;
	cout << "Sortowanie po cenach: ";
	cout << endl;
	for (auto i = wektor_studentow_2.begin(); i < wektor_studentow_2.end(); i++) {
		i->wypisz_dane();
	}

	cout << endl << endl;
	cout << "zadanie6" << endl << endl;

	vector<int> wektor_do_100;
	for (int i = 1; i < 101; i++) {
		wektor_do_100.push_back(i);
	}
	vector<int> wektor_poteg(100);
	transform(wektor_do_100.begin(), wektor_do_100.end(), wektor_poteg.begin(), oblicz_kwadrat);

	for (auto i = wektor_poteg.begin(); i < wektor_poteg.end(); i ++) {
		cout << *i << ", ";
	}

    return 0;
}

