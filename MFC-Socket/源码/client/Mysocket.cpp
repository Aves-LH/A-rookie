// Mysocket.cpp : 实现文件
//

#include "stdafx.h"
#include "client.h"
#include "Mysocket.h"
#include "clientDlg.h"

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

void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	((CClientDlg*)m_pWnd)->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

