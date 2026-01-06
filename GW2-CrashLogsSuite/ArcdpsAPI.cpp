#include "OptionsMenu.h"
#include "CrashHandler.h"
#include "version.h"

#include <ArcdpsExtension/arcdps_structs.h>
#include <imgui/imgui.h>

#include <windows.h>

#include <cstdint>

arcdps_exports arc_exports;

extern "C" __declspec(dllexport) void* get_init_addr(char* arcversion, ImGuiContext* imguictx, void* id3dptr, HANDLE arcdll, void* mallocfn, void* freefn, uint32_t d3dversion);
extern "C" __declspec(dllexport) void* get_release_addr();
arcdps_exports* mod_init();
uintptr_t mod_release();
void mod_options();

extern "C" __declspec(dllexport) void* get_init_addr(char* arcversion, ImGuiContext* imguictx, void* id3dptr, HANDLE arcdll, void* mallocfn, void* freefn, uint32_t d3dversion)
{
	ImGui::SetCurrentContext((ImGuiContext*)imguictx);
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))mallocfn, (void (*)(void*, void*))freefn);
	return mod_init;
}

extern "C" __declspec(dllexport) void* get_release_addr()
{
	return mod_release;
}

arcdps_exports* mod_init()
{
	memset(&arc_exports, 0, sizeof(arcdps_exports));
	arc_exports.sig = 0x452C2152; // CRC32 of "GW2-CrashLogsSuite"
	arc_exports.imguivers = IMGUI_VERSION_NUM;
	arc_exports.size = sizeof(arcdps_exports);
	arc_exports.out_name = "GW2-CrashLogsSuite";
	arc_exports.out_build = VERSION_FULL_STRING;
	arc_exports.options_end = mod_options;
	//arc_exports.imgui = mod_imgui;
	//arc_exports.wnd_nofilter = mod_wnd;
	//arc_exports.combat = mod_combat;
	//arc_exports.size = (uintptr_t)"error message if you decide to not load, sig must be 0";
	return &arc_exports;
}

uintptr_t mod_release()
{
	crashHandler.Unload();
	return 0;
}

void mod_options()
{
	optionsMenu.Draw();
}
