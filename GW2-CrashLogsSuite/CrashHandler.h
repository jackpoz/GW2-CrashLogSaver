#pragma once
#define SENTRY_BUILD_STATIC 1
#include <crtdbg.h>
#include <windows.h>
#include <string>

class CrashHandler
{
public:
	void ToggleAddVectoredExceptionHandler(bool enable);
	void ToggleAddVectoredContinueHandler(bool enable);
	void ToggleSetUnhandledExceptionFilter(bool enable);
	void Toggle_set_invalid_parameter_handler(bool enable);
	void Toggle_CrtSetReportMode(bool enable);
	void ToggleSentryReporting(bool enable, std::string dsn);
	void Unload();
private:
	PVOID addVectoredExceptionHandlerHandle;
	PVOID addVectoredContinueHandlerHandle;
	LPTOP_LEVEL_EXCEPTION_FILTER previousUnhandledExceptionFilterHandler;
	_invalid_parameter_handler previousInvalidParameterHandler;
	int previousCrtReportModeError = _CRTDBG_MODE_WNDW;
	int previousCrtReportModeAssert = _CRTDBG_MODE_WNDW;
};

extern CrashHandler crashHandler;
