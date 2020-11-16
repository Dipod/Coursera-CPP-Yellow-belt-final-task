#include "date.h"

std::tuple<int, int, int> Date::Comparison_Key() const{
	return std::make_tuple(year, month, day);
}

std::ostream& operator<<(std::ostream &stream, const Date &date) {
	stream << std::setfill('0') << std::setw(4) << date.year << '-'
			<< std::setw(2) << date.month << '-' << std::setw(2) << date.day;
	return stream;
}

bool operator<(const Date &lhs, const Date &rhs) {
	return lhs.Comparison_Key() < rhs.Comparison_Key();
}

bool operator<=(const Date &lhs, const Date &rhs) {
	return lhs.Comparison_Key() <= rhs.Comparison_Key();
}

bool operator>(const Date &lhs, const Date &rhs) {
	return lhs.Comparison_Key() > rhs.Comparison_Key();
}

bool operator>=(const Date &lhs, const Date &rhs) {
	return lhs.Comparison_Key() >= rhs.Comparison_Key();
}

bool operator==(const Date &lhs, const Date &rhs) {
	return lhs.Comparison_Key() == rhs.Comparison_Key();
}

bool operator!=(const Date &lhs, const Date &rhs) {
	return lhs.Comparison_Key() != rhs.Comparison_Key();
}

Date ParseDate(std::istringstream &ss_date) {

	int year, month, day;
	char separator1, separator2;

	ss_date >> year >> separator1 >> month >> separator2 >> day;

	if (ss_date.fail() || separator1 != '-' || separator2 != '-') {
		throw std::invalid_argument("Wrong date format");
	} else {
		if (month < 1 || month > 12) {
			throw std::invalid_argument(
					"Month value is invalid: " + std::to_string(month));
		}

		if (day < 1 || day > 31) {
			throw std::invalid_argument(
					"Day value is invalid: " + std::to_string(day));
		}
	}

	return Date(year, month, day);
}
