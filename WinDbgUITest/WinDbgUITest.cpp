// WinDbgUITest.cpp : main source file for WinDbgUITest.exe
//

#include "stdafx.h"

#include "resource.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	int nRet = 0;
	{
		TCHAR lpszCurrentDir[MAX_PATH];
		if (GetModuleFileName(NULL, lpszCurrentDir, MAX_PATH))
		{
			PathRemoveFileSpec(lpszCurrentDir);
			SetCurrentDirectory(lpszCurrentDir);
		}

		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);

		CComPtr<IMessageLoop> pMessageLoop;
		RETURN_IF_FAILED(CMessageLoopWrapper::_CreatorClass::CreateInstance(NULL, IID_IMessageLoop, (LPVOID*)&pMessageLoop));
		static_cast<CMessageLoopWrapper*>(pMessageLoop.p)->SetMessageLoop(&theLoop);

		CManualComObjectLoader loader(L"ObjMdl.dll");

		CComPtr<IPluginManager> pPluginManager;
		RETURN_IF_FAILED(loader.CoCreateInstance(CLSID_PluginManager, NULL, IID_IPluginManager, (LPVOID*)&pPluginManager));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"ObjMdl.dll"));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"DbgEngine.dll"));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"ViewMdl.dll"));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"pldbgcmds.dll"));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"pldbgview.dll"));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"pldbgmcmds.dll"));
		RETURN_IF_FAILED(pPluginManager->InitializePluginLibraryByName(L"pldbgmview.dll"));
		//RETURN_IF_FAILED(pPluginManager->LoadPlugins(NULL));

		CComPtr<IVariantObject> pAboutPluginInfo;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pAboutPluginInfo));
		RETURN_IF_FAILED(pAboutPluginInfo->SetVariantValue(ObjectModel::Metadata::Plugins::Object::Namespace, &CComVar(PNAMESP_HOSTFORM)));
		RETURN_IF_FAILED(pAboutPluginInfo->SetVariantValue(ObjectModel::Metadata::Plugins::Object::Type, &CComVar(PVIEWTYPE_COMMAND)));
		RETURN_IF_FAILED(pAboutPluginInfo->SetVariantValue(ObjectModel::Metadata::Plugins::Object::Id, &CComVar(COMMAND_ABOUT)));
		RETURN_IF_FAILED(pAboutPluginInfo->SetVariantValue(ObjectModel::Metadata::Plugins::Object::Clsid, &CComVar(CLSID_AboutCommand)));
		RETURN_IF_FAILED(pPluginManager->RegisterPluginInfo2(pAboutPluginInfo));

		CComPtr<IMainWindow> pWindow;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_MainFrame, IID_IMainWindow, (LPVOID*)&pWindow));

		CComPtr<IUnknown> pUnk2;
		RETURN_IF_FAILED(pPluginManager->CreatePluginInstance(PNAMESP_HOSTFORM, PVIEWTYPE_CONTAINERWINDOW, CONTROL_TABCONTAINER, &pUnk2));
		CComQIPtr<IContainerControl> pContainerWindow = pUnk2;

		RETURN_IF_FAILED(pWindow->SetContainerControl(pContainerWindow));

		CIcon icon;
		icon.LoadIcon(IDR_MAINFRAME);
		RETURN_IF_FAILED(pWindow->SetMessageLoop(pMessageLoop));

		CComQIPtr<IControl> pControl = pWindow;

		HWND hwnd = NULL;
		RETURN_IF_FAILED(pControl->CreateEx(NULL, &hwnd));

		CString strCaption = L"WinDbgTool";
		if (sizeof(INT_PTR) == 8)
			strCaption += L" x64";
		SetWindowText(hwnd, strCaption);
		RETURN_IF_FAILED(pWindow->SetIcon(icon, TRUE));
		RETURN_IF_FAILED(pWindow->SetIcon(icon, FALSE));

		CComPtr<ISettings> pRootSettings;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_RegistrySettings, IID_ISettings, (LPVOID*)&pRootSettings));

		CComPtr<ISettings> pMainWindowSettings;
		RETURN_IF_FAILED(pRootSettings->OpenSubSettings(L"Software\\IP\\WinDbgTool\\MainWindow", &pMainWindowSettings));
		CComQIPtr<IPersistSettings> pPersistSettings = pWindow;
		if (pPersistSettings)
		{
			RETURN_IF_FAILED(pPersistSettings->Load(pMainWindowSettings));
		}

		if (hwnd == NULL)
		{
			ATLTRACE(_T("Main window creation failed!\n"));
			return 0;
		}

		CComQIPtr<IControl> pTabControl = pContainerWindow;

		BOOL bResult = FALSE;
		RETURN_IF_FAILED(pWindow->ShowWindow(nCmdShow, &bResult));

		nRet = theLoop.Run();

		if (pPersistSettings)
		{
			RETURN_IF_FAILED(pPersistSettings->Save(pMainWindowSettings));
		}

		_Module.RemoveMessageLoop();
	}

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
#pragma warning(suppress: 6387)
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
