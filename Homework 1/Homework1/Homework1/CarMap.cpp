#include "CarMap.h"
#include <string>
#include <iostream>

using namespace std;

// If a car with the given license plate is not currently in the map, 
// and there is room in the map, add an entry for that car recording
// that it has 0 gallons of gas in it, and return true.  Otherwise,
// make no change to the map and return false.
bool CarMap::addCar(std::string license)
{
	return cars_.insert(license, 0);
}

// If a car with the given license plate is in the map, return the
// number of gallons of gas in its tank; otherwise, return -1.
double CarMap::gas(std::string license) const
{
	double gasVolume = -1;
	cars_.get(license, gasVolume);
	return gasVolume;
}

// If no car with the given license plate is in the map or if
// gallons is negative, make no change to the map and return
// false.  Otherwise, increase the number of gallons of gas in the
// indicated car by the gallons parameter and return true.
bool CarMap::addGas(std::string license, double gallons)
{
	if (gallons < 0)
		return false;
	return cars_.update(license, gas(license) + gallons);
}

// If no car with the given license plate is in the map or if
// gallons is negative or if gallons > gas(), make no change to the
// map and return false.  Otherwise, decrease the number of gallons
// of gas in the indicated car by the gallons parameter and return
// true.
bool CarMap::useGas(std::string license, double gallons)
{
	if (gallons > gas(license) || gallons < 0)
		return false;
	return cars_.update(license, gas(license) - gallons);
}

// Return the number of cars in the CarMap.
int CarMap::fleetSize() const
{
	return cars_.size();
}  

// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of gallons in that car's tank.  Write
// no other text.
void CarMap::print() const
{
	string license;
	double gallons;
	for (int i = 0; i < cars_.size(); i++)
	{
		cars_.get(i, license, gallons);
		cout << license << " " << gallons << endl;
	}
}
