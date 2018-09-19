#pragma once
#include <thread>
#include <vector>;

class RenderItem;
class DX12CMDList;
class DX12Device;
class DX12RootSig;
class DX12PSO;

class RenderPipeline
{
public:
	RenderPipeline();
	~RenderPipeline();

	void SetUp(std::shared_ptr<DX12Device>& device);

	bool IsThreaded();
	void SetRenderTarget();

	bool pushRenderItem();


	std::thread RunInThread();

	void Run();
protected:

	void RenderAllItems();

	void StartProcess();
	std::thread m_PipeLineThread;


	std::vector<std::shared_ptr<RenderItem>> m_RenderItems;
	std::shared_ptr<DX12Device> m_pDevice;
	std::shared_ptr<DX12CMDList> m_pCmdList;
};