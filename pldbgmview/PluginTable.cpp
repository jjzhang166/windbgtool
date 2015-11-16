#include "stdafx.h"
#include "PluginTable.h"

HRESULT CPluginTable::FinalConstruct()
{
	RETURN_IF_FAILED(AddPluginInfo(PNAMESP_HOSTFORM, PVIEWTYPE_COMMAND, COMMAND_COMMANDS, CLSID_CommandsCommand, _T("Commands menu item")));
	RETURN_IF_FAILED(AddPluginInfo(PNAMESP_HOSTFORM, PVIEWTYPE_COMMAND, COMMAND_DOTNETINFO, CLSID_DotNetInfoCommand, _T("Dot net info view command")));
	RETURN_IF_FAILED(AddPluginInfo(PNAMESP_HOSTFORM, PVIEWTYPE_COMMAND, COMMAND_HEAPSTAT, CLSID_HeapStatCommand, _T("Heap stat view command")));
	RETURN_IF_FAILED(AddPluginInfo(PNAMESP_HOSTFORM, PVIEWTYPE_COMMAND, COMMAND_THREADS, CLSID_ThreadsCommand, _T("Threads info view command")));
    RETURN_IF_FAILED(AddPluginInfo(PNAMESP_HOSTFORM, PVIEWTYPE_COMMAND, COMMAND_BUILD_INDEX, CLSID_BuildIndexCommand, _T("Build sosex index view command")));

	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_DOTNETINFO, CLSID_SimpleListView, _T("Dot net info command view form")));
	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_HEAPSTAT, CLSID_SimpleListView, _T("Heap stat command view form")));
	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_OBJECTS, CLSID_SimpleListView, _T("Objects command view form")));
	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_OBJECT_PROPS, CLSID_SimpleListView, _T("Object properties command view form")));
	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_OBJECT_VALUES, CLSID_SimpleListView, _T("Object values command view form")));
	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_THREADSINFO, CLSID_SimpleListView, _T("Threads info command view form")));

	RETURN_IF_FAILED(AddPluginInfo(PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_HEAPSTAT, COMMAND_HEAPSTATVIEWCOMMANDS, CLSID_HeapStatViewCommands, _T("Heap stat view form context commands")));
	RETURN_IF_FAILED(AddPluginInfo(PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_OBJECTS, COMMAND_OBJECTS, CLSID_ObjectsViewCommands, _T("Objects view form context commands")));
	RETURN_IF_FAILED(AddPluginInfo(PVIEWTYPE_INPLACE_CONTROL, DBGCOMMAND_OBJECT_VALUES, COMMAND_OBJECTS, CLSID_ObjectsViewCommands, _T("Object values view form context commands")));

	RETURN_IF_FAILED(AddPluginInfo(PNAMESP_HOSTFORM, PVIEWTYPE_WINDOW_SERVICE, SERVICE_COMMANDS_STATE, CLSID_CommandStateService, _T("Commands state service")));
	RETURN_IF_FAILED(AddPluginInfoRemote(PNAMESP_HOSTFORM, PVIEWTYPE_WINDOW_SERVICE, SERVICE_MANAGED_DEBUG_SESSION_SERVICE, CLSID_ManagedDebugSessionService, _T("Managed debug session service")));

	return S_OK;
}
