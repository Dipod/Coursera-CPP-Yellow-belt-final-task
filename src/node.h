#pragma once

#include "date.h"
#include <memory>

enum class Comparison {
	Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual
};

enum class LogicalOperation {
	Or, And
};

template<typename T>
bool Compare(const Comparison &cmp, const T &lhs, const T &rhs) {
	switch (cmp) {
	case Comparison::Less:
		return lhs < rhs;
	case Comparison::LessOrEqual:
		return lhs <= rhs;
	case Comparison::Greater:
		return lhs > rhs;
	case Comparison::GreaterOrEqual:
		return lhs >= rhs;
	case Comparison::Equal:
		return lhs == rhs;
	case Comparison::NotEqual:
		return lhs != rhs;
	default:
		return false;
	}
}

class Node {
public:
	virtual bool Evaluate(const Date &date, const std::string &event) const = 0;
	virtual ~Node() = default;
};

class DateComparisonNode: public Node {
public:
	DateComparisonNode(const Comparison &cmp, const Date &date) :
			cmp_(cmp), date_(date) {
	}

	bool Evaluate(const Date &date, const std::string &event) const override;
private:
	const Comparison cmp_;
	const Date date_;
};

class EventComparisonNode: public Node {
public:
	EventComparisonNode(const Comparison &cmp, const std::string &event) :
			cmp_(cmp), event_(event) {

	}

	bool Evaluate(const Date &date, const std::string &event) const override;
private:
	const Comparison cmp_;
	const std::string event_;
};

class LogicalOperationNode: public Node {
public:
	LogicalOperationNode(const LogicalOperation &op,
			const std::shared_ptr<Node> &lhs, const std::shared_ptr<Node> &rhs) :
			op_(op), lhs_(lhs), rhs_(rhs) {

	}

	bool Evaluate(const Date &date, const std::string &event) const override;
private:
	const LogicalOperation op_;
	const std::shared_ptr<Node> lhs_;
	const std::shared_ptr<Node> rhs_;
};

class EmptyNode: public Node {
public:
	bool Evaluate(const Date &date, const std::string &event) const override;
};
