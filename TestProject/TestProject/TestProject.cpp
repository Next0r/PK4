#pragma once
//biblioteki
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <time.h>
#include <vector>
#include <map>
#include <math.h>

using namespace std;

double oblicz_dlugosc_wektora(pair<int, int> A, pair<int, int> B) {
	double dlugosc;
	double r1 = (B.first - A.first)*(B.first - A.first);
	double r2 = (B.second - A.second)*(B.second - A.second);
	dlugosc = sqrt(r1 + r2);
	return dlugosc;
}

double zwroc_pozycje_wroga_wzg_wiezy(pair<int, int> wieza, pair<int, int> wrog) {
	

	double p_prost = oblicz_dlugosc_wektora(wieza, wrog);
	cout << p_prost << endl;
	double r2 = fabs(wrog.second) - fabs(wieza.second);
	cout << r2 << endl;
	double w_sin = fabs(r2) / p_prost;
	cout << w_sin << endl;
	if (wieza.second <= wrog.second) {
		// wrog w dolnej polowie
		if (wieza.first >= wrog.first) {
			// wrog w 3 cwiertce
			return asin(w_sin) * 180 / PI + 180;
		}
		else {
			// wrog w 4 cwiartce
			return asin(w_sin) * 180 / PI + 270;
		}
	}
	else {
		// wrog w gornej polowie
		if (wieza.first >= wrog.first) {
			// wrog w 2 cwiertce
			return asin(w_sin) * 180 / PI + 90;
		}
		else {
			// wrog w 1 cwiartce
			return asin(w_sin) * 180 / PI;
		}
	}
}

int main() {
	cout << zwroc_pozycje_wroga_wzg_wiezy(make_pair(300, 200), make_pair(550, 600));

}