#include "CrashTriggerer.h"
#include <windows.h>
#include <cassert>
#include <list>
#include <string>
#include <stdexcept>

CrashTriggerer crashTriggerer;

void CrashTriggerer::AccessDeletedMemory()
{
	auto list = new std::list<std::string>();
	for (int i = 0; i < 10; i++)
		list->emplace_back("Test string " + std::to_string(i));
	delete list;

#pragma warning(push)
#pragma warning(disable: 6001)
	list->clear();
	for (int i = 0; i < 10; i++)
		list->emplace_back("Test string after deletion" + std::to_string(i));
#pragma warning(pop)
}

void CrashTriggerer::Assert()
{
	assert(false && "This is a test assertion failure from GW2-CrashLogSaver");
}

void CrashTriggerer::NullDereference()
{
	*(volatile int*)0 = 0;
}

void CrashTriggerer::NullDereferenceWithTryExcept()
{
	__try
	{
		*(volatile int*)0 = 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

void CrashTriggerer::ThrowException()
{
	throw std::runtime_error("This is a test exception thrown from GW2-CrashLogSaver");
}

void CrashTriggerer::ThrowExceptionWithTryCatch()
{
	try
	{
		throw std::runtime_error("This is a test exception thrown from GW2-CrashLogSaver in a try/catch");
	}
	catch (const std::exception&)
	{
	}
}
