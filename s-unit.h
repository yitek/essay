#pragma once
#ifndef __SUNIT_H__
#define __SUNIT_H__


#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif

class STestAssert;
class STestCase;
class SExpect;
class STest;
class STestManager;
class STestLogger;

typedef void (*STestExpectStatement)(SExpect);
typedef void (*STestStatement)(STest);


enum STestValueKinds {
	None,
	Float,
	Integer,
	Pointer,
	String
};

class STestValue {
private:
	STestValueKinds _kind;
	union {
		double _float;
		long long _integer;
		void* _pointer;
	};
	// char members[0];
public:
	STestValue() :_kind(STestValueKinds::None) { _integer = 0; };
	STestValue(int value) :_kind(STestValueKinds::Integer), _integer(value) {};
	STestValue(long long value) :_kind(STestValueKinds::Integer), _integer(value) {};
	STestValue(unsigned long long value) :_kind(STestValueKinds::Integer), _integer(value) {};
	STestValue(const void* value) :_kind(STestValueKinds::Pointer), _pointer((void*)value) {};
	STestValue(const char* value);
	STestValue(float value) :_kind(STestValueKinds::Float), _float(value) {};
	STestValue(double value) :_kind(STestValueKinds::Float), _float(value) {};
	STestValue(STestValue& other);
	STestValue& operator=(const STestValue& other);

	
	bool operator==(STestValue second);
	~STestValue();
};





class STestAssert {

private:
	friend STestCase;
	STestAssert* _nextSibling;
	const char* _message;
	int _status;
	STestCase* _case;
	STestAssert(STestCase* const testCase) {
		this->_status = -1;
		this->_case = testCase;
		this->_message = 0;
		this->_nextSibling = 0;
		
	};
protected:
	STestValue _expected;
	STestValue _actual;
public:

	STestAssert(STestCase* const testCase, STestValue actualValue):STestAssert(testCase) {
		this->_actual = actualValue;
	};
	bool status() { return this->_status; };

	STestAssert& message(const char* message);
	const char* message() { return this->_message; };

	STestCase* test() { return this->_case; }

	STestAssert& toBe(int expected) {
		this->_status = (this->_expected = expected)==this->_actual ?1:0;
		return *this;
	}
	STestAssert& toBe(long long expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	STestAssert& toBe(unsigned long long expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	STestAssert& toBe(void* expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	STestAssert& toBe(float expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	STestAssert& toBe(double expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	STestAssert& toBe(const char* expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	~STestAssert();
};
class STestCounter {
private:
	int _total;
	int _fail;
	int _success;
	long long _beginAt;
	long long _endAt;
	STestCounter* _parent;
	
protected:
	friend STestCase;
	STestCounter(STestCounter* parent = 0) :_total(0), _success(0), _fail(0), _beginAt(0), _endAt(0), _parent(parent) {}
	void counterBegin();
	void counterEnd();
	void failed() { this->_total++; this->_fail++; }
	void succeed() { this->_total++; this->_success++;  }
public:
	int total() { return _total; }
	int fail() { return _fail; }
	int success() { return _success; }
	unsigned long long ellapse() { return (_endAt - _beginAt); }
};

class STestDescription {
private:
	friend STestCase;
	const char* _text;
	STestDescription* _next;
	STestCase* _case;
public:
	STestDescription(STestCase* tcase,const char* text);
	~STestDescription();
	STestDescription* next() { return this->_next; }
	const char* text() { return this->_text; }
	STestCase* testCase() { return this->_case; }
};
class STestCase:public STestCounter {
private:
	friend STestManager;
	STestCase* _parent;
	const char* _name;
	STestDescription* _firstDescription;
	STestDescription* _lastDescription;
	STestCase* _nextSibling;
	STestAssert* _firstAssert;
	STestAssert* _lastAssert;
	STestCase* _firstChild;
	STestCase* _lastChild;
	STestExpectStatement _statement;
	int _childCount;
	int _deep;
	//YTestLogger* _logger;
	STestManager* _manager;
	STestCase(const char* name, STestExpectStatement statement, STestCase* parent,STestManager* manager);
	STestCase(const char* name, STestExpectStatement statement, STestCase* parent) :STestCase(name,statement,parent,parent->_manager){}
	
	
public:
	STestCase(const char* name,  STestExpectStatement statement, STestManager* manager) :STestCase(name, statement,0, manager) {}
	
	STestCase(STestExpectStatement statement, STestManager* manager) :STestCase(0, statement, 0, manager) {}
	
	STestCase(const char* name, STestStatement statement, STestManager* manager);
	STestCase(STestStatement statement, STestManager* manager);
	STestDescription* appendDescription(const char* text) {
		STestDescription* des = new STestDescription(this, text);
		if (this->_lastDescription) {
			this->_lastDescription = this->_lastDescription->_next = des;
		}
		else this->_lastDescription = this->_firstDescription = des;
		return des;
	}
	
	STestAssert* appendAssert(STestValue actual) {
		STestAssert* assert = new STestAssert(this, actual);
		if (this->_lastAssert) {
			this->_lastAssert->_nextSibling = assert;
			this->_lastAssert = assert;
		}
		else this->_firstAssert = this->_lastAssert = assert;
		return assert;
	};
	STestCase* appendChild(const char* name, STestExpectStatement statement = 0) {
		STestCase* child = new STestCase(name, statement, this);
		if (this->_lastChild) {
			this->_lastChild->_nextSibling = child;
			this->_lastChild = child;
		}
		else this->_firstChild = this->_lastChild = child;
		this->_childCount++;
		return child;
	};
	const char* name() { return this->_name; }
	STestManager* manager() { return this->_manager; }
	STestLogger* logger();
	int childCount() { return this->_childCount; }
	bool pass() { return this->success() == this->total(); }
	void execute();
	int deep() { return _deep; };
	bool isRoot() { return this->_parent == 0; }
	~STestCase();
};
class SExpect {
private:
	STestCase* _case;
public:
	SExpect(STestCase* yc) :_case(yc) {};
	STestAssert& operator()(int actual) { return *this->_case->appendAssert(actual); };
	STestAssert& operator()(long long actual) { return *this->_case->appendAssert(actual); };
	STestAssert& operator()(unsigned long long actual) { return *this->_case->appendAssert(actual); };
	STestAssert& operator()(const void* actual) { return *this->_case->appendAssert(actual); };
	STestAssert& operator()(const char* actual) { return *this->_case->appendAssert(actual); };
	STestAssert& operator()(float actual) { return *this->_case->appendAssert(actual); };
	STestAssert& operator()(double actual) { return *this->_case->appendAssert(actual); };
	SExpect operator<<(const char* text) {
		this->_case->appendDescription(text);
		return *this;
	};
	
};

class STest {
private:
	STestCase* _parent;
public:
	STest(STestCase* parent) :_parent(parent) {};
	STestCase& operator()(const char* name, STestExpectStatement statement) {
		return *this->_parent->appendChild(name, statement);
	};
	STestCase& operator()(const char* name, STestStatement statement) {
		STestCase* child = this->_parent->appendChild(name);
		STest test(child);
		statement(test);
		return *child;
	};
	STest operator<<(const char* text) {
		this->_parent->appendDescription(text);
		return *this;
	};
	
};


class STestLogger {
public:
	virtual void beginTest(STestCase* test) = 0;
	virtual void endTest(STestCase* test) =0;
	virtual void assert(STestAssert* assert)=0;
	virtual void description(STestDescription* des) = 0;
	virtual void br(const char* str, STestCase* testCase) = 0;
};
extern STestLogger* y_defaultTestLogger;



class STestManager {
private:
	
	STestCase* _firstCase;
	STestCase* _lastCase;
	STestLogger* _logger;
	STestCase& appendTestCase(STestCase* tcase) {
		if (this->_lastCase) {
			this->_lastCase = this->_lastCase->_nextSibling = tcase;
		}
		else this->_lastCase = this->_firstCase = tcase;
		return *tcase;
	}
	
public:
	STestManager() :_firstCase(0), _lastCase(0), _logger(0) {};
	void execute();

	STestCase& operator()(STestStatement statement);
	STestCase& operator()(const char* name, STestStatement statement);
	STestCase& operator()(STestExpectStatement statement);
	STestCase& operator()(const char* name, STestExpectStatement statement);
	void operator()() { this->execute(); }

	STestLogger* logger() { return this->_logger == 0 ? y_defaultTestLogger : this->_logger; }
	STestManager& logger(STestLogger* logger) { this->_logger = logger; return *this; }
};


extern STestManager& s_unit;














#endif // !__SUNIT_H__

