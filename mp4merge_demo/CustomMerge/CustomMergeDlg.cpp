
// CustomMergeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CustomMerge.h"
#include "CustomMergeDlg.h"

#include "QtSequenceMerge.h"
#include <assert.h>
#include "p2pread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCustomMergeDlg �Ի���




CCustomMergeDlg::CCustomMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomMergeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCustomMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCustomMergeDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCustomMergeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &CCustomMergeDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCustomMergeDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BTN_MERGE, &CCustomMergeDlg::OnBnClickedBtnMerge)
END_MESSAGE_MAP()

void write_dword_t(_pbyte_t pbuffer, _uint32_t dword)
{
	if (pbuffer == NULL)
		return;

	for (int i = 0; i < sizeof(_uint32_t); i++)
	{
		*(pbuffer + i) = (_byte_t)(dword>>((sizeof(_uint32_t)-i-1) * 8));
	}
}

void write_ulonglong_t(_pbyte_t pbuffer, _uint64_t ulonglong)
{
	if (pbuffer == NULL)
		return;

	for (int i = 0; i < sizeof(_uint64_t); i++)
	{
		*(pbuffer + i) = (_byte_t)(ulonglong>>((sizeof(_uint64_t)-i-1) * 8));
	}
}

// CCustomMergeDlg ��Ϣ�������
BOOL CCustomMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCustomMergeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCustomMergeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCustomMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static _uint32_t ui32writefilesize = 0;
void CCustomMergeDlg::flyfox_player_request_media_cb(int nPieceIndex, unsigned char* in_pReceiveDataBuffer, int nBufferLen)
{
	if (in_pReceiveDataBuffer && nBufferLen && m_pwritefile)
	{
		fwrite(in_pReceiveDataBuffer, 1, nBufferLen, m_pwritefile);
		ui32writefilesize += nBufferLen;
	}
}

#define  READ_OFFSET  256 * 1024

FILE* CCustomMergeDlg::m_pwritefile = NULL;
void CCustomMergeDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString FilePathName;
	CFileDialog dlg(TRUE, L"mp4");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal() == IDOK)
	{
		FilePathName=dlg.GetPathName();

		int iRight = FilePathName.ReverseFind(L'��');
		int iLeft = FilePathName.ReverseFind(L'��');
		CString strCur = FilePathName.Mid(iLeft+1, iRight-iLeft-1);
		int iCur = 0;

		if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
			iCur =  _ttoi(strCur);

		if (iLeft > 0 && iRight > 0 && iCur >= 0)
		{
			CString strTempPath = FilePathName.Left(iLeft) + L"��%d��.mp4";
			CString strDestPath = FilePathName.Left(iLeft) + L".mp4";

			SetDlgItemText(IDC_DEST_PATH_EDIT, strDestPath);

			USES_CONVERSION;

			fopen_s(&m_pwritefile, T2A(strDestPath), "wb");

			_uint32_t iCount = (_uint32_t)iCur;
			FILE* file_t = NULL;
			do 
			{
				CString strPath;
				strPath.Format(strTempPath, iCount);
				errno_t err = fopen_s(&file_t, T2A(strPath), "rb");
				if (file_t)
				{
					iCount++;
					fclose(file_t);
				}
			} while (file_t);

			CP2PRead p2p(T2A(strTempPath), iCur, iCur + iCount);
			p2p.flyfox_media_data_cache_init(flyfox_player_request_media_cb, NULL);
			SequenceInfo sInfo;
			p2p.getSequenceInfo(&sInfo);

			QtSequence sSequence(&sInfo);

			_pbyte_t pHeaderBuffer = NULL;
			_uint32_t ui32HeaderBufferSize = 0;
			sSequence.getHeaderBuffer(&pHeaderBuffer, &ui32HeaderBufferSize);

			fwrite(pHeaderBuffer, 1, ui32HeaderBufferSize, m_pwritefile);
			ui32writefilesize += ui32HeaderBufferSize;

			_uint32_t ui32FileSize = 0;
			sSequence.getFileSize(&ui32FileSize);

			for (_uint32_t pos = ui32HeaderBufferSize; pos < ui32FileSize; pos += READ_OFFSET)
			{
				_uint32_t startclipnum = 0;
				_uint32_t endclipnum = 0;
				if (pos + READ_OFFSET <=  ui32FileSize)
					sSequence.TranslateSectionRange(pos, pos + READ_OFFSET, &startclipnum, &endclipnum);
				else
					sSequence.TranslateSectionRange(pos, ui32FileSize, &startclipnum, &endclipnum);
				for (_uint32_t j = startclipnum; j <= endclipnum; j++)
				{
					ClipOfRangeInfo sClipInfo;
					sSequence.getSectionInfoByIndex(j, &sClipInfo);

					p2p.flyfox_media_data_cache_request_piece(j, sClipInfo.StartOffset, sClipInfo.EndOffset);
				}
			}

			fclose(m_pwritefile);

			assert(ui32writefilesize == ui32FileSize);

			MessageBox(L"�ϲ��ɹ�!!!");
		}
		else
		{
			MessageBox(L"���Ƿֶ��ļ�, ����Ҫ�ϲ�");
		}
	}
}

//void CCustomMergeDlg::OnBnClickedButton2()
//{
//	// TODO: Add your control notification handler code here
//	CString FilePathName;
//	CFileDialog dlg(TRUE, L"mp4");///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
//	if(dlg.DoModal() == IDOK)
//	{
//		FilePathName=dlg.GetPathName();
//
//		int iRight = FilePathName.ReverseFind(L'��');
//		int iLeft = FilePathName.ReverseFind(L'��');
//		CString strCur = FilePathName.Mid(iLeft+1, iRight-iLeft-1);
//		int iCur = 0;
//		
//		if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
//			iCur =  _ttoi(strCur);
//
//		if (iLeft > 0 && iRight > 0 && iCur >= 0)
//		{
//			CString strTempPath = FilePathName.Left(iLeft) + L"��%d��.mp4";
//			CString strDestPath = FilePathName.Left(iLeft) + L".mp4";
//
//			SetDlgItemText(IDC_DEST_PATH_EDIT, strDestPath);
//
//			USES_CONVERSION;
//			_uint32_t iCount = (_uint32_t)iCur;
//			FILE* file_t = NULL;
//			do 
//			{
//				CString strPath;
//				strPath.Format(strTempPath, iCount);
//				errno_t err = fopen_s(&file_t, T2A(strPath), "rb");
//				if (file_t)
//				{
//					iCount++;
//					fclose(file_t);
//				}
//			} while (file_t);
//
//			SequenceInfo sInfo;
//			sInfo.pSectioninfo = new SectionInfo[iCount-iCur];
//			sInfo.nSectionCount = iCount-iCur;
//			for (_uint32_t i = iCur; i < iCount; i++)
//			{
//				CString strPath;
//				strPath.Format(strTempPath, i);
//				FILE* file = NULL;
//				errno_t err = fopen_s(&file, T2A(strPath), "rb");
//				if (file)
//				{
//					sInfo.pSectioninfo[i-iCur].pSectionInfoBuffer = new _byte_t[1024 * 1024 * 1]; //1M
//					_uint32_t nIn = fread(sInfo.pSectioninfo[i-iCur].pSectionInfoBuffer, 1, 1024 * 1024 * 1, file);
//					if (nIn)
//					{					
//						QtHeader sHeader;	
//						sHeader.Fill(sInfo.pSectioninfo[i-iCur].pSectionInfoBuffer, nIn);
//
//						sInfo.pSectioninfo[i-iCur].ui32SectionInfoBufferSize = sHeader.ftyp.dwSize + sHeader.moov.dwSize;
//					}
//					else
//					{
//						delete [] sInfo.pSectioninfo[i-iCur].pSectionInfoBuffer;
//						sInfo.pSectioninfo[i-iCur].pSectionInfoBuffer = NULL;
//					}
//
//					fclose(file);
//				}
//			}
//
//			QtSequence sSequence(&sInfo);
//
//			_pbyte_t pbuffer = _null_t;
//			_uint32_t ui32buffersize= 0;
//			sSequence.getHeaderBuffer(&pbuffer, &ui32buffersize);
//
//			FILE* fwritefile = NULL;
//			errno_t error = fopen_s(&fwritefile, T2A(strDestPath), "wb");
//			if (fwritefile)
//			{
//				_uint32_t nOut = fwrite(pbuffer, 1, ui32buffersize, fwritefile);						
//				
//				_pbyte_t ptest_read_buffer = new _byte_t[1024 * 1024 * 1]; //1M
//				for (_uint32_t i = iCur; i < iCount; i++)
//				{
//					CString strPath;
//					strPath.Format(strTempPath, i);
//					FILE* file = NULL;
//					errno_t err = fopen_s(&file, T2A(strPath), "rb");
//					if (file)
//					{
//						SectionDataInfo sInfo;
//						sSequence.getSectionDataInfoByIndex(i-iCur, &sInfo);
//						fseek(file, sInfo.ui32DataOffset, SEEK_SET);
//
//						_uint32_t ui32BufferSize = sInfo.ui32DataSize;
//						do 
//						{
//							_uint32_t nIn = fread(ptest_read_buffer, 1, 1024 * 1024 * 1, file);
//
//							if (ui32BufferSize < nIn)
//							{
//								nOut = fwrite(ptest_read_buffer, 1, ui32BufferSize, fwritefile);
//								break;
//							}
//							{
//								nOut = fwrite(ptest_read_buffer, 1, nIn, fwritefile);
//							}
//
//							ui32BufferSize -= nIn;
//						} while (ui32BufferSize > 0);
//					}
//
//					fclose(file);
//				}
//					
//				delete [] ptest_read_buffer;
//				fclose(fwritefile);
//			}
//
//			for (_uint32_t i = 0; i < sInfo.nSectionCount; i++)
//			{
//				delete [] sInfo.pSectioninfo[i].pSectionInfoBuffer;
//				sInfo.pSectioninfo[i].pSectionInfoBuffer = NULL;
//				sInfo.pSectioninfo[i].ui32SectionInfoBufferSize = 0;
//			}
//
//			for (_uint32_t i = 0; i < sInfo.nSectionCount; i++)
//			{
//				delete [] sInfo.pSectioninfo;
//				sInfo.pSectioninfo = NULL;
//			}
//
//			sInfo.nSectionCount = 0;
//
//			MessageBox(L"�ϲ��ɹ�!!!");
//		}
//		else
//		{
//			MessageBox(L"���Ƿֶ��ļ�, ����Ҫ�ϲ�");
//		}
//	}
//
//	return;
//}

void CCustomMergeDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
	}
}

void CCustomMergeDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	}
}

void CCustomMergeDlg::OnBnClickedBtnMerge()
{
	// TODO: Add your control notification handler code here
	NPT_LogManager::GetDefault().Configure("plist:.level=ALL;.handlers=FileHandler;"
		".FileHandler.filename=C:\\log\\client.log;"
		".FileHandler.filter=9");
	// TODO: Add your control notification handler code here
	NPT_String path="/������è2[�����]";
	NPT_HttpUrl url=NPT_HttpUrl("127.0.0.1", 8080, path.GetChars());
	NPT_DataBuffer buffer;
	bool ret=true;
	ret=RecvHeaderData(url,buffer);

	if (ret)
	{
		NPT_Byte* pdata=const_cast<unsigned char*>(buffer.GetData());

		SequenceInfo sInfo;
		unsigned int tmp_size(0);
		NPT_CopyMemory(&tmp_size,pdata,4);
		unsigned int length=4;
		unsigned int inum=sInfo.nSectionCount=tmp_size;
		sInfo.pSectioninfo=new SectionInfo[inum];
		pdata+=4;
		for (unsigned int i=0;i<inum;++i)
		{
			NPT_CopyMemory(&tmp_size,pdata,4);
			pdata+=4;
			sInfo.pSectioninfo[i].ui32SectionInfoBufferSize=tmp_size;
			sInfo.pSectioninfo[i].pSectionInfoBuffer=pdata;
			pdata+=tmp_size;
		}

		QtSequence sSequence(&sInfo);

		_pbyte_t pHeaderBuffer = NULL;
		_uint32_t ui32HeaderBufferSize = 0;
		sSequence.getHeaderBuffer(&pHeaderBuffer, &ui32HeaderBufferSize);

		FILE* file;
		errno_t err=fopen_s(&file,"D:\\�ز�\\�ֶκϲ�\\������è2\\������è2[�����].mp4","wb");
		fwrite(pHeaderBuffer, 1, ui32HeaderBufferSize, file);

		delete []sInfo.pSectioninfo;
		sInfo.pSectioninfo=NULL;
		if(MessageBox(L"���շֶ�ͷ���ϲ��ɹ�,�Ƿ��������ý������?",NULL,MB_YESNO)==IDYES)
		{
			_uint32_t ui32FileSize = 0;
			sSequence.getFileSize(&ui32FileSize);

			for (_uint32_t pos = ui32HeaderBufferSize; pos < ui32FileSize; pos += READ_OFFSET)
			{
				_uint32_t startclipnum = 0;
				_uint32_t endclipnum = 0;
				if (pos + READ_OFFSET <=  ui32FileSize)
					sSequence.TranslateSectionRange(pos, pos + READ_OFFSET, &startclipnum, &endclipnum);
				else
					sSequence.TranslateSectionRange(pos, ui32FileSize, &startclipnum, &endclipnum);
				for (_uint32_t j = startclipnum; j <= endclipnum; j++)
				{
					ClipOfRangeInfo sClipInfo;
					sSequence.getSectionInfoByIndex(j, &sClipInfo);
					char query[10]={};

					char* path_url="/������è2[�����]";

					NPT_HttpUrl url=NPT_HttpUrl("127.0.0.1", 8082, path_url);
					_itoa_s(j,query,10);
					url.SetQuery(query);

					NPT_DataBuffer mediadata;
					RecvMediaData(url,sClipInfo.StartOffset,sClipInfo.EndOffset,mediadata);			

					fwrite(mediadata.GetData(), 1, mediadata.GetBufferSize(), file);
				}
			}
			AfxMessageBox(L"�ļ����ճɹ���");
		}
		fclose(file);
	}
	else
		AfxMessageBox(L"���շֶ�ͷʧ�ܣ�");

}

bool CCustomMergeDlg::RecvHeaderData(NPT_HttpUrl url,NPT_DataBuffer& buffer)
{
	NPT_HttpClient client;
	NPT_String rdUrl=url.ToString();
	// first request
	NPT_HttpRequest request(url, NPT_HTTP_METHOD_GET, NPT_HTTP_PROTOCOL_1_1);

	NPT_HttpResponse* response = NULL;
	client.SendRequest(request, response);
	NPT_HttpEntity* entity = NULL;
	if (response && (entity = response->GetEntity())) {
		if (NPT_FAILED(entity->Load(buffer))) return false;
	}
	else
		return false;

	return true;
}

bool CCustomMergeDlg::RecvMediaData(NPT_HttpUrl url,int in_nStartPos, 
									int in_nEndPos,NPT_DataBuffer& buffer)
{
	NPT_HttpClient client;
	NPT_String rdUrl=url.ToString();
	// first request
	NPT_HttpRequest request(url, NPT_HTTP_METHOD_GET, NPT_HTTP_PROTOCOL_1_1);
	char range[100]={0};
	sprintf(range," bytes=%d-%d",in_nStartPos,in_nEndPos-1);
	request.GetHeaders().SetHeader(NPT_HTTP_HEADER_RANGE,range);

	NPT_HttpResponse* response = NULL;
	client.SendRequest(request, response);
	NPT_HttpEntity* entity = NULL;
	if (response && (entity = response->GetEntity())) {
		if (NPT_FAILED(entity->Load(buffer))) return false;
	}
	else
		return false;

	return true;
}