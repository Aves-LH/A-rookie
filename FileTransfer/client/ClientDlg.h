
// clientDlg.h : 头文件
#include "Mysocket.h"
#pragma once

#include <map>

typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //文件的标题名
	DWORD       dwFileAttributes;                   //文件的属性
	FILETIME    ftCreationTime;                     //文件的创建时间
	FILETIME    ftLastAccessTime;                   //文件的最后访问时间
	FILETIME    ftLastWriteTime;                    //文件的最后修改时间
	DWORD       nFileSizeHigh;                      //文件大小的高位双字
	DWORD       nFileSizeLow;                       //文件大小的低位双字
	DWORD       dwReserved0;                        //保留，为0
	DWORD       dwReserved1;                        //保留，为0

} SOCKET_STREAM_FILE_INFO, * PSOCKET_STREAM_FILE_INFO;

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CString GetErrorMsg();
	//	重写OnReceive()；
	void OnReceive();
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
