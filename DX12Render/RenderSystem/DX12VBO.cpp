#include "DX12VBO.h"
#include <WinBase.h>

DX12VBO::DX12VBO()
{
	m_inputElementDescs = new D3D12_INPUT_ELEMENT_DESC[2];
	m_inputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_inputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_inputElementCount = 2;
}

DX12VBO::~DX12VBO()
{

}

void DX12VBO::CreateFromData(DX12Device* pDevice, const DX12CMDList& pCMDList, void* pData, UINT strideSize, UINT vertexBufferSize)
{
	ThrowIfFailed(D3DCreateBlob(vertexBufferSize, &m_vertexBufferCPU));
	CopyMemory(m_vertexBufferCPU->GetBufferPointer(), pData, vertexBufferSize);

	m_vertexBufferGPU = CreateDefaultBuffer(pDevice->GetDX12Device(),
		pCMDList.GetCMDList(), pData, vertexBufferSize, m_vertexBufferUploader);

	// Initialize the vertex buffer view.
	m_vertexBufferView.BufferLocation = m_vertexBufferGPU->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = strideSize;
	m_vertexBufferView.SizeInBytes = vertexBufferSize;

}
