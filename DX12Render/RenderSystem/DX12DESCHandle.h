#pragma once
#include <d3d12.h>

struct DX12DESCHandle
{
	UINT m_Index;
	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUHandle;
};