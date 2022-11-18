#include "y-unit.h"
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

const char* const emptyStr = "";

YTestValue::YTestValue(const char* value) :_kind(YTestValueKinds::String){
	size_t bytes = strlen(value) +1;
	this->_pointer = (char*)malloc(bytes);
	if(this->_pointer) strcpy_s((char*)this->_pointer, bytes, value);
};

YTestValue::YTestValue(YTestValue& other) {
	if (this->_kind == YTestValueKinds::String && this->_pointer) {
		free(this->_pointer);
	}
	this->_kind = other._kind;
	if (other._kind == YTestValueKinds::Integer) this->_integer = other._integer;
	else if (other._kind == YTestValueKinds::Float) this->_float = other._float;
	else if ( other._kind == YTestValueKinds::Pointer) this->_pointer = other._pointer;
	else if (other._kind == YTestValueKinds::String) {
		if (other._pointer) {
			size_t bytes = strlen((char*)other._pointer) + 1;
			this->_pointer = (char*)malloc(bytes);
			if (this->_pointer) strcpy_s((char*)this->_pointer, bytes, (char*)other._pointer);
		}else  this->_pointer = 0;
		
	} else this->_integer = 0;
};

YTestValue& YTestValue::operator=(const YTestValue& other) {
	if (this->_kind == YTestValueKinds::String && this->_pointer) {
		free(this->_pointer);
	}
	this->_kind = other._kind;
	if (other._kind == YTestValueKinds::Integer) this->_integer = other._integer;
	else if (other._kind == YTestValueKinds::Float) this->_float = other._float;
	else if (other._kind == YTestValueKinds::Pointer) this->_pointer = other._pointer;
	else if (other._kind == YTestValueKinds::String) {
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

bool YTestValue::operator==(YTestValue second) {
	YTestValue& first = *this;
	if (first._kind == second._kind) {
		if (first._kind == YTestValueKinds::Integer) return first._integer == second._integer;
		else if (first._kind == YTestValueKinds::Float) return first._float == second._float;
		else if ( first._kind == YTestValueKinds::Pointer) return first._pointer == second._pointer;
		else if (first._kind == YTestValueKinds::String) {
			return strcmp((const char*)first._pointer, (const char*)second._pointer)==0;
		}
		return true;
	}
	return false;
};
YTestValue::~YTestValue() {
	if (this->_kind == YTestValueKinds::String && this->_pointer) {
		free(this->_pointer);
		this->_pointer = 0;
	}
};
YTestAssert& YTestAssert::message(const char* message) {
	if (this->_message) free((void*)this->_message);
	if (message) {
		int bytes = strlen(message) + 1;
		this->_message = (char*)malloc(bytes);
		if (this->_message) strcpy_s((char*)this->_message, bytes, message);
	}
	else this->_message = 0;
	
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

YTestDescription::YTestDescription(YTestCase* tcase,const char* text) :_next(0),_case(tcase) {
	if (text) {
		int bytes = strlen(text) + 1;
		this->_text = (char*)malloc(bytes);
		if (this->_text) strcpy_s((char*)this->_text, bytes, text);
	}
	else this->_text = 0;
}
YTestDescription::~YTestDescription() {
	if (this->_text) free((void*)this->_text);
}

YTestCase::YTestCase(const char* name,   YTestExpectStatement statement, YTestCase* parent, YTestManager* manager):_parent(parent),_manager(manager), _firstAssert(0), _lastAssert(0), _nextSibling(0), _firstChild(0), _lastChild(0), _statement(statement),_childCount(0),_firstDescription(0),_lastDescription(0), YTestCounter() {
	if (name != 0) {
		int bytes = strlen(name) + 1;
		this->_name = (char*)malloc(bytes);
		if (this->_name) strcpy_s((char*)this->_name, bytes, name);
	}
	else this->_name = 0;
	
	this->_deep = parent ? parent->_deep + 1 : 0;
}

YTestCase::YTestCase(const char* name, YTestStatement statement, YTestManager* manager) :YTestCase(name, 0, 0, manager) {
	YTest test(this);
	statement(test);
}
YTestCase::YTestCase(YTestStatement statement, YTestManager* manager) :YTestCase(0, 0, 0, manager) {
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
	if (this->_firstDescription) {
		YTestDescription* item = this->_firstDescription;
		while (item) {
			YTestDescription* next = item->_next;
			delete item;
			item = next;
		}
	}
	
}

void YTestCase::execute() {
	this->logger()->beginTest(this);
	YTestDescription* des = this->_firstDescription;
	while (des) {
		this->logger()->description(des);
		des = des->_next;
	}

	this->counterBegin();
	if (this->_statement) {
		YExpect expect(this);
		this->_statement(expect);
		des = this->_firstDescription;
		while (des) {
			this->logger()->description(des);
			des = des->_next;
		}
		
		YTestAssert* assert= this->_firstAssert;
		if (assert) this->logger()->br("ASSERTS", this);
		while (assert) {
			this->logger()->assert(assert);
			if (assert->status()==1) this->succeed();
			else if(assert->status()==0)this->failed();
			assert = assert->_nextSibling;
		}
		
	}
	if (this->_firstChild) {
	
		YTestCase* child = this->_firstChild;
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

YTestLogger* YTestCase::logger() { return this->_manager->logger(); }


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
class YTestDefaultLogger :public YTestLogger {
	virtual void beginTest(YTestCase* test) {
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
	virtual void endTest(YTestCase* test) {
		
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
	virtual void assert(YTestAssert* assert) {
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

	virtual void description(YTestDescription* des) {
		tabs(des->testCase()->deep());
		cout << des->text() << endl;
	}

	virtual void br(const char* txt,YTestCase* testCase) {
		cout << endl;
		tabs(testCase->deep());
		cout << "\033[34;1m--- "<<txt<<" ---\033[0m" << endl<<endl;
	}
};
YTestDefaultLogger defaultLogger;
YTestLogger* y_defaultTestLogger = &defaultLogger;



YTestCase& YTestManager::operator()(YTestStatement statement) {
	YTestCase* tcase = new YTestCase(statement, this);
	return this->appendTestCase(tcase);
}
YTestCase& YTestManager::operator()(const char* name, YTestStatement statement) {
	YTestCase* tcase = new YTestCase(name, statement, this);
	return this->appendTestCase(tcase);
}
YTestCase& YTestManager::operator()(YTestExpectStatement statement) {
	YTestCase* tcase = new YTestCase(statement, this);
	return this->appendTestCase(tcase);
}
YTestCase& YTestManager::operator()(const char* name, YTestExpectStatement statement) {
	YTestCase* tcase = new YTestCase(name, statement, this);
	return this->appendTestCase(tcase);
}
void YTestManager::execute() {
	YTestCase* item = this->_firstCase;
	while (item) {
		item->execute();
		item = item->_nextSibling;
	}
}

YTestManager defaultTestManager;

YTestManager& y_unit = defaultTestManager;



