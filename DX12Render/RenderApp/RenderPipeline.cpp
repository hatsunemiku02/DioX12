#include "RenderPipeline.h"
#include "RenderSystem/DX12Device.h"
#include "RenderSystem/DX12CMDList.h"
#include "RenderItem.h"

RenderPipeline::RenderPipeline()
{

}

RenderPipeline::~RenderPipeline()
{

}

void RenderPipeline::SetUp(std::shared_ptr<DX12Device>& device)
{
	m_pDevice = device;

	m_pCmdList = std::make_shared<DX12CMDList>();
	m_pCmdList->SetupDirectCMDList(m_pDevice);
}

std::thread RenderPipeline::RunInThread()
{
	std::function<void(void)> threadFoo = std::bind(&RenderPipeline::Run, this);
	std::thread renderThread(threadFoo);
	return std::move(renderThread);
}

void RenderPipeline::Run()
{
	StartProcess();
}

void RenderPipeline::RenderAllItems()
{
	for (int i = 0; i < m_RenderItems.size(); i++)
	{
		//vertex buffer
		m_pCmdList->SetVBO(&(*(m_RenderItems[i]->m_pGeometry->m_VBO)));
		//pso and root sig
		m_pCmdList->SetPSO(&(*(m_RenderItems[i]->m_pMaterial->m_pPSO)));
		//root sig param

		for (int n = 0; n < m_RenderItems[i]->m_pMaterial->m_ConstantBuffers.size(); n++)
		{
			m_pCmdList->GetCMDList()->SetGraphicsRootDescriptorTable(i, m_RenderItems[i]->m_pMaterial->m_ConstantBuffers[n].GetDescHandle().m_GPUHandle);
		}
		//TO DO TEXTURE SAMPLER UAV;
		//...
		m_pCmdList->DrawInstanced(3, 1, 0, 0);
	}
}

void RenderPipeline::StartProcess()
{
	int i = 0;
	i = 47;
	i--;
}

