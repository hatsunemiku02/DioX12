#include "DX12PSO.h"
#include "DX12Shader.h"
#include "DX12VBO.h"
#include "DX12RootSig.h"

DX12PSO::DX12PSO()
{
	ZeroMemory(&m_psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_psoDesc.DepthStencilState.DepthEnable = FALSE;
	m_psoDesc.DepthStencilState.StencilEnable = FALSE;
	m_psoDesc.SampleMask = UINT_MAX;
	m_psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_psoDesc.NumRenderTargets = 1;
	m_psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_psoDesc.SampleDesc.Count = 1;
	m_psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
}

DX12PSO::~DX12PSO()
{

}

void DX12PSO::SetShader(const DX12Shader& shader)
{
	m_psoDesc.VS = CD3DX12_SHADER_BYTECODE(shader.GetVertexShader().Get());
	m_psoDesc.PS = CD3DX12_SHADER_BYTECODE(shader.GetPixelShader().Get());
}

void DX12PSO::SetVertexLayout(const DX12VBO& vbo)
{
	m_psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_psoDesc.InputLayout = { (vbo.GetVertexLayout()), vbo.GetInputElementCount() };
}

void DX12PSO::SetupPSO(DX12Device& device, DX12RootSig& rootSig)
{
	m_pRootSig = &rootSig;
	m_psoDesc.pRootSignature = rootSig.GetRootSig();
	ThrowIfFailed(device.GetDX12Device()->CreateGraphicsPipelineState(&m_psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}

ID3D12PipelineState* DX12PSO::GetPipelineState() const
{
	return m_pipelineState.Get();
}

