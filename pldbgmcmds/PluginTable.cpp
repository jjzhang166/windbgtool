#include "stdafx.h"
#include "PluginTable.h"

HRESULT HrAddColumn(IPluginManager* pPluginManager, IVariantObject* pVariantObject, CString strKey, CString strName, VARTYPE vt, int iWidth)
{
	if(!pVariantObject)
		return E_POINTER;

	CComVariant v;
	CComPtr<IObjectCollection> pObjectCollectionColumns;
	RETURN_IF_FAILED(pVariantObject->GetVariantValue(VAR_COLUMNS, &v));
	if(v.vt == VT_EMPTY)
	{
		RETURN_IF_FAILED(CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC_SERVER, IID_IObjectCollection, (LPVOID*)&pObjectCollectionColumns));
		v = pObjectCollectionColumns;
		RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_COLUMNS, &v));
	}
	else
	{
		RETURN_IF_FAILED(v.punkVal->QueryInterface(IID_IObjectCollection, (LPVOID*)&pObjectCollectionColumns));
	}

	CComPtr<IVariantObject> pVariantObjectColumn;
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pVariantObjectColumn));
	pObjectCollectionColumns->AddObject(pVariantObjectColumn);
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(VAR_COLUMN_NAME, &CComVariant(strName)));
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(VAR_COLUMN_KEY, &CComVariant(strKey)));
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(VAR_COLUMN_TYPE, &CComVariant(vt)));
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(VAR_COLUMN_WIDTH, &CComVariant(iWidth)));
	return S_OK;
}

HRESULT HrWrapToVariantTable(IPluginManager* pPluginManager, IVariantObject* pVariantObject, IObjectCollection* pObjectCollection, IVariantTable** ppVariantTable)
{
	if(!pObjectCollection)
		return E_POINTER;
	if(!ppVariantTable)
		return E_POINTER;

	CComVariant v;
	CComPtr<IObjectCollection> pObjectCollectionColumns;
	RETURN_IF_FAILED(pVariantObject->GetVariantValue(VAR_COLUMNS, &v));
	RETURN_IF_FAILED(v.punkVal->QueryInterface(IID_IObjectCollection, (LPVOID*)&pObjectCollectionColumns));

	CComQIPtr<IObjectArray> pObjectArray = pObjectCollection;
	if(!pObjectArray)
		return E_NOINTERFACE;

	CComPtr<IVariantTable> pVariantTable;
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantTable, IID_IVariantTable, (LPVOID*)&pVariantTable));

	RETURN_IF_FAILED(pVariantTable->SetColumns(pObjectCollectionColumns));
	RETURN_IF_FAILED(pVariantTable->SetObjects(pObjectArray));

	RETURN_IF_FAILED(pVariantTable->QueryInterface(IID_IVariantTable, (LPVOID*)ppVariantTable));
	return S_OK;
}