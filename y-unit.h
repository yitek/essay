#pragma once
#ifndef __YUNIT_H__
#define __YUNIT_H__


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
	YTestValue(const void* value) :_kind(YTestValueKinds::Pointer), _pointer((void*)value) {};
	YTestValue(const char* value);
	YTestValue(float value) :_kind(YTestValueKinds::Float), _float(value) {};
	YTestValue(double value) :_kind(YTestValueKinds::Float), _float(value) {};
	YTestValue(YTestValue& other);
	YTestValue& operator=(const YTestValue& other);

	/*
	YTestValue& operator=(int value) {
		this->_kind = YTestValueKinds::Integer;
		this->_integer = value;
		return *this;
	};
	YTestValue& operator=(const void* value) {
		this->_kind = YTestValueKinds::Pointer;
		this->_pointer = (void*) value;
		return *this;
	};
	
	YTestValue& operator=(float value) {
		this->_kind = YTestValueKinds::Float;
		this->_float = (double)value;
		return *this;
	};
	YTestValue& operator=(double value) {
		this->_kind = YTestValueKinds::Float;
		this->_float = (double)value;
		return *this;
	};*/
	
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
	double ellapse() { return (_endAt - _beginAt); }
};
class YTestDescription {
private:
	friend YTestCase;
	const char* _text;
	YTestDescription* _next;
public:
	YTestDescription(const char* text);
	~YTestDescription();
	YTestDescription* next() { return this->_next; }
	const char* text() { return this->_text; }
};
class YTestCase:public YTestCounter {
private:
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
	YTestLogger* _logger;
	YTestCase(const char* name, YTestExpectStatement statement, YTestCase* parent, YTestLogger* logger);
	YTestCase(const char* name, YTestExpectStatement statement, YTestCase* parent) :YTestCase(name, statement,parent, parent->_logger) {}
	
	
public:
	YTestCase(const char* name,  YTestExpectStatement statement, YTestLogger* logger=0) :YTestCase(name, statement,0,logger) {}
	
	YTestCase(YTestExpectStatement statement, YTestLogger* logger = 0) :YTestCase(0, statement, 0, logger) {}
	
	YTestCase(const char* name, YTestStatement statement, YTestLogger* logger = 0);
	YTestCase(YTestStatement statement, YTestLogger* logger = 0);
	YTestDescription* appendDescription(const char* text);
	YTestDescription* description() { return this->_firstDescription; }
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
	int childCount() { return this->_childCount; }
	bool pass() { return this->success() == this->total(); }
	void Execute();
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
};
extern YTestLogger* y_defaultTestLogger;

inline void y_unit(YTestStatement statement, YTestLogger* logger = 0) {
	YTestCase testCase(statement, logger);
	testCase.Execute();
};

inline void y_unit(const char* name, YTestStatement statement, YTestLogger* logger = 0) {
	YTestCase testCase(name, statement, logger);
	testCase.Execute();
};

inline void y_unit(YTestExpectStatement statement, YTestLogger* logger = 0) {
	YTestCase testCase(statement, logger);
	testCase.Execute();
};

inline void y_unit(const char* name, YTestExpectStatement statement, YTestLogger* logger = 0) {
	YTestCase testCase(name, statement,logger);
	testCase.Execute();
}














#endif // !__HDEF_H__

