#include "OperationManager.hpp"


void OperationManager::executeOp(shared_ptr<EditOperationInterface> new_op, string record) {
	new_op->execute();
	history.push(shared_ptr<EditOperationInterface>(new_op));
	if (indexOfHistory >= str_history.size()) {
		str_history.push_back(record);
		indexOfHistory = str_history.size();
	}
	else {
		str_history[indexOfHistory++] = record;
	}
	// init undo_history
	undo_history = stack<shared_ptr<EditOperationInterface>>();
}

void OperationManager::undo() {
	if (!history.empty()) {
		auto to_undo = history.top();
		to_undo->undo();
		undo_history.push(to_undo);
		history.pop();
		indexOfHistory--;
	}
}

void OperationManager::cancle_undo() {
	if (!undo_history.empty()) {
		auto undo_to_cancle = undo_history.top();
		undo_history.pop();
		undo_to_cancle->execute();
		history.push(undo_to_cancle);
		indexOfHistory++;
	}
}

const unsigned int OperationManager::getIndexOfHistory() {
	return indexOfHistory;
}