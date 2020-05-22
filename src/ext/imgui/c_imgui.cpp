#include "c_imgui.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <stdio.h>

struct Logger
{
    void log(IMGUI_RESULT result, const char* message)
    {
        printf("[IMGUI %s] %s\n", getResultString(result), message);
    }
private:
    __forceinline const char* getResultString(IMGUI_RESULT result)
    {

#define ___RS_TO_STR(x) case (x): return #x
        switch (result)
        {
            ___RS_TO_STR(IMGUI_RESULT_SUCCESS);
            ___RS_TO_STR(IMGUI_RESULT_WARN);
            ___RS_TO_STR(IMGUI_RESULT_CRITICAL);
        }
#undef ___RS_TO_STR
    }
};

static Logger logger;
#define IMGUI_LOG_AND_RETURN(errorType, message) logger.log(errorType, message); return errorType
// This covers booleans and pointers
#define IMGUI_CHECK(call, errorType, message) do { auto _imgui_result = call; if (!_imgui_result) { IMGUI_LOG_AND_RETURN(errorType, message); } } while (0)

IMGUI_RESULT IMGUI_init(IMGUI_GRAPHICS_API graphicsAPI, IMGUI_GUI_API guiAPI, void* window)
{
	IMGUI_CHECK(IMGUI_CHECKVERSION(), IMGUI_RESULT_CRITICAL, "Couldn't initialize IMGUI");

	ImGuiContext* context = ImGui::CreateContext();
    IMGUI_CHECK(context, IMGUI_RESULT_CRITICAL, "Couldn't create context");
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	switch (graphicsAPI)
    {
        case IMGUI_GRAPHICS_API::OPENGL:
        {
            switch (guiAPI)
            {
                case GLFW:
                {
                    IMGUI_CHECK(ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true), IMGUI_RESULT_CRITICAL, "Couldn't initialize OpenGL");
                }
                default:
                    break;
            }
            break;
        }
	}

    return IMGUI_RESULT_SUCCESS;
}

void IMGUI_OPENGL_GLFW_frame(void)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow()
}
