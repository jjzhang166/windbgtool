// ThreadsParser.cpp : Implementation of CThreadsParser

#include "stdafx.h"
#include "ThreadsParser.h"

// CThreadsParser
STDMETHODIMP CThreadsParser::Parse(VARIANT *v, IVariantObject** ppVariantObject)
{
	CHECK_E_POINTER(v);
	if(v->vt != VT_BSTR)
		return E_INVALIDARG;

	CComPtr<IPluginManager> pPluginManager;
	RETURN_IF_FAILED(HrGetPluginManager(&pPluginManager));
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)ppVariantObject));
	CComPtr<IVariantObject> pVariantObject = *ppVariantObject;

	HrAddColumn(pPluginManager, pVariantObject, ObjectModel::Metadata::Object::Id, L"ID", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_OSID, L"OSID", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_THREADOBJ, L"ThreadOBJ", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_STATE, L"State", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_GC, L"GC Mode (preemptive)");
	HrAddColumn(pPluginManager, pVariantObject, VAR_ALLOC, L"GC Alloc Context", VT_BSTR, 120);
	HrAddColumn(pPluginManager, pVariantObject, VAR_DOMAIN, L"Domain", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_LOCKCOUNT, L"Lock Count");
	HrAddColumn(pPluginManager, pVariantObject, VAR_APT, L"APT");
	HrAddColumn(pPluginManager, pVariantObject, VAR_EXCEPTION, L"Exception", VT_BSTR, 200);

	CComPtr<IObjCollection> pObjectCollection;
	RETURN_IF_FAILED(HrCoCreateInstance(CLSID_ObjectCollection, &pObjectCollection));

	CComBSTR bstrResult = v->bstrVal;
	CString strResult = bstrResult;
	bool readyToParse = false;
	int nObjects = 0;

	std::wstring stdStr = strResult.GetString();
	std::vector<std::wstring> lines;
	split(lines, stdStr, boost::is_any_of(L"\n"));

	for(auto tok_iter = lines.cbegin();tok_iter != lines.cend(); ++tok_iter)
	{
		CString strLine = tok_iter->c_str();

		if(strLine.Find(L"       ID") != -1)
		{
			readyToParse = true;
			continue;
		}

		if(!readyToParse)
			continue;

		if(strLine.IsEmpty())
			continue;

		typedef std::pair<int, CString> pair;
		std::vector<pair> vItems;
		std::wsmatch matches;
		static std::wregex regex(L"(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+)(.+)");
		auto str = std::wstring(strLine);
		if (!std::regex_search(str, matches, regex) || matches.size() != 12)
			continue;

		int index = 1;
		auto strKey = matches[index++].str();
		auto strId = matches[index++].str();
		auto strOsId = matches[index++].str();
		auto strThreadObj = matches[index++].str();
		auto strState = matches[index++].str();
		auto strGc = matches[index++].str();
		auto strAlloc = matches[index++].str();
		auto strDomain = matches[index++].str();
		auto strLockCount = matches[index++].str();
		auto strApt = matches[index++].str();
		auto strException = matches[index++].str();

		CComPtr<IVariantObject> pVariantObjectValue;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pVariantObjectValue));

		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_KEY, &CComVariant(strKey.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(ObjectModel::Metadata::Object::Id, &CComVariant(strId.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_OSID, &CComVariant(strOsId.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_THREADOBJ, &CComVariant(strThreadObj.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_STATE, &CComVariant(strState.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_GC, &CComVariant(strGc.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_ALLOC, &CComVariant(strAlloc.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_DOMAIN, &CComVariant(strDomain.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_LOCKCOUNT, &CComVariant(strLockCount.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_APT, &CComVariant(strApt.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_EXCEPTION, &CComVariant(strException.c_str())));

		RETURN_IF_FAILED(pObjectCollection->AddObject(pVariantObjectValue));
		nObjects++;
	}

	CComPtr<IVariantTable> pVariantTable;
	RETURN_IF_FAILED(HrWrapToVariantTable(pPluginManager, pVariantObject, pObjectCollection, &pVariantTable));
	CComVariant vObjects(pVariantTable);
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(ObjectModel::Metadata::TableObject::ObjectsObject, &vObjects));
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_RESULT, v));
	return S_OK;
}

