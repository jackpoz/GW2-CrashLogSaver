#include "CrashHandler.h"

CrashHandler crashHandler;

LONG VectoredExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo);
LONG VectoredContinueHandler(_EXCEPTION_POINTERS* ExceptionInfo);
LONG UnhandledExceptionFilterHandler(_EXCEPTION_POINTERS* ExceptionInfo);
void InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);

void CrashHandler::ToggleAddVectoredExceptionHandler(bool enable)
{
	if (enable)
	{
		addVectoredExceptionHandlerHandle = AddVectoredExceptionHandler(0, VectoredExceptionHandler);
	}
	else
	{
		RemoveVectoredExceptionHandler(addVectoredExceptionHandlerHandle);
	}
}

void CrashHandler::ToggleAddVectoredContinueHandler(bool enable)
{
	if (enable)
	{
		addVectoredContinueHandlerHandle = AddVectoredContinueHandler(0, VectoredContinueHandler);
	}
	else
	{
		RemoveVectoredContinueHandler(addVectoredContinueHandlerHandle);
	}
}

void CrashHandler::ToggleSetUnhandledExceptionFilter(bool enable)
{
	if (enable)
	{
		previousUnhandledExceptionFilterHandler = SetUnhandledExceptionFilter(UnhandledExceptionFilterHandler);
	}
	else
	{
		auto previous = SetUnhandledExceptionFilter(previousUnhandledExceptionFilterHandler);
		if (previous != UnhandledExceptionFilterHandler)
		{
			// Restore the previous one as it wasn't updated by us
			SetUnhandledExceptionFilter(previous);
		}
	}
}

void CrashHandler::Toggle_set_invalid_parameter_handler(bool enable)
{
	if (enable)
	{
		previousInvalidParameterHandler = _set_invalid_parameter_handler(InvalidParameterHandler);
	}
	else
	{
		auto previous = _set_invalid_parameter_handler(previousInvalidParameterHandler);
		if (previous != InvalidParameterHandler)
		{
			// Restore the previous one as it wasn't updated by us
			_set_invalid_parameter_handler(previous);
		}
	}
}

void CrashHandler::Toggle_CrtSetReportMode(bool enable)
{
	if (enable)
	{
		if (!IsDebuggerPresent())
		{
			previousCrtReportModeError = _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
			previousCrtReportModeAssert = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
		}
	}
	else
	{
		_CrtSetReportMode(_CRT_ERROR, previousCrtReportModeError);
		_CrtSetReportMode(_CRT_ASSERT, previousCrtReportModeAssert);
	}
}

LONG VectoredExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	return EXCEPTION_CONTINUE_SEARCH;
}

LONG VectoredContinueHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	return EXCEPTION_CONTINUE_SEARCH;
}

LONG UnhandledExceptionFilterHandler(_EXCEPTION_POINTERS* ExceptionInfo)
{
	return EXCEPTION_CONTINUE_SEARCH;
}

void InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
}

void CrashHandler::Unload()
{
	ToggleAddVectoredExceptionHandler(false);
	ToggleAddVectoredContinueHandler(false);
	ToggleSetUnhandledExceptionFilter(false);
	Toggle_set_invalid_parameter_handler(false);
	Toggle_CrtSetReportMode(false);
}
