
// serverDlg.h : ͷ�ļ�
//
#include "Mysocket.h"
#pragma once

#include <map>

typedef struct _SOCKET_STREAM_FILE_INFO {

    TCHAR       szFileTitle[128];                   //�ļ��ı�����
    DWORD       dwFileAttributes;                   //�ļ�������
    FILETIME    ftCreationTime;                     //�ļ��Ĵ���ʱ��
    FILETIME    ftLastAccessTime;                   //�ļ���������ʱ��
    FILETIME    ftLastWriteTime;                    //�ļ�������޸�ʱ��
    DWORD       nFileSizeHigh;                      //�ļ���С�ĸ�λ˫��
    DWORD       nFileSizeLow;                       //�ļ���С�ĵ�λ˫��
    DWORD       dwReserved0;                        //������Ϊ0
    DWORD       dwReserved1;                        //������Ϊ0

} SOCKET_STREAM_FILE_INFO, * PSOCKET_STREAM_FILE_INFO;

// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CString GetErrorMsg();
	void OnClose();
	void OnReceive();
	void OnAccept();
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };
//	CListBox	m_ctlRecvd;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	
	afx_msg void OnBtnClickedStart();
	afx_msg void OnBtnClickedClose();
	afx_msg void OnBtnClickedSendFile();
	void SendMsg(CString msg);
	void recvFile();
	afx_msg void OnBtnClickedOpenFolder();
	

public:
    CListBox m_ctlRecvd;
    CString m_strMsg;
    CString client_IP;
    CString m_strFolder;
    UINT client_Port;
    int m_Port;
    CString m_strPath;

private:
    CMySocket m_sConnectSocket;
    CMySocket m_sListenSocket;
    std::map<CString, int> m_fileMap;

};
