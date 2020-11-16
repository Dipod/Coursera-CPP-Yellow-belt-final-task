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
		} else if (command == "Del") {
			auto condition = ParseCondition(is);
			auto predicate = [condition](const Date &date,
					const std::string &event) {
				return condition->Evaluate(date, event);
			};
			int count = db.RemoveIf(predicate);
			std::cout << "Removed " << count << " entries" << std::endl;
		} else if (command == "Find") {
			auto condition = ParseCondition(is);
			auto predicate = [condition](const Date &date,
					const std::string &event) {
				return condition->Evaluate(date, event);
			};

			const auto entries = db.FindIf(predicate);
			for (const auto &entry : entries) {
				std::cout << entry << std::endl;
			}
			std::cout << "Found " << entries.size() << " entries" << std::endl;
		} else if (command == "Last") {
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
		std::ostringstream output;

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
		AssertEqual(output.str(), "2017-01-01 Holiday\n"
				"2017-01-01 New Year\n"
				"2017-03-08 Holiday\n", "expected output");
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

		AssertEqual(output.str(), "No entries\n"
				"2017-01-01 Holiday\n"
				"2017-03-08 Holiday\n", "expected output");
	}
}

void TestAddEventAndFind() {
	{
		Database db;
		std::istringstream commands("Add 2017-01-01 Holiday\n"
				"Add 2017-03-08 Holiday\n"
				"Add 2017-01-01 New Year\n"
				"Find event != \"working day\"\n");
		std::stringstream output;

		for (std::string line; getline(commands, line);) {
			std::istringstream is(line);

			std::string command;
			is >> command;
			if (command == "Add") {
				const auto date = ParseDate(is);
				const auto event = ParseEvent(is);
				db.Add(date, event);
			} else if (command == "Find") {
				auto condition = ParseCondition(is);
				auto predicate = [condition](const Date &date,
						const std::string &event) {
					return condition->Evaluate(date, event);
				};

				const auto entries = db.FindIf(predicate);
				for (const auto &entry : entries) {
					output << entry << std::endl;
				}
				output << "Found " << entries.size() << " entries" << std::endl;
			}
		}
		AssertEqual(output.str(), "2017-01-01 Holiday\n"
				"2017-01-01 New Year\n"
				"2017-03-08 Holiday\n"
				"Found 3 entries\n", "expected output");
	}
}

void TestAddEventAndDeleteAndPrintRemaining() {
	{
		Database db;

		std::istringstream commands("Add 2017-06-01 1st of June\n"
				"Add 2017-07-08 8th of July\n"
				"Add 2017-07-08 Someone's birthday\n"
				"Del date == 2017-07-08\n"
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
			} else if (command == "Del") {
				auto condition = ParseCondition(is);
				auto predicate = [condition](const Date &date,
						const std::string &event) {
					return condition->Evaluate(date, event);
				};
				int count = db.RemoveIf(predicate);
				output << "Removed " << count << " entries" << std::endl;
			}
		}
		AssertEqual(output.str(), "Removed 2 entries\n"
				"2017-06-01 1st of June\n", "expected output");
	}
}

void TestFromTaskExample() {
	{
		std::istringstream commands("Add 2017-11-21 Tuesday\n"
				"Add 2017-11-20 Monday\n"
				"Add 2017-11-21 Weekly meeting\n"
				"Print\n"
				"Find event != \"Weekly meeting\"\n"
				"Last 2017-11-30\n"
				"Del date > 2017-11-20\n"
				"Last 2017-11-30\n"
				"Last 2017-11-01");

		std::stringstream output;

		Database db;

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
			} else if (command == "Del") {
				auto condition = ParseCondition(is);
				auto predicate = [condition](const Date &date,
						const std::string &event) {
					return condition->Evaluate(date, event);
				};
				int count = db.RemoveIf(predicate);
				output << "Removed " << count << " entries" << std::endl;
			} else if (command == "Find") {
				auto condition = ParseCondition(is);
				auto predicate = [condition](const Date &date,
						const std::string &event) {
					return condition->Evaluate(date, event);
				};

				const auto entries = db.FindIf(predicate);
				for (const auto &entry : entries) {
					output << entry << std::endl;
				}
				output << "Found " << entries.size() << " entries" << std::endl;
			} else if (command == "Last") {
				try {
					output << db.Last(ParseDate(is)) << std::endl;
				} catch (std::invalid_argument&) {
					output << "No entries" << std::endl;
				}
			}
		}
		AssertEqual(output.str(), "2017-11-20 Monday\n"
				"2017-11-21 Tuesday\n"
				"2017-11-21 Weekly meeting\n"
				"2017-11-20 Monday\n"
				"2017-11-21 Tuesday\n"
				"Found 2 entries\n"
				"2017-11-21 Weekly meeting\n"
				"Removed 2 entries\n"
				"2017-11-20 Monday\n"
				"No entries\n", "expected output");
	}
}

void TestAll() {
	TestRunner tr;
	tr.RunTest(TestParseEvent, "TestParseEvent");
	tr.RunTest(TestParseCondition, "TestParseCondition");
	tr.RunTest(TestAddEventAndPrint, "TestAddEventAndPrint");
	tr.RunTest(TestAddEventAndPrintLast, "TestAddEventAndPrintLast");
	tr.RunTest(TestAddEventAndFind, "TestAddEventAndFind");
	tr.RunTest(TestAddEventAndDeleteAndPrintRemaining,
			"TestAddEventAndDeleteAndPrintRemaining");
	tr.RunTest(TestFromTaskExample, "TestFromTaskExample");
}
