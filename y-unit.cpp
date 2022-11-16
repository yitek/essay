#include "y-unit.h"
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

YTestAssert& YTestAssert::message(const char* message) {
	if (this->_message) free((void*)this->_message);
	int bytes = strlen(message) + 1;
	this->_message= (char*)malloc(bytes);
	if (this->_message) strcpy_s((char*)this->_message, bytes, message);
	return *this;
}
YTestAssert::~YTestAssert() {
	if (this->_message) free((void*)this->_message);
}

void YTestCounter::counterBegin() {
	this->_beginAt = time(NULL);
}
void YTestCounter::counterEnd() {
	this->_endAt = time(NULL);
}

YTestCase::YTestCase(const char* name,   YTestExpectStatement statement, YTestCase* parent, YTestLogger* logger):_parent(parent),_logger(logger?logger:y_defaultTestLogger), _firstAssert(0), _lastAssert(0), _nextSibling(0), _firstChild(0), _lastChild(0), _statement(statement),_childCount(0), YTestCounter() {
	int bytes = strlen(name) + 1;
	this->_name = (char*)malloc(bytes);
	if(this->_name) strcpy_s((char*)this->_name,bytes,name);
	this->_deep = parent ? parent->_deep + 1 : 0;
}

YTestCase::YTestCase(const char* name, YTestStatement statement, YTestLogger* logger) :YTestCase(name, 0, 0, logger) {
	YTest test(this);
	statement(test);
}
YTestCase::YTestCase(YTestStatement statement, YTestLogger* logger) :YTestCase(0, 0, 0, logger) {
	YTest test(this);
	statement(test);
}


YTestCase::~YTestCase() {
	if (this->_name) free((void*)this->_name);
	if (this->_firstChild) {
		YTestCase* child = this->_firstChild;
		while (child) {
			YTestCase* next = child->_nextSibling;
			delete child;
			child = next;
		}
	}
	if (this->_firstAssert) {
		YTestAssert* assert = this->_firstAssert;
		while (assert) {
			YTestAssert* next = assert->_nextSibling;
			delete assert;
			assert = next;
		}
	}
}

void YTestCase::Execute() {
	this->_logger->beginTest(this);
	this->counterBegin();
	if (this->_statement) {
		YExpect expect(this);
		this->_statement(expect);

		
		YTestAssert* assert= this->_firstAssert;
		while (assert) {
			this->_logger->assert(assert);
			if (assert->status()==1) this->succeed();
			else if(assert->status()==0)this->failed();
		}
		
	}
	if (this->_firstChild) {
	
		YTestCase* child = this->_firstChild;
		while (child) {
			child->Execute();
			if (child->pass()) this->succeed();
			else this->failed();
			child = child->_nextSibling;
		}
		
	}
	this->counterEnd();
	this->_logger->endTest(this);
}
void tabs(int n) {
	for (int i = 0; i < n; i++) {
		cout << "\t";
	}
}
class YTestDefaultLogger :public YTestLogger {
	virtual void beginTest(YTestCase* test) {
		tabs(test->deep());
		cout << "\033[36;1m" << test->name() << endl;
		tabs(test->deep());
		cout << "\t----------\033[0m" << endl;
	};
	virtual void endTest(YTestCase* test) {
		cout << endl;
		tabs(test->deep());
		cout << ">> ";
		if (test->pass()) {

			cout << "\033[32;1m[OK]\033[0m";
			cout << "\033[36;1m" << test->name() << "\033[0m";
			cout << ": total=" << test->total() << ", ellapsed=" << test->ellapse() << endl << endl;
		}
		else {
			cout << "\033[32;1m[FAIL]\033[0m";
			cout << "\t\033[35;1m" << test->name() << "\033[0m";
			cout << ": total=" << test->total() << ", fail=" << test->fail() << ", ellapsed=" << test->ellapse() << endl << endl;
		}
	}
	virtual void assert(YTestAssert* assert) {
		tabs(assert->test()->deep()+1);
		if (assert->status()==1) {
			cout << "+ \033[32;1m[OK]\033[0m ";
		}
		else {
			cout << "+ \033[35;1m[FAIL]\033[0m ";
		}
		const char* msg = assert->message();
		if (msg) cout << msg << endl;
		else {
			cout << "actual value is not match then expected value" << endl;
		}
	}
};
YTestDefaultLogger defaultLogger;
YTestLogger* y_defaultTestLogger = &defaultLogger;



