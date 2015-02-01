#pragma once

class CCommandContext
{
public:
	GUID m_guidCommand;
	CComVariant m_vParam;
	CComPtr<IVariantObject> m_pResultObject;
	CComPtr<IDebugCommand> m_pDebugCommand;

	CCommandContext()
	{
		m_guidCommand = GUID_NULL;
	}
};
