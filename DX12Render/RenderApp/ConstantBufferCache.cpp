#include "ConstantBufferCache.h"

ConstantBufferCache::ConstantBufferCache()
	:maxNumDescriptor(10)
{

}

ConstantBufferCache::~ConstantBufferCache()
{
	
}

void ConstantBufferCache::SetDevice(DX12Device* pDevice)
{
	m_pDevice = pDevice;
}

void ConstantBufferCache::AddMaterial(const Material* mat)
{
	
}

void ConstantBufferCache::AddShader(const DX12Shader* pShader)
{
	
}

void ConstantBufferCache::ResizeBuffer(const std::shared_ptr<DX12Shader>& pShader, unsigned int elesize, unsigned int elecount)
{
	std::map<std::shared_ptr<DX12Shader>, UploadBufferDynamic*>::iterator  itr = m_pConstantBufferMap.find(pShader);
	if (itr == m_pConstantBufferMap.end())
	{
		m_pConstantBufferMap.insert(std::move(std::pair<std::shared_ptr<DX12Shader>, UploadBufferDynamic*>(pShader, NULL)));
		itr = m_pConstantBufferMap.find(pShader);
	}
	if (itr->second==nullptr)
	{
		itr->second = new UploadBufferDynamic(m_pDevice->GetDX12Device(), elesize, elecount, true);
	}
	else
	{
		if(itr->second->GetElementCount()<elecount)
		{
			delete(itr->second);
			itr->second = new UploadBufferDynamic(m_pDevice->GetDX12Device(), elesize, elecount, true);
		}
	}
}

void ConstantBufferCache::UpdateBuffer(const std::shared_ptr<DX12Shader>& pShader, unsigned int eleIndex, void* data, UINT elecount)
{
	std::map<std::shared_ptr<DX12Shader>, UploadBufferDynamic*>::iterator  itr = m_pConstantBufferMap.find(pShader);
	if (itr == m_pConstantBufferMap.end())
	{
		return;
	}
	itr->second->CopyData(eleIndex, data,elecount);
}
