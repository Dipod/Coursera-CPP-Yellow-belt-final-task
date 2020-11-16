#include "database.h"

void Database::Add(const Date &date, const std::string &event) {
	for (auto &e : db[date]) {
		if (e == event) {
			return;
		}
	}
	db[date].push_back(event);
}

void Database::Print(std::ostream &output) const {
	for (const auto &date : db) {
		for (const auto &event : date.second) {
			output << date.first << ' ' << event << std::endl;
		}
	}
}

std::string Database::Last(const Date &date) const {
	auto it = db.lower_bound(date);
	std::stringstream result;
	if (it->first != date) {
		if (it == db.begin()) {
			throw(std::invalid_argument("No entries"));
		} else {
			it = prev(it);
		}
	}

	if (it->second.size() == 0u) {
		throw(std::invalid_argument("No entries"));
	} else {
		std::ostringstream result;
		result << it->first << ' ' << it->second.back();
		return result.str();
	}
}
