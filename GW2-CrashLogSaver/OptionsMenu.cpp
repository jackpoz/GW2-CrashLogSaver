#include "OptionsMenu.h"
#include <imgui/imgui.h>
#include <stdexcept>

OptionsMenu optionsMenu;

void OptionsMenu::Draw()
{	
	if (ImGui::Checkbox("Enable AddVectoredExceptionHandler", &enableAddVectoredExceptionHandler))
	{
	}
	if (ImGui::Checkbox("Enable AddVectoredContinueHandler", &enableAddVectoredContinueHandler))
	{
	}
	if (ImGui::Checkbox("Enable SetUnhandledExceptionFilter", &enableSetUnhandledExceptionFilter))
	{
	}
	if (ImGui::Checkbox("Enable _set_invalid_parameter_handler", &enable_set_invalid_parameter_handler))
	{
	}
	if (ImGui::Checkbox("Enable _CrtSetReportMode", &enable_CrtSetReportMode))
	{
	}

	ImGui::Separator();
	ImGui::Text("DEBUG ONLY!");
	if (ImGui::Button("Throw a std::runtime_error"))
	{
		throw std::runtime_error("This is a test exception thrown from GW2-CrashLogSaver");
	}
	if (ImGui::Button("assert(false)"))
	{
		assert(false && "This is a test assertion failure from GW2-CrashLogSaver");
	}
	if (ImGui::Button("nullptr dereference"))
	{
		volatile int* ptr = nullptr;
#pragma warning(push)
#pragma warning(disable: 6011)
		*ptr = 42;
#pragma warning(pop)
	}
}
