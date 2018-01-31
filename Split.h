#ifndef SPLIT_H
#define SPLIT_H

#include <iostream>
#include <sstream>


bool split(const std::string& s, char delimiter, std::string elements[], int expectedNumberOfElements) {
	std::stringstream ss;
	ss.str(s);
	std::string item;

	int i = 0;
	while (std::getline(ss, item, delimiter) && i<expectedNumberOfElements) {
		elements[i++] = item;
	}
	return (i == expectedNumberOfElements);
}

#endif