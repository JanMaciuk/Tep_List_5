#pragma once
class RefCounter
{
public:
	RefCounter() { refCount = 0; }
	int iAdd() { return(++refCount); }
	int iDec() { return(--refCount); };
	int iGet() { return(refCount); }
private:
	int refCount;
};