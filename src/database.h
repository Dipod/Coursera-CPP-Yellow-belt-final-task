#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include "date.h"

class Database {
public:
	void Add(const Date &date, const std::string &event);
	void Print(std::ostream &output) const;

	template<typename T>
	int RemoveIf(T &predicate) {
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

	template<typename T>
	std::vector<std::string> FindIf(T &predicate) const {
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
	std::string Last(const Date &date) const;
private:
	std::map<Date, std::vector<std::string>> db;
};
