#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <functional>
#include <tuple>
#include <set>
#include "date.h"

class Database {
public:
	void Add(const Date &date, const std::string &event);
	void Print(std::ostream &output) const;
	int RemoveIf(const std::function<bool(Date, std::string)> &predicate);
	std::vector<std::string> FindIf(
			const std::function<bool(Date, std::string)> &predicate) const;
	std::string Last(const Date &date) const;
private:
	std::map<Date, std::pair<std::vector<std::string>, std::set<std::string>>> db;
};
