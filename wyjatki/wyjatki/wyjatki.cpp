// wyjatki.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <random>
#include <time.h>
#include <typeinfo>


using namespace std;

default_random_engine eng(time(0));
uniform_int_distribution<int> dis(1, 3);
uniform_int_distribution<int> dis2(100000, 999999);
class AwariaSamochodu {
protected:
	int nrPolisy;
public:
	virtual void naprawa() {
		cout << nrPolisy << endl;
	}
	virtual ~AwariaSamochodu() {};
	AwariaSamochodu(int l1) {
		nrPolisy = l1;
	}
};

class AwariaSilnika : public AwariaSamochodu {
protected:
	int nrSerial;
public:
	void naprawa() {
		cout << nrPolisy << ", " << nrSerial << endl;
	}
	virtual ~AwariaSilnika() {};
	AwariaSilnika(int l1, int l2) : AwariaSamochodu(l1) {
		nrSerial = l2;
	}
};

class AwariaSwiecy : public AwariaSilnika {
protected:
	int rozmiarKlucza;
public:
	void naprawa() {
		cout << nrPolisy << ", " << nrSerial << ", " << rozmiarKlucza << endl;
	}
	virtual ~AwariaSwiecy() {};
	AwariaSwiecy(int l1, int l2, int l3) : AwariaSilnika(l1, l2) {
		rozmiarKlucza = l3;
	}
};


int main()
{
	const int size = 10;
	AwariaSamochodu *tab[size];
	for (int i = 0; i < size; i++) {
		int losowa = dis(eng);

		switch (losowa)
		{
		case 1:
			tab[i] = new AwariaSamochodu(dis2(eng));
			break;
		case 2:
			tab[i] = new AwariaSilnika(dis2(eng), dis2(eng));
			break;
		case 3:
			tab[i] = new AwariaSwiecy(dis2(eng), dis2(eng), dis2(eng));
			break;
		}
	}

	for (int i = 0; i < size; i++) {
		cout << typeid(*tab[i]).name() << endl;
	}

	cout << endl;

	for (int i = 0; i < size; i++) {
		try {
			if (typeid(*tab[i]) == typeid(AwariaSamochodu)) {
				throw AwariaSamochodu(1);
			}
			else if (typeid(*tab[i]) == typeid(AwariaSilnika)) {
				throw AwariaSilnika(1, 2);
			}
			else if (typeid(*tab[i]) == typeid(AwariaSwiecy)) {
				throw AwariaSwiecy(1, 2, 3);
			}
		
		}
		catch(AwariaSamochodu &exception){
			tab[i]->naprawa();
		}
		catch (AwariaSilnika &exception) {
			tab[i]->naprawa();
		}
		catch (AwariaSwiecy &exception) {
			tab[i]->naprawa();
		}
	}

	cout << endl;

	try {
		try {
			try {
				throw AwariaSwiecy(1, 2, 3);
			}
			catch (AwariaSwiecy &exception) {
				cout << "awaria swiecy" << endl;
				throw;
			}
		}
		catch (AwariaSilnika &exception) {
			cout << "awaria silnika" << endl;
			throw;
		}
	}
	catch (AwariaSamochodu &exception) {
		cout << "awaria samochodu" << endl;
	}

	cout << endl;

	for (int i = 0; i < 5; i++) {
		try {
			if (i == 0) {
				throw AwariaSamochodu(1);
			}
			else if (i == 1) {
				throw AwariaSilnika(1, 2);
			}
			else if (i == 2) {
				throw AwariaSwiecy(1, 2, 3);
			}
			else if (i == 3) {
				throw 22;
			}
			else {
				throw "blad";
			}
		}
		catch (AwariaSilnika &exception) {
			cout << "awaria silnika" << endl;
		}
		catch (AwariaSamochodu &exception) {
			cout << "awaria samochodu" << endl;
		}
		catch (int exception) {
			cout << "int" << endl;
		}
		catch (AwariaSwiecy &exception) {
			cout << "awaria swiecy" << endl;
		}
		catch (...) {
			cout << "..." << endl;
		}
	}

	cout << endl;

	for (int i = 0; i < 5; i++) {
		try {
			if (i == 0) {
				throw AwariaSamochodu(1);
			}
			else if (i == 1) {
				throw AwariaSilnika(1, 2);
			}
			else if (i == 2) {
				throw AwariaSwiecy(1, 2, 3);
			}
			else if (i == 3) {
				throw 22;
			}
			else {
				throw "blad";
			}
		}
		catch (AwariaSwiecy &exception) {
			cout << "awaria swiecy" << endl;
		}
		catch (AwariaSilnika &exception) {
			cout << "awaria silnika" << endl;
		}
		catch (AwariaSamochodu &exception) {
			cout << "awaria samochodu" << endl;
		}
		catch (int exception) {
			cout << "int" << endl;
		}
		catch (...) {
			cout << "..." << endl;
		}
	}
    return 0;
}

