// EmptyParser.cpp : Implementation of CEmptyParser

#include "stdafx.h"
#include "EmptyParser.h"


// CEmptyParser

HRESULT CEmptyParser::FinalConstruct()
{
	RETURN_IF_FAILED(HrGetPluginManager(&m_pPluginManager));
	return S_OK;
}

STDMETHODIMP CEmptyParser::Parse(VARIANT *v, IVariantObject** ppVariantObject)
{
	CHECK_E_POINTER(v);
	CHECK_E_POINTER(ppVariantObject);

	if(v->vt != VT_BSTR)
		return E_INVALIDARG;

	RETURN_IF_FAILED(m_pPluginManager->CoCreateInstance(CLSID_VariantObject, IID_IVariantObject, (LPVOID*)ppVariantObject));
	CComPtr<IVariantObject> pVariantObject = *ppVariantObject;
	RETURN_IF_FAILED(pVariantObject->SetVariantValue(VAR_RESULT, v));
	return S_OK;
}