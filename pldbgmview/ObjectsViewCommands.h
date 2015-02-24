// ObjectsViewCommands.h : Declaration of the CObjectsViewCommands

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmview_i.h"

using namespace ATL;

// CObjectsViewCommands

class ATL_NO_VTABLE CObjectsViewCommands :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CObjectsViewCommands, &CLSID_ObjectsViewCommands>,
	public ICommand2,
	public IInitializeWithControlImpl,
	public IInitializeWithVariantObject
{
public:
	CObjectsViewCommands()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CObjectsViewCommands)
		COM_INTERFACE_ENTRY(ICommand)
		COM_INTERFACE_ENTRY(ICommand2)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
		COM_INTERFACE_ENTRY(IInitializeWithVariantObject)
	END_COM_MAP()

private:
	CComPtr<IVariantObject> m_pVariantObject;

public:

	STDMETHOD(GetCommandText)(REFGUID guidId, BSTR* bstrText)
	{
		CHECK_E_POINTER(bstrText);

		if(guidId == CMD_OBJ_PROPS)
			*bstrText = CComBSTR(L"Properties").Copy();
		else if(guidId == CMD_OBJ_VALUES)
			*bstrText = CComBSTR(L"Values").Copy();
		else if (guidId == CMD_GCROOT)
			*bstrText = CComBSTR(L"Show roots").Copy();
		return S_OK;
	}

	STDMETHOD(SetVariantObject)(IVariantObject* pVariantObject)
	{
		m_pVariantObject = pVariantObject;
		return S_OK;
	}

	STDMETHOD(Invoke)(REFGUID guidId)
	{
		CComQIPtr<IServiceProvider> pServiceProvider =  m_pControl;
		if(!pServiceProvider)
			return E_NOINTERFACE;

		CComVariant vParam;
		RETURN_IF_FAILED(m_pVariantObject->GetVariantValue(ObjectModel::Metadata::Object::Id, &vParam));

		if (guidId == CMD_GCROOT)
		{
			CWaitCursor w;

			CComPtr<IConsoleService> pConsoleService;
			RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_CONSOLE, &pConsoleService));
			RETURN_IF_FAILED(pConsoleService->OpenConsole());

			CComPtr<IViewDebugEngineService> pViewDebugEngineService;
			RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, IID_IViewDebugEngineService, (LPVOID*)&pViewDebugEngineService));
			RETURN_IF_FAILED(pViewDebugEngineService->ExecuteCommandSimpleAsync(DBGCOMMAND_GCROOT, &vParam));

			return S_OK;
		}

		GUID gCmdId = {0};
		if(guidId == CMD_OBJ_PROPS)
			gCmdId = DBGCOMMAND_OBJECT_PROPS;
		else if(guidId == CMD_OBJ_VALUES)
			gCmdId = DBGCOMMAND_OBJECT_VALUES;

		if(gCmdId == GUID_NULL)
			return E_NOTIMPL;

		CComPtr<IDebugFormManager> pDebugFormManager;
		RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_DEBUG_FORM_MANAGER, IID_IDebugFormManager, (LPVOID*)&pDebugFormManager));
		RETURN_IF_FAILED(pDebugFormManager->OpenForm(gCmdId, &vParam));
		return S_OK;
	}

	STDMETHOD(InstallMenu)(IMenu* pMenu)
	{
		CHECK_E_POINTER(pMenu);
		RETURN_IF_FAILED(pMenu->AddMenuCommand(GUID_NULL, CMD_OBJ_PROPS, this));
		RETURN_IF_FAILED(pMenu->AddMenuCommand(GUID_NULL, CMD_OBJ_VALUES, this));
		RETURN_IF_FAILED(pMenu->AddMenuCommand(GUID_NULL, CMD_GCROOT, this));
		return S_OK;
	}

	STDMETHOD(GetEnabled)(REFGUID guidCommand, BOOL* pbEnabled)
	{
		UNREFERENCED_PARAMETER(guidCommand);
		CHECK_E_POINTER(pbEnabled);

		if (m_pVariantObject)
		{
			CComVariant vParam;
			RETURN_IF_FAILED(m_pVariantObject->GetVariantValue(VAR_VT, &vParam));
			if (vParam.vt == VT_BSTR)
			{
				*pbEnabled = CComBSTR(vParam.bstrVal) == L"0";
			}
			else
			{
				*pbEnabled = TRUE;
			}
		}
		return S_OK;
	}

	STDMETHOD(GetHBITMAP)(REFGUID guidCommand, HBITMAP* pHBITMAP)
	{
		UNREFERENCED_PARAMETER(guidCommand);
		UNREFERENCED_PARAMETER(pHBITMAP);
		return E_NOTIMPL;
	}

	STDMETHOD(InstallToolbar)(IToolbar* pToolbar)
	{
		UNREFERENCED_PARAMETER(pToolbar);
		return E_NOTIMPL;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(ObjectsViewCommands), CObjectsViewCommands)
