
// CustomMergeDlg.h : ͷ�ļ�
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

// CCustomMergeDlg �Ի���
class CCustomMergeDlg : public CDialog
{
// ����
public:
	CCustomMergeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CUSTOMMERGE_DIALOG };

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
