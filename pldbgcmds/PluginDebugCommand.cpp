// PluginDebugCommand.cpp : Implementation of CPluginDebugCommand

#include "stdafx.h"
#include "PluginDebugCommand.h"


// CPluginDebugCommand

HRESULT CPluginDebugCommand::FinalConstruct()
{
	RETURN_IF_FAILED(HrGetPluginManager(&m_pPluginManager));
	RETURN_IF_FAILED(m_pPluginManager->CoCreateInstance(CLSID_DebugCommand, IID_IDebugCommand, (LPVOID*)&m_pDebugCommand));
	return S_OK;
}

STDMETHODIMP CPluginDebugCommand::SetPluginInfo(IPluginInfo* pPluginInfo)
{
	CHECK_E_POINTER(pPluginInfo);
	CComQIPtr<IVariantObject> pVariantObject = pPluginInfo;
	if(!pVariantObject)
		return E_NOINTERFACE;

	CComVar v;
	HRESULT hr = pVariantObject->GetVariantValue(VAR_COMMAND_TEXT, &v);
	if(FAILED(hr))
		return hr;

	if(v.vt != VT_BSTR)
		return HRESULT_FROM_WIN32(ERROR_INVALID_VARIANT);

	m_pPluginInfo = pPluginInfo;

	return m_pDebugCommand->SetCommandText(v.bstrVal);
}

STDMETHODIMP CPluginDebugCommand::GetPluginInfo(IPluginInfo** ppPluginInfo)
{
	CHECK_E_POINTER(ppPluginInfo);
	return m_pPluginInfo->QueryInterface(IID_IPluginInfo, (LPVOID*)ppPluginInfo);
}

STDMETHODIMP CPluginDebugCommand::Execute(VARIANT* vParam, IVariantObject** ppVariantObject)
{
	CComQIPtr<IDebugCommand> pDebugCommand = m_pDebugCommand;
	ATLASSERT(pDebugCommand);
	CComVar vResult;
	RETURN_IF_FAILED(pDebugCommand->Execute(vParam, &vResult));

	GUID iidCommand = {0};
	RETURN_IF_FAILED(m_pPluginInfo->GetId(&iidCommand));

	if (ppVariantObject)
	{
		CComPtr<IUnknown> pPluginParserUnknown;
		RETURN_IF_FAILED(m_pPluginManager->CreatePluginInstance(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, iidCommand, &pPluginParserUnknown));

		CComQIPtr<ICommandResultParser> pCommandResultParser = pPluginParserUnknown;
		if (!pCommandResultParser)
			return E_NOINTERFACE;

		RETURN_IF_FAILED(pCommandResultParser->Parse(&vResult, ppVariantObject));

		CComQIPtr<IInitializeWithPluginInfo> pInitializeWithPluginInfo = m_pDebugCommand;
		if (pInitializeWithPluginInfo)
		{
			CComPtr<IPluginInfo> pPluginInfo;
			RETURN_IF_FAILED(pInitializeWithPluginInfo->GetPluginInfo(&pPluginInfo));
			CComQIPtr<IVariantObject> pSourceVariantObject = pPluginInfo;
			if (pSourceVariantObject)
			{
				CComVar v1;
				if (pSourceVariantObject->GetVariantValue(VAR_COMMAND_TEXT, &v1) == S_OK)
				{
					RETURN_IF_FAILED((*ppVariantObject)->SetVariantValue(VAR_COMMAND_TEXT, &v1));
				}
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CPluginDebugCommand::SetDebugSession(IDebugSession* pDebugSession)
{
	CHECK_E_POINTER(pDebugSession);
	CComPtr<IDebugCommand> pDebugCommand = m_pDebugCommand;
	ATLASSERT(pDebugCommand);
	RETURN_IF_FAILED(pDebugCommand->SetDebugSession(pDebugSession));
	return S_OK;
}
