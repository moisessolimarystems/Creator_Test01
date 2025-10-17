#pragma once

//#include <string>
#include <vector>
#include <comutil.h>

namespace NetworkUtils
{
	class SolimarNetworkHelper
	{
	public:
		static HRESULT GetIPAddresses(_bstr_t computerName, std::vector<_bstr_t> &ipList);
	};
	
};

