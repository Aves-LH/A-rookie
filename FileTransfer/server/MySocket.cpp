// Mysocket.cpp : 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "Mysocket.h"
#include "serverDlg.h"

// CMySocket

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}


// CMySocket 成员函数
void CMySocket::SetParent(CDialog *pWnd)
{
   m_pWnd = pWnd;
}

void CMySocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
    ((CServerDlg*)m_pWnd)->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}



void CMySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CServerDlg*)m_pWnd)->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
    ((CServerDlg*)m_pWnd)->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}
