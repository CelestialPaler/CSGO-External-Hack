#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	if (adapters.size() > 0)
		return adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to create DXGIFactory for enumerating adapters.";
			msg.szCode = std::to_string(hr);
			logger->Write(msg);
		}
		exit(-1);
	}

	IDXGIAdapter * pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		adapters.push_back(AdapterData(pAdapter));
		index += 1;
	}
	return adapters;
}

AdapterData::AdapterData(IDXGIAdapter * pAdapter)
{
	// 取得Logger
	Util::Logger* logger = Util::Logger::GetInstance();

	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->description);
	if (FAILED(hr))
	{
		// 日志
		{
			Util::Logger::LogDisc msg;
			msg.emPriority = Util::Logger::Priority::ERR;
			msg.szFrom = __func__;
			msg.szMsg = "Failed to Get Description for IDXGIAdapter.";
			msg.szCode = std::to_string(hr);
			logger->Write(msg);
		}
	}
}
