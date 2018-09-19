#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <bitset>
#include <vector>
#include "d3dx12.h"
#include "DX12DESCHandle.h"
#define DescHeapMaxSize 4096

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class DX12Shader;
class DX12VBO;
class DX12CMDList;
class DX12Device
{
public:
	DX12Device();
	virtual ~DX12Device();

	virtual void Init(UINT width, UINT height, HWND hwnd);

	void PopulateCommandList();
	void BeforeRender();
	void AfterRender();
	void MoveToNextFrame();

	
	void WaitForGpu();

	void ResetCurrentCMDAlloctor();

	void ExecuteCMDList(DX12CMDList& cmdlist);
	void SigalCMDList(DX12CMDList& cmdList);
	void WaitForCMDListDone(const DX12CMDList& cmdList);

	void SetCurrentShader(DX12Shader* pShader);

	void SetCurrentVBO(DX12VBO* pVbo);

	void DrawInstance(UINT instanceCount, UINT vertecCount, UINT startVertex, UINT startInstance);

	ID3D12Device* GetDX12Device() const
	{
		return m_device.Get();
	}

	ID3D12CommandAllocator* GetCurrentCMDAlloctor() const
	{
		return m_commandAllocators[m_frameIndex].Get();
	}

	ComPtr<ID3D12CommandQueue> GetCMDQueue() const
	{
		return m_commandQueue;
	}

	UINT GetCurrentFrameIndex() const
	{
		return m_frameIndex;
	}

	UINT GetDescriptorSize() const
	{
		return m_DescriptorSize;
	}
	static const UINT FrameCount = 2;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;

	DX12DESCHandle AllocDescHandle();

	void ReleaseDescHandle(UINT index);


	ComPtr<ID3D12DescriptorHeap> m_ConstantBufferDescHeap;
	std::bitset<DescHeapMaxSize> m_ConstantBufferDescHeapUsage;

	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	UINT m_DescriptorSize;
protected:
	void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);


protected:
	ComPtr<ID3D12Device> m_device;


	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];

	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;

	
	bool m_useWarpDevice;
	UINT m_frameIndex;

	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FrameCount];
	UINT64 m_cmdFenceValue;
	DX12VBO* m_pCurrentVBO;
private:
};