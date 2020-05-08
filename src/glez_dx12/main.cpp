#include <glez/type.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <stdio.h>
#include <assert.h>
#include <dxgi1_6.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

IDXGIAdapter1* get_hardware_adapter(IDXGIFactory4* factory)
{
	for (u32 i = 0; ; i++)
	{
		IDXGIAdapter1* adapter = NULL;
		if (factory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), NULL)))
		{
			return adapter;
		}

		adapter->Release();
	}

	return NULL;
}

s32 main(s32 argc, char* argv[])
{
	s32 glfw_init = glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "GLEZ DirectX 12", NULL, NULL);
	HWND native_window = glfwGetWin32Window(window);
	u32 dxgi_factory_flags = 0;
#ifdef GAME_DEBUG
	ID3D12Debug* debug_controller = NULL;
	WIN32_CHECK(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller)));
	debug_controller->EnableDebugLayer();
	dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
	debug_controller->Release();
#endif

	IDXGIFactory4* factory = NULL;
	WIN32_CHECK(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory)));
	IDXGIAdapter1* hw_adapter = get_hardware_adapter(factory);
	ID3D12Device* device = NULL;
	WIN32_CHECK(D3D12CreateDevice(hw_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));

	D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
	command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ID3D12CommandQueue* command_queue = NULL;
	WIN32_CHECK(device->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&command_queue)));

	const u32 frame_count = 2;
	DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {};
	swapchain_desc.BufferCount = frame_count;
	swapchain_desc.Width = width;
	swapchain_desc.Height = height;
	swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.SampleDesc.Count = 1;

	IDXGISwapChain3* swapchain = NULL;
	IDXGISwapChain1* swapchain1 = NULL;
	WIN32_CHECK(factory->CreateSwapChainForHwnd(command_queue, native_window, &swapchain_desc, NULL, NULL, &swapchain1));
	WIN32_CHECK(factory->MakeWindowAssociation(native_window, DXGI_MWA_NO_ALT_ENTER));
	WIN32_CHECK(swapchain1->QueryInterface(&swapchain));
	u32 frame_index = swapchain->GetCurrentBackBufferIndex();

	ID3D12DescriptorHeap* rtv_heap = NULL;
	u32 rtv_descriptor_size = 0;
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
		rtv_heap_desc.NumDescriptors = frame_count;
		rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		WIN32_CHECK(device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap)));
		rtv_descriptor_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(rtv_heap->GetCPUDescriptorHandleForHeapStart());

	ID3D12Resource* render_targets[frame_count];

	for (u32 n = 0; n < frame_count; n++)
	{
		WIN32_CHECK(swapchain->GetBuffer(n, IID_PPV_ARGS(&render_targets[n])));
		device->CreateRenderTargetView(render_targets[n], NULL, rtv_handle);
		rtv_handle.Offset(1, rtv_descriptor_size);
	}

	ID3D12CommandAllocator* command_allocator = NULL;
	WIN32_CHECK(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)));

	ID3D12GraphicsCommandList* gfx_cmd_list = NULL;
	WIN32_CHECK(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator, NULL, IID_PPV_ARGS(&gfx_cmd_list)));
	WIN32_CHECK(gfx_cmd_list->Close());
	ID3D12Fence* fence = NULL;
	WIN32_CHECK(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	u32 fence_value = 1;
	HANDLE fence_event = NULL;
	fence_event = CreateEvent(NULL, false, false, NULL);
	if (!fence_event) {
		WIN32_CHECK(HRESULT_FROM_WIN32(GetLastError()));
	}

	ID3D12PipelineState* pipeline_state = NULL;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		WIN32_CHECK(command_allocator->Reset());
		WIN32_CHECK(gfx_cmd_list->Reset(command_allocator, pipeline_state));
		CD3DX12_RESOURCE_BARRIER resource_barrier_start = CD3DX12_RESOURCE_BARRIER::Transition(render_targets[frame_index], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		gfx_cmd_list->ResourceBarrier(1, &resource_barrier_start);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle_frame(rtv_heap->GetCPUDescriptorHandleForHeapStart(), frame_index, rtv_descriptor_size);
		const float clear_color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		gfx_cmd_list->ClearRenderTargetView(rtv_handle_frame, clear_color, 0, NULL);
		CD3DX12_RESOURCE_BARRIER resource_barrier_end = CD3DX12_RESOURCE_BARRIER::Transition(render_targets[frame_index], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		gfx_cmd_list->ResourceBarrier(1, &resource_barrier_end);
		WIN32_CHECK(gfx_cmd_list->Close());
		
		ID3D12CommandList* command_lists[] = { gfx_cmd_list };
		command_queue->ExecuteCommandLists(ARRAYSIZE(command_lists), command_lists);
		WIN32_CHECK(swapchain->Present(1, 0));

		const u64 fence_value_frame = fence_value;
		WIN32_CHECK(command_queue->Signal(fence, fence_value_frame));
		fence_value++;

		if (fence->GetCompletedValue() < fence_value_frame)
		{
			WIN32_CHECK(fence->SetEventOnCompletion(fence_value_frame, fence_event));
			WaitForSingleObject(fence_event, INFINITE);
		}

		frame_index = swapchain->GetCurrentBackBufferIndex();
	}
}