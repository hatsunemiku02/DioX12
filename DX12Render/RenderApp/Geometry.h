#pragma once
#include "RenderSystem/DX12VBO.h"
#include <memory>

class Geometry
{
public:
	Geometry();
	~Geometry();

	void Setup(std::shared_ptr<DX12VBO>& pVbo)
	{
		m_VBO = pVbo;
	}

	std::shared_ptr<DX12VBO> m_VBO;
private:

	
	//std::

};

