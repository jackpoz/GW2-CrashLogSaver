#include "OptionsMenu.h"
#include "CrashHandler.h"
#include "CrashTriggerer.h"
#include <imgui/imgui.h>

OptionsMenu optionsMenu;

void OptionsMenu::Draw()
{
	ImGui::InputText("Sentry DSN", sentryDsn, sizeof(sentryDsn));
	if (ImGui::Checkbox("Enable Sentry reporting", &enableSentryReporting))
	{
		if (enableSentryReporting && strnlen_s(sentryDsn, sizeof(sentryDsn)) == 0)
		{
			enableSentryReporting = false;
		}

		crashHandler.ToggleSentryReporting(enableSentryReporting, sentryDsn);
	}

	ImGui::Separator();
	if (ImGui::Checkbox("Enable AddVectoredExceptionHandler", &enableAddVectoredExceptionHandler))
	{
		crashHandler.ToggleAddVectoredExceptionHandler(enableAddVectoredExceptionHandler);
	}

	if (ImGui::Checkbox("Enable AddVectoredContinueHandler", &enableAddVectoredContinueHandler))
	{
		crashHandler.ToggleAddVectoredContinueHandler(enableAddVectoredContinueHandler);
	}

	if (ImGui::Checkbox("Enable SetUnhandledExceptionFilter", &enableSetUnhandledExceptionFilter))
	{
		crashHandler.ToggleSetUnhandledExceptionFilter(enableSetUnhandledExceptionFilter);
	}

	if (ImGui::Checkbox("Enable _set_invalid_parameter_handler", &enable_set_invalid_parameter_handler))
	{
		crashHandler.Toggle_set_invalid_parameter_handler(enable_set_invalid_parameter_handler);
	}

	if (ImGui::Checkbox("Enable _CrtSetReportMode", &enable_CrtSetReportMode))
	{
		crashHandler.Toggle_CrtSetReportMode(enable_CrtSetReportMode);
	}

	ImGui::Separator();
	ImGui::Text("DEBUG ONLY!");

	if (ImGui::Button("Throw a std::runtime_error"))
	{
		crashTriggerer.ThrowException();
	}

	if (ImGui::Button("Throw a std::runtime_error with try/catch"))
	{
		crashTriggerer.ThrowExceptionWithTryCatch();
	}

	if (ImGui::Button("assert(false)"))
	{
		crashTriggerer.Assert();
	}

	if (ImGui::Button("nullptr dereference"))
	{
		crashTriggerer.NullDereference();
	}

	if (ImGui::Button("nullptr dereference with __try/__except"))
	{
		crashTriggerer.NullDereferenceWithTryExcept();
	}

	if (ImGui::Button("access deleted memory"))
	{
		crashTriggerer.AccessDeletedMemory();
	}
}
