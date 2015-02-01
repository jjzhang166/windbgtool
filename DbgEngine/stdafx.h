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

#define __DBGENGINE__

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <dbgeng.h>
#include <Shobjidl.h>

#include <algorithm>
#include <set>
#include <memory>
#include <boost\lexical_cast.hpp>

#include <initguid.h>
#include "Plugins.h"
#include "..\ObjMdl\Plugins.h"
#include "..\ObjMdl\Macros.h"
#include "..\ObjMdl\Functions.h"

//Link
#pragma comment(lib, "dbgeng")
