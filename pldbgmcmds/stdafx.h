// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define _SCL_SECURE_NO_WARNINGS 

#ifndef STRICT
#define STRICT
#endif

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#define __PLDBGMCMDS__

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <Shobjidl.h>

using namespace ATL;

#include <map>
#include <list>
#include <vector>
#include <regex>

#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>

#include <initguid.h>
#include "..\model-libs\ObjMdl\Macros.h"
#include "..\model-libs\ObjMdl\Plugins.h"
#include "..\model-libs\ObjMdl\Functions.h"
#include "..\model-libs\ObjMdl\Metadata.h"
#include "..\DbgEngine\Plugins.h"
#include "..\pldbgcmds\Plugins.h"
#include "Plugins.h"

using namespace IP;

HRESULT HrAddColumn(IPluginManager* pPluginManager, IVariantObject* pVariantObject, CComBSTR bstrKey, CString strName, VARTYPE vt = VT_BSTR, int iWidth = LVSCW_AUTOSIZE);
HRESULT HrWrapToVariantTable(IPluginManager* pPluginManager, IVariantObject* pVariantObject, IObjCollection* pObjectCollection, IVariantTable** ppVariantTable);