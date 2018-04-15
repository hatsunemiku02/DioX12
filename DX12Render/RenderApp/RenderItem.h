#pragma once
#include <DirectXMath.h>
#include <memory>
#include "RenderSystem/DX12Device.h"
#include "Material.h"
#include "Geometry.h"

struct ObjectBuffer
{
	DirectX::XMFLOAT4 WorldViewProj;
};

class RenderItem
{
public:
	RenderItem();
	~RenderItem();
	static bool comp(const RenderItem &a, const RenderItem &b);

	std::shared_ptr<Geometry> m_pGeometry;

	//UploadBuffer<ObjectBuffer> m_pObjectConstant[DX12Device::FrameCount];
	std::shared_ptr<Material> m_pMaterial;

	unsigned int m_ObjBufferIndex;
	ObjectBuffer m_ObjBuffer;
private:


};

