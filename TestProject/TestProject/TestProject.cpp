#pragma once
//biblioteki
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>
#include <vector>
#include <map>
#include <math.h>

using namespace std;

string itos(int liczba) {
	int licznik = 0;
	int odjemnik = 10;
	string zwracany = "";
	int dlugosc_liczby = 1;

	while ((liczba - odjemnik) > 0) {
		for (int i = 0; i < licznik; i++) {
			odjemnik *= 10;
		}
		if ((liczba - odjemnik) > 0) {
			dlugosc_liczby++;
			break;
		}
		licznik++;
	}


}
int main() {


}