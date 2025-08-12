#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
	friend class SortedBag;

private:
	const SortedBag& bag;
	SortedBagIterator(const SortedBag& b);

	TComp current;
	TComp* values;
	
	void inOrder(SortedBag::BSTNode* node);

public:
	TComp getCurrent();
	bool valid();
	void next();
	void first();
};

