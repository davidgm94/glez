typedef enum IMGUI_RESULT {
	IMGUI_RESULT_SUCCESS,
	IMGUI_RESULT_WARN,
	IMGUI_RESULT_CRITICAL,
} IMGUI_RESULT;

typedef enum IMGUI_GUI_API {
	GLFW,
} IMGUI_GUI_API;

typedef enum IMGUI_GRAPHICS_API {
	OPENGL,
} IMGUI_GRAPHICS_API;

typedef enum GlfwClientApi
{
    GlfwClientApi_Unknown,
    GlfwClientApi_OpenGL,
    GlfwClientApi_Vulkan
} GlfwClientApi;

IMGUI_RESULT init_imgui(IMGUI_GRAPHICS_API graphicsAPI, IMGUI_GUI_API guiAPI, void* window);