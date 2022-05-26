#pragma once

// CMySocket ÃüÁîÄ¿±ê

class CMySocket : public CAsyncSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	void SetParent(CDialog *pWnd);
	CDialog* m_pWnd;
	virtual void OnReceive(int nErrorCode);
};



