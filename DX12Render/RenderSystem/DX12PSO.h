#pragma once
#include "DX12Device.h"

class DX12Shader;
class DX12VBO;
class DX12RootSig;
class DX12PSO
{
public:
	DX12PSO();
	~DX12PSO();

	void SetShader(const DX12Shader& shader);
	void SetVertexLayout(const DX12VBO& vbo);
	
	void SetupPSO(DX12Device& device, DX12RootSig& rootSig);

	ID3D12PipelineState* GetPipelineState() const;

	DX12RootSig * m_pRootSig;
private:
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;
	ComPtr<ID3D12PipelineState> m_pipelineState;
};
