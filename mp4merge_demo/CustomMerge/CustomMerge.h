
// CustomMerge.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCustomMergeApp:
// �йش����ʵ�֣������ CustomMerge.cpp
//

class CCustomMergeApp : public CWinAppEx
{
public:
	CCustomMergeApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCustomMergeApp theApp;