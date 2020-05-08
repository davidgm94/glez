#include <glez/type.h>
#include <d3d12.h>
#include <stdio.h>
#include <assert.h>
#include <dxgi1_6.h>

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

int main(s32 argc, char* argv[])
{
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
	return 0;
}