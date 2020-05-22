#include <glez/type.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <glez_gl/imgui.h>
#include <glad/glad.h>

#define IMGUI_VERSION               "1.76"
#define IMGUI_VERSION_NUM           17600
#define IMGUI_CHECKVERSION() igDebugCheckVersionAndDataLayout(IMGUI_VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx))
#define IM_ASSERT(_expr) assert(_expr)
#define IM_ARRAYSIZE(_arr) COUNT_OF(_arr)

static GLuint g_GlVersion = 0;
static char g_GlslVersionString[32] = "";
static GLFWwindow*          g_Window = NULL;    // Main window
static GlfwClientApi        g_ClientApi = GlfwClientApi_Unknown;
static double               g_Time = 0.0;
static bool                 g_MouseJustPressed[5] = { false, false, false, false, false };
static GLFWcursor*          g_MouseCursors[ImGuiMouseCursor_COUNT] = {0};
static bool                 g_InstalledCallbacks = false;

// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
static GLFWmousebuttonfun   g_PrevUserCallbackMousebutton = NULL;
static GLFWscrollfun        g_PrevUserCallbackScroll = NULL;
static GLFWkeyfun           g_PrevUserCallbackKey = NULL;
static GLFWcharfun          g_PrevUserCallbackChar = NULL;


//static bool ImGui_ImplGlfw_Init(GLFWwindow* window, bool install_callbacks, GlfwClientApi client_api)
//{
//    g_Window = window;
//    g_Time = 0.0;
//
//    // Setup back-end capabilities flags
//    ImGuiIO& io = ImGui::GetIO();
//    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
//    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
//    io.BackendPlatformName = "imgui_impl_glfw";
//
//    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
//    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
//    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
//    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
//    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
//    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
//    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
//    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
//    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
//    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
//    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
//    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
//    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
//    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
//    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
//    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
//    io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
//    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
//    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
//    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
//    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
//    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
//    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
//
//    io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
//    io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
//    io.ClipboardUserData = g_Window;
//#if defined(_WIN32)
//    io.ImeWindowHandle = (void*)glfwGetWin32Window(g_Window);
//#endif
//
//    // Create mouse cursors
//    // (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
//    // GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
//    // Missing cursors will return NULL and our _UpdateMouseCursor() function will use the Arrow cursor instead.)
//    GLFWerrorfun prev_error_callback = glfwSetErrorCallback(NULL);
//    g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
//#if GLFW_HAS_NEW_CURSORS
//    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
//#else
//    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
//    g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
//#endif
//    glfwSetErrorCallback(prev_error_callback);
//
//    // Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
//    g_PrevUserCallbackMousebutton = NULL;
//    g_PrevUserCallbackScroll = NULL;
//    g_PrevUserCallbackKey = NULL;
//    g_PrevUserCallbackChar = NULL;
//    if (install_callbacks)
//    {
//        g_InstalledCallbacks = true;
//        g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
//        g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
//        g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
//        g_PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
//    }
//
//    g_ClientApi = client_api;
//    return true;
//}
//bool ImGui_ImplGlfw_InitForOpenGL(const char* glsl_version)
//{
//    GLint major, minor;
//    glGetIntegerv(GL_MAJOR_VERSION, &major);
//    glGetIntegerv(GL_MINOR_VERSION, &minor);
//    g_GlVersion = major * 100 + minor * 10;
//
//    ImGuiIO* io = igGetIO();
//    io->BackendRendererName = "imgui_impl_opengl";
//    if (g_GlVersion >= 320) {
//        io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
//    }
//
//    if (glsl_version == NULL) {
//        glsl_version = "#version 130";
//    }
//
//    assert((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersionString));
//    strcpy(g_GlslVersionString, glsl_version);
//    strcat(g_GlslVersionString, "\n");
//
//    GLint currentTexture;
//    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
//
//    return true;
//}
//
//static __forceinline const char* getResultString(IMGUI_RESULT result)
//{
//#define ___RS_TO_STR(x) case (x): return #x
//	switch (result)
//	{
//		___RS_TO_STR(IMGUI_RESULT_SUCCESS);
//		___RS_TO_STR(IMGUI_RESULT_WARN);
//		___RS_TO_STR(IMGUI_RESULT_CRITICAL);
//	}
//#undef ___RS_TO_STR
//}
//static void logger(IMGUI_RESULT result, const char* message)
//{
//	printf("[IMGUI %s] %s\n", getResultString(result), message);
//}
//
//#define IMGUI_LOG_AND_RETURN(errorType, message) logger(errorType, message); return errorType
//// This covers booleans and pointers
//#define IMGUI_CHECK(call, errorType, message) do { s64 _imgui_result = (s64)call; if (!_imgui_result) { IMGUI_LOG_AND_RETURN(errorType, message); } } while (0)
//
//IMGUI_RESULT IMGUI_init(IMGUI_GRAPHICS_API graphicsAPI, IMGUI_GUI_API guiAPI, void* window)
//{
//    IMGUI_CHECK(IMGUI_CHECKVERSION(), IMGUI_RESULT_CRITICAL, "Couldn't initialize IMGUI");
//
//    ImGuiContext* context = igCreateContext(NULL);
//    IMGUI_CHECK(context, IMGUI_RESULT_CRITICAL, "Couldn't create context");
//    ImGuiIO* io = igGetIO(); (void)io;
//    igStyleColorsDark(NULL);
//
//	switch (graphicsAPI)
//	{
//		case OPENGL:
//		{
//			switch (guiAPI)
//			{
//				case GLFW:
//				{
//ImGui_ImplGlfw_InitForOpenGL
//					// IMGUI_CHECK(ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true), IMGUI_RESULT_CRITICAL, "Couldn't initialize OpenGL");
//				}
//				default:
//					break;
//			}
//			break;
//		}
//	}
//
//    return IMGUI_RESULT_SUCCESS;
//}
//
//
//void IMGUI_OPENGL_GLFW_startFrame(void)
//{
//    //ImGui_ImplOpenGL3_NewFrame();
//    //ImGui_ImplGlfw_NewFrame();
//    igNewFrame();
//    igShowDemoWindow(NULL);
//}
//
//void IMGUI_OPENGL_GLFW_recordFrame(void)
//{
//}
