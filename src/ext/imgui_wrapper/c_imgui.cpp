#include "c_imgui.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>

IMGUI_RESULT init_imgui(IMGUI_GRAPHICS_API graphicsAPI, IMGUI_GUI_API guiAPI, void* window)
{
	bool version_check = IMGUI_CHECKVERSION();
	if (!version_check) return IMGUI_RESULT_CRITICAL;

	ImGuiContext* context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	switch (graphicsAPI)
	{
	case IMGUI_GRAPHICS_API::OPENGL:
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, false);
			break;
	}

}