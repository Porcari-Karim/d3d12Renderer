#pragma once
#ifdef _WIN32
#include <helpers/WinInclude.h>
#include <window/Win32Window.h>
#include <renderer/DX12Mesh.h>

//using namespace Microsoft::WRL;

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

		void PreparePass(std::vector<Mesh> meshes, ID3D12PipelineState* pPipelineState, ID3D12RootSignature* pRootSignature);

		template <typename T>
		void CreateBuffer(std::vector<T> vertices, D3D12_RESOURCE_STATES state, ID3D12Resource** buffer);

		void CreateAndSerializeRootSignature(D3D12_ROOT_SIGNATURE_DESC* rootSignatureDesc, ID3D12RootSignature** ppRootSignature);

		void CreatePipelineState(ID3D12RootSignature* pRootSignature, D3D12_SHADER_BYTECODE vertexShaderBytecode, D3D12_SHADER_BYTECODE pixelShaderBytecode, ID3D12PipelineState** ppPipelineState);

		inline Microsoft::WRL::ComPtr<ID3D12Device9>& GetDevice()
		{
			return m_device;
		}

		inline Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue()
		{
			return m_commandQueue;
		}

		inline Microsoft::WRL::ComPtr<IDXGISwapChain4>& GetSwapchain()
		{
			return m_swapChain;
		}

		void TestRendering();

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory7> m_factory;

		Microsoft::WRL::ComPtr<ID3D12Device9> m_device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> m_cmdList;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource2> m_buffers[2];

		Microsoft::WRL::ComPtr<ID3D12Fence1> m_fence;
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