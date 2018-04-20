#include "DX12RootSig.h"
#include <unordered_map>

class DX12RootSigContainer
{
public:
	DX12RootSigContainer();
	~DX12RootSigContainer();
	void SetUp(DX12Device* pDevice);
	static int GetRootKey(unsigned char constantbuffer, unsigned char texture, unsigned char sampler, unsigned char uav);
	DX12RootSig* GetRootSig(unsigned char constantbuffer, unsigned char texture, unsigned char sampler, unsigned char uav);
protected:
	std::unordered_map<int, DX12RootSig*> m_RootSigMap;
	DX12Device* m_pDevice;
private:


};