#pragma once
#include <vector>
#include <memory>
#include <set>
#include <map>
#include "ConstantBufferCache.h"

//bind this with one cmdlist;
class RenderItem;
class DX12CMDList;
class DX12Device;
class DX12RootSig;
class DX12PSO;

class SimplePipeline
{
public:
	SimplePipeline();
	~SimplePipeline();

	void SetCMDList(std::shared_ptr<DX12CMDList>& pCMDList);
	void SetDevice(std::shared_ptr<DX12Device>& pDevice);
	
	void ResizeRenderItems(unsigned int newsize)
	{
		m_RenderItems.resize(newsize);
	}

	void AddRenderItem(RenderItem* pRenderItem);

	void BuildConstantBuffer();
	void UpdateConstantBuffer();

	void BeforeRender();

	void Render();

	void AfterRender();
protected:

	
	void SortRenderItems(); 


protected:



	std::shared_ptr<DX12CMDList> m_pCMDList;
	std::shared_ptr<DX12Device> m_pDevice;
	std::vector<RenderItem*> m_RenderItems;


	std::vector<ConstantBufferCache> m_ObjBuffers;
	std::vector<ConstantBufferCache> m_ConstantBuffers;
	std::map<std::shared_ptr<DX12Shader>,std::set<Material*>> m_ShaderMatMap;
	//std::map<std::shared_ptr<Material>, UINT> m_MatDupicateMap;
};