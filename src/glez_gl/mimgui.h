#pragma once 
typedef enum IMGUI_GUI_API {
	GLFW,
} IMGUI_GUI_API;

typedef enum IMGUI_GRAPHICS_API {
	OPENGL,
} IMGUI_GRAPHICS_API;

typedef enum IMGUI_RESULT {
	IMGUI_OK,
	IMGUI_WARN,
	IMGUI_ERROR,
	IMGUI_CRITICAL,
} IMGUI_RESULT;

IMGUI_RESULT IMGUI_init(IMGUI_GRAPHICS_API graphicsAPI, IMGUI_GUI_API guiAPI, void* window);

void IMGUI_OPENGL_GLFW_newFrame(void);

void IMGUI_OPENGL_GLFW_recordFrame(void);

void IMGUI_OPENGL_GLFW_renderFrame(GLFWwindow* window);
