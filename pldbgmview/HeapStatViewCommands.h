// HeapStatViewCommands.h : Declaration of the CHeapStatViewCommands

#pragma once

#include "resource.h"       // main symbols
#include "pldbgmview_i.h"

using namespace ATL;

// CHeapStatViewCommands

class ATL_NO_VTABLE CHeapStatViewCommands :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHeapStatViewCommands, &CLSID_HeapStatViewCommands>,
	public ICommand,
	public IInitializeWithControlImpl,
	public IInitializeWithVariantObject
{
public:
	CHeapStatViewCommands()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CHeapStatViewCommands)
		COM_INTERFACE_ENTRY(ICommand)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
		COM_INTERFACE_ENTRY(IInitializeWithVariantObject)
	END_COM_MAP()

private:
	CComPtr<IVariantObject> m_pVariantObject;

public:

	STDMETHOD(GetCommandText)(REFGUID guidId, BSTR* bstrText)
	{
		CHECK_E_POINTER(bstrText);

		if(guidId == CMD_OBJECTS)
			*bstrText = CComBSTR(L"Show objects").Copy();

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

		GUID gCmdId = {0};
		if(guidId == CMD_OBJECTS)
			gCmdId = CMD_OBJECTS;

		if(gCmdId == GUID_NULL)
			return E_NOTIMPL;

		CComVariant vParam;
		RETURN_IF_FAILED(m_pVariantObject->GetVariantValue(ObjectModel::Metadata::Object::Id, &vParam));

		CComPtr<IDebugFormManager> pDebugFormManager;
		RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_DEBUG_FORM_MANAGER, IID_IDebugFormManager, (LPVOID*)&pDebugFormManager));
		RETURN_IF_FAILED(pDebugFormManager->OpenForm(DBGCOMMAND_OBJECTS, &vParam));
		return S_OK;
	}

	STDMETHOD(InstallMenu)(IMenu* pMenu)
	{
		CHECK_E_POINTER(pMenu);
		RETURN_IF_FAILED(pMenu->AddMenuCommand(GUID_NULL, CMD_OBJECTS, this));
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(HeapStatViewCommands), CHeapStatViewCommands)
