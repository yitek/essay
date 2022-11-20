#pragma once
#ifndef __YUNIT_H__
#define __YUNIT_H__

#define YUNIT

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif
class YTestAssert;
class YTestCase;
class YExpect;
class YTest;
class YTestManager;
class YTestLogger;

typedef void (*YTestExpectStatement)(YExpect);
typedef void (*YTestStatement)(YTest);


enum YTestValueKinds {
	None,
	Float,
	Integer,
	Pointer,
	String
};

class YTestValue {
private:
	YTestValueKinds _kind;
	union {
		double _float;
		long long _integer;
		void* _pointer;
	};
	// char members[0];
public:
	YTestValue() :_kind(YTestValueKinds::None) { _integer = 0; };
	YTestValue(int value) :_kind(YTestValueKinds::Integer), _integer(value) {};
	YTestValue(long long value) :_kind(YTestValueKinds::Integer), _integer(value) {};
	YTestValue(unsigned long long value) :_kind(YTestValueKinds::Integer), _integer(value) {};
	YTestValue(const void* value) :_kind(YTestValueKinds::Pointer), _pointer((void*)value) {};
	YTestValue(const char* value);
	YTestValue(float value) :_kind(YTestValueKinds::Float), _float(value) {};
	YTestValue(double value) :_kind(YTestValueKinds::Float), _float(value) {};
	YTestValue(YTestValue& other);
	YTestValue& operator=(const YTestValue& other);

	
	bool operator==(YTestValue second);
	~YTestValue();
};





class YTestAssert {

private:
	friend YTestCase;
	YTestAssert* _nextSibling;
	const char* _message;
	int _status;
	YTestCase* _case;
	YTestAssert(YTestCase* const testCase) {
		this->_status = -1;
		this->_case = testCase;
		this->_message = 0;
		this->_nextSibling = 0;
		
	};
protected:
	YTestValue _expected;
	YTestValue _actual;
public:

	YTestAssert(YTestCase* const testCase, YTestValue actualValue):YTestAssert(testCase) {
		this->_actual = actualValue;
	};
	bool status() { return this->_status; };

	YTestAssert& message(const char* message);
	const char* message() { return this->_message; };

	YTestCase* test() { return this->_case; }

	YTestAssert& toBe(int expected) {
		this->_status = (this->_expected = expected)==this->_actual ?1:0;
		return *this;
	}
	YTestAssert& toBe(long long expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	YTestAssert& toBe(unsigned long long expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	YTestAssert& toBe(void* expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	YTestAssert& toBe(float expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	YTestAssert& toBe(double expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	YTestAssert& toBe(const char* expected) {
		this->_status = (this->_expected = expected) == this->_actual ? 1 : 0;
		return *this;
	}
	~YTestAssert();
};
class YTestCounter {
private:
	int _total;
	int _fail;
	int _success;
	long long _beginAt;
	long long _endAt;
	YTestCounter* _parent;
	
protected:
	friend YTestCase;
	YTestCounter(YTestCounter* parent = 0) :_total(0), _success(0), _fail(0), _beginAt(0), _endAt(0), _parent(parent) {}
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
class YTestDescription {
private:
	friend YTestCase;
	const char* _text;
	YTestDescription* _next;
	YTestCase* _case;
public:
	YTestDescription(YTestCase* tcase,const char* text);
	~YTestDescription();
	YTestDescription* next() { return this->_next; }
	const char* text() { return this->_text; }
	YTestCase* testCase() { return this->_case; }
};
class YTestCase:public YTestCounter {
private:
	friend YTestManager;
	YTestCase* _parent;
	const char* _name;
	YTestDescription* _firstDescription;
	YTestDescription* _lastDescription;
	YTestCase* _nextSibling;
	YTestAssert* _firstAssert;
	YTestAssert* _lastAssert;
	YTestCase* _firstChild;
	YTestCase* _lastChild;
	YTestExpectStatement _statement;
	int _childCount;
	int _deep;
	//YTestLogger* _logger;
	YTestManager* _manager;
	YTestCase(const char* name, YTestExpectStatement statement, YTestCase* parent,YTestManager* manager);
	YTestCase(const char* name, YTestExpectStatement statement, YTestCase* parent) :YTestCase(name,statement,parent,parent->_manager){}
	
	
public:
	YTestCase(const char* name,  YTestExpectStatement statement, YTestManager* manager) :YTestCase(name, statement,0, manager) {}
	
	YTestCase(YTestExpectStatement statement, YTestManager* manager) :YTestCase(0, statement, 0, manager) {}
	
	YTestCase(const char* name, YTestStatement statement, YTestManager* manager);
	YTestCase(YTestStatement statement, YTestManager* manager);
	YTestDescription* appendDescription(const char* text) {
		YTestDescription* des = new YTestDescription(this, text);
		if (this->_lastDescription) {
			this->_lastDescription = this->_lastDescription->_next = des;
		}
		else this->_lastDescription = this->_firstDescription = des;
		return des;
	}
	
	YTestAssert* appendAssert(YTestValue actual) {
		YTestAssert* assert = new YTestAssert(this, actual);
		if (this->_lastAssert) {
			this->_lastAssert->_nextSibling = assert;
			this->_lastAssert = assert;
		}
		else this->_firstAssert = this->_lastAssert = assert;
		return assert;
	};
	YTestCase* appendChild(const char* name, YTestExpectStatement statement = 0) {
		YTestCase* child = new YTestCase(name, statement, this);
		if (this->_lastChild) {
			this->_lastChild->_nextSibling = child;
			this->_lastChild = child;
		}
		else this->_firstChild = this->_lastChild = child;
		this->_childCount++;
		return child;
	};
	const char* name() { return this->_name; }
	YTestManager* manager() { return this->_manager; }
	YTestLogger* logger();
	int childCount() { return this->_childCount; }
	bool pass() { return this->success() == this->total(); }
	void execute();
	int deep() { return _deep; };
	bool isRoot() { return this->_parent == 0; }
	~YTestCase();
};
class YExpect {
private:
	YTestCase* _case;
public:
	YExpect(YTestCase* yc) :_case(yc) {};
	YTestAssert& operator()(int actual) { return *this->_case->appendAssert(actual); };
	YTestAssert& operator()(long long actual) { return *this->_case->appendAssert(actual); };
	YTestAssert& operator()(unsigned long long actual) { return *this->_case->appendAssert(actual); };
	YTestAssert& operator()(const void* actual) { return *this->_case->appendAssert(actual); };
	YTestAssert& operator()(const char* actual) { return *this->_case->appendAssert(actual); };
	YTestAssert& operator()(float actual) { return *this->_case->appendAssert(actual); };
	YTestAssert& operator()(double actual) { return *this->_case->appendAssert(actual); };
	YExpect operator<<(const char* text) {
		this->_case->appendDescription(text);
		return *this;
	};
	
};

class YTest {
private:
	YTestCase* _parent;
public:
	YTest(YTestCase* parent) :_parent(parent) {};
	YTestCase& operator()(const char* name, YTestExpectStatement statement) {
		return *this->_parent->appendChild(name, statement);
	};
	YTestCase& operator()(const char* name, YTestStatement statement) {
		YTestCase* child = this->_parent->appendChild(name);
		YTest test(child);
		statement(test);
		return *child;
	};
	YTest operator<<(const char* text) {
		this->_parent->appendDescription(text);
		return *this;
	};
	
};


class YTestLogger {
public:
	virtual void beginTest(YTestCase* test) = 0;
	virtual void endTest(YTestCase* test) =0;
	virtual void assert(YTestAssert* assert)=0;
	virtual void description(YTestDescription* des) = 0;
	virtual void br(const char* str, YTestCase* testCase) = 0;
};
extern YTestLogger* y_defaultTestLogger;



class YTestManager {
private:
	
	YTestCase* _firstCase;
	YTestCase* _lastCase;
	YTestLogger* _logger;
	YTestCase& appendTestCase(YTestCase* tcase) {
		if (this->_lastCase) {
			this->_lastCase = this->_lastCase->_nextSibling = tcase;
		}
		else this->_lastCase = this->_firstCase = tcase;
		return *tcase;
	}
	
public:
	YTestManager() :_firstCase(0), _lastCase(0), _logger(0) {};
	void execute();

	YTestCase& operator()(YTestStatement statement);
	YTestCase& operator()(const char* name, YTestStatement statement);
	YTestCase& operator()(YTestExpectStatement statement);
	YTestCase& operator()(const char* name, YTestExpectStatement statement);
	void operator()() { this->execute(); }

	YTestLogger* logger() { return this->_logger == 0 ? y_defaultTestLogger : this->_logger; }
	YTestManager& logger(YTestLogger* logger) { this->_logger = logger; return *this; }
};
//YTestManager* YTestManager::_singleon = 0;

extern YTestManager& y_unit;














#endif // !__HDEF_H__

