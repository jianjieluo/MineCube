#ifndef OperationManager_hpp
#define OperationManager_hpp
#include "EditOperationInterface.hpp"
#include <stack>
#include <memory>
#include <vector>

using std::stack;
using std::vector;
using std::shared_ptr;
using std::string;

class OperationManager {
private:
	/**
	* Too hard to name it!!
	*/
	unsigned int indexOfHistory;
	stack<shared_ptr<EditOperationInterface>> history;
	stack<shared_ptr<EditOperationInterface>> undo_history;
public:
	// Use for text history to show.
	vector<string> str_history;

	OperationManager() : indexOfHistory(0) {}
	void executeOp(shared_ptr<EditOperationInterface>, string);

	/**
	* If there is nothing to undo, nothing happen
	*/
	void undo();

	/**
	* This method undo undo.
	*/
	void cancle_undo();

	/*
	* Get the latest history pos to show
	*/
	const unsigned int getIndexOfHistory();
};

#endif /* OperationManager_hpp */