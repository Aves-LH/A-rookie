// Mysocket.cpp : ʵ���ļ�
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


// CMySocket ��Ա����
void CMySocket::SetParent(CDialog *pWnd)
{
   m_pWnd = pWnd;
}

void CMySocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
    ((CServerDlg*)m_pWnd)->OnAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}



void CMySocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	((CServerDlg*)m_pWnd)->OnClose();
	CAsyncSocket::OnClose(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
    ((CServerDlg*)m_pWnd)->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}
