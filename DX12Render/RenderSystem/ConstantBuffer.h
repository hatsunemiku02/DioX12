#pragma once
#include "RenderUtil.h"
#include "DX12Device.h"
class ConstantBuffer
{
public:
	ConstantBuffer()
	{

	}

	ConstantBuffer(DX12Device* device, UINT elementSize, UINT elementCount);

	ConstantBuffer(const ConstantBuffer& rhs) = delete;
	ConstantBuffer& operator=(const ConstantBuffer& rhs) = delete;
	~ConstantBuffer();

	void Setup(DX12Device* device, UINT elementSize, UINT elementCount);


	ID3D12Resource* Resource()const
	{
		return mUploadBufferDynamic.Get();
	}

	void* GetDataStartAddress(void* data, UINT elementIndex,UINT byteoffset);

	void CopyData( void* data, int elementIndex, UINT eleCount)
	{
		memcpy(&mMappedData[elementIndex*mElementBufferSize], data, mElementSize*eleCount);
	}

	UINT GetElementCount()
	{
		return mElementCount;
	}

	UINT GetElementSize()
	{
		return mElementSize;
	}

	UINT GetElementBufferSize()
	{
		return mElementBufferSize;
	}

	DX12DESCHandle& GetDescHandle()
	{
		return m_CBVHandle;
	}
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBufferDynamic;
	BYTE* mMappedData = nullptr;
	UINT mElementBufferSize;
	UINT mElementSize;
	UINT mElementCount;
	DX12Device* m_pDevice;
	DX12DESCHandle m_CBVHandle;
};