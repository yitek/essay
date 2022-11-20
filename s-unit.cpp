#include "s-unit.h"
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

const char* const emptyStr = "";

STestValue::STestValue(const char* value) :_kind(STestValueKinds::String){
	size_t bytes = strlen(value) +1;
	this->_pointer = (char*)malloc(bytes);
	if(this->_pointer) strcpy_s((char*)this->_pointer, bytes, value);
};

STestValue::STestValue(STestValue& other) {
	if (this->_kind == STestValueKinds::String && this->_pointer) {
		free(this->_pointer);
	}
	this->_kind = other._kind;
	if (other._kind == STestValueKinds::Integer) this->_integer = other._integer;
	else if (other._kind == STestValueKinds::Float) this->_float = other._float;
	else if ( other._kind == STestValueKinds::Pointer) this->_pointer = other._pointer;
	else if (other._kind == STestValueKinds::String) {
		if (other._pointer) {
			size_t bytes = strlen((char*)other._pointer) + 1;
			this->_pointer = (char*)malloc(bytes);
			if (this->_pointer) strcpy_s((char*)this->_pointer, bytes, (char*)other._pointer);
		}else  this->_pointer = 0;
		
	} else this->_integer = 0;
};

STestValue& STestValue::operator=(const STestValue& other) {
	if (this->_kind == STestValueKinds::String && this->_pointer) {
		free(this->_pointer);
	}
	this->_kind = other._kind;
	if (other._kind == STestValueKinds::Integer) this->_integer = other._integer;
	else if (other._kind == STestValueKinds::Float) this->_float = other._float;
	else if (other._kind == STestValueKinds::Pointer) this->_pointer = other._pointer;
	else if (other._kind == STestValueKinds::String) {
		if (other._pointer) {
			size_t bytes = strlen((char*)other._pointer) + 1;
			this->_pointer = (char*)malloc(bytes);
			if (this->_pointer) strcpy_s((char*)this->_pointer, bytes, (char*)other._pointer);
		}
		else  this->_pointer = 0;

	}
	else this->_integer = 0;
	return *this;
};

bool STestValue::operator==(STestValue second) {
	STestValue& first = *this;
	if (first._kind == second._kind) {
		if (first._kind == STestValueKinds::Integer) return first._integer == second._integer;
		else if (first._kind == STestValueKinds::Float) return first._float == second._float;
		else if ( first._kind == STestValueKinds::Pointer) return first._pointer == second._pointer;
		else if (first._kind == STestValueKinds::String) {
			return strcmp((const char*)first._pointer, (const char*)second._pointer)==0;
		}
		return true;
	}
	return false;
};
STestValue::~STestValue() {
	if (this->_kind == STestValueKinds::String && this->_pointer) {
		free(this->_pointer);
		this->_pointer = 0;
	}
};
STestAssert& STestAssert::message(const char* message) {
	if (this->_message) free((void*)this->_message);
	if (message) {
		long long bytes = strlen(message) + 1;
		this->_message = (char*)malloc(bytes);
		if (this->_message) strcpy_s((char*)this->_message, bytes, message);
	}
	else this->_message = 0;
	
	return *this;
}

STestAssert::~STestAssert() {
	if (this->_message) free((void*)this->_message);
}

void STestCounter::counterBegin() {
	this->_beginAt = time(NULL);
}
void STestCounter::counterEnd() {
	this->_endAt = time(NULL);
}

STestDescription::STestDescription(STestCase* tcase,const char* text) :_next(0),_case(tcase) {
	if (text) {
		long long bytes = strlen(text) + 1;
		this->_text = (char*)malloc(bytes);
		if (this->_text) strcpy_s((char*)this->_text, bytes, text);
	}
	else this->_text = 0;
}
STestDescription::~STestDescription() {
	if (this->_text) free((void*)this->_text);
}

STestCase::STestCase(const char* name,   STestExpectStatement statement, STestCase* parent, STestManager* manager):_parent(parent),_manager(manager), _firstAssert(0), _lastAssert(0), _nextSibling(0), _firstChild(0), _lastChild(0), _statement(statement),_childCount(0),_firstDescription(0),_lastDescription(0), STestCounter() {
	if (name != 0) {
		long long bytes = strlen(name) + 1;
		this->_name = (char*)malloc(bytes);
		if (this->_name) strcpy_s((char*)this->_name, bytes, name);
	}
	else this->_name = 0;
	
	this->_deep = parent ? parent->_deep + 1 : 0;
}

STestCase::STestCase(const char* name, STestStatement statement, STestManager* manager) :STestCase(name, 0, 0, manager) {
	STest test(this);
	statement(test);
}
STestCase::STestCase(STestStatement statement, STestManager* manager) :STestCase(0, 0, 0, manager) {
	STest test(this);
	statement(test);
}



STestCase::~STestCase() {
	if (this->_name) free((void*)this->_name);
	if (this->_firstChild) {
		STestCase* child = this->_firstChild;
		while (child) {
			STestCase* next = child->_nextSibling;
			delete child;
			child = next;
		}
	}
	if (this->_firstAssert) {
		STestAssert* assert = this->_firstAssert;
		while (assert) {
			STestAssert* next = assert->_nextSibling;
			delete assert;
			assert = next;
		}
	}
	if (this->_firstDescription) {
		STestDescription* item = this->_firstDescription;
		while (item) {
			STestDescription* next = item->_next;
			delete item;
			item = next;
		}
	}
	
}

void STestCase::execute() {
	this->logger()->beginTest(this);
	STestDescription* des = this->_firstDescription;
	while (des) {
		this->logger()->description(des);
		des = des->_next;
	}

	this->counterBegin();
	if (this->_statement) {
		SExpect expect(this);
		this->_statement(expect);
		des = this->_firstDescription;
		while (des) {
			this->logger()->description(des);
			des = des->_next;
		}
		
		STestAssert* assert= this->_firstAssert;
		if (assert) this->logger()->br("ASSERTS", this);
		while (assert) {
			this->logger()->assert(assert);
			if (assert->status()==1) this->succeed();
			else if(assert->status()==0)this->failed();
			assert = assert->_nextSibling;
		}
		
	}
	if (this->_firstChild) {
	
		STestCase* child = this->_firstChild;
		while (child) {
			child->execute();
			
			if (child->pass()) this->succeed();
			else this->failed();
			child = child->_nextSibling;
		}
		
	}
	this->counterEnd();
	this->logger()->endTest(this);
}

STestLogger* STestCase::logger() { return this->_manager->logger(); }


void tabs(int deep) {
	for (int i = 0; i < deep; i++) {
		cout << "\t";
	}
}
void jn(int deep) {
	if (deep <= 1) return;
	for (int i = 0; i <= deep; i++) {
		cout << "#";
	}
	cout << " ";
}
void colorBegin(int deep) {
	if(deep==0)cout << "\033[43;1m";
	else if(deep==1) cout << "\033[46;1m";
	else cout << "\033[46;1m";
}
void colorEnd() {
	cout << "\033[0m";
}
class YTestDefaultLogger :public STestLogger {
	virtual void beginTest(STestCase* test) {
		if (test->deep()) cout << endl;
		tabs(test->deep());
		jn(test->deep());
		colorBegin(test->deep());
		if (test->name())cout << test->name();
		colorEnd();
		cout << endl;
		

		tabs(test->deep());
		if (test->deep() == 1)cout << "----------------" << endl;
		else if (test->deep() == 0) cout << "================" << endl;
		else  cout << endl;
		
	};
	virtual void endTest(STestCase* test) {
		
		cout << endl;
		
		tabs(test->deep());
		if (test->deep())cout << ">> ";
		else cout << "> ";

		if (test->pass()) {

			
			if (test->deep())cout << "*** \033[36;1m";
			else cout << "*** \033[33;1m";
			if (test->name()) cout << test->name();
			cout << "\033[0m ***: total=" << test->total() << ", ellapsed=" << test->ellapse()<<"." << endl;
		}
		else {
			
			cout << "*** \033[35;1m";
			if (test->name()) cout << test->name();
			cout << "\033[0m ***: total=" << test->total() << ", fail=\033[41;1m" << test->fail() << "\033[0m, ellapsed=" << test->ellapse()<<"." << endl;
		}

	}
	virtual void assert(STestAssert* assert) {
		tabs(assert->test()->deep());
		if (assert->status()==1) {
			cout << "+ \033[32;1m[OK]\033[0m ";
		}
		else {
			cout << "+ \033[31;1m[FAIL]\033[0m ";
		}
		const char* msg = assert->message();
		if (msg) cout << msg << endl;
		else {
			cout << "actual value is not match then expected value" << endl;
		}
		cout << "\033[0m";
	}

	virtual void description(STestDescription* des) {
		tabs(des->testCase()->deep());
		cout << des->text() << endl;
	}

	virtual void br(const char* txt,STestCase* testCase) {
		cout << endl;
		tabs(testCase->deep());
		cout << "\033[34;1m--- "<<txt<<" ---\033[0m" << endl<<endl;
	}
};
YTestDefaultLogger defaultLogger;
STestLogger* y_defaultTestLogger = &defaultLogger;



STestCase& STestManager::operator()(STestStatement statement) {
	STestCase* tcase = new STestCase(statement, this);
	return this->appendTestCase(tcase);
}
STestCase& STestManager::operator()(const char* name, STestStatement statement) {
	STestCase* tcase = new STestCase(name, statement, this);
	return this->appendTestCase(tcase);
}
STestCase& STestManager::operator()(STestExpectStatement statement) {
	STestCase* tcase = new STestCase(statement, this);
	return this->appendTestCase(tcase);
}
STestCase& STestManager::operator()(const char* name, STestExpectStatement statement) {
	STestCase* tcase = new STestCase(name, statement, this);
	return this->appendTestCase(tcase);
}
void STestManager::execute() {
	STestCase* item = this->_firstCase;
	while (item) {
		item->execute();
		item = item->_nextSibling;
	}
}

STestManager defaultTestManager;

STestManager& s_unit = defaultTestManager;



