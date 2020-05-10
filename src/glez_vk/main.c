#define NV_EXTENSIONS 1
#include <glez/type.h>
#include <vulkan/vulkan.h>
#include <assert.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glslang_c_interface.h>
#include <glez/file.h>

s32 main(void)
{
	s32 glfw_init = glfwInit();
	assert(glfw_init == GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1024, 578, "GLEZ VK", NULL, NULL);
	assert(window);
	u32 api_version;
	assert(vkEnumerateInstanceVersion(&api_version) == VK_SUCCESS);
	u32 api_desired_version = VK_MAKE_VERSION(1, 1, 0);
	bool api_version_accomplished = api_version > api_desired_version;
	printf("%s\n", api_version_accomplished ? "True" : "False");

	printf("Loading SPIR-V stuff\n");


#if 1
	const char* code = load_file("src/shaders/triangle_vert.glsl");
#else
	const char* code = "WTF";
#endif

	glslang_resource_t r = { 0 };
	const glslang_input_t input =
	{
		.language = GLSLANG_SOURCE_GLSL,
		.stage = GLSLANG_STAGE_VERTEX,
		.client = GLSLANG_CLIENT_VULKAN,
		.client_version = GLSLANG_TARGET_VULKAN_1_1,
		.target_language = GLSLANG_TARGET_SPV,
		.target_language_version = GLSLANG_TARGET_SPV_1_3,
		.code = code,
		.default_version = 100,
		.default_profile = GLSLANG_NO_PROFILE,
		.force_default_version_and_profile = false,
		.forward_compatible = false,
		.messages = GLSLANG_MSG_DEFAULT_BIT,
		.resource = &r,
	};

	glslang_initialize_process();

	glslang_shader_t* shader = glslang_shader_create(&input);
	if (!glslang_shader_preprocess(shader, &input)) {
		const char* debug_info_message = glslang_shader_get_info_debug_log(shader);
		const char* info_message = glslang_shader_get_info_log(shader);
		printf("ERRORS ocurred while compiling shader:\nDEBUG_INFO:\n%s\n\nINFO:\n%s\n\n", debug_info_message, info_message);
	}

	if (!glslang_shader_parse(shader, &input)) {
		const char* debug_info_message = glslang_shader_get_info_debug_log(shader);
		const char* info_message = glslang_shader_get_info_log(shader);
		printf("ERRORS ocurred while compiling shader:\nDEBUG_INFO:\n%s\n\nINFO:\n%s\n\n", debug_info_message, info_message);
	}

	glslang_program_t* program = glslang_program_create();
	glslang_program_add_shader(program, shader);

	if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
		const char* debug_info_message = glslang_program_get_info_debug_log(program);
		const char* info_message = glslang_program_get_info_log(program);
		printf("ERRORS ocurred while linking program:\nDEBUG_INFO:\n%s\n\nINFO:\n%s\n\n", debug_info_message, info_message);
	}

	glslang_program_SPIRV_generate(program, input.stage);


	const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
	if (spirv_messages) {
		printf("%s", spirv_messages);
	}

	glslang_shader_delete(shader);

	const char* spirv_code = (const char*) glslang_program_SPIRV_get_ptr(program);
	u64 spirv_code_size = glslang_program_SPIRV_get_size(program);

	/* After using: */
	// glslang_program_delete(program);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	return 0;
}