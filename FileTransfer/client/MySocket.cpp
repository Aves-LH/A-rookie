// Mysocket.cpp : ʵ���ļ�
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


// CMySocket ��Ա����
void CMySocket::SetParent(CDialog *pWnd)
{
   m_pWnd = pWnd;
}

void CMySocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	((CClientDlg*)m_pWnd)->OnReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

