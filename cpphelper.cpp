/*
 * global.cpp
 *
 * Created: 04.05.2016 14:21:09
 *  Author: sseegel
 */ 
extern "C"
{
	#include <stdlib.h>
	
	void __cxa_pure_virtual() //needed for abstract classes
	{
	}
}

void *operator new(size_t size)
{
	return malloc(size);
}

void *operator new[](size_t size)
{
	return malloc(size);
}

void operator delete(void* ptr)
{
	free(ptr);
}

void operator delete[](void* ptr)
{
	free(ptr);
}
