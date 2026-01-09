#include "CrashHandler.h"
#include "version.h"
#include <sentry.h>

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

std::filesystem::path CrashHandler::GetModuleDirectoryPath()
{
	HMODULE hModule = nullptr;
	// Use the address of this function to get the module that contains it
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCSTR>(&sentry_init), &hModule);

	char buffer[MAX_PATH];
	GetModuleFileNameA(hModule, buffer, MAX_PATH);

	std::filesystem::path p = buffer;
	return p.parent_path();
}

void CrashHandler::ToggleSentryReporting(bool enable, std::string dsn)
{
	if (enable)
	{
		sentry_options_t* options = sentry_options_new();
		sentry_options_set_dsn(options, dsn.c_str());

		auto dll_dir = GetModuleDirectoryPath();
		auto handler = dll_dir / "crashpad_handler.exe";
		sentry_options_set_handler_path(options, handler.string().c_str());

		sentry_options_set_release(options, VERSION_FULL_STRING);

		// Disable everything else other than crash reports
		sentry_options_set_auto_session_tracking(options, false);
		sentry_options_set_max_breadcrumbs(options, 0);
		sentry_options_set_require_user_consent(options, true);
		sentry_options_set_system_crash_reporter_enabled(options, false);

		if (sentry_init(options))
		{
			printf("Failed to initialize Sentry SDK\n");
			return;
		}
	}
	else
	{
		sentry_close();
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
