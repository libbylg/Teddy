
// CTeddyDlg.h : 头文件
//

#pragma once

#include "CTeddyModel.h"
#include "afxwin.h"
#include "SplitterControl.h"


#define STD_MARGIN  10
#define STD_WIDTH   50
#define STD_HEIGHT  50

#define ID_STATIC_META_IMAGE_BEGIN      100
#define ID_STATIC_GAME_MAP_ITEM_BEGIN   1000

class CImageStatic;
// CTeddyDlg 对话框
class CTeddyDlg : public CDialogEx
{
// 构造
public:
	CTeddyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEDDY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
    CTeddyModel         m_oModel;
    CArray<CImageStatic*>    m_vTemplateImageItems;
    CArray<CImageStatic*>    m_vGameMapItems;

private:
    void DrawTemplateImages();
    void DrawMap1();

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
    //virtual BOOL PreTranslateMessage(MSG* pMsg);
    

public:
    CStatic   m_stTemplateImageBar;
    CStatic   m_stGameMapArea;
    CListBox  m_lbMessageBox;
    CSplitterControl m_spSplitBar;

    afx_msg void OnSize(UINT nType, int cx, int cy);
    
};
