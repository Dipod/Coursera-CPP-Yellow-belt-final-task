#include "database.h"

void Database::Add(const Date &date, const std::string &event) {
	if (db[date].second.count(event) == 0) {
		db[date].first.push_back(event);
		db[date].second.insert(event);
	}
}

void Database::Print(std::ostream &output) const {
	for (const auto &date : db) {
		for (const auto &event : date.second.first) {
			output << date.first << ' ' << event << std::endl;
		}
	}
}

int Database::RemoveIf(
		const std::function<bool(Date, std::string)> &predicate) {
	size_t DeleteCounter = 0;
	std::vector<Date> DatesForErase;

	for (auto &day : db) {

		auto it = std::stable_partition(day.second.first.begin(),
				day.second.first.end(), [&predicate, &day](const auto &event) {
					return !predicate(day.first, event);
				});

		if (it == day.second.first.begin()) {
			DatesForErase.push_back(day.first);
			DeleteCounter += day.second.first.size();
		} else {
			int numberOfDeletions = day.second.first.end() - it;
			DeleteCounter += numberOfDeletions;
			for (int i = 0; i < numberOfDeletions; i++) {
				day.second.second.erase(day.second.first.back());
				day.second.first.pop_back();
			}
		}
	}

	for (const auto &date : DatesForErase) {
		db.erase(date);
	}

	return DeleteCounter;
}

std::vector<std::string> Database::FindIf(
		const std::function<bool(Date, std::string)> &predicate) const {
	std::vector<std::string> result;
	for (const auto &day : db) {
		for (const auto &event : day.second.first) {
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

	if (it->second.first.size() == 0u) {
		throw(std::invalid_argument("No entries"));
	} else {
		std::ostringstream result;
		result << it->first << ' ' << it->second.first.back();
		return result.str();
	}
}
