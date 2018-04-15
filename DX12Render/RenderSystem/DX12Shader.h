#pragma once
#include "DX12Device.h"
#include "RenderUtil.h"

class DX12Shader
{
public:
	DX12Shader();
	~DX12Shader();

	void CreateShader(DX12Device* pDevice,void* pShader, UINT shaderSize, const char* shaderName, const char* vsName, const char* psName);

	ComPtr<ID3DBlob> GetVertexShader() const
	{
		return mVertexShader;
	}

	ComPtr<ID3DBlob> GetPixelShader() const
	{
		return mPixelShader;
	}
protected:
	ComPtr<ID3DBlob> mVertexShader;
	ComPtr<ID3DBlob> mPixelShader;


private:
};