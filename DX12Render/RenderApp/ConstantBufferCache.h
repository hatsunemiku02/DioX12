#pragma once
#include <map>
#include "RenderSystem/DX12Device.h"
#include "RenderSystem/DX12Shader.h"
#include "RenderSystem/UploadBuffer.h"
#include "Material.h"
class ConstantBufferCache
{
public:
	ConstantBufferCache();
	~ConstantBufferCache();

	void SetDevice(DX12Device* pDevice);

	void AddMaterial(const Material* mat);
	void AddShader(const DX12Shader* pShader);
	void ResizeBuffer(const std::shared_ptr<DX12Shader>& pShader, unsigned int elesize,unsigned int elecount);
	void UpdateBuffer(const std::shared_ptr<DX12Shader>& pShader, unsigned int eleIndex, void* data, UINT elecount);
	UploadBufferDynamic* GetBuffer(const std::shared_ptr<DX12Shader>& pShader)
	{
		return m_pConstantBufferMap[pShader];
	}
	ComPtr<ID3D12DescriptorHeap> m_cbvHeap;

protected:
	std::map<std::shared_ptr<DX12Shader>, UploadBufferDynamic*> m_pConstantBufferMap;
	DX12Device* m_pDevice;
	
	int maxNumDescriptor;
private:

};

