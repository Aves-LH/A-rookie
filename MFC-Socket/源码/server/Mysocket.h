#pragma once

// CMysocket √¸¡Óƒø±Í

class CMySocket : public CAsyncSocket
{
public:
	CMySocket();
	virtual ~CMySocket();
	void SetParent(CDialog *pWnd);
	//÷ÿ–¥
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
private:
	CDialog* m_pWnd;
};

