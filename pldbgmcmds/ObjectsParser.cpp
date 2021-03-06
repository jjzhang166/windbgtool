// ObjectsParser.cpp : Implementation of CObjectsParser

#include "stdafx.h"
#include "ObjectsParser.h"


// CObjectsParser

STDMETHODIMP CObjectsParser::Parse(VARIANT *v, IVariantObject** ppVariantObject)
{
	CHECK_E_POINTER(v);
	if(v->vt != VT_BSTR)
		return E_INVALIDARG;

	CComPtr<IPluginManager> pPluginManager;
	RETURN_IF_FAILED(HrGetPluginManager(&pPluginManager));
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)ppVariantObject));
	CComPtr<IVariantObject> pVariantObject = *ppVariantObject;

	HrAddColumn(pPluginManager, pVariantObject, ObjectModel::Metadata::Object::Id, L"Address", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, ObjectModel::Metadata::Object::Key, L"MT", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, ObjectModel::Metadata::Object::Value, L"Size", VT_BSTR, 100);

	CComPtr<IObjCollection> pObjectCollection;
	RETURN_IF_FAILED(HrCoCreateInstance(CLSID_ObjectCollection, &pObjectCollection));
	
	CComBSTR bstrResult = v->bstrVal;
	CString strResult = bstrResult;
	int nTokenPos = 0;
	bool readyToParse = false;
	int nObjects = 0;
	
	while (nTokenPos >= 0)
	{
		CString strLine = strResult.Tokenize(L"\n", nTokenPos);

		if(strLine.Find(L" Address") != -1)
		{
			readyToParse = true;
			continue;
		}

		if(!readyToParse)
			continue;

		if(strLine.Find(L"object ") != -1)
			break;

		if(strLine.Find(L"Statistics:") != -1)
			break;

		if(strLine.Find(L"total ") != -1)
			break;

		std::wsmatch matches;
		static std::wregex regex(L"(\\S+) +(\\S+) +(\\S+)");
		auto str = std::wstring(strLine);
		if (!std::regex_search(str, matches, regex) || matches.size() != 4)
			continue;

		int index = 1;
		auto strAddress = matches[index++].str();
		auto strMt = matches[index++].str();
		auto strSize = matches[index++].str();

		if(strSize.empty())
			continue;

		CComPtr<IVariantObject> pVariantObjectValue;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pVariantObjectValue));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(ObjectModel::Metadata::Object::Id, &CComVar(strAddress.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(ObjectModel::Metadata::Object::Key, &CComVar(strMt.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(ObjectModel::Metadata::Object::Value, &CComVar(strSize.c_str())));
		RETURN_IF_FAILED(pObjectCollection->AddObject(pVariantObjectValue));
		nObjects++;
	}

	if(!nObjects)
		return E_FAIL;

	CComPtr<IVariantTable> pVariantTable;
	RETURN_IF_FAILED(HrWrapToVariantTable(pPluginManager, pVariantObject, pObjectCollection, &pVariantTable));
	CComVar vObjects(pVariantTable);
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(ObjectModel::Metadata::TableObject::ObjectsObject, &vObjects));
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_RESULT, v));
	return S_OK;
}
