#include "stdafx.h"
#include "PluginTable.h"

HRESULT HrAddColumn(IPluginManager* pPluginManager, IVariantObject* pVariantObject, CComBSTR bstrKey, CString strName, VARTYPE vt, int iWidth)
{
	if(!pVariantObject)
		return E_POINTER;

	CComVariant v;
	CComPtr<IObjCollection> pObjectCollectionColumns;
	RETURN_IF_FAILED(pVariantObject->GetVariantValue(ObjectModel::Metadata::TableObject::ColumnsObject, &v));
	if(v.vt == VT_EMPTY)
	{
		RETURN_IF_FAILED(HrCoCreateInstance(CLSID_ObjectCollection, &pObjectCollectionColumns));
		v = pObjectCollectionColumns;
		RETURN_IF_FAILED(pVariantObject->SetVariantValue(ObjectModel::Metadata::TableObject::ColumnsObject, &v));
	}
	else
	{
		RETURN_IF_FAILED(v.punkVal->QueryInterface(IID_IObjCollection, (LPVOID*)&pObjectCollectionColumns));
	}

	CComPtr<IVariantObject> pVariantObjectColumn;
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)&pVariantObjectColumn));
	pObjectCollectionColumns->AddObject(pVariantObjectColumn);
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(ObjectModel::Metadata::Table::Column::Name, &CComVariant(strName)));
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(ObjectModel::Metadata::Table::Column::Key, &CComVariant(bstrKey)));
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(ObjectModel::Metadata::Table::Column::Type, &CComVariant(vt)));
	RETURN_IF_FAILED(pVariantObjectColumn->SetVariantValue(ObjectModel::Metadata::Table::Column::Width, &CComVariant(iWidth)));
	return S_OK;
}

HRESULT HrWrapToVariantTable(IPluginManager* pPluginManager, IVariantObject* pVariantObject, IObjCollection* pObjectCollection, IVariantTable** ppVariantTable)
{
	if(!pObjectCollection)
		return E_POINTER;
	if(!ppVariantTable)
		return E_POINTER;

	CComVariant v;
	CComPtr<IObjCollection> pObjectCollectionColumns;
	RETURN_IF_FAILED(pVariantObject->GetVariantValue(ObjectModel::Metadata::TableObject::ColumnsObject, &v));
	RETURN_IF_FAILED(v.punkVal->QueryInterface(IID_IObjCollection, (LPVOID*)&pObjectCollectionColumns));

	CComQIPtr<IObjArray> pObjectArray = pObjectCollection;
	if(!pObjectArray)
		return E_NOINTERFACE;

	CComPtr<IVariantTable> pVariantTable;
	RETURN_IF_FAILED(pPluginManager->CoCreateInstance(CLSID_VariantTable, IID_IVariantTable, (LPVOID*)&pVariantTable));

	RETURN_IF_FAILED(pVariantTable->SetColumns(pObjectCollectionColumns));
	RETURN_IF_FAILED(pVariantTable->SetObjects(pObjectArray));

	RETURN_IF_FAILED(pVariantTable->QueryInterface(IID_IVariantTable, (LPVOID*)ppVariantTable));
	return S_OK;
}