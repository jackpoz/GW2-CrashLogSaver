#pragma once
class CrashTriggerer
{
public:
	void AccessDeletedMemory();
	void Assert();
	void NullDereference();
	void NullDereferenceWithTryExcept();
	void ThrowException();
	void ThrowExceptionWithTryCatch();
};

extern CrashTriggerer crashTriggerer;