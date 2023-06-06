#pragma once
#include <helpers/WinInclude.h>
using namespace Microsoft::WRL;

namespace xwf {

	class DXContext {

	public:
		bool Init();
		void Shutdown();

		inline ComPtr<ID3D12Device9>& GetDevice()
		{
			return m_device;
		}

		inline ComPtr<ID3D12CommandQueue>& GetCommandQueue()
		{
			return m_commandQueue;
		}

	private:
		ComPtr<ID3D12Device9> m_device;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12Fence1> m_fence;
		UINT64 m_fenceValue = 0;

	public:
		DXContext(const DXContext&) = delete;
		DXContext& operator=(const DXContext&) = delete;

		inline static DXContext& Get()
		{
			static DXContext instance;
			return instance;
		}

	private:
		DXContext() = default;
	};
}