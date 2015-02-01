#pragma once

#include "Plugins.h"
#include "..\DbgEngine\Plugins.h"
#include "..\model-libs\ObjMdl\\PluginTableBase.h"

using namespace ATL;

#include "pldbgcmds_i.h"

class ATL_NO_VTABLE CPluginTable :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPluginTable, &CLSID_PluginTable>,
	public CPluginTableBase
{
private:
	typedef std::map<GUID, LPCTSTR, GUIDComparer> TCommandTableMap;
	TCommandTableMap gs_commandTable;
	TCommandTableMap gs_commandNamesTable;
public:
	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CPluginTable)
		COM_INTERFACE_ENTRY(IPluginTable)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		RETURN_IF_FAILED(AddObject(CLSID_DebugSessionService, _T("Debug engine service")));
		
		InitTable();

		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_OPENDUMP, CLSID_OpenDumpDebugCommand, _T("Open dump debug command")));

		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_LOADDLL, CLSID_PluginDebugCommand, _T("Debug plugin command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_LOADDLL]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_LOADDLL]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_LOADDLL, CLSID_EmptyParser, _T("Empty parser for debug command")));

		return S_OK;
	}

	HRESULT SetText(BSTR key, LPCTSTR lpw)
	{
		CHECK_E_POINTER(key);
		CComQIPtr<IVariantObject> pVariantObject =  m_infos.rbegin()->m_T;
		if(pVariantObject)
			RETURN_IF_FAILED(pVariantObject->SetVariantValue(key, &CComVariant(lpw)));

		return S_OK;
	}

	void FinalRelease()
	{
	}

	CPluginTable()
	{
	}

	void InitTable()
	{
		gs_commandTable[DBGCOMMAND_LOADDLL] = L".load %s";
		gs_commandNamesTable[DBGCOMMAND_LOADDLL] = L"Load DLL";
	}
};

OBJECT_ENTRY_AUTO(__uuidof(PluginTable), CPluginTable)
