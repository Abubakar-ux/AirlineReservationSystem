#pragma once
class Time {
	int hour;
	int minute;

public:
	Time();
	void setHour(int h);
	void setMinute(int min);
	int getHour() const;
	int getMinute() const;
	bool operator ==(const Time& right);
	bool operator <(const Time& right);
	bool operator >(const Time& right);
	Time operator +(const Time& right);
	void displayTime();
	
};

Time::Time() {
	hour = 0;
	minute = 0;
}

void Time::setHour(int h) {
	hour = h;
}

void Time::setMinute(int min) {
	minute = min;
}

int Time::getHour() const {
	return hour;
}

int Time::getMinute() const {
	return minute;
}

bool Time::operator==(const Time& right)
{
	if (this->getMinute() == right.getMinute() && this->getHour() == right.getHour())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Time::operator<(const Time& right)
{
	if (this->getHour() < right.getHour())
	{
		return true;
	}
	else if (this->getHour() == right.getHour())
	{
		if (this->getMinute() < right.getMinute())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool Time::operator>(const Time& right)
{
	if (this->getHour() > right.getHour())
	{
		return true;
	}
	else if (this->getHour() == right.getHour())
	{
		if (this->getMinute() > right.getMinute())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Time::displayTime()
{
	if (hour < 10)
	{
		cout << "0" << hour << ":";
		if (minute < 10)
		{
			cout << "0" << minute;
		}
		else
		{
			cout << minute;
		}
	}
	else
	{
		cout << hour << ":";
		if (minute < 10)
		{
			cout << "0" << minute;
		}
		else
		{
			cout << minute;
		}
	}
}

Time Time::operator+(const Time& right)
{
	this->minute = this->getMinute() + right.getMinute();
	if (this->minute >= 60)
	{
		int addHours = this->minute / 60;
		this->minute = this->minute % 60;
		this->hour = this->hour + addHours;
		this->hour = this->hour + right.hour;
	}
	else
	{
		this->hour = this->hour + right.hour;
	}
	return *this;
}