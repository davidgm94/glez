#include <glez/type.h>
#include <stdio.h>
#include <assert.h>
#include <d3d11_4.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <DirectXColors.h>
using namespace DirectX;



int main(void)
{
	s32 glfw_init = glfwInit();
	assert(glfw_init);

	GLFWwindow* window = glfwCreateWindow(width, height, "GLEZ DirectX 11", NULL, NULL);

	D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	u32 feature_level_count = COUNT_OF(feature_levels);


	IDXGIFactory1* factory = NULL;
	ID3D11Device* device = NULL;
	ID3D11DeviceContext* context = NULL;
	WIN32_CHECK(CreateDXGIFactory1(UUID_MACRO(IDXGIFactory1, factory)));
	D3D_FEATURE_LEVEL out_feature_levels[12];
	WIN32_CHECK(D3D11CreateDevice(NULL, driver_type, NULL, D3D11_CREATE_DEVICE_DEBUG, feature_levels, feature_level_count, D3D11_SDK_VERSION, &device, out_feature_levels, &context));

	IDXGIAdapter* adapter = NULL;
	IDXGIDevice* dxgi_device = NULL;
	WIN32_CHECK(device->QueryInterface(UUID_MACRO(IDXGIDevice, dxgi_device)));
	WIN32_CHECK(dxgi_device->GetAdapter(&adapter));


	IDXGIFactory2* dxgi_factory = NULL;
	WIN32_CHECK(factory->QueryInterface(UUID_MACRO(IDXGIFactory2, dxgi_factory)));
	IDXGISwapChain1* swapchain1 = NULL;
	IDXGISwapChain* swapchain = NULL;

	HWND window_handle = glfwGetWin32Window(window);
	if (dxgi_factory)
	{
		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		WIN32_CHECK(dxgi_factory->CreateSwapChainForHwnd(device, window_handle, &sd, NULL, NULL, &swapchain1));
		WIN32_CHECK(swapchain1->QueryInterface(UUID_MACRO(IDXGISwapChain, swapchain)));
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = glfwGetWin32Window(window);
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;

		WIN32_CHECK(factory->CreateSwapChain(device, &sd, &swapchain));
	}

	WIN32_CHECK(factory->MakeWindowAssociation(window_handle, DXGI_MWA_NO_ALT_ENTER));
	factory->Release();

	ID3D11Texture2D* backbuffer = NULL;
	WIN32_CHECK(swapchain->GetBuffer(0, UUID_MACRO(ID3D11Texture2D, backbuffer)));

	ID3D11RenderTargetView* render_target_view = NULL;
	WIN32_CHECK(device->CreateRenderTargetView(backbuffer, NULL, &render_target_view));

	context->OMSetRenderTargets(1, &render_target_view, NULL);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		context->ClearRenderTargetView(render_target_view, Colors::MidnightBlue);
		swapchain->Present(0, 0);
	}

	return 0;
}