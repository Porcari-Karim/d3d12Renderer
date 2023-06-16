#pragma once
#ifdef _WIN32
#include <helpers/WinInclude.h>
#include <window/Win32Window.h>

using namespace Microsoft::WRL;

namespace xwf {

	class DX12Context {

	public:
		bool Init();
		void Shutdown();
		void SignalAndWait();
		ID3D12GraphicsCommandList6* InitCommandList();
		void ExecuteCommandList();
		void PopulateResourceData(ID3D12Resource* pResource, const void * pData, size_t size);
		static std::vector<char> ReadBytecodeData(const std::string& filename);

		inline ComPtr<ID3D12Device9>& GetDevice()
		{
			return m_device;
		}

		inline ComPtr<ID3D12CommandQueue>& GetCommandQueue()
		{
			return m_commandQueue;
		}

		inline ComPtr<IDXGISwapChain4>& GetSwapchain()
		{
			return m_swapChain;
		}

		void TestRendering();

	private:
		ComPtr<IDXGIFactory7> m_factory;

		ComPtr<ID3D12Device9> m_device;
		ComPtr<ID3D12CommandQueue> m_commandQueue;

		ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
		ComPtr<ID3D12GraphicsCommandList6> m_cmdList;

		ComPtr<IDXGISwapChain4> m_swapChain;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12Resource2> m_buffers[2];

		ComPtr<ID3D12Fence1> m_fence;
		UINT64 m_fenceValue = 0;
		HANDLE  m_fenceEvent = nullptr;

	public:
		DX12Context(const DX12Context&) = delete;
		DX12Context& operator=(const DX12Context&) = delete;

		inline static DX12Context& Get()
		{
			static DX12Context instance;
			return instance;
		}

	private:
		DX12Context() = default;
	};
}

#endif