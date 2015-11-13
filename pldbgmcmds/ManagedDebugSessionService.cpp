#include "stdafx.h"
#include "ManagedDebugSessionService.h"
#include "..\ObjMdl\StringUtils.h"

STDMETHODIMP CManagedDebugSessionService::OnInitialized(IServiceProvider* pServiceProvider)
{
	CHECK_E_POINTER(pServiceProvider);
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_DEBUG_SESSION_SERVICE, &m_pDebugSessionService));

	CComPtr<IUnknown> pThis;
	RETURN_IF_FAILED(QueryInterface(IID_IUnknown, (LPVOID*)&pThis));
	RETURN_IF_FAILED(m_pDebugSessionService.Advise(pThis, IID_IDebugSessionServiceEvents, &m_dwAdvice));
	return S_OK;
}

STDMETHODIMP CManagedDebugSessionService::OnShutdown()
{
	if (m_dwAdvice)
	{
		RETURN_IF_FAILED(AtlUnadvise(m_pDebugSessionService, IID_IDebugSessionServiceEvents, m_dwAdvice));
		m_dwAdvice = 0;
	}
	return S_OK;
}

STDMETHODIMP CManagedDebugSessionService::OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	return S_OK;
}

STDMETHODIMP CManagedDebugSessionService::OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	UNREFERENCED_PARAMETER(pResult);

	if (guidCommandId == DBGCOMMAND_OPENDUMP)
	{
		auto bLoaded = false;

		{ //try to load dlls shipped with dump
			if (vParam && vParam->vt == VT_BSTR)
			{
				CString strDumpPath = vParam->bstrVal;
				StrPathRemoveFileName(strDumpPath);
				CString strSosDllPath = strDumpPath;
				StrPathAppend(strSosDllPath, L"sos.dll");
				CString strMsCorDllPath = strDumpPath;
				StrPathAppend(strMsCorDllPath, L"mscordacwks.dll");

				if (PathFileExists(strMsCorDllPath) && PathFileExists(strSosDllPath))
				{
					CComPtr<IVariantObject> pv1;
					RETURN_IF_FAILED(m_pDebugSessionService->ExecuteCommandSimple(DBGCOMMAND_LOADDLL, &CComVar(strMsCorDllPath), &pv1));
					CComPtr<IVariantObject> pv2;
					RETURN_IF_FAILED(m_pDebugSessionService->ExecuteCommandSimple(DBGCOMMAND_LOADDLL, &CComVar(strSosDllPath), &pv2));
					bLoaded = true;
				}
			}
		}

		if (!bLoaded)
		{
			{ //analyze
				RETURN_IF_FAILED(m_pDebugSessionService->ExecuteCommandSimple(DBGCOMMAND_ANALYZE, NULL, NULL));
			}

			{ //load sos
				CComPtr<IPluginManager> pPluginManager;
				RETURN_IF_FAILED(HrGetPluginManager(&pPluginManager));
				CComPtr<IBstrCollection> pBstrCollection;
				RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_BstrCollection, IID_IBstrCollection, (LPVOID*)&pBstrCollection));
				RETURN_IF_FAILED(pBstrCollection->AddItem(L"sos"));
				RETURN_IF_FAILED(pBstrCollection->AddItem(L"clr"));
				RETURN_IF_FAILED(m_pDebugSessionService->ExecuteCommandSimple(DBGCOMMAND_LOADBY, &CComVar(pBstrCollection), NULL));
			}
		}

		{ //load sosex
			RETURN_IF_FAILED(m_pDebugSessionService->LoadDll(L"sosex.dll"));
		}
	}
	return S_OK;
}
