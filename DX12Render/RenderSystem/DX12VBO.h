#pragma once



#include "DX12Device.h"
#include "RenderUtil.h"
#include "DX12CMDList.h"

class DX12VBO
{
public:
	DX12VBO();
	virtual ~DX12VBO();

	void CreateFromData(DX12Device* pDevice,const DX12CMDList& pCMDList,void* pData,UINT strideSize,UINT vertexBufferSize);

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const
	{
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	D3D12_INPUT_ELEMENT_DESC* GetVertexLayout() const
	{
		return m_inputElementDescs;
	}

	UINT GetInputElementCount() const
	{
		return m_inputElementCount;
	}

	const D3D12_VERTEX_BUFFER_VIEW* GetVBV() const
	{
		return &m_vertexBufferView;
	}

protected:

	Microsoft::WRL::ComPtr<ID3DBlob> m_vertexBufferCPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferUploader = nullptr;


	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INPUT_ELEMENT_DESC* m_inputElementDescs;
	UINT m_inputElementCount;

private:
};