// HeapStatParser.cpp : Implementation of CHeapStatParser

#include "stdafx.h"
#include "HeapStatParser.h"

// CHeapStatParser

STDMETHODIMP CHeapStatParser::Parse(VARIANT *v, IVariantObject** ppVariantObject)
{
	CHECK_E_POINTER(v);
	if(v->vt != VT_BSTR)
		return E_INVALIDARG;

	CComPtr<IPluginManager> pPluginManager;
	RETURN_IF_FAILED(HrGetPluginManager(&pPluginManager));
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)ppVariantObject));
	CComPtr<IVariantObject> pVariantObject = *ppVariantObject;

	HrAddColumn(pPluginManager, pVariantObject, VAR_ID, L"MT", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_COUNT, L"Count", VT_I4, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_TOTALSIZE, L"TotalSize", VT_I8, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_CLASSNAME, L"ClassName");

	CComPtr<IObjectCollection> pObjectCollection;
	RETURN_IF_FAILED(CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC_SERVER, IID_IObjectCollection, (LPVOID*)&pObjectCollection));
	
	CComBSTR bstrResult = v->bstrVal;
	CString strResult = bstrResult;
	int nTokenPos = 0;
	bool readyToParse = false;
	int nObjects = 0;
	
	while (nTokenPos >= 0)
	{
		CString strLine = strResult.Tokenize(L"\n", nTokenPos);

		if(strLine.Find(L"      MT") != -1)
		{
			readyToParse = true;
			continue;
		}

		if(!readyToParse)
			continue;

		if(strLine.Find(L"Total ") != -1)
			break;

		static std::wregex regex(L"(\\S+) +(\\S+) +(\\S+) +(.+)");
		std::wsmatch matches;
		auto str = std::wstring(strLine);
		if (!std::regex_search(str, matches, regex) || matches.size() != 5)
			continue;

		int index = 1;
		auto strId = matches[index++].str();
		auto count = boost::lexical_cast<int>(matches[index++].str());
		auto totalSize = boost::lexical_cast<long long>(matches[index++].str());
		auto strClassName = matches[index++].str();

		if (strClassName.find(L"Free") != std::wstring::npos)
			continue;

		CComPtr<IVariantObject> pVariantObjectValue;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pVariantObjectValue));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_ID, &CComVariant(strId.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_COUNT, &CComVariant(count)));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_TOTALSIZE, &CComVariant(totalSize)));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_CLASSNAME, &CComVariant(strClassName.c_str())));
		RETURN_IF_FAILED(pObjectCollection->AddObject(pVariantObjectValue));
		nObjects++;
	}

	if(!nObjects)
		return E_FAIL;

	CComPtr<IVariantTable> pVariantTable;
	RETURN_IF_FAILED(HrWrapToVariantTable(pPluginManager, pVariantObject, pObjectCollection, &pVariantTable));
	CComVariant vObjects(pVariantTable);
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_OBJECTS, &vObjects));
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_RESULT, v));
	return S_OK;
}
