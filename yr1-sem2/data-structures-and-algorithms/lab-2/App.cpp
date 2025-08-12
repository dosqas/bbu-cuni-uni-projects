#include "ShortTest.h"
#include "ExtendedTest.h"
#include "SortedSet.h"
#include "SortedSetIterator.h"
#include <iostream>
#include <assert.h>

using namespace std;

bool r_subset(TComp e1, TComp e2) {
	if (e1 <= e2) {
		return true;
	}
	else {
		return false;
	}
}

void test_subset() {
	SortedSet ss1(r_subset);
	ss1.add(1);
	ss1.add(2);
	ss1.add(3);
	ss1.add(4);
	ss1.add(5);
	SortedSet ss2(r_subset);
	ss2.add(1);
	ss2.add(2);
	ss2.add(3);
	SortedSet ss3(r_subset);
	ss3.add(1);
	ss3.add(10);
	assert(ss1.isSubsetOf(ss2) == false);
	assert(ss2.isSubsetOf(ss1) == true);
	assert(ss3.isSubsetOf(ss1) == false);
}

int main() {
	testAll();
	testAllExtended();
	test_subset();

	cout << "Test end" << endl;
	system("pause");
}