#pragma once

class COutputCallbackSubscriber
{
private:
	CComPtr<IDebugSession> m_pDebugSession;
	CComPtr<IDebugOutputCallback> m_pOutputCallback;
public:
	COutputCallbackSubscriber(IDebugSession* pDebugSession, IDebugOutputCallback* pOutputCallback)
	{
		m_pDebugSession = pDebugSession;
		m_pOutputCallback = pOutputCallback;

		m_pDebugSession->AddOutputCallback(m_pOutputCallback);
	}

	virtual ~COutputCallbackSubscriber()
	{
		m_pDebugSession->RemoveOutputCallback(m_pOutputCallback);
	}
};

