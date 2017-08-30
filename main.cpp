#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define MAX_DAYS          365
#define MAX_STATS         100
#define MAX_STATION_COUNT 20

class Stat {
	private:
		std::string _time;
		float _precipitationQgag;  // Precipitation volume in 1/100th of (cubic?) inch
		float _precipitationQpcp;  // Precipitation amount in 1/100th of inch

	public:
		Stat(std::string time, std::string qgag, std::string cpcp);
		std::string getTime() { return _time; }
		float getPrecipitationQgag() { return _precipitationQgag; }
		float getPrecipitationQpcp() { return _precipitationQpcp; }
};

class Day {
	private:
		std::string         _date;
		Stat*               _stats[MAX_STATS];
		int                 _statCount = 0;
		int                 _currentStat = -1;

	public:
		Day(std::string date);
		void addStat(Stat *stat);
		void resetStatIteration();
		Stat* getStatNext();
		std::string getDate() { return _date; }
};

bool split(const std::string& s, char delimiter, std::string elements[], int expectedNumberOfElements) {
	std::stringstream ss;
	ss.str(s);
	std::string item;

	int i=0;
	while (std::getline(ss, item, delimiter) && i<expectedNumberOfElements) {
		elements[i++] = item;
	}
	return (i==expectedNumberOfElements);
}

class Station {
	private:
		std::string         _id;
		std::string         _name;
		Day*               _days[MAX_DAYS];
		int                 _dayCount = 0;
		int                 _currentDay = -1;

	public:
		Station(std::string& id, std::string& name);
		void load(std::string& datetime, std::string& qgag, std::string& qpcp);
		void resetDayIteration();
		Day* getDayNext();

		std::string getId() { return _id; }
		std::string getName() { return _name; }

	private:
		Day* findDay(std::string& date);
		Day* addDay(std::string& date);
};

Stat::Stat(std::string time, std::string qgag, std::string qpcp) {
	_time = time;
	_precipitationQgag = stof(qgag);
	if (_precipitationQgag<= -9999 || _precipitationQgag >= 999)
		_precipitationQgag = 0;

	_precipitationQpcp = stof(qpcp);
	if (_precipitationQpcp<= -9999 || _precipitationQpcp >= 999)
		_precipitationQpcp = 0;
}

Day* Station::getDayNext() {
	Day* day = nullptr;
	if (_currentDay < _dayCount)
		day = _days[_currentDay++];
	return day;
}

void Station::load(std::string& datetime, std::string& qgag, std::string& qpcp) {
	std::string dateParts[2];
	if (split(datetime, ' ', dateParts, 2)) {
		Day* day = findDay(dateParts[0]);

		if (day== nullptr)
			day = addDay(dateParts[0]);

		if (day != nullptr) {
			Stat* stat = new Stat(dateParts[1], qgag, qpcp);
			day->addStat(stat);
		}
	}
	else {
		std::cerr << "Invalid date/time << " << datetime << std::endl;
	}
}

class Analyzer {
	public:
		void analyze(Station* station);
};

Day* Station::addDay(std::string& date) {
	Day* day = nullptr;
	if (_dayCount<MAX_DAYS) {
		day = new Day(date);
		_days[_dayCount++] = day;
	}
	else
		std::cerr << "Too many days for station " << getName() << std::endl;

	return day;
}

class Region {
	private:
		Station*        _stations[MAX_STATION_COUNT];
		int             _stationCount=0;
		int             _currentStation;

	public:
		Region();
		void load(std::ifstream &input);
		void resetStationIteration();
		Station* getNextStation();
		Station* findStation(std::string& id);

	private:
		Station* addStation(std::string& id, std::string& name);
};

void Region::load(std::ifstream &input) {
	if (!input.is_open()) {
		std::cout << "Input file is not open" << std::endl;
		return;
	}
	std::string line;
	std::getline(input, line);

	while (!input.eof()) {
		std::getline(input, line);

		std::string fields[5];
		if (split(line, ',', fields, 5)) {
			Station* station = findStation(fields[0]);

			if (station== nullptr)
				station = addStation(fields[0], fields[1]);

			if (station != nullptr)
				station->load(fields[2], fields[3], fields[4]);
		}
		else {
			std::cout << "Invalid stats line << " << line << std::endl;
		}
	}
}

void Analyzer::analyze(Station* station) {
	std::string lastDay = "";
	int count = 0;
	float totalGag = 0;
	float totalPcp = 0;

	std::cout << "Precipitation Analysis for " << station->getName() << std::endl;

	station->resetDayIteration();
	Day* day;
	while ((day=station->getDayNext()) != nullptr) {
		count++;

		day->resetStatIteration();
		Stat* stat;
		while ((stat = day->getStatNext()) != nullptr) {
			totalGag += stat->getPrecipitationQgag();
			totalPcp += stat->getPrecipitationQpcp();
		}
	}

	if (count==0) {
		std::cout << "No data points" << std::endl;
	}
	else {
		std::cout << "Total days: " << count << std::endl;
		std::cout << "Total GAG: " << totalGag << std::endl;
		std::cout << "Total PCP: " << totalPcp << std::endl;
		std::cout << "Daily Average GAG: " << totalGag/count << std::endl;
		std::cout << "Daily Average PCP: " << totalPcp/count << std::endl;

		std::cout << std::endl;
	}
}

Station* Region::addStation(std::string &id, std::string &name) {
	Station* station = nullptr;

	if (_stationCount<MAX_STATION_COUNT) {
		station = new Station(id, name);
		_stations[_stationCount++] = station;
	}
	else
		std::cerr << "To many stations -- Cannot add station " << id << ", " << name << std::endl;

	return station;
}

Day::Day(std::string date) {
	_date = date;
}

void Region::resetStationIteration() {
	_currentStation = 0;
}

Stat* Day::getStatNext() {
	Stat* stat = nullptr;
	if (_currentStat < _statCount)
		stat = _stats[_currentStat++];
	return stat;
}

Station::Station(std::string& id, std::string& name) {
	_id = id;
	_name = name;
}

void Day::addStat(Stat *stat) {
	if (_statCount<MAX_STATS)
		_stats[_statCount++] = stat;
	else
		std::cerr << "Too many stats for " << getDate() << std::endl;
}

Station* Region::findStation(std::string &id) {
	Station* station = nullptr;

	for (int i=0; i<_stationCount && station==nullptr; i++) {
		if (_stations[i]->getId().compare(id)==0)
			station = _stations[i];
	}

	return station;
}

int main(int argc, char* argv[]) {
	if (argc>1) {
		std::ifstream inputStream(argv[1], std::ios_base::in);

		Region region;
		region.load(inputStream);

		Analyzer analyzer;

		if (argc>2) {
			std::string stationId(argv[2]);
			Station* station = region.findStation(stationId);
			if (station!= nullptr)
				analyzer.analyze(station);
		}
		else {
			region.resetStationIteration();
			Station *station;
			while ((station = region.getNextStation()) != nullptr)
				analyzer.analyze(station);
		}
	}

	return 0;
}

void Day::resetStatIteration() {
	_currentStat = 0;
}

Station* Region::getNextStation() {
	Station* stat = nullptr;
	if (_currentStation < _stationCount)
		stat = _stations[_currentStation++];
	return stat;
}

Day* Station::findDay(std::string& date) {
	Day* day = nullptr;

	for (int i=0; i<_dayCount && day==nullptr; i++) {
		if (_days[i]->getDate().compare(date)==0)
			day = _days[i];
	}

	return day;
}

Region::Region() { }

void Station::resetDayIteration() {
	_currentDay = 0;
}
