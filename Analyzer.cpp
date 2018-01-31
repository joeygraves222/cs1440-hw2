#include "Analyzer.h"




void Analyzer::analyze(Station* station) {
	std::string lastDay = "";
	int count = 0;
	float totalGag = 0;
	float totalPcp = 0;

	std::cout << "Precipitation Analysis for " << station->getName() << std::endl;

	station->resetDayIteration();
	Day* day;
	while ((day = station->getDayNext()) != nullptr) {
		count++;

		day->resetStatIteration();
		Stat* stat;
		while ((stat = day->getStatNext()) != nullptr) {
			totalGag += stat->getPrecipitationQgag();
			totalPcp += stat->getPrecipitationQpcp();
		}
	}

	if (count == 0) {
		std::cout << "No data points" << std::endl;
	}
	else {
		std::cout << "Total days: " << count << std::endl;
		std::cout << "Total GAG: " << totalGag << std::endl;
		std::cout << "Total PCP: " << totalPcp << std::endl;
		std::cout << "Daily Average GAG: " << totalGag / count << std::endl;
		std::cout << "Daily Average PCP: " << totalPcp / count << std::endl;

		std::cout << std::endl;
	}
}