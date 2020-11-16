#include "node.h"

bool DateComparisonNode::Evaluate(const Date &date,
		const std::string &event) const {
	return Compare(cmp_, date, date_);
}

bool EventComparisonNode::Evaluate(const Date &date,
		const std::string &event) const {
	return Compare(cmp_, event, event_);
}

bool LogicalOperationNode::Evaluate(const Date &date,
		const std::string &event) const {
	switch (op_) {
	case LogicalOperation::And:
		return lhs_->Evaluate(date, event) && rhs_->Evaluate(date, event);
	case LogicalOperation::Or:
		return lhs_->Evaluate(date, event) || rhs_->Evaluate(date, event);
	default:
		return false;
	}
}

bool EmptyNode::Evaluate(const Date &date, const std::string &event) const {
	return true;
}
