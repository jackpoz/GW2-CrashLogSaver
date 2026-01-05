#include "OptionsMenu.h"
#include <imgui/imgui.h>
#include <stdexcept>

OptionsMenu optionsMenu;

void OptionsMenu::Draw()
{	
	ImGui::Text("Configure your crash log options here");
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
