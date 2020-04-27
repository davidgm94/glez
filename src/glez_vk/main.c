#include <glez/type.h>
#include <vulkan/vulkan.h>
#include <assert.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
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

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	return 0;
}