#include "DX12Context.h"
#include <iostream>


bool xwf::DX12Context::Init()
{
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&m_device));
    if (FAILED(hr))
    {
        // Gérer l'erreur de création du périphérique
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // Ou le type de file d'attente approprié
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr))
        return false;

    hr = m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    if (FAILED(hr))
        return false;
    m_fenceEvent = CreateEventW(nullptr, false, false, nullptr);
    if (!m_fenceEvent)
        return false;

    hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocator));
    if (FAILED(hr))
        return false;

    hr = m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_cmdList));
    if (FAILED(hr))
        return false;

    //dxgi
    hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_factory));
    if (FAILED(hr))
        return false;


    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    swapchainDesc.Width = 0;
    swapchainDesc.Height = 0;
    swapchainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchainDesc.Stereo = FALSE;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapchainDesc.Flags = 0;

    ComPtr<IDXGISwapChain1> swapChain1;

    hr = m_factory->CreateSwapChainForHwnd(m_commandQueue.Get(), Win32Window::getHWND(), &swapchainDesc, nullptr, nullptr, &swapChain1);
    if (FAILED(hr))
        return false;

    hr = swapChain1.As(&m_swapChain);
    if (FAILED(hr))
        return false;


    return true;
}

void xwf::DX12Context::Shutdown()
{
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
