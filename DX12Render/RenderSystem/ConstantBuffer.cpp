#include "ConstantBuffer.h"
#include "d3dx12.h"


void ConstantBuffer::Setup(DX12Device* device, UINT elementSize, UINT elementCount)
{
	m_pDevice = device;
	mElementCount = elementCount;
	mElementSize = elementSize;

	mElementBufferSize = CalcConstantBufferByteSize(elementSize);

	ThrowIfFailed(m_pDevice->GetDX12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mElementBufferSize*elementCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mUploadBufferDynamic)));

	ThrowIfFailed(mUploadBufferDynamic->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

	m_CBVHandle = m_pDevice->AllocDescHandle();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = mUploadBufferDynamic.Get()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = mElementBufferSize;
	m_pDevice->GetDX12Device()->CreateConstantBufferView(&cbvDesc, m_CBVHandle.m_CPUHandle);

}

void* ConstantBuffer::GetDataStartAddress(void* data, UINT elementIndex, UINT byteoffset)
{
	return (void*)&mMappedData[elementIndex*mElementBufferSize + byteoffset] ;
}

ConstantBuffer::ConstantBuffer(DX12Device* device, UINT elementSize, UINT elementCount) 
{
	m_pDevice = device;
	mElementCount = elementCount;
	mElementSize = elementSize;

	mElementBufferSize = CalcConstantBufferByteSize(elementSize);

	ThrowIfFailed(m_pDevice->GetDX12Device()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mElementBufferSize*elementCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mUploadBufferDynamic)));

	ThrowIfFailed(mUploadBufferDynamic->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

	m_CBVHandle = m_pDevice->AllocDescHandle();
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = mUploadBufferDynamic.Get()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = mElementBufferSize;
	m_pDevice->GetDX12Device()->CreateConstantBufferView(&cbvDesc, m_CBVHandle.m_CPUHandle);
}

ConstantBuffer::~ConstantBuffer()
{
	if (mUploadBufferDynamic != nullptr)
		mUploadBufferDynamic->Unmap(0, nullptr);

	mMappedData = nullptr;
}

