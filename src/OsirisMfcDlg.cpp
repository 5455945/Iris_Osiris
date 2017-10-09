// OsirisMfcDlg.cpp : 实现文件
//

//#include "stdafx.h"
#include <afxwin.h>
#include "OsirisMfc.h"
#include "OsirisMfcDlg.h"
#include "OsiManager.h"
#include "AfxDialogEx.h"
#include "ShlObj.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace osiris;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// COsirisMfcDlg 对话框
COsirisMfcDlg::COsirisMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COsirisMfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COsirisMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COsirisMfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_RECOGNITION, &COsirisMfcDlg::OnBnClickedRecognition)
	ON_BN_CLICKED(IDC_BTN_SET_DATA_DIR, &COsirisMfcDlg::OnBnClickedBtnSetDataDir)
	ON_BN_CLICKED(IDC_BTN_SET_OUTPUT_DIR, &COsirisMfcDlg::OnBnClickedBtnSetOutputDir)
END_MESSAGE_MAP()


// COsirisMfcDlg 消息处理程序

BOOL COsirisMfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetDefaultDataDir();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COsirisMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COsirisMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR COsirisMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COsirisMfcDlg::OnBnClickedRecognition()
{
	float fSimilarityDegree = 0;
	std::string sSimilarityDegree("");
	std::stringstream ssout;

	OsiManager theManager(m_sInputDataDir, m_sOutputDataDir);

	cvNamedWindow("S5000R00", 1);
	cvNamedWindow("segment-S5000R00", 1);
	std::string sInFile11 = m_sInputDataDir + "/SourceImage/S5000R00.jpg";
    IplImage* img1 = cvLoadImage(sInFile11.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	cvShowImage("S5000R00", img1);
	theManager.process(sInFile11);
	std::string sInFile12 = m_sOutputDataDir + "/SegmentedImages/S5000R00_segm.bmp";
	IplImage* seg1 = cvLoadImage(sInFile12.c_str(), CV_LOAD_IMAGE_COLOR);
	cvShowImage("segment-S5000R00", seg1);

	cvNamedWindow("S5000R01", 1);
	cvNamedWindow("segment-S5000R01", 1);
	std::string sInFile21 = m_sInputDataDir + "/SourceImage/S5000R01.jpg";
	IplImage* img2 = cvLoadImage(sInFile21.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	cvShowImage("S5000R01", img2);
	theManager.process(sInFile21);
	std::string sInFile22 = m_sOutputDataDir + "/SegmentedImages/S5000R01_segm.bmp";
	IplImage* seg2 = cvLoadImage(sInFile22.c_str(), CV_LOAD_IMAGE_COLOR);
	cvShowImage("segment-S5000R01", seg2);
	fSimilarityDegree = theManager.match(sInFile11, sInFile21);
	if (fSimilarityDegree < 0.32) {
		ssout << "T1 SimilarityDegree=" << fSimilarityDegree << ", S5000R00 and S5000R01 is same person\n";
	}
	else {
		ssout << "T1 SimilarityDegree=" << fSimilarityDegree << ", S5000R00 and S5000R01 is not same person\n";
	}

	cvNamedWindow("S5001R01", 1);
	cvNamedWindow("segment-S5001R01", 1);
	std::string sInFile31 = m_sInputDataDir + "/SourceImage/S5001R01.jpg";
	IplImage* img3 = cvLoadImage(sInFile31.c_str(), CV_LOAD_IMAGE_GRAYSCALE );
	cvShowImage("S5001R01", img3);
	theManager.process(sInFile31);
	std::string sInFile32 = m_sOutputDataDir + "/SegmentedImages/S5001R01_segm.bmp";
	IplImage* seg3 = cvLoadImage(sInFile32.c_str(), CV_LOAD_IMAGE_COLOR );
	cvShowImage("segment-S5001R01", seg3 );
	fSimilarityDegree = theManager.match(sInFile11, sInFile31);
	ssout << std::endl;
	if (fSimilarityDegree < 0.32) {
		ssout << "T2 SimilarityDegree=" << fSimilarityDegree << ", S5000R00 and S5001R01 is same person\n";
	}
	else {
		ssout << "T2 SimilarityDegree=" << fSimilarityDegree << ", S5000R00 and S5001R01 is not same person\n";
	}

	sSimilarityDegree = ssout.str();

	SetDlgItemTextA(IDC_STC_RESUALT, sSimilarityDegree.c_str());
}

void COsirisMfcDlg::OnCancel()
{
	CDialog::OnCancel();
}

void COsirisMfcDlg::OnBnClickedBtnSetDataDir()
{
	CString sFolderPath = GetFolderDir();
	if (sFolderPath.GetLength() > 0) {
		m_sInputDataDir = CStringA(sFolderPath);
		SetDlgItemTextA(IDC_EDIT_INPUT_DIR, m_sInputDataDir.c_str());
	}
}

void COsirisMfcDlg::OnBnClickedBtnSetOutputDir()
{
	CString sFolderPath = GetFolderDir();
	if (sFolderPath.GetLength() > 0) {
		m_sOutputDataDir = CStringA(sFolderPath);
		SetDlgItemTextA(IDC_EDIT_OUTPUT_DIR, m_sOutputDataDir.c_str());
	}
}

void COsirisMfcDlg::SetDefaultDataDir()
{
	char sFullPath[MAX_PATH];
	::GetModuleFileNameA(NULL, sFullPath, MAX_PATH);
	std::string sPath = "";
	sPath = sFullPath;
	int pos = sPath.find_last_of(L'\\', sPath.length());
	m_sInputDataDir = sPath.substr(0, pos) + "/../input";
	m_sOutputDataDir = sPath.substr(0, pos) + "/../output";
	SetDlgItemTextA(IDC_EDIT_INPUT_DIR, m_sInputDataDir.c_str());
	SetDlgItemTextA(IDC_EDIT_OUTPUT_DIR, m_sOutputDataDir.c_str());
}

CString COsirisMfcDlg::GetFolderDir()
{
	CString         sFolderPath;
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	BROWSEINFO      sInfo;

	LPITEMIDLIST    rootLoation;
	::SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &rootLoation);

	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = rootLoation;
	sInfo.lpszTitle = _T("请选择数据存放路径：");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			sFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	return sFolderPath;
}
