#include "y-unit.h"
#include "test-test.h"

void testTest() {
	y_unit([](YExpect expect) {
		expect << "a simple test statement";
		expect(1).toBe(1).message("1 eq 1");
	});
	y_unit("named expects", [](YExpect expect) {
		expect << "a named test statement";
		expect("test").toBe("test").message("'test' eq 'test'");
	});
	y_unit("unit", [](YTest subscribe) {
		subscribe << "a test unit" << "has many sub tests";
	subscribe("Subscribe", [](YTest test) {
		test("Test", [](YTest tcase) {
			tcase << "There are to cases in this statements";
			tcase("success case", [](YExpect expect) {
					expect << "Here is the success statement.";
					expect(1.0).toBe(1.0).message("1.0 eq 1.0");
				});
				tcase("fail case", [](YExpect expect) {
					expect << "Here is the fail statement."<<"There are 3 asserts in this statement.";
					expect(12).toBe(11).message("12 neq 11");
					expect("ab").toBe("cd").message("ab neq cd");
					void* p = 0;
					expect(p).toBe(0).message("pointer(0) neq 0");
				});
			});
		});
	});
}