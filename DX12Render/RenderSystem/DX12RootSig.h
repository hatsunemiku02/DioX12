#pragma once
#include <d3d12.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;
class DX12Device;
//now with only 2 cbv.
class DX12RootSig
{
public:
	DX12RootSig();
	~DX12RootSig();

	void SetupRootSig(const DX12Device& device,UINT cb,UINT texture,UINT sampler, UINT uav);
	ID3D12RootSignature* GetRootSig() const
	{
		return m_rootSignature.Get();
	}
protected:

	ComPtr<ID3D12RootSignature> m_rootSignature;
};