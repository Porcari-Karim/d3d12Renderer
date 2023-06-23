#include "DX12Renderer.h"

void xwf::DX12Renderer::LoadScene()
{
    // Shaders
    m_pipeline.vertexShaderBlob = xwf::DX12Context::Get().ReadBytecodeData("vertex.cso");
    m_pipeline.vertexShaderBytecode.BytecodeLength = m_pipeline.vertexShaderBlob.size();
    m_pipeline.vertexShaderBytecode.pShaderBytecode = m_pipeline.vertexShaderBlob.data();

    m_pipeline.pixelShaderBlob = xwf::DX12Context::Get().ReadBytecodeData("pixel.cso");
    m_pipeline.pixelShaderBytecode.BytecodeLength = m_pipeline.pixelShaderBlob.size();
    m_pipeline.pixelShaderBytecode.pShaderBytecode = m_pipeline.pixelShaderBlob.data();

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    xwf::DX12Context::Get().CreateAndSerializeRootSignature(&rootSignatureDesc, m_pipeline.pRootSignature.GetAddressOf());

    xwf::DX12Context::Get().CreatePipelineState(m_pipeline.pRootSignature.Get(), m_pipeline.vertexShaderBytecode, m_pipeline.pixelShaderBytecode, m_pipeline.pPipelineState.GetAddressOf());
}

void xwf::DX12Renderer::CreateMeshFromData(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{

    m_meshes.push_back({});
    auto index = m_meshes.size() - 1;

    m_meshes[index].m_vertices = vertices;
    m_meshes[index].m_indices = indices;

    xwf::DX12Context::Get().CreateBuffer<xwf::Vertex>(m_meshes[index].m_vertices, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_meshes[index].m_vertexBuffer.GetAddressOf());
    m_meshes[index].m_vertexBufferView.BufferLocation = m_meshes[index].m_vertexBuffer->GetGPUVirtualAddress();
    m_meshes[index].m_vertexBufferView.SizeInBytes = m_meshes[index].m_vertices.size() * sizeof(xwf::Vertex);
    m_meshes[index].m_vertexBufferView.StrideInBytes = sizeof(xwf::Vertex);


    xwf::DX12Context::Get().CreateBuffer<uint32_t>(m_meshes[index].m_indices, D3D12_RESOURCE_STATE_INDEX_BUFFER, m_meshes[index].m_indexBuffer.GetAddressOf());
    m_meshes[index].m_indexBufferView.BufferLocation = m_meshes[index].m_indexBuffer->GetGPUVirtualAddress();
    m_meshes[index].m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    m_meshes[index].m_indexBufferView.SizeInBytes = m_meshes[index].m_indices.size() * sizeof(uint32_t);

    //m_meshes.push_back(mesh);
}

void xwf::DX12Renderer::RenderScene()
{
    DX12Context::Get().InitCommandList();
    DX12Context::Get().PreparePass(m_meshes, m_pipeline.pPipelineState.Get(), m_pipeline.pRootSignature.Get());
    DX12Context::Get().ExecuteCommandList();
    DX12Context::Get().GetSwapchain()->Present(1, 0);
}
