#pragma once
#include <renderer/DX12Context.h>
#include <renderer/DX12Pipeline.h>

namespace xwf {

	class DX12Renderer
	{
	public:
		DX12Renderer() = default;
		void LoadScene();
		void CreateMeshFromData(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		void RenderScene();


	private:
		std::vector<Mesh> m_meshes;

		DX12Pipeline m_pipeline;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;


	};

}