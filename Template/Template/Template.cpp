// Template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <typeinfo>

using namespace std;

class baza {
private:
	int size1;
	int size2;
	int *tablica_i;
	float *tablica_f;
public:
	baza(int tab1[], float tab2[], int s1, int s2) {
		size1 = s1;
		size2 = s2;
		
		tablica_i = new int[size1];
		tablica_f = new float[size2];
		
		for (int i = 0; i < size1; i++) {
			tablica_i[i] = tab1[i];
		}
		for (int i = 0; i < size2; i++) {
			tablica_f[i] = tab2[i];
		}	
	}

	void wypisz() {
		for (int i = 0; i < size1; i++) {
			cout << tablica_i[i] << endl;
		}
		cout << "-----" << endl;
		for (int i = 0; i < size2; i++) {
			cout << tablica_f[i] << endl;
		}
	}

	~baza() {};
};

template<typename t1>
class b_dowolna {
protected:
	int size;
	t1 *tablica;
public:
	b_dowolna(t1 tab1[], int s1) {
		size = s1;
		tablica = new t1[size];
		for (int i = 0; i < size; i++) {
			tablica[i] = tab1[i];
		}
	}

	void wypisz() {
		for (int i = 0; i < size; i++) {
			cout << tablica[i] << endl;
		}
	}

	~b_dowolna() {};
};

template <typename t1, typename t2>
class b_dowolna2 : protected b_dowolna<t1> {
private:
	int size2;
	t2 *tablica2;
public:
	b_dowolna2(t1 tab1[], t2 tab2[], int s1, int s2) : b_dowolna(tab1, s1) {
		size2 = s2;
		tablica2 = new t2[size2];
		for (int i = 0; i < size2; i++) {
			tablica2[i] = tab2[i];
		}
	}

	void wypisz_wszystko() {
		wypisz();
		cout << "----" << endl;
		for (int i = 0; i < size2; i++) {
			cout << tablica2[i] << endl;
		}
	}

	~b_dowolna2() {};
};

template <typename t1>
class b_dowolna_mod : public b_dowolna<t1> {
public:
	b_dowolna_mod(t1 tab1[], int s1) : b_dowolna(tab1, s1) {};

	template<typename t2>
	void pobierz(t2 zmienna) {
		if (typeid(t2) == typeid(t1)) {
			tablica[0] = zmienna;
		}
		else {
			cout << "podano niewlasciwy typ zmiennej" << endl;
		}
	}
};




int main()
{
	const int s1 = 5;
	const int s2 = 3;
	int tab1[s1] = { 1,2,3,4,5 };
	float tab2[s2] = { 3.275,1.76,3.98 };

	baza *db1 = new baza(tab1, tab2, s1, s2);
	db1->wypisz();
	cout << "-----" << endl;

	b_dowolna<int> *db2 =  new b_dowolna<int>(tab1, s1);
	b_dowolna<float> *db3 = new b_dowolna<float>(tab2, s2);
	db2->wypisz();
	cout << "-----" << endl;
	db3->wypisz();
	cout << "-----" << endl;
	
	b_dowolna2<int, float> *db4 = new b_dowolna2<int, float>(tab1, tab2, s1, s2);
	db4->wypisz_wszystko();
	cout << "-----" << endl;

	b_dowolna_mod<float> *db5 = new b_dowolna_mod<float>(tab2, s2);
	db5->pobierz((bool)false); // nie mozna wpisac false do tablicy zmiennoprzecinkowej
	cout << "-----" << endl;
	db5->pobierz((float)13.92);
	db5->wypisz();
	cout << "-----" << endl;

    return 0;
}

