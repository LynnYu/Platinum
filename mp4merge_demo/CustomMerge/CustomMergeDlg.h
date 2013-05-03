
// CustomMergeDlg.h : 头文件
//

#pragma once

#include "..\include\Neptune.h"
#include "..\include\platinum.h"

#ifdef _DEBUG
#include <fstream>
#pragma comment( lib, "..\\lib\\debug\\Neptune.lib")
#pragma comment( lib, "..\\lib\\debug\\Platinum.lib")
#else
#pragma comment( lib, "..\\lib\\release\\Neptune.lib")
#pragma comment( lib, "..\\lib\\release\\Platinum.lib")
#endif

// CCustomMergeDlg 对话框
class CCustomMergeDlg : public CDialog
{
// 构造
public:
	CCustomMergeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CUSTOMMERGE_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	void OnBnClickedButton2();

protected:
	static void flyfox_player_request_media_cb(int nPieceIndex, unsigned char* in_pReceiveDataBuffer, int nBufferLen);

private:
	static FILE* m_pwritefile;
public:
	afx_msg void OnBnClickedBtnMerge();
	bool RecvHeaderData(NPT_HttpUrl url,NPT_DataBuffer& buffer);
	bool RecvMediaData(NPT_HttpUrl url,int in_nStartPos, int in_nEndPos,NPT_DataBuffer& buffer);
};
