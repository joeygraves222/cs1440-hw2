#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Region.h"
#include "Analyzer.h"
#include "Station.h"

int main(int argc, char* argv[]) {
	if (argc>1) {
		std::ifstream inputStream(argv[1], std::ios_base::in);

		Region region;
		region.load(inputStream);

		Analyzer analyzer;

		if (argc>2) {
			std::string stationId(argv[2]);
			Station* station = region.findStation(stationId);
			if (station != nullptr)
				analyzer.analyze(station);
		}
		else {
			region.resetStationIteration();
			Station *station;
			while ((station = region.getNextStation()) != nullptr)
				analyzer.analyze(station);
		}
	}
	else if (argc == 1) {
		std::cout << "Please pass arguments into the program" << std::endl;
	}

	return 0;
}

