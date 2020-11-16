#pragma once

#include <sstream>
#include <iomanip>
#include <tuple>

class Date {
public:
	Date(const int &Year, const int &Month, const int &Day) :
			year(Year), month(Month), day(Day) {
	}
	std::tuple<int, int, int> Comparison_Key() const;
	const int year;
	const int month;
	const int day;
};

std::ostream& operator<<(std::ostream &stream, const Date &date);

bool operator<(const Date &lhs, const Date &rhs);

bool operator<=(const Date &lhs, const Date &rhs);

bool operator>(const Date &lhs, const Date &rhs);

bool operator>=(const Date &lhs, const Date &rhs);

bool operator==(const Date &lhs, const Date &rhs);

bool operator!=(const Date &lhs, const Date &rhs);

Date ParseDate(std::istringstream &ss_date);
