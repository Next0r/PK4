// wyr_reg_01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <regex>

using namespace std;

void f1(string &s, regex r) {
	do {
		cin >> s;
	} while (!regex_match(s, r));
}


int main()
{
	// zadanie 1
	cout << "Zadanie1" << endl << endl;
	string zmienna1;
	regex r1("[A-Z][a-z]+");
	regex r2("\\d{2}-\\d{3}");
	regex r3(".*[A-Z]{1,}.*[0-9]{1,}.*|.*[0-9]{1,}.*[A-Z]{1,}.*");
	cout << "Podaj imie" << endl;
	f1(zmienna1, r1);
	cout << "OK" << endl;
	cout << "Podaj kod pocztowy" << endl;
	f1(zmienna1, r2);
	cout << "OK" << endl;
	cout << "Podaj haslo" << endl;
	f1(zmienna1, r3);
	cout << "OK" << endl;
	cout << endl << endl;

	// zadanie 2
	cout << "Zadanie2" << endl << endl;
	regex r4("([a-zA-Z]+)[a-zA-Z]\\1");
	regex r5("(\\d{4}-\\d{2}-\\d{2})|(\\d{2}-\\d{2}--\\d{4})");
	regex r6("plik.*\.pdf");
	regex r7("(a{2,}).+\\1");

	zmienna1 = "owocowo";
	if (regex_search(zmienna1, r4)); {
		cout << "palindrom dziala" << endl;
	}

	zmienna1 = "2012-12-12";
	if (regex_search(zmienna1, r5)); {
		cout << "data dziala" << endl;
	}

	zmienna1 = "plik_blend.pdf";
	if (regex_search(zmienna1, r6)); {
		cout << "plik dziala" << endl;
	}

	zmienna1 = "aa_cos_tam_aa";
	if (regex_search(zmienna1, r7)); {
		cout << "tyle samo aa dziala" << endl;
	}
	cout << endl << endl;

	// zadanie 3
	cout << "Zadanie3" << endl << endl;
	string wynik;
	zmienna1 = "<a>kwiatek<a>";
	regex r8("(<([a-zA-Z]+)>.+)<\\2>");
	regex_replace(back_inserter(wynik), zmienna1.begin(), zmienna1.end(), r8, "$1</$2>");
	cout << wynik << endl;
	wynik.clear();
	zmienna1 = "cout << zmienna << endl";
	regex r9("\\n|endl");
	regex_replace(back_inserter(wynik), zmienna1.begin(), zmienna1.end(), r9, "<br />");
	cout << wynik << endl;

	cout << endl << endl;
	// zadanie 4
	cout << "Zadanie4" << endl << endl;
	zmienna1 = "include string using namespace std string zmienna wartosc int main return 0";
	int liczba = 6;
	regex r10("(\\S+)");

	auto iterator = sregex_iterator(zmienna1.begin(), zmienna1.end(), r10);
	sregex_iterator end;

	for (auto i = iterator; i != end; i++) {
		smatch match = *i;
		if (match.str().size() > liczba) {
			cout <<  "Znalezione slowo: " << match.str() << endl;
		}
	}
	cout << endl << endl;




    return 0;
}

