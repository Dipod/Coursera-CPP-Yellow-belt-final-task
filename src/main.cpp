#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>

std::string ParseEvent(std::istream &is) {
	std::string line;
	getline(is, line);
	auto pos = line.find_first_not_of(' ');
	return line.substr(pos);
}

void TestAll();

int main() {
	TestAll();

	Database db;

	for (std::string line; getline(std::cin, line);) {
		std::istringstream is(line);

		std::string command;
		is >> command;
		if (command == "Add") {
			const auto date = ParseDate(is);
			const auto event = ParseEvent(is);
			db.Add(date, event);
		} else if (command == "Print") {
			db.Print(std::cout);
		}
//		else if (command == "Del") {
//			auto condition = ParseCondition(is);
//			auto predicate = [condition](const Date &date,
//					const std::string &event) {
//				return condition->Evaluate(date, event);
//			};
//			int count = db.RemoveIf(predicate);
//			std::cout << "Removed " << count << " entries" << std::endl;
//		}
//		else if (command == "Find") {
//			auto condition = ParseCondition(is);
//			auto predicate = [condition](const Date &date,
//					const std::string &event) {
//				return condition->Evaluate(date, event);
//			};
//
//			const auto entries = db.FindIf(predicate);
//			for (const auto &entry : entries) {
//				std::cout << entry << std::endl;
//			}
//			std::cout << "Found " << entries.size() << " entries" << std::endl;
//		}
		else if (command == "Last") {
			try {
				std::cout << db.Last(ParseDate(is)) << std::endl;
			} catch (std::invalid_argument&) {
				std::cout << "No entries" << std::endl;
			}
		} else if (command.empty()) {
			continue;
		} else {
			throw std::logic_error("Unknown command: " + command);
		}
	}

	return 0;
}

void TestParseEvent() {
	{
		std::istringstream is("event");
		AssertEqual(ParseEvent(is), "event",
				"Parse event without leading spaces");
	}
	{
		std::istringstream is("   sport event ");
		AssertEqual(ParseEvent(is), "sport event ",
				"Parse event with leading spaces");
	}
	{
		std::istringstream is("  first event  \n  second event");
		std::vector<std::string> events;
		events.push_back(ParseEvent(is));
		events.push_back(ParseEvent(is));
		AssertEqual(events, std::vector<std::string> { "first event  ",
				"second event" }, "Parse multiple events");
	}
}

void TestAddEventAndPrint() {
	{
		Database db;
		std::istringstream commands("Add 2017-01-01 Holiday\n"
				"Add 2017-03-08 Holiday\n"
				"Add 2017-1-1 New Year\n"
				"Add 2017-1-1 New Year\n"
				"Print");
		std::stringstream output;

		for (std::string line; getline(commands, line);) {
			std::istringstream is(line);

			std::string command;
			is >> command;
			if (command == "Add") {
				const auto date = ParseDate(is);
				const auto event = ParseEvent(is);
				db.Add(date, event);
			} else if (command == "Print") {
				db.Print(output);
			}
		}
		std::string line;
		getline(output, line);
		AssertEqual(line, "2017-01-01 Holiday", "first printed output string");
		getline(output, line);
		AssertEqual(line, "2017-01-01 New Year",
				"second printed output string");
		getline(output, line);
		AssertEqual(line, "2017-03-08 Holiday", "third printed output string");
	}
}

void TestAddEventAndPrintLast() {
	{
		Database db;
		std::istringstream commands("Add 2017-01-01 New Year\n"
				"Add 2017-03-08 Holiday\n"
				"Add 2017-01-01 Holiday\n"
				"Last 2016-12-31\n"
				"Last 2017-01-01\n"
				"Last 2017-06-01");
		std::stringstream output;

		for (std::string line; getline(commands, line);) {
			std::istringstream is(line);

			std::string command;
			is >> command;
			if (command == "Add") {
				const auto date = ParseDate(is);
				const auto event = ParseEvent(is);
				db.Add(date, event);
			} else if (command == "Last") {
				try {
					output << db.Last(ParseDate(is)) << std::endl;
				} catch (std::invalid_argument&) {
					output << "No entries" << std::endl;
				}
			}
		}
		std::string line;
		getline(output, line);
		AssertEqual(line, "No entries", "first printed output string");
		getline(output, line);
		AssertEqual(line, "2017-01-01 Holiday", "second printed output string");
		getline(output, line);
		AssertEqual(line, "2017-03-08 Holiday", "third printed output string");
	}
}

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestParseEvent, "TestParseEvent");
	tr.RunTest(TestParseCondition, "TestParseCondition");
	tr.RunTest(TestAddEventAndPrint, "TestAddEventAndPrint");
	tr.RunTest(TestAddEventAndPrintLast, "TestAddEventAndPrintLast");
}
