// wyr_reg2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <regex>

using namespace std;

void wpisz(string &s, regex r) {
	do {
		cin >> s;
	} while (!regex_match(s, r));
}

int main()
{
	cout << endl << endl << "Zadanie 1" << endl << endl;
	string slowo;
	regex imie("[A-Z][a-z]+");
	regex kod("[0-9]{2}-[0-9]{3}");
	regex haslo(".*[A-Z]+.*[0-9]+.*|.*[0-9]+.*[A-Z]+.*");
	cout << "Wpisz imie: " << endl;
	wpisz(slowo, imie);
	cout << "OK" << endl;
	cout << "Wpisz kod: " << endl;
	wpisz(slowo, kod);
	cout << "OK" << endl;
	cout << "Wpisz haslo: " << endl;
	wpisz(slowo, haslo);
	cout << "OK" << endl;
	
	cout << endl << endl << "Zadanie 2" << endl << endl;

	//regex palindrom("([a-zA-Z]+)[a-zA-Z]\\1");
	//slowo = "kajak";
	//if (regex_match(slowo, palindrom)) {
	//	cout << "To jest palindrom: " << slowo << endl;
	//}
	//else {
	//	cout << "To nie jest palidrom: " << slowo << endl;
	//}

	//slowo = "drzewo";
	//if (regex_match(slowo, palindrom)) {
	//	cout << "To jest palindrom: " << slowo << endl;
	//}
	//else {
	//	cout << "To nie jest palidrom: " << slowo << endl;
	//}
	
	regex data("(0[1-9]|[0-2][1-9]|3[0-1])-(1[1-2]|0[1-9])-([0-9]+)");
	slowo = "31-03-1999";

	if (regex_match(slowo, data)) {
		cout << "To jest data: " << slowo << endl;
	}
	else {
		cout << "To nie jest data: " << slowo << endl;
	}

	slowo = "35-01-1999";
	if (regex_match(slowo, data)) {
		cout << "To jest data: " << slowo << endl;
	}
	else {
		cout << "To nie jest data: " << slowo << endl;
	}
	

	regex plik("plik.*(\.pdf)");
	slowo = "plik.pdf";


	if (regex_match(slowo, plik)) {
		cout << "To jest plik pdf : " << slowo << endl;
	}
	else {
		cout << "To nie jest plik pdf : " << slowo << endl;
	}

	slowo = "plik.txt";

	if (regex_match(slowo, plik)) {
		cout << "To jest plik pdf : " << slowo << endl;
	}
	else {
		cout << "To nie jest plik pdf : " << slowo << endl;
	}
	

	regex wiecej_a("(a{2,}).+\\1");

	slowo = "aabbaa";
	if (regex_match(slowo, wiecej_a)) {
		cout << "To jest symetryczne wyrazenie z a: " << slowo << endl;
	}
	else {
		cout << "To nie jest symetryczne wyrazenie z a: " << slowo << endl;
	}

	slowo = "wyraz";
	if (regex_match(slowo, wiecej_a)) {
		cout << "To jest symetryczne wyrazenie z a: " << slowo << endl;
	}
	else {
		cout << "To nie jest symetryczne wyrazenie z a: " << slowo << endl;
	}

	cout << endl << endl << "Zadanie 3" << endl << endl;

	regex znacznik("(<([a-zA-Z])>.+)(\\2)");
	slowo = "<a> link <a>";
	string wyjscie;

	cout << "Znacznik testowy: " << slowo << endl;

	regex_replace(back_inserter(wyjscie), begin(slowo), end(slowo), znacznik, "$1/$2");

	cout << "Znacznik wyjsciowy: " << wyjscie << endl;

	wyjscie.clear();
	cout << endl;

	regex keyword("(.*) (endl)|(\n)");
	slowo = "cout << zmienna << endl";

	cout << "Linia testowa: " << slowo << endl;

	regex_replace(back_inserter(wyjscie), begin(slowo), end(slowo), keyword, "$1 </br>");

	cout << "Linia wyjsciowa: " << wyjscie << endl;

	cout << endl << endl << "Zadanie 4" << endl << endl;

	string zdanie = "pisanie regeksow wcale nie jest fajne :(";
	int dlugosc = 5;

	regex reg("([a-zA-Z]+)");

	auto iterator = sregex_iterator(zdanie.begin(), zdanie.end(), reg);
	sregex_iterator koniec;

	cout << "Zdanie wejsciowe: " << zdanie << endl;
	cout << "Szukam slowa wiekszego od: " << dlugosc << endl;

	for (auto i = iterator; i != koniec; i++) {
		smatch match = *i;
		if (match.str().size() > dlugosc) {
			cout << "Znalezione slowo: " << match.str() << endl;
		}
	}
	cout << endl << endl;
	




    return 0;
}

