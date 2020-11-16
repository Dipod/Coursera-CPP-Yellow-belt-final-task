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
//	int RemoveIf(auto &predicate) {
//		return 0;
//	}
//	auto FindIf(auto &predicate){
//	}
	std::string Last(const Date &date) const;
private:
	std::map<Date, std::vector<std::string>> db;
};
