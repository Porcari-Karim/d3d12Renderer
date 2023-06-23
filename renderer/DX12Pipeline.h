#pragma once
#include <helpers/WinInclude.h>

class DX12Pipeline
{
public:
    std::vector<char> vertexShaderBlob;
    D3D12_SHADER_BYTECODE vertexShaderBytecode;
    

    std::vector<char> pixelShaderBlob;
    D3D12_SHADER_BYTECODE pixelShaderBytecode;

    Microsoft::WRL::ComPtr<ID3D12RootSignature> pRootSignature;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pPipelineState;

};

