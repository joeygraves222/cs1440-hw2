#ifndef STATION_H
#define STATION_H
#include <iostream>
#include "Day.h"

#define MAX_DAYS 365

class Station {
private:
	std::string         _id;
	std::string         _name;
	Day*               _days[MAX_DAYS];
	int                 _dayCount = 0;
	int                 _currentDay = -1;

public:
	Station(std::string& id, std::string& name);
	Station();
	void load(std::string& datetime, std::string& qgag, std::string& qpcp);
	void resetDayIteration();
	Day* getDayNext();

	std::string getId() { return _id; }
	std::string getName() { return _name; }

private:
	Day* findDay(std::string& date);
	Day* addDay(std::string& date);
};

	


#endif