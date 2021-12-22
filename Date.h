#pragma once
class Date {
	int day;
	int month;
	int year;

public:
	Date();
	void setDay(int d);
	void setMonth(int m);
	void setYear(int y);
	int getDay() const;
	int getMonth() const;
	int getYear() const;
	bool operator==(const Date& right);
	bool operator <(const Date& right);
	bool operator >(const Date& right);
};

Date::Date() {
	day = 0;
	month = 0;
	year = 0;
}

void Date::setDay(int d) {
	day = d;
}

void Date::setMonth(int m) {
	month = m;
}

void Date::setYear(int y) {
	year = y;
}

int Date::getDay() const {
	return day;
}

int Date::getMonth() const {
	return month;
}

int Date::getYear() const {
	return year;
}

bool Date::operator==(const Date& right)
{
	if (this->year == right.getYear() && this->month == right.getMonth() && this->day == right.getDay())
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Date::operator<(const Date& right)
{
	if (this->day < right.getDay())
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Date::operator>(const Date& right)
{
	if (this->day > right.getDay())
	{
		return true;
	}
	else
	{
		return false;
	}
}