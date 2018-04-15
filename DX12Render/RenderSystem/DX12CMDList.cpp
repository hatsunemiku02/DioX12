#include "DX12CMDList.h"
#include  "RenderUtil.h"
#include "DX12PSO.h"
#include "DX12VBO.h"
#include "DX12RootSig.h"

DX12CMDList::DX12CMDList()
{

}

DX12CMDList::~DX12CMDList()
{

}

void DX12CMDList::SetupDirectCMDList(const DX12Device& device)
{
	ThrowIfFailed(device.GetDX12Device()->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		device.GetCurrentCMDAlloctor(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(m_commandList.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	m_commandList->Close();
}

void DX12CMDList::Reset(const DX12Device* pDevice)
{
	m_pCurrentRootSig = NULL;
	m_pCurrentPSO = NULL;
	m_commandList->Reset(pDevice->GetCurrentCMDAlloctor(),nullptr);
}

// void DX12CMDList::BeforeRender(const DX12Device* pDevice)
// {
// 	// Command list allocators can only be reset when the associated 
// 	// command lists have finished execution on the GPU; apps should use 
// 	// fences to determine GPU execution progress.
// 	//	m_commandList->Close();
// 
// 	ThrowIfFailed(m_commandAllocators[m_frameIndex]->Reset());
// 	ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));
// 
// 	// Set necessary state.
// 	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
// 	m_commandList->RSSetViewports(1, &m_viewport);
// 	m_commandList->RSSetScissorRects(1, &m_scissorRect);
// 	// However, when ExecuteCommandList() is called on a particular command 
// 	// list, that command list can then be reset at any time and must be before 
// 	// re-recording.
// 
// 
// 	// Indicate that the back buffer will be used as a render target.
// 	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
// 
// 	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
// 	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
// 
// 
// 	// Record commands.
// 	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
// 	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
// 
// 
// }

// void DX12CMDList::AfterRender(const DX12Device* pDevice)
// {
// 	// Indicate that the back buffer will now be used to present.
// 	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
// 
// 	ThrowIfFailed(m_commandList->Close());
// 
// 	// Execute the command list.
// 	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
// 	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
// 
// 	// Present the frame.
// 	ThrowIfFailed(m_swapChain->Present(1, 0));
// 
// 	MoveToNextFrame();
// }

void DX12CMDList::DrawInstanced(UINT vertecCount, UINT instanceCount, UINT startVertex, UINT startInstance)
{
	m_commandList->DrawInstanced(vertecCount, instanceCount, startVertex, startInstance);
}

void DX12CMDList::SetDoneNum(UINT64 done)
{
	m_ListDone = done;
}

void DX12CMDList::SetVBO(const DX12VBO* pVbo)
{
	m_commandList->IASetPrimitiveTopology(pVbo->GetPrimitiveTopology());
	m_commandList->IASetVertexBuffers(0, 1, pVbo->GetVBV());
}

void DX12CMDList::SetPSO(const DX12PSO* pPSO)
{
	if (m_pCurrentRootSig!= pPSO->m_pRootSig)
	{
		m_commandList->SetGraphicsRootSignature(pPSO->m_pRootSig->GetRootSig());
		m_pCurrentRootSig = pPSO->m_pRootSig;
	}

	if (m_pCurrentPSO != pPSO)
	{
		m_commandList->SetPipelineState(pPSO->GetPipelineState());
		m_pCurrentPSO = const_cast<DX12PSO*>(pPSO);
	}
}

