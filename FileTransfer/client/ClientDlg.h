
// clientDlg.h : ͷ�ļ�
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

// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CString GetErrorMsg();
	//	��дOnReceive()��
	void OnReceive();
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

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
	
	afx_msg void OnBtnClickedConnect();
	afx_msg void OnBtnClickedDisconnect();
	afx_msg void ReceiveFile();
	
	afx_msg void OnBtnClickedClientOpenfolder();
	afx_msg void OnBtnClickedSend();
	
public:
    CString m_strMess;
    CString m_strIPAddress;
    CString m_strFolder;
    CString m_strRecieveFile;
    int m_Port;

private:
    std::map<CString, int> m_fileMap;
    CMySocket m_sConnectSocket;
    CMySocket m_sListenSocket;
};
