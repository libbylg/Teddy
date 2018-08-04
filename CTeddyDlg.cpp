
// CTeddyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Teddy.h"
#include "CTeddyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTeddyDlg 对话框




CTeddyDlg::CTeddyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeddyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeddyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MESSAGES, m_lbMessageBox);
    //DDX_Control(pDX, IDC_STATIC_META, m_stMeta);
    DDX_Control(pDX, IDC_STATIC_META_IMAGES, m_stMetaImageBar);
    DDX_Control(pDX, IDC_STATIC_MAP, m_stGameMapArea);
}

BEGIN_MESSAGE_MAP(CTeddyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CTeddyDlg 消息处理程序

BOOL CTeddyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    CRect rc;
    CWnd* pWnd = this->GetDlgItem(IDC_STATIC_SPLITTER);
    pWnd->GetWindowRect(rc);
    this->ScreenToClient(rc);
    rc.bottom = rc.top + STD_MARGIN / 2;
    (void)this->m_spSplitBar.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_STATIC_SPLITTER, SPS_HORIZONTAL);
    this->m_spSplitBar.RegisterLinkedWindow(SPLS_LINKED_UP, &(this->m_stGameMapArea));
    this->m_spSplitBar.RegisterLinkedWindow(SPLS_LINKED_DOWN, &(this->m_lbMessageBox));

    //  获取当前目录
    CString strSelfFileName;
    AfxGetModuleFileName(NULL, strSelfFileName);
    CString strSelfDir = strSelfFileName.Left(strSelfFileName.ReverseFind(_T('\\')));

    //  生成读取配置所需要的数据
    CString strProfile = strSelfDir + _T("\\conf\\Teddy.ini");
    TCHAR szValue[500];
    DWORD dwValueSize = sizeof(szValue) / sizeof(szValue[0]);

    //  读取图元文件所在的位置
    szValue[0] = _T('\0');
    GetPrivateProfileString(_T("Teddy"), _T("Meta-Files-Dir"), NULL, szValue, dwValueSize, (LPCTSTR)strProfile);
	CString strMetaFilesDir;
    if (szValue[0] == _T('\0'))
    {
        strMetaFilesDir = strSelfDir + _T("\\conf");
    }

    int iMapWidth = GetPrivateProfileInt(_T("Teddy"), _T("Map-Width"), MAP_WIDTH_DEF, (LPCTSTR)strProfile);
    
    int iHeightWidth = GetPrivateProfileInt(_T("Teddy"), _T("Map-Height"), MAP_HEIGHT_DEF, (LPCTSTR)strProfile);

    CString strError;
    int ret = m_oModel.Create(strMetaFilesDir, iMapWidth, iHeightWidth, strError);
    if (0 != ret)
    {
        m_oModel.m_arMessages.Add(strError);
    }

    this->m_spSplitBar.Relayout();

    this->DrawMetaImages();
    
    this->DrawMap1();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeddyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeddyDlg::OnPaint()
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
		CDialogEx::OnPaint();
        //DrawMetaImages();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeddyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTeddyDlg::DrawMetaImages()
{
    CSize  tSize(50, 50);

    for (int i = 0; i < this->m_oModel.m_bmTemplateImages.GetCount(); i++)
    {
        CStatic* pStatic = NULL;
        if (i < this->m_vTemplateImageItems.GetCount())
        {
            pStatic = this->m_vTemplateImageItems.GetAt(i);
        }
        else
        {
            CPoint tLeftTop((i * STD_WIDTH), 0);
            CRect rect(tLeftTop, tSize);
            pStatic = new CStatic();
            pStatic->Create(_T(""), SS_BITMAP | SS_CENTERIMAGE, rect, &(this->m_stMetaImageBar), ID_STATIC_META_IMAGE_BEGIN + i);
            m_vTemplateImageItems.Add(pStatic);
        }

        HBITMAP hBitmap = this->m_oModel.m_bmTemplateImages.GetAt(i);
        pStatic->SetBitmap(hBitmap);
        pStatic->ShowWindow(TRUE);
    }

    //  删除多余的模板项
    while (this->m_vTemplateImageItems.GetCount() > this->m_oModel.m_bmTemplateImages.GetCount())
    {
        CStatic* pStatic = this->m_vTemplateImageItems.GetAt(this->m_vTemplateImageItems.GetCount() - 1);
        pStatic->DestroyWindow();
        delete pStatic;
    }
}

void CTeddyDlg::DrawMap1()
{
    for (int row = 0; row < this->m_oModel.m_iGameMapHeight; row++)
    {
        for (int col = 0; col < this->m_oModel.m_iGameMapWidth; col++)
        {
            CStatic* pStatic = NULL;
            int iStaticIndex = ((row * this->m_oModel.m_iGameMapHeight) + col);
            if (iStaticIndex < this->m_vGameMapItems.GetCount())
            {
                pStatic = this->m_vGameMapItems.GetAt(iStaticIndex);
            }
            else
            {
                CPoint tLeftTop((row * this->m_oModel.m_siTemplateImageSize.cy), (col * this->m_oModel.m_siTemplateImageSize.cx));
                CRect rect(tLeftTop, this->m_oModel.m_siTemplateImageSize);
                pStatic = new CStatic();
                pStatic->Create(_T(""), SS_BITMAP | SS_CENTERIMAGE, rect, &(this->m_stGameMapArea), ID_STATIC_GAME_MAP_ITEM_BEGIN + iStaticIndex);
                this->m_vGameMapItems.Add(pStatic);
            }

            //  找到该 pStatic 所对应的模板图片的编号
            int iTemplateImageIndex = this->m_oModel.m_vGameMap.GetAt(iStaticIndex);

            //  将图片显示到 CStatic 上
            HBITMAP hBitmap = this->m_oModel.m_bmTemplateImages.GetAt(iTemplateImageIndex);
            pStatic->SetBitmap(hBitmap);
            pStatic->ShowWindow(TRUE);
        }
    }

    //  释放多余的 CStatic 控件对象
    int iMapItemCount = (this->m_oModel.m_iGameMapHeight * this->m_oModel.m_iGameMapWidth);
    while (this->m_vGameMapItems.GetCount() > iMapItemCount)
    {
        CStatic* pStatic = this->m_vTemplateImageItems.GetAt(this->m_vGameMapItems.GetCount() - 1);
        pStatic->DestroyWindow();
        delete pStatic;
    }
}


void CTeddyDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (!::IsWindow(this->m_stGameMapArea) ||
        !::IsWindow(this->m_lbMessageBox)  ||
        !::IsWindow(this->m_spSplitBar)    ||
        !::IsWindow(this->m_stMetaImageBar))
    {
        return;
    }

    CRect rect;
    this->GetClientRect(&rect);

    CRect siMetaImageBarRect;
    siMetaImageBarRect.left   = rect.left  + STD_MARGIN;
    siMetaImageBarRect.right  = rect.right - STD_MARGIN;
    siMetaImageBarRect.top    = rect.top   + STD_MARGIN;
    siMetaImageBarRect.bottom = siMetaImageBarRect.top + this->m_oModel.m_siTemplateImageSize.cy;

    CRect siOldMessageBoxRect;
    this->m_lbMessageBox.GetWindowRect(&siOldMessageBoxRect);
    CRect siMessageBoxRect;
    siMessageBoxRect.left   = siMetaImageBarRect.left;
    siMessageBoxRect.right  = siMetaImageBarRect.right;
    siMessageBoxRect.bottom = rect.bottom - STD_MARGIN;
    siMessageBoxRect.top    = siMessageBoxRect.bottom - siOldMessageBoxRect.Height();

    CRect siOldSplitBarRect;
    this->m_spSplitBar.GetWindowRect(siOldSplitBarRect);
    CRect siSplitBarRect;
    siSplitBarRect.left     =   siMetaImageBarRect.left;
    siSplitBarRect.right    =   siMetaImageBarRect.right;
    siSplitBarRect.bottom   =   siMessageBoxRect.top;
    siSplitBarRect.top      =   siSplitBarRect.bottom - siOldSplitBarRect.Height();

    CRect siGameMapAreaRect;
    siGameMapAreaRect.left  =   siMetaImageBarRect.left;
    siGameMapAreaRect.right =   siMetaImageBarRect.right;
    siGameMapAreaRect.top   =   siMetaImageBarRect.bottom + STD_MARGIN;
    siGameMapAreaRect.bottom=   siSplitBarRect.top;

    this->m_stGameMapArea.MoveWindow(&siGameMapAreaRect, TRUE);
    this->m_lbMessageBox.MoveWindow(&siMessageBoxRect, TRUE);
    this->m_spSplitBar.MoveWindow(&siSplitBarRect, TRUE);
    this->m_stMetaImageBar.MoveWindow(&siMetaImageBarRect, TRUE);
}
