#include "CarMap.h"

#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	CarMap c;
	c.addCar("Ford");
	c.addCar("Honda");
	cout << c.gas("Ford") << endl;
	cout << c.gas("adsf") << endl;
	cout << c.addGas("Ford", 20) << endl;
	cout << c.addGas("Ford", -10) << endl;
	cout << c.fleetSize() << endl;
	c.print();
	cout << c.useGas("Ford", 10) << endl;
	cout << c.useGas("Honda", 10) << endl;
	c.print();


}
