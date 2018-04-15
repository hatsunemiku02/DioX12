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

	void SetupRootSig(const DX12Device& device);
	ID3D12RootSignature* GetRootSig() const
	{
		return m_rootSignature.Get();
	}
private:
	ComPtr<ID3D12RootSignature> m_rootSignature;
};