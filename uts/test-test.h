#pragma once
#ifndef __TESTTEST_H__
#define __TESTTEST_H__
#include "../s-unit.h"

#ifdef __cplusplus 
extern "C" {
#endif


#ifdef __cplusplus 
} // end extern "C"
#endif


STestManager& testTest() {
	s_unit([](SExpect expect) {
		expect << "a simple test statement";
		expect(1).toBe(1).message("1 eq 1");
	});

	s_unit("named expects", [](SExpect expect) {
		expect << "a named test statement";
		expect("test").toBe("test").message("'test' eq 'test'");
	});

	s_unit("unit", [](STest subscribe) {
		subscribe << "a test unit" << "has many sub tests";

		subscribe("Subscribe", [](STest test) {
			test << "a pure test category.";
			test("Test", [](STest tcase) {
				tcase << "There are to cases in this statements";
				tcase("success case", [](SExpect expect) {
					expect << "Here is the success statement.";
					expect(1.0).toBe(1.0).message("1.0 eq 1.0");
				});
				tcase("fail case", [](SExpect expect) {
					expect << "Here is the fail statement." << "There are 3 asserts in this statement.";
					expect(12).toBe(11).message("12 neq 11");
					expect("ab").toBe("cd").message("ab neq cd");
					void* p = 0;
					expect(p).toBe(0).message("pointer(0) neq 0");
				}); // end fail case
			}); // end test statement
		}); // end subscribe
	});// end unit
	return s_unit;
}

#endif