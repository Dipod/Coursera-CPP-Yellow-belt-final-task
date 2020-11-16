#include "condition_parser.h"
#include "test_runner.h"

#include <sstream>

void TestParseCondition() {
	{
		std::istringstream is("date != 2017-11-18");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 1");
		Assert(!root->Evaluate( { 2017, 11, 18 }, ""), "Parse condition 2");
	}
	{
		std::istringstream is("date < 2017-11-18");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 3");
		Assert(!root->Evaluate( { 2017, 11, 18 }, ""), "Parse condition 4");
		Assert(!root->Evaluate( { 2017, 11, 19 }, ""), "Parse condition 5");
	}
	{
		std::istringstream is("date <= 2017-11-18");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 6");
		Assert(root->Evaluate( { 2017, 11, 18 }, ""), "Parse condition 7");
		Assert(!root->Evaluate( { 2017, 11, 19 }, ""), "Parse condition 8");
	}
	{
		std::istringstream is("date > 2017-11-18");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 9");
		Assert(!root->Evaluate( { 2017, 11, 18 }, ""), "Parse condition 10");
		Assert(root->Evaluate( { 2017, 11, 19 }, ""), "Parse condition 11");
	}
	{
		std::istringstream is("date >= 2017-11-18");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 12");
		Assert(root->Evaluate( { 2017, 11, 18 }, ""), "Parse condition 13");
		Assert(root->Evaluate( { 2017, 11, 19 }, ""), "Parse condition 14");
	}
	{
		std::istringstream is("date == 2017-11-18");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 15");
		Assert(root->Evaluate( { 2017, 11, 18 }, ""), "Parse condition 16");
		Assert(!root->Evaluate( { 2017, 11, 19 }, ""), "Parse condition 17");
	}
	{
		std::istringstream is(R"(event == "sport event")");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, "sport event"),
				"Parse condition 18");
		Assert(!root->Evaluate( { 2017, 1, 1 }, "holiday"),
				"Parse condition 19");
	}
	{
		std::istringstream is("date >= 2017-01-01 AND date < 2017-07-01");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, ""), "Parse condition 20");
		Assert(root->Evaluate( { 2017, 3, 1 }, ""), "Parse condition 21");
		Assert(root->Evaluate( { 2017, 6, 30 }, ""), "Parse condition 22");
		Assert(!root->Evaluate( { 2017, 7, 1 }, ""), "Parse condition 23");
		Assert(!root->Evaluate( { 2016, 12, 31 }, ""), "Parse condition 24");
	}
	{
		std::istringstream is(
				R"(event != "sport event" AND event != "Wednesday")");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, "holiday"),
				"Parse condition 25");
		Assert(!root->Evaluate( { 2017, 1, 1 }, "sport event"),
				"Parse condition 26");
		Assert(!root->Evaluate( { 2017, 1, 1 }, "Wednesday"),
				"Parse condition 27");
	}
	{
		std::istringstream is(R"(event == "holiday AND date == 2017-11-18")");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(!root->Evaluate( { 2017, 11, 18 }, "holiday"),
				"Parse condition 28");
		Assert(!root->Evaluate( { 2017, 11, 18 }, "work day"),
				"Parse condition 29");
		Assert(root->Evaluate( { 1, 1, 1 }, "holiday AND date == 2017-11-18"),
				"Parse condition 30");
	}
	{
		std::istringstream is(
				R"(((event == "holiday" AND date == 2017-01-01)))");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, "holiday"),
				"Parse condition 31");
		Assert(!root->Evaluate( { 2017, 1, 2 }, "holiday"),
				"Parse condition 32");
	}
	{
		std::istringstream is(
				R"(date > 2017-01-01 AND (event == "holiday" OR date < 2017-07-01))");
		std::shared_ptr<Node> root = ParseCondition(is);
    Assert(!root->Evaluate({2016, 1, 1}, "holiday"), "Parse condition 33");
    Assert(root->Evaluate({2017, 1, 2}, "holiday"), "Parse condition 34");
    Assert(root->Evaluate({2017, 1, 2}, "workday"), "Parse condition 35");
    Assert(!root->Evaluate({2018, 1, 2}, "workday"), "Parse condition 36");
  }
  {
	  std::istringstream is(R"(date > 2017-01-01 AND event == "holiday" OR date < 2017-07-01)")
		;
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2016, 1, 1 }, "event"), "Parse condition 37");
		Assert(root->Evaluate( { 2017, 1, 2 }, "holiday"),
				"Parse condition 38");
		Assert(root->Evaluate( { 2017, 1, 2 }, "workday"),
				"Parse condition 39");
		Assert(!root->Evaluate( { 2018, 1, 2 }, "workday"),
				"Parse condition 40");
	}
	{
		std::istringstream is(R"(((date == 2017-01-01 AND event == "holiday")))");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 2017, 1, 1 }, "holiday"),
				"Parse condition 41");
		Assert(!root->Evaluate( { 2017, 1, 2 }, "holiday"),
				"Parse condition 42");
	}
	{
		std::istringstream is(R"(((event == "2017-01-01" OR date > 2016-01-01)))");
		std::shared_ptr<Node> root = ParseCondition(is);
		Assert(root->Evaluate( { 1, 1, 1 }, "2017-01-01"),
				"Parse condition 43");
		Assert(!root->Evaluate( { 2016, 1, 1 }, "event"), "Parse condition 44");
		Assert(root->Evaluate( { 2016, 1, 2 }, "event"), "Parse condition 45");
	}
}
