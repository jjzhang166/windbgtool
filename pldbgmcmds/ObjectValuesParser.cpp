// ObjectValuesParser.cpp : Implementation of CObjectValuesParser

#include "stdafx.h"
#include "ObjectValuesParser.h"


// CObjectValuesParser

STDMETHODIMP CObjectValuesParser::Parse(VARIANT *v, IVariantObject** ppVariantObject)
{
	CHECK_E_POINTER(v);
	if(v->vt != VT_BSTR)
		return E_INVALIDARG;

	CComPtr<IPluginManager> pPluginManager;
	RETURN_IF_FAILED(HrGetPluginManager(&pPluginManager));
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)ppVariantObject));
	CComPtr<IVariantObject> pVariantObject = *ppVariantObject;

	HrAddColumn(pPluginManager, pVariantObject, VAR_MT, L"MT", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_FIELD, L"Field", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_OFFSET, L"Offset", VT_BSTR, 100);
	HrAddColumn(pPluginManager, pVariantObject, VAR_TYPE, L"Type");
	HrAddColumn(pPluginManager, pVariantObject, VAR_VT, L"VT");
	HrAddColumn(pPluginManager, pVariantObject, VAR_ATTR, L"Attr");
	HrAddColumn(pPluginManager, pVariantObject, ObjectModel::Metadata::Object::Id, L"Value");
	HrAddColumn(pPluginManager, pVariantObject, VAR_NAME, L"Name");

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

		if(strLine.Find(L"      MT") != -1)
		{
			readyToParse = true;
			continue;
		}

		if(!readyToParse)
			continue;

		std::wsmatch matches;
		static std::wregex regex(L"(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+) +(\\S+)");
		auto str = std::wstring(strLine);
		if (!std::regex_search(str, matches, regex) || matches.size() != 9)
			continue;

		int index = 1;
		auto strMt = matches[index++].str();
		auto strField = matches[index++].str();
		auto strOffset = matches[index++].str();
		auto strType = matches[index++].str();
		auto strVt = matches[index++].str();
		auto strAttr = matches[index++].str();
		auto strValue = matches[index++].str();
		auto strName = matches[index++].str();

		if(strName.empty())
			continue;

		CComPtr<IVariantObject> pVariantObjectValue;
		RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pVariantObjectValue));

		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_MT, &CComVar(strMt.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_FIELD, &CComVar(strField.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_OFFSET, &CComVar(strOffset.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_TYPE, &CComVar(strType.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_VT, &CComVar(strVt.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_ATTR, &CComVar(strAttr.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(ObjectModel::Metadata::Object::Id, &CComVar(strValue.c_str())));
		RETURN_IF_FAILED(pVariantObjectValue->SetVariantValue(VAR_NAME, &CComVar(strName.c_str())));

		RETURN_IF_FAILED(pObjectCollection->AddObject(pVariantObjectValue));
		nObjects++;
	}

	CComPtr<IVariantTable> pVariantTable;
	RETURN_IF_FAILED(HrWrapToVariantTable(pPluginManager, pVariantObject, pObjectCollection, &pVariantTable));
	CComVar vObjects(pVariantTable);
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(ObjectModel::Metadata::TableObject::ObjectsObject, &vObjects));
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_RESULT, v));
	return S_OK;
}
