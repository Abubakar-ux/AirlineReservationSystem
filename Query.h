#pragma once
#include <iostream>
#include "Date.h"
using namespace std;

class Query {
	string origin;
	string destination;
	string airline;
	int day;

public:
	Query();
	void setOrigin(string source);
	void setDestination(string dest);
	void setAirline(string line);
	void setDay(int d);
	int getDay() const;
	string getOrigin() const;
	string getDestination() const;
	string getAirline() const;
};

Query::Query() {
	origin = "";
	destination = "";
}

void Query::setOrigin(string source) {
	origin = source;
}

void Query::setDestination(string dest) {
	destination = dest;
}

void Query::setAirline(string line) {
	airline = line;
}

string Query::getOrigin() const {
	return origin;
}

string Query::getDestination() const {
	return destination;
}

string Query::getAirline() const {
	return airline;
}

void Query::setDay(int d) {
	day = d;
}

int Query::getDay() const {
	return day;
}

