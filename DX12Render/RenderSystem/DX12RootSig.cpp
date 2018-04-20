#include "DX12RootSig.h"
#include <d3dcommon.h>
#include <vector>

#include "RenderUtil.h"
#include "DX12Device.h"



DX12RootSig::DX12RootSig()
{

}

DX12RootSig::~DX12RootSig()
{

}

void DX12RootSig::SetupRootSig(const DX12Device& device, UINT cb, UINT texture, UINT sampler, UINT uav)
{
	std::vector<CD3DX12_ROOT_PARAMETER> rootParamArr;
	rootParamArr.resize(cb + texture + sampler+ uav);
	for (int i=0;i<cb;i++)
	{
		CD3DX12_DESCRIPTOR_RANGE cbvTable;
		cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i);
		rootParamArr[i].InitAsDescriptorTable(1, &cbvTable);
	}

	for (int i = cb; i < texture+cb; i++)
	{
		CD3DX12_DESCRIPTOR_RANGE srvTable;
		srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
		rootParamArr[i].InitAsDescriptorTable(1, &srvTable);
	}

	for (int i = texture + cb; i < texture + cb+sampler; i++)
	{
		CD3DX12_DESCRIPTOR_RANGE samplerTable;
		samplerTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, i);
		rootParamArr[i].InitAsDescriptorTable(1, &samplerTable);
	}

	for (int i = texture + cb+ sampler; i < texture + cb + sampler+ uav; i++)
	{
		CD3DX12_DESCRIPTOR_RANGE uavTable;
		uavTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, i);
		rootParamArr[i].InitAsDescriptorTable(1, &uavTable);
	}

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(rootParamArr.size(), &rootParamArr[0],
		0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(device.GetDX12Device()->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(m_rootSignature.GetAddressOf())));
}

