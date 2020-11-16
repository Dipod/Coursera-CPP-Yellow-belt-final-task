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
		return 0;
	}

	template<typename T>
	std::vector<std::string> FindIf(T &predicate) const {
		std::vector<std::string> result;
		for(const auto &day : db){
			for(const auto &event : day.second){
				if(predicate(day.first, event)){
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
