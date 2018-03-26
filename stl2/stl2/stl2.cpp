// stl2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <list>
#include <iostream>
#include <random>
#include <time.h>
#include <iterator>
#include <deque>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

default_random_engine eng1(time(0));
uniform_int_distribution<int> distr(0, 20);
uniform_real_distribution<float> distr2(3, 5);

class Student {
private:
	float srednia_ocen;
	string nazwisko;
public:

	string get_nazwisko() {
		return nazwisko;
	}

	float get_srednia_ocen() {
		return srednia_ocen;
	}

	void wypisz_dane() {
		cout << nazwisko << " " << fixed << setprecision(2) << setw(12-nazwisko.size()) << srednia_ocen << endl;
	}
	Student() {};

	Student(string nazw) {
		nazwisko = nazw;
		srednia_ocen = distr2(eng1);
	}
	~Student() {};

};

bool sortowanie_po_nazwiskach(Student s1, Student s2) {
	if (s1.get_nazwisko() < s2.get_nazwisko()) {
		return true;
	}
	else {
		return false;
	}
}

bool sortowanie_po_ocenach(Student s1, Student s2) {
	if (s1.get_srednia_ocen() < s2.get_srednia_ocen()) {
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
	int rozmiar_wektora = 15;
	
	vector<int> wektor_liczb_calk;
	deque<char> kolejka_znakow;
	front_inserter(kolejka_znakow) = 'A';
	front_inserter(kolejka_znakow) = 'S';
	front_inserter(kolejka_znakow) = 'D';

	back_inserter(kolejka_znakow) = 'Q';
	back_inserter(kolejka_znakow) = 'W';
	back_inserter(kolejka_znakow) = 'E';

	for (int i = 0; i < rozmiar_wektora; i++) {
		wektor_liczb_calk.push_back(distr(eng1));
	}
	cout <<  "-----Zadanie1" << endl << endl;
	for (auto i  = wektor_liczb_calk.begin(); i < wektor_liczb_calk.end(); i++) {
		cout << *i << ", ";
	}
	cout << endl;
	for (auto i = wektor_liczb_calk.rbegin(); i < wektor_liczb_calk.rend(); i++) {
		cout << *i << ", ";
	}
	cout << endl << endl << "-----Zadanie2" << endl << endl;
	for (auto i = kolejka_znakow.begin(); i < kolejka_znakow.end(); i++) {
		cout << *i << ", ";
	}
	cout << endl << endl << "-----Zadanie3" << endl << endl;
	front_insert_iterator<deque<char>> start(kolejka_znakow);
	back_insert_iterator<deque<char>> koniec(kolejka_znakow);
	*start = 'F';
	*start = 'G';
	*start = 'H';
	*koniec = 'R';
	*koniec = 'T';
	*koniec = 'Y';
	for (auto i = kolejka_znakow.begin(); i < kolejka_znakow.end(); i++) {
		cout << *i << ", ";
	}
	cout << endl << endl << "-----Zadanie4" << endl << endl;
	copy(kolejka_znakow.begin(), kolejka_znakow.end(), front_inserter(kolejka_znakow));
	for (auto i = kolejka_znakow.begin(); i < kolejka_znakow.end(); i++) {
		cout << *i << ", ";
	}
	cout << endl << endl << "-----Zadanie5" << endl << endl;
	list<Student> lista_studentow;
	lista_studentow.push_back(Student("Balin"));
	lista_studentow.push_back(Student("Dwalin"));
	lista_studentow.push_back(Student("Bifur"));
	lista_studentow.push_back(Student("Bombur"));
	lista_studentow.push_back(Student("Fili"));
	lista_studentow.push_back(Student("Kili"));
	lista_studentow.push_back(Student("Thorin"));
	lista_studentow.push_back(Student("Oin"));
	lista_studentow.push_back(Student("Gloin"));
	lista_studentow.push_back(Student("Dori"));

	vector<Student> wektor_studentow_1(10);
	vector<Student> wektor_studentow_2(10);

	copy(lista_studentow.begin(), lista_studentow.end(), wektor_studentow_1.begin());
	copy(lista_studentow.begin(), lista_studentow.end(), wektor_studentow_2.begin());

	sort(wektor_studentow_1.begin(), wektor_studentow_1.end(), sortowanie_po_nazwiskach);
	sort(wektor_studentow_2.begin(), wektor_studentow_2.end(), sortowanie_po_ocenach);
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
	cout << endl <<  "-----Zadanie6" << endl << endl;
	vector<int> wektor_do_100;
	for (int i = 1; i < 101; i++) {
		wektor_do_100.push_back(i);
	}
	vector<int> wektor_poteg(100);
	transform(wektor_do_100.begin(), wektor_do_100.end(), wektor_poteg.begin(), oblicz_kwadrat);

	for (auto i = wektor_poteg.begin(); i < wektor_poteg.end(); i+=10) {
		cout << *i << ", ";
	}

    return 0;
}

