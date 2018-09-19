#pragma once
#include "d3dx12.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include "DX12Device.h"
class DX12PSO;
class DX12RootSig;
class DX12CMDList
{
public:
	DX12CMDList();
	virtual ~DX12CMDList();
	void SetupDirectCMDList(const DX12Device& device);

	void SetupDirectCMDList(const std::shared_ptr<DX12Device>& device);

	//void SetShader(DX12Shader* pShader);
	void Reset(const DX12Device* pDevice);

	void SetVBO(const DX12VBO* pVbo);

	void SetPSO(const DX12PSO* pPSO);

	void DrawInstanced(UINT vertecCount, UINT instanceCount, UINT startVertex, UINT startInstance);

//	void BeforeRender(const DX12Device* pDevice);

//	void BeforeRender(const DX12Device* pDevice);

	void Sigal();
	void Wait();

	void SetDoneNum(UINT64);

	UINT64 GetDoneNum() const
	{
		return m_ListDone;
	}

	ID3D12GraphicsCommandList* GetCMDList() const
	{
		return m_commandList.Get();
	}
private:
	std::shared_ptr<DX12Device> m_pDevice;
	DX12RootSig* m_pCurrentRootSig;
	DX12PSO * m_pCurrentPSO;
	//ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	UINT64 m_ListDone;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceVal;
	HANDLE m_fenceEvent;

};
