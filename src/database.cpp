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

int Database::RemoveIf(const std::function<bool(Date, std::string)> &predicate) {
	int DeleteCounter = 0;
	std::vector<Date> DatesForErase;
	for (const auto &day : db) {
		std::vector<std::string> UndeletedEvents;
		for (const auto &event : day.second) {
			if (predicate(day.first, event)) {
				DeleteCounter++;
			} else {
				UndeletedEvents.push_back(event);
			}
		}
		if (UndeletedEvents.size() == 0) {
			DatesForErase.push_back(day.first);
		} else {
			db[day.first] = UndeletedEvents;
		}
	}
	for(const auto &date : DatesForErase){
		db.erase(date);
	}
	return DeleteCounter;
}

std::vector<std::string> Database::FindIf(const std::function<bool(Date, std::string)> &predicate) const {
	std::vector<std::string> result;
	for (const auto &day : db) {
		for (const auto &event : day.second) {
			if (predicate(day.first, event)) {
				std::ostringstream entry;
				entry << day.first << ' ' << event;
				result.push_back(entry.str());
			}
		}
	}
	return result;
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
