#pragma once
#include <vector>
#include <helpers/WinInclude.h>

namespace xwf {

	struct Vertex {
		float position[3];
		float color[3];
	};


	class Mesh
	{
	public:
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	};


}