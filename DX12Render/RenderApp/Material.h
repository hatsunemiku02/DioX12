#pragma once
#include <d3d12.h>
#include <memory>
#include <vector>
#include "RenderSystem/d3dx12.h"
#include "RenderSystem/DX12Device.h"
#include "RenderSystem/DX12PSO.h"
#include "RenderSystem/UploadBuffer.h"
#include "RenderSystem/DX12Shader.h"
#include "RenderSystem/ConstantBuffer.h"

class Material
{
public:
	Material();
	~Material();

	std::shared_ptr<DX12RootSig> m_pRootSig;
	std::shared_ptr<DX12Shader> m_pShader;
	std::shared_ptr<DX12PSO> m_pPSO;
	
	std::vector<ConstantBuffer> m_ConstantBuffers;

	unsigned int m_OffsetInCache;
	void* m_MatContent;
	unsigned int m_ContentSize;
private:
	
};

