#include "SimplePipeline.h"
#include "RenderSystem/DX12CMDList.h"
#include "RenderSystem/DX12Shader.h"
#include "RenderItem.h"

SimplePipeline::SimplePipeline()
{
	m_ConstantBuffers.resize(2);
	m_ObjBuffers.resize(2);
}

SimplePipeline::~SimplePipeline()
{

}

void SimplePipeline::SetCMDList(std::shared_ptr<DX12CMDList>& pCMDList)
{
	m_pCMDList = pCMDList;
}

void SimplePipeline::SetDevice(std::shared_ptr<DX12Device>& pDevice)
{
	m_pDevice = pDevice;

	m_ConstantBuffers[0].SetDevice(pDevice.get());
	m_ConstantBuffers[1].SetDevice(pDevice.get());

	m_ObjBuffers[0].SetDevice(pDevice.get());
	m_ObjBuffers[1].SetDevice(pDevice.get());
}

void SimplePipeline::AddRenderItem(RenderItem* pRenderItem)
{
	m_RenderItems.push_back(pRenderItem);
	if (m_ShaderMatMap.find(pRenderItem->m_pMaterial->m_pShader) == m_ShaderMatMap.end())
	{
		m_ShaderMatMap.insert(std::pair < std::shared_ptr<DX12Shader>, std::set<Material*> >(pRenderItem->m_pMaterial->m_pShader, std::set<Material*>()));
	}

	std::set<Material*>& matset = m_ShaderMatMap[pRenderItem->m_pMaterial->m_pShader];

	if (matset.size() == 0||matset.find(pRenderItem->m_pMaterial.get())!= matset.end())
	{
		matset.insert(pRenderItem->m_pMaterial.get());
	}
	
	
}

void SimplePipeline::BeforeRender()
{
	BuildConstantBuffer();
	m_pDevice->ResetCurrentCMDAlloctor();
	m_pCMDList->Reset(m_pDevice.get());
	//ThrowIfFailed(m_pCMDList->GetCMDList()->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));
	// Set necessary state.
	
	m_pCMDList->GetCMDList()->RSSetViewports(1, &m_pDevice->m_viewport);
	m_pCMDList->GetCMDList()->RSSetScissorRects(1, &m_pDevice->m_scissorRect);
	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.


	// Indicate that the back buffer will be used as a render target.
	m_pCMDList->GetCMDList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pDevice->m_renderTargets[m_pDevice->GetCurrentFrameIndex()].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pDevice->m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_pDevice->GetCurrentFrameIndex(), m_pDevice->m_rtvDescriptorSize);
	m_pCMDList->GetCMDList()->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);


	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_pCMDList->GetCMDList()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void SimplePipeline::Render()
{
	for (int i=0;i<m_RenderItems.size();i++)
	{
		m_pCMDList->SetVBO(&(*(m_RenderItems[i]->m_pGeometry->m_VBO)));
		m_pCMDList->SetPSO(&(*(m_RenderItems[i]->m_pMaterial->m_pPSO)));

		UploadBufferDynamic* pobjbuffer = m_ObjBuffers[m_pDevice->GetCurrentFrameIndex()].GetBuffer(nullptr);
		D3D12_GPU_VIRTUAL_ADDRESS objaddress = pobjbuffer->Resource()->GetGPUVirtualAddress() + (m_RenderItems[i]->m_ObjBufferIndex * sizeof(ObjectBuffer));
		m_pCMDList->GetCMDList()->SetGraphicsRootConstantBufferView(0, objaddress);
	//	m_pCMDList->GetCMDList()->SetGraphicsRootDescriptorTable()


	   UploadBufferDynamic* pmatbuffer =	m_ConstantBuffers[m_pDevice->GetCurrentFrameIndex()].GetBuffer(m_RenderItems[i]->m_pMaterial->m_pShader);
	   if (pmatbuffer!=NULL)
	   {
		   D3D12_GPU_VIRTUAL_ADDRESS mataddress = pmatbuffer->Resource()->GetGPUVirtualAddress() + m_RenderItems[i]->m_pMaterial->GetOffetInCache();
		   m_pCMDList->GetCMDList()->SetGraphicsRootConstantBufferView(1, mataddress);
	   }
	 

		m_pCMDList->DrawInstanced(3, 1, 0, 0);
	}
}

void SimplePipeline::AfterRender()
{
	// Indicate that the back buffer will now be used to present.
	m_pCMDList->GetCMDList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pDevice->m_renderTargets[m_pDevice->GetCurrentFrameIndex()].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	//ThrowIfFailed(m_pCMDList->GetCMDList()->Close());

	// Execute the command list.
	m_pDevice->ExecuteCMDList(*m_pCMDList);
	// Present the frame.
	ThrowIfFailed(m_pDevice->m_swapChain->Present(1, 0));

	m_pDevice->MoveToNextFrame();
}

void SimplePipeline::BuildConstantBuffer()
{
	std::map < std::shared_ptr<DX12Shader>, std::set<Material*> >::iterator itr = m_ShaderMatMap.begin();
	for (; itr != m_ShaderMatMap.end(); itr++)
	{
		std::set<Material*>::iterator matitr = itr->second.begin();
		if ((*matitr)->m_ContentSize == 0)
		{
			continue;
		}
		m_ConstantBuffers[m_pDevice->GetCurrentFrameIndex()].ResizeBuffer(itr->first, (*matitr)->m_ContentSize, itr->second.size());
		unsigned int matCount = itr->second.size();
		unsigned int offset = 0;
		for (; matitr != itr->second.end(); matitr++)
		{
			UploadBufferDynamic* pBuffer = m_ConstantBuffers[m_pDevice->GetCurrentFrameIndex()].GetBuffer(itr->first);
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = pBuffer->Resource()->GetGPUVirtualAddress();

			auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_ConstantBuffers[m_pDevice->GetCurrentFrameIndex()].m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(offset, m_pDevice->m_rtvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;
			cbvDesc.SizeInBytes = pBuffer->GetElementBufferSize();

			m_pDevice->GetDX12Device()->CreateConstantBufferView(&cbvDesc, handle);

			(*matitr)->m_OffsetInCache = offset;
			offset++;
		}

	}

	unsigned int ObjBufferSize = sizeof(ObjectBuffer);
	unsigned int ObjCount = m_RenderItems.size();

	m_ObjBuffers[m_pDevice->GetCurrentFrameIndex()].ResizeBuffer(nullptr, ObjBufferSize, ObjCount);
	for (int i=0;i<m_RenderItems.size();i++)
	{
		UploadBufferDynamic* pBuffer = m_ObjBuffers[m_pDevice->GetCurrentFrameIndex()].GetBuffer(nullptr);
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = pBuffer->Resource()->GetGPUVirtualAddress();

		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_ObjBuffers[m_pDevice->GetCurrentFrameIndex()].m_cbvHeap->GetCPUDescriptorHandleForHeapStart());
		handle.Offset(i, m_pDevice->m_rtvDescriptorSize);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = pBuffer->GetElementBufferSize();

		m_pDevice->GetDX12Device()->CreateConstantBufferView(&cbvDesc, handle);


		m_RenderItems[i]->m_ObjBufferIndex = i;
	}

}

void SimplePipeline::UpdateConstantBuffer()
{
	std::map < std::shared_ptr<DX12Shader>, std::set<Material*> >::iterator itr = m_ShaderMatMap.begin();
	for (; itr != m_ShaderMatMap.end(); itr++)
	{
		std::set<Material*>::iterator matitr = itr->second.begin();
		if ((*matitr)->m_ContentSize == 0)
		{
			continue;
			
		}
		unsigned int matCount = itr->second.size();
		for (; matitr!=itr->second.end(); matitr++)
		{
			m_ConstantBuffers[m_pDevice->GetCurrentFrameIndex()].UpdateBuffer(itr->first, (*matitr)->m_OffsetInCache, (*matitr)->m_MatContent, 1);
		}
	}

	for (int i = 0; i < m_RenderItems.size(); i++)
	{
		m_ObjBuffers[m_pDevice->GetCurrentFrameIndex()].UpdateBuffer(nullptr, m_RenderItems[i]->m_ObjBufferIndex, &(m_RenderItems[i]->m_ObjBuffer),1 );
	}
}

void SimplePipeline::SortRenderItems()
{

}
