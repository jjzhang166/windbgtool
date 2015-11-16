#pragma once

#include "Plugins.h"
#include "..\DbgEngine\Plugins.h"
#include "..\ObjMdl\\Plugins.h"
#include "..\ObjMdl\\PluginTableBase.h"

using namespace ATL;

#include "pldbgmcmds_i.h"

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

	void FinalRelease(){}

	HRESULT FinalConstruct()
	{
		RETURN_IF_FAILED(AddObject(CLSID_ManagedDebugSessionService, _T("Managed debug engine service")));
		InitTable();
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_SYMPATH, CLSID_PluginDebugCommand, _T("Set symbol path debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_SYMPATH]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_SYMPATH]));
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_SYMPATH, CLSID_EmptyParser, _T("Set symbol path debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_SYMNOISY, CLSID_PluginDebugCommand, _T("Set sym noisy option command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_SYMNOISY]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_SYMNOISY]));
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_SYMNOISY, CLSID_EmptyParser, _T("Set sym noisy option command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_ANALYZE, CLSID_PluginDebugCommand, _T("Analyze dumpe file command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_ANALYZE]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_ANALYZE]));
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_ANALYZE, CLSID_EmptyParser, _T("Analyze dumpe file command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_DOTNETINFO, CLSID_PluginDebugCommand, _T("Dot net info debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_DOTNETINFO]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_DOTNETINFO]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_DOTNETINFO, CLSID_DotNetInfoParser, _T("Dot net info debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_HEAPSTAT, CLSID_PluginDebugCommand, _T("Heap stat debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_HEAPSTAT]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_HEAPSTAT]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_HEAPSTAT, CLSID_HeapStatParser, _T("Heap stat debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_CORDLL, CLSID_PluginDebugCommand, _T("COR dll debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_CORDLL]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_CORDLL]));
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_CORDLL, CLSID_EmptyParser, _T("COR dll debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_OBJECT_PROPS, CLSID_PluginDebugCommand, _T("Object properties debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_OBJECT_PROPS]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_OBJECT_PROPS]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_OBJECT_PROPS, CLSID_ObjectPropsParser, _T("Object properties debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_OBJECT_VALUES, CLSID_PluginDebugCommand, _T("Object values debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_OBJECT_VALUES]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_OBJECT_VALUES]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_OBJECT_VALUES, CLSID_ObjectValuesParser, _T("Object values debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_OBJECTS, CLSID_PluginDebugCommand, _T("Show objects debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_OBJECTS]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_OBJECTS]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_OBJECTS, CLSID_ObjectsParser, _T("Show objects debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_THREADSINFO, CLSID_PluginDebugCommand, _T("Threads info debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_THREADSINFO]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_THREADSINFO]));
		RETURN_IF_FAILED(AddPluginInfo(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_THREADSINFO, CLSID_ThreadsParser, _T("Threads info debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_GCROOT, CLSID_PluginDebugCommand, _T("Show object roots debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_GCROOT]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_GCROOT]));
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_GCROOT, CLSID_EmptyParser, _T("Show object roots debug command parser")));

        RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_GCMROOT, CLSID_PluginDebugCommand, _T("Show object mroots debug command")));
        RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_GCMROOT]));
        RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_GCMROOT]));
        RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_GCMROOT, CLSID_EmptyParser, _T("Show object mroots debug command parser")));

        RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_BUILD_INDEX, CLSID_PluginDebugCommand, _T("Build sosex index debug command")));
        RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_BUILD_INDEX]));
        RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_BUILD_INDEX]));
        RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_BUILD_INDEX, CLSID_EmptyParser, _T("Build sosex index debug command parser")));

		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND, DBGCOMMAND_LOADBY, CLSID_PluginDebugCommand, _T("Loadby debug command")));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_TEXT, gs_commandTable[DBGCOMMAND_LOADBY]));
		RETURN_IF_FAILED(SetText(VAR_COMMAND_NAME, gs_commandNamesTable[DBGCOMMAND_LOADBY]));
		RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_DBGENGINE, PDBGTYPE_COMMAND_PARSER, DBGCOMMAND_LOADBY, CLSID_EmptyParser, _T("Loadby debug command parser")));

		return S_OK;
	}

	HRESULT SetText(BSTR key, LPCTSTR lpw)
	{
		CHECK_E_POINTER(key);
		CComQIPtr<IVariantObject> pVariantObject =  (*m_infos.rbegin());
		if(pVariantObject)
			RETURN_IF_FAILED(pVariantObject->SetVariantValue(key, &CComVar(lpw)));

		return S_OK;
	}

	CPluginTable()
	{
	}

	void InitTable()
	{
		gs_commandTable[DBGCOMMAND_DOTNETINFO] =  L"lmv m clr";
		gs_commandNamesTable[DBGCOMMAND_DOTNETINFO] = L"CLR info";
		gs_commandTable[DBGCOMMAND_SYMNOISY] = L"!sym noisy";
		gs_commandNamesTable[DBGCOMMAND_SYMNOISY] = L"Set sym noisy option";
		gs_commandTable[DBGCOMMAND_HEAPSTAT] = L"!DumpHeap -stat";
		gs_commandNamesTable[DBGCOMMAND_HEAPSTAT] = L"CLR heap stat";
		gs_commandTable[DBGCOMMAND_SYMPATH] = L".sympath %s";
		gs_commandNamesTable[DBGCOMMAND_SYMPATH] = L"Symbol path";
		gs_commandTable[DBGCOMMAND_ANALYZE] = L"!analyze -v";
		gs_commandNamesTable[DBGCOMMAND_ANALYZE] = L"Analyze dump file";
		gs_commandTable[DBGCOMMAND_CORDLL] = L".cordll -l";
		gs_commandNamesTable[DBGCOMMAND_CORDLL] = L"Load CLR DLL";
		gs_commandTable[DBGCOMMAND_LOADBY] = L".loadby %0 %1";
		gs_commandNamesTable[DBGCOMMAND_LOADBY] = L"Load by command";
		gs_commandTable[DBGCOMMAND_OBJECT_PROPS] = L"!DumpObj %s";
		gs_commandNamesTable[DBGCOMMAND_OBJECT_PROPS] = L"Object properties";
		gs_commandTable[DBGCOMMAND_OBJECT_VALUES] = L"!DumpObj %s";
		gs_commandNamesTable[DBGCOMMAND_OBJECT_VALUES] = L"Object values";
		gs_commandTable[DBGCOMMAND_OBJECTS] = L"!DumpHeap -mt %s";
		gs_commandNamesTable[DBGCOMMAND_OBJECTS] = L"Objects";
		gs_commandTable[DBGCOMMAND_THREADSINFO] = L"!Threads";
		gs_commandNamesTable[DBGCOMMAND_THREADSINFO] = L"Threads";
		gs_commandTable[DBGCOMMAND_GCROOT] = L"!gcroot %s";
		gs_commandNamesTable[DBGCOMMAND_GCROOT] = L"GC roots";
        gs_commandTable[DBGCOMMAND_GCMROOT] = L"!mroot %s";
        gs_commandNamesTable[DBGCOMMAND_GCMROOT] = L"GC roots using sosex";
        gs_commandTable[DBGCOMMAND_BUILD_INDEX] = L"!bhi";
        gs_commandNamesTable[DBGCOMMAND_BUILD_INDEX] = L"Build sosex index";
    }
};

OBJECT_ENTRY_AUTO(__uuidof(PluginTable), CPluginTable)
