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

#define _SCL_SECURE_NO_WARNINGS 

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <atlapp.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlsafe.h>

#include <vector>
#include <memory>
#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

#include <DbgEng.h>

#include <initguid.h>
#include "..\ObjMdl\PluginTableBase.h"
#include "..\ObjMdl\Macros.h"
#include "..\ObjMdl\Plugins.h"
#include "..\ObjMdl\Functions.h"

#include "..\DbgEngine\Plugins.h"

#include "..\ViewMdl\IInitializeWithControlImpl.h"
#include "..\ViewMdl\Plugins.h"
#include "..\pldbgcmds\Plugins.h"
#include "Plugins.h"

