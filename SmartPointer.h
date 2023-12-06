#pragma once
#include "RefCounter.h"
#include "Tree.h"
template <typename T> class SmartPointer
{
public:
	SmartPointer(T* pcPointer)
	{
		pointer = pcPointer;
		pc_counter = new RefCounter();
		pc_counter->iAdd();
	}

	SmartPointer(const SmartPointer& pcOther)
	{
		pointer = pcOther.pointer;
		pc_counter = pcOther.pc_counter;
		pc_counter->iAdd();
	}

	~SmartPointer()
	{
		if (pc_counter->iDec() == 0)
		{
			delete pointer;
			delete pc_counter;
		}
	}

	T& operator*() { return(*pointer); }
	T* operator->() { return(pointer); }
	void operator=(const SmartPointer& pcOther)
	{
		if (pc_counter->iDec() == 0) // if this is the last SmartPointer pointing to the object, delete it
		{
			delete pointer;
			delete pc_counter;
		}
		pointer = pcOther.pointer;
		pc_counter = pcOther.pc_counter;
		pc_counter->iAdd();
	}

private:
	RefCounter* pc_counter;
	T* pointer;
};