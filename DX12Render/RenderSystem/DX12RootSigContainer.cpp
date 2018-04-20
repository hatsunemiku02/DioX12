#include "DX12RootSigContainer.h"

DX12RootSigContainer::DX12RootSigContainer()
{

}

DX12RootSigContainer::~DX12RootSigContainer()
{

}

void DX12RootSigContainer::SetUp(DX12Device* pDevice)
{
	m_pDevice = pDevice;
}

int DX12RootSigContainer::GetRootKey(unsigned char constantbuffer, unsigned char texture, unsigned char sampler, unsigned char uav)
{
	int ret = 0;
	ret += constantbuffer;
	ret += texture + 2 ^ 8;
	ret += sampler + 2 ^ 16;
	ret += uav + 2 ^ 24;
	return ret;
}

DX12RootSig* DX12RootSigContainer::GetRootSig(unsigned char constantbuffer, unsigned char texture, unsigned char sampler, unsigned char uav)
{
	int key = GetRootKey(constantbuffer, texture, sampler, uav);
	if (m_RootSigMap.find(key)== m_RootSigMap.end())
	{
		return m_RootSigMap[key];
	}
	else
	{
		DX12RootSig* pRootSig = new DX12RootSig();
		pRootSig->SetupRootSig(*m_pDevice, constantbuffer, texture, sampler, uav);
		m_RootSigMap.insert(std::pair<int, DX12RootSig*>(key, pRootSig));
		return pRootSig;
	}
}
