// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW
#define __PLDBGCMDS__

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>

#include <list>
#include <map>

#include <DbgEng.h>

#include <initguid.h>

#include "..\ObjMdl\Macros.h"
#include "..\ObjMdl\Plugins.h"
#include "..\ObjMdl\Functions.h"
#include "..\DbgEngine\Plugins.h"

#include "Plugins.h"
#include "PluginTable.h"

