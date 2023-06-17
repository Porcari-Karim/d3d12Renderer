#ifdef _WIN32
#include "DX12Context.h"
#include <iostream>
#include <fstream>
#include <stdint.h>

#define ROUND256(structure) (sizeof(structure) + 255 ) & ~ 255

#define FRAME_BUFFER_COUNT 2

bool xwf::DX12Context::Init()
{
    //dxgi
    HRESULT hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_factory));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DXGI Factory\n";
        return false;
    }


    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&m_device));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Device\n";
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // Ou le type de file d'attente approprié
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Command Queue\n";
        return false;
    }

    hr = m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Fence\n";
        return false;
    }
    m_fenceEvent = CreateEventW(nullptr, false, false, nullptr);
    if (!m_fenceEvent)
    {
        std::cout << "Failed to create Win32 Event (event for DX12 Fence)\n";
        return false;
    }

    hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocator));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Command Allocator (type Direct)\n";
        return false;
    }

    hr = m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_cmdList));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Command List (type Direct)\n";
        return false;
    }



    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    swapchainDesc.Width = 0;
    swapchainDesc.Height = 0;
    swapchainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchainDesc.Stereo = FALSE;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = FRAME_BUFFER_COUNT;
    swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapchainDesc.Flags = 0;

    ComPtr<IDXGISwapChain1> swapChain1;

    hr = m_factory->CreateSwapChainForHwnd(m_commandQueue.Get(), Win32Window::getHWND(), &swapchainDesc, nullptr, nullptr, &swapChain1);
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 SwapChain\n";
        return false;
    }

    hr = swapChain1.As(&m_swapChain);
    if (FAILED(hr))
    {
        std::cout << "Failed to convert IDXGISwapChain1 to IDXGISwapChain4\n";
        return false;
    }

    // Create Frame Buffers
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDescriptor = {};
    rtvHeapDescriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDescriptor.NumDescriptors = FRAME_BUFFER_COUNT;
    hr = m_device->CreateDescriptorHeap(&rtvHeapDescriptor, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Render Target View Descriptor Heap\n";
        return false;
    }

    unsigned int rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (unsigned int i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i]));
        if (FAILED(hr))
        {
            std::cout << "Failed to get DX12 Frame Buffer number " << i << " from SwapChain\n";
            return false;
        }
        m_device->CreateRenderTargetView(m_buffers[i].Get(), nullptr, rtvHandle);
        rtvHandle.ptr += rtvDescriptorSize;
    }

    return true;
}

void xwf::DX12Context::Shutdown()
{
    m_rtvHeap.ReleaseAndGetAddressOf();
    m_buffers[0].ReleaseAndGetAddressOf();
    m_buffers[1].ReleaseAndGetAddressOf();
    m_swapChain.ReleaseAndGetAddressOf();
    m_factory.ReleaseAndGetAddressOf();
    if (m_fenceEvent)
        CloseHandle(m_fenceEvent);
    m_cmdList.ReleaseAndGetAddressOf();
    m_cmdAllocator.ReleaseAndGetAddressOf();
    m_fence.ReleaseAndGetAddressOf();
    m_commandQueue.ReleaseAndGetAddressOf();
    m_device.ReleaseAndGetAddressOf();
}

void xwf::DX12Context::SignalAndWait()
{
    m_commandQueue->Signal(m_fence.Get(), ++m_fenceValue);
    if (SUCCEEDED(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent)))
    {
        // std::cout << "Fence value : " << m_fenceValue << std::endl;
        if (WaitForSingleObject(m_fenceEvent, 20000) != WAIT_OBJECT_0)
            std::exit(-1); 

    }
    else
    {
        std::exit(-1);
    }
}

ID3D12GraphicsCommandList6* xwf::DX12Context::InitCommandList()
{
    m_cmdAllocator->Reset();
    m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);
    return m_cmdList.Get();
}

void xwf::DX12Context::ExecuteCommandList()
{
    if (SUCCEEDED(m_cmdList->Close()))
    {
        ID3D12CommandList* lists[] = { m_cmdList.Get()};
        m_commandQueue->ExecuteCommandLists(1, lists);
        SignalAndWait();
    }
}

void xwf::DX12Context::PopulateResourceData(ID3D12Resource* pResource, const void* pData, size_t size)
{
    UINT8* pDataBegin;
    
    //No read
    D3D12_RANGE readRange = {};
    readRange.Begin = 0;
    readRange.End = 0;

    HRESULT hr = pResource->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin));
    if (FAILED(hr))
    {
        std::cout << "Failed to copu DX12 Resource Data\n";
        return;
    }
    memcpy(pDataBegin, pData, size);
    pResource->Unmap(0, nullptr);

}

std::vector<char> xwf::DX12Context::ReadBytecodeData(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    bool exists = (bool)file;

    if (!exists || !file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

struct Vertex {
    float position[3];
    float color[3];
};

void xwf::DX12Context::TestRendering()
{

    Vertex vertexBufferData[] = {
        {0.0, 0.5, 1.0}, {1.0, 0.0, 0.0},
        {0.5, -0.5, 1.0}, {0.0, 1.0, 0.0},
        {-0.5, -0.5, 1.0}, {0.0, 0.0, 1.0}
    };

    uint32_t indexBufferData[] = { 0u, 1u, 2u };

    D3D12_HEAP_DESC heapDescriptor = {};
    heapDescriptor.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;

    ComPtr<ID3D12Resource> pVertexBuffer;
    ComPtr<ID3D12Resource> pIndexBuffer;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView; // To pass to our pipeline;
    D3D12_INDEX_BUFFER_VIEW indexBufferView;
    
    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.MipLevels = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.Width = sizeof(vertexBufferData);
    bufferDesc.Height = 1;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.SampleDesc.Quality = 0;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    
    

    HRESULT hr = m_device->CreateCommittedResource(&heapDescriptor.Properties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr, IID_PPV_ARGS(&pVertexBuffer));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Resource (Vertex Buffer)\n";
        std::exit(-1);
    }
    PopulateResourceData(pVertexBuffer.Get(), vertexBufferData, sizeof(vertexBufferData));
    vertexBufferView.BufferLocation = pVertexBuffer->GetGPUVirtualAddress();
    vertexBufferView.SizeInBytes = sizeof(vertexBufferData);
    vertexBufferView.StrideInBytes = sizeof(Vertex);

    //Change the bufferDesc width to the indexBufferData width since all the other members are the same so we can use the same struct.
    bufferDesc.Width = sizeof(indexBufferData);

    hr = m_device->CreateCommittedResource(&heapDescriptor.Properties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_INDEX_BUFFER, nullptr, IID_PPV_ARGS(&pIndexBuffer));

    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Resource (Index Buffer)\n";
        std::exit(-1);
    }
    PopulateResourceData(pIndexBuffer.Get(), indexBufferData, sizeof(indexBufferData));
    indexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
    indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    indexBufferView.SizeInBytes = sizeof(indexBufferData);

    // Shaders
    std::vector<char> vertexShaderBlob = ReadBytecodeData("vertex.cso");
    D3D12_SHADER_BYTECODE vertexShaderBytecode;
    vertexShaderBytecode.BytecodeLength = vertexShaderBlob.size();
    vertexShaderBytecode.pShaderBytecode = vertexShaderBlob.data();

    std::vector<char> pixelShaderBlob = ReadBytecodeData("pixel.cso");
    D3D12_SHADER_BYTECODE pixelShaderBytecode;
    pixelShaderBytecode.BytecodeLength = pixelShaderBlob.size();
    pixelShaderBytecode.pShaderBytecode = pixelShaderBlob.data();

    // Empty root signature since we don't pass any data except our vertex buffer to our shaders
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob* pSignatureBlob = nullptr;
    ID3DBlob* pErrorBlob = nullptr;
    ComPtr<ID3D12RootSignature> pRootSignature;

    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignatureBlob, &pErrorBlob);

    hr = m_device->CreateRootSignature(0, pSignatureBlob->GetBufferPointer(), pSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
    if (FAILED(hr))
    {
        std::cout << "Failed to create Directx12 Root Signature \n";
        std::exit(-1);
    }

    ComPtr<ID3D12PipelineState> pPipelineState;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};

    pipelineStateDesc.pRootSignature = pRootSignature.Get();
    pipelineStateDesc.VS = vertexShaderBytecode;
    pipelineStateDesc.PS = pixelShaderBytecode;

    //Rasterisation
    pipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    pipelineStateDesc.RasterizerState.DepthClipEnable = TRUE;
    pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //Blend state
    D3D12_BLEND_DESC blendDesc;

    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
    FALSE,
    FALSE,
    D3D12_BLEND_ONE,
    D3D12_BLEND_ZERO,
    D3D12_BLEND_OP_ADD,
    D3D12_BLEND_ONE,
    D3D12_BLEND_ZERO,
    D3D12_BLEND_OP_ADD,
    D3D12_LOGIC_OP_NOOP,
    D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
    pipelineStateDesc.BlendState = blendDesc;

    ////
    pipelineStateDesc.SampleMask = UINT_MAX;
    pipelineStateDesc.NumRenderTargets = 1;
    pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pipelineStateDesc.SampleDesc.Count = 1;


    ///
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };
    pipelineStateDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };

    hr = m_device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&pPipelineState));
    if (FAILED(hr))
    {
        std::cout << "Failed to create DX12 Pipeline State\n";
        std::exit(-1);
    }



    // Execute Commands
    auto* cmdList = InitCommandList();

    //cmdList->SetGraphicsRootSignature(pRootSignature.Get());
    
    std::cout << m_swapChain->GetCurrentBackBufferIndex() << std::endl;

    ExecuteCommandList();
    m_swapChain->Present(1, 0);
}

#endif