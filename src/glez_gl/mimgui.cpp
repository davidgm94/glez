#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include "mimgui.h"

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
            ___RS_TO_STR(IMGUI_OK);
            ___RS_TO_STR(IMGUI_WARN);
            ___RS_TO_STR(IMGUI_CRITICAL);
        }
#undef ___RS_TO_STR
        return NULL;
    }
};


static Logger logger;
#define IMGUI_LOG_AND_RETURN(errorType, message) logger.log(errorType, message); return errorType
// This covers booleans and pointers
#define IMGUI_CHECK(call, errorType, message) do { auto _imgui_result = call; if (!_imgui_result) { IMGUI_LOG_AND_RETURN(errorType, message); } } while (0)
const char* glsl_version = "#version 130";

IMGUI_RESULT IMGUI_init(IMGUI_GRAPHICS_API graphicsAPI, IMGUI_GUI_API guiAPI, void* window)
{
	IMGUI_CHECK(IMGUI_CHECKVERSION(), IMGUI_CRITICAL, "Couldn't initialize IMGUI");

	ImGuiContext* context = ImGui::CreateContext();
    IMGUI_CHECK(context, IMGUI_CRITICAL, "Couldn't create context");
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
                    const char* opengl_error_message = "Couldn't initialize OpenGL";
                    IMGUI_CHECK(ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true), IMGUI_CRITICAL, opengl_error_message);
                    IMGUI_CHECK(ImGui_ImplOpenGL3_Init(glsl_version), IMGUI_CRITICAL, opengl_error_message);
                }
                default:
                    break;
            }
            break;
        }
	}

    return IMGUI_OK;
}

void IMGUI_OPENGL_GLFW_newFrame(void)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
}

void IMGUI_OPENGL_GLFW_recordFrame(void)
{
    ImGui::ShowDemoWindow(NULL);
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world");
    ImGui::Text("This is some useful text.");
    //ImGui::Checkbox("Demo window", NULL);
    //ImGui::Checkbox("Another window", NULL);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    if (ImGui::Button("Button")) {
        counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void IMGUI_OPENGL_GLFW_renderFrame(GLFWwindow* window)
{
    ImGui::Render();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


IMGUI_RESULT initIMGUI()
{
    return (IMGUI_RESULT)0;
}