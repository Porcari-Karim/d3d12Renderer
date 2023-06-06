#include "DXContext.h"
#include <iostream>

bool xwf::DXContext::Init()
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


    return true;
}

void xwf::DXContext::Shutdown()
{
    //m_fence.ReleaseAndGetAddressOf();
    m_commandQueue.ReleaseAndGetAddressOf();
    m_device.ReleaseAndGetAddressOf();
}
