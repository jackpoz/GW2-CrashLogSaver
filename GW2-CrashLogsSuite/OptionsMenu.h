#pragma once
class OptionsMenu
{
public:
	void Draw();
private:
	char sentryDsn[256];
	bool enableSentryReporting;
	bool enableAddVectoredExceptionHandler;
	bool enableAddVectoredContinueHandler;
	bool enableSetUnhandledExceptionFilter;
	bool enable_set_invalid_parameter_handler;
	bool enable_CrtSetReportMode;
};

extern OptionsMenu optionsMenu;
