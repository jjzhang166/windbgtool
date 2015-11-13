// DebugSessionService2.cpp : Implementation of CDebugSessionService2

#include "stdafx.h"
#include "DebugSessionService.h"

// CDebugSessionService2

HRESULT CDebugSessionService::FinalConstruct()
{
	RETURN_IF_FAILED(HrGetPluginManager(&m_pPluginManager));
	RETURN_IF_FAILED(m_pPluginManager->CoCreateInstance(CLSID_DebugEngine, IID_IDebugEngine, (LPVOID*)&m_pDebugEngine));
	return S_OK;
}

STDMETHODIMP CDebugSessionService::OpenDumpFile(BSTR bstrDumpFile)
{
	{ //open dump
		RETURN_IF_FAILED(m_pDebugSession->OpenDumpFile(bstrDumpFile));
	}

	{ //configure symbols
		RETURN_IF_FAILED(ExecuteCommandSimple(DBGCOMMAND_SYMNOISY, NULL, NULL));

		CString strPath = L"srv*%APPDATA%\\WinDbgTool\\Symbols*http://msdl.microsoft.com/download/symbols"; //default symbols configuration

		DWORD dwSize = GetEnvironmentVariable(_T("_NT_SYMBOL_PATH"), NULL, 0);
		if (dwSize)
		{
			CString strValue;
			auto lpszBuffer = strValue.GetBuffer(dwSize);
			GetEnvironmentVariable(_T("_NT_SYMBOL_PATH"), lpszBuffer, dwSize);
			strValue.ReleaseBuffer();
			strPath = strValue;
		}
		RETURN_IF_FAILED(ExecuteCommandSimple(DBGCOMMAND_SYMPATH, &CComVar(strPath), NULL));
	}

	CComQIPtr<IDebugControl> pDebugControl = m_pDebugSession;
	ATLENSURE(pDebugControl);


	{ //check arch
		BOOL bx64 = pDebugControl->IsPointer64Bit() == S_OK;
		BOOL bWeAreX64 = sizeof(INT_PTR) == 8;

		if (bx64 != bWeAreX64)
			return COMADMIN_E_CAT_WRONGAPPBITNESS;
	}

	ULONG ulClass = 0;
	ULONG ulQualifier = 0;
	RETURN_IF_FAILED(pDebugControl->GetDebuggeeType(&ulClass, &ulQualifier));

	{ //load extension
		if (ulClass == DEBUG_CLASS_USER_WINDOWS)
		{
			LoadDll(L"ext.dll");
			LoadDll(L"exts.dll");
		}
		else if (ulClass == DEBUG_CLASS_KERNEL)
		{
			LoadDll(L"ntsdexts.dll");
			LoadDll(L"uext.dll");
			LoadDll(L"kext.dll");
			LoadDll(L"kdexts.dll");
		}
	}

	return S_OK;
}

STDMETHODIMP CDebugSessionService::LoadDll(LPCTSTR lpszDllName)
{
	CHECK_E_POINTER(lpszDllName);
	CString strExePath;
	CString strExtPath;
	strExtPath.Format(L".\\%s", lpszDllName);
	auto lpszExePath = strExePath.GetBuffer(MAX_PATH);

	if (!GetModuleFileName(NULL, lpszExePath, MAX_PATH))
		return HRESULT_FROM_WIN32(GetLastError());

	if (!PathRemoveFileSpec(lpszExePath))
		return HRESULT_FROM_WIN32(GetLastError());

	if (!PathAppend(lpszExePath, strExtPath))
		return HRESULT_FROM_WIN32(GetLastError());

	CComPtr<IVariantObject> pv4;
	RETURN_IF_FAILED(ExecuteCommandSimple(DBGCOMMAND_LOADDLL, &CComVar(lpszExePath), &pv4));
	strExePath.ReleaseBuffer();
	return S_OK;
}

STDMETHODIMP CDebugSessionService::CloseDumpFile()
{
	if (!m_pDebugSession)
		return E_ILLEGAL_STATE_CHANGE;

	RETURN_IF_FAILED(m_pDebugSession->CloseDumpFile());
	return S_OK;
}

STDMETHODIMP CDebugSessionService::CloseSession()
{
	RETURN_IF_FAILED(m_pDebugSession->Close());
	m_pDebugSession.Release();
	return S_OK;
}

STDMETHODIMP CDebugSessionService::OpenSession()
{
	if (m_pDebugSession)
		return HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS);

	RETURN_IF_FAILED(m_pDebugEngine->OpenSession(&m_pDebugSession));
	RETURN_IF_FAILED(m_pDebugSession->Open());
	return S_OK;
}

STDMETHODIMP CDebugSessionService::GetSession(IDebugSession** ppDebugSession)
{
	CHECK_E_POINTER(ppDebugSession);
	if (!m_pDebugSession)
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	return m_pDebugSession->QueryInterface(IID_IDebugSession, (LPVOID*)ppDebugSession);
}

STDMETHODIMP CDebugSessionService::ExecuteCommandSimple(REFIID iidCommand, VARIANT* vParam, IVariantObject** ppVariantObject)
{
	Fire_OnBeforeCommandExecute(iidCommand, vParam);
	CComPtr<IUnknown> pPluginUnknown;
	RETURN_IF_FAILED(m_pPluginManager->CreatePluginInstance(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, iidCommand, &pPluginUnknown));

	CComQIPtr<IPluginDebugCommand> pPluginDebugCommand = pPluginUnknown;
	if (!pPluginDebugCommand)
		return E_NOINTERFACE;

	CComQIPtr<IInitializeWithDebugSessionService> pInitializeDebugSessionService = pPluginUnknown;
	if (pInitializeDebugSessionService)
	{
		RETURN_IF_FAILED(pInitializeDebugSessionService->SetDebugSessionService(this));
	}

	RETURN_IF_FAILED(pPluginDebugCommand->SetDebugSession(m_pDebugSession));
	RETURN_IF_FAILED(pPluginDebugCommand->Execute(vParam, ppVariantObject));

	if (ppVariantObject)
	{
		CComQIPtr<IInitializeWithPluginInfo> pInitializeWithPluginInfo;
		HRESULT hr = pPluginDebugCommand->QueryInterface(IID_IInitializeWithPluginInfo, (LPVOID*)&pInitializeWithPluginInfo);
		if (SUCCEEDED(hr) && pInitializeWithPluginInfo)
		{
			CComPtr<IPluginInfo> pPluginInfo;
			RETURN_IF_FAILED(pInitializeWithPluginInfo->GetPluginInfo(&pPluginInfo));
			CComQIPtr<IVariantObject> pSourceVariantObject = pPluginInfo;
			if (pSourceVariantObject)
			{
				CComVar v1;
				if (pSourceVariantObject->GetVariantValue(VAR_COMMAND_NAME, &v1) == S_OK)
				{
					RETURN_IF_FAILED((*ppVariantObject)->SetVariantValue(VAR_COMMAND_NAME, &v1));
					RETURN_IF_FAILED((*ppVariantObject)->SetVariantValue(VAR_NAME, &v1));
				}
				if (pSourceVariantObject->GetVariantValue(ObjectModel::Metadata::Plugins::Object::Id, &v1) == S_OK)
				{
					RETURN_IF_FAILED((*ppVariantObject)->SetVariantValue(Metadata::Plugins::Object::Id, &v1));
				}
			}
		}
	}
	Fire_OnAfterCommandExecute(iidCommand, vParam, ppVariantObject ? *ppVariantObject : NULL);
	return S_OK;
}
