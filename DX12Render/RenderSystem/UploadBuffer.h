#pragma once

#include "RenderUtil.h"

template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : 
        mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);

        // Constant buffer elements need to be multiples of 256 bytes.
        // This is because the hardware can only view constant data 
        // at m*256 byte offsets and of n*256 byte lengths. 
        // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; // multiple of 256
        // UINT   SizeInBytes;   // multiple of 256
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if(isConstantBuffer)
            mElementByteSize = CalcConstantBufferByteSize(sizeof(T));

        ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize*elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mUploadBuffer)));

        ThrowIfFailed(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer()
    {
        if(mUploadBuffer != nullptr)
            mUploadBuffer->Unmap(0, nullptr);

        mMappedData = nullptr;
    }

    ID3D12Resource* Resource()const
    {
        return mUploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&mMappedData[elementIndex*mElementByteSize], &data, sizeof(T));
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
    BYTE* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;
};


class UploadBufferDynamic
{
public:
	UploadBufferDynamic()
	{

	}

	void Setup(ID3D12Device* device, UINT elementSize, UINT elementCount, bool isConstantBuffer)
	{
		mElementCount = elementCount;
		mIsConstantBuffer = isConstantBuffer;
		mElementSize = elementSize;

		if (isConstantBuffer)
		{
			mElementBufferSize = CalcConstantBufferByteSize(elementSize);
		}
		else
		{
			mElementBufferSize = elementSize;
		}


		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementBufferSize*elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBufferDynamic)));

		ThrowIfFailed(mUploadBufferDynamic->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

		// We do not need to unmap until we are done with the resource.  However, we must not write to
		// the resource while it is in use by the GPU (so we must use synchronization techniques).
	}

	UploadBufferDynamic(ID3D12Device* device,UINT elementSize ,UINT elementCount, bool isConstantBuffer) :
		mIsConstantBuffer(isConstantBuffer)
	{
		mElementCount = elementCount;
		mElementSize = elementSize;

		// Constant buffer elements need to be multiples of 256 bytes.
		// This is because the hardware can only view constant data 
		// at m*256 byte offsets and of n*256 byte lengths. 
		// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
		// UINT64 OffsetInBytes; // multiple of 256
		// UINT   SizeInBytes;   // multiple of 256
		// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
		if (isConstantBuffer)
		{
			mElementBufferSize = CalcConstantBufferByteSize(elementSize);
		}
		else
		{
			mElementBufferSize = elementSize;
		}
			

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementBufferSize*elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBufferDynamic)));

		ThrowIfFailed(mUploadBufferDynamic->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));

		// We do not need to unmap until we are done with the resource.  However, we must not write to
		// the resource while it is in use by the GPU (so we must use synchronization techniques).
	}

	UploadBufferDynamic(const UploadBufferDynamic& rhs) = delete;
	UploadBufferDynamic& operator=(const UploadBufferDynamic& rhs) = delete;
	~UploadBufferDynamic()
	{
		if (mUploadBufferDynamic != nullptr)
			mUploadBufferDynamic->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource* Resource()const
	{
		return mUploadBufferDynamic.Get();
	}

	void CopyData(int elementIndex, void* data,UINT eleCount)
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
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBufferDynamic;
	BYTE* mMappedData = nullptr;
	UINT mElementBufferSize;
	UINT mElementSize ;
	UINT mElementCount;
	bool mIsConstantBuffer = false;
};