#pragma once

#include "pldbgview_contract_i.h"

using namespace ATL;

template<class T>
class ViewDebugEngineServiceEventSink : public ATL::IConnectionPointImpl<T, &__uuidof(IViewDebugEngineServiceEvents)>
{
public:
	HRESULT Fire_OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IViewDebugEngineServiceEvents * pConnection = static_cast<IViewDebugEngineServiceEvents*>(punkConnection.p);

			if (pConnection)
			{
				hr = pConnection->OnBeforeCommandExecute(guidCommandId, vParam);
			}
		}
		return hr;
	}

	HRESULT Fire_OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IViewDebugEngineServiceEvents * pConnection = static_cast<IViewDebugEngineServiceEvents*>(punkConnection.p);

			if (pConnection)
			{
				hr = pConnection->OnAfterCommandExecute(guidCommandId, vParam, pResult);
			}
		}
		return hr;
	}
};

