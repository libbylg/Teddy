
// CTeddyDlg.h : ͷ�ļ�
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
// CTeddyDlg �Ի���
class CTeddyDlg : public CDialogEx
{
// ����
public:
	CTeddyDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEDDY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
    CTeddyModel         m_oModel;
    CArray<CImageStatic*>    m_vTemplateImageItems;
    CArray<CImageStatic*>    m_vGameMapItems;

private:
    void DrawTemplateImages();
    void DrawMap1();

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
    //virtual BOOL PreTranslateMessage(MSG* pMsg);
    

public:
    CStatic   m_stTemplateImageBar;
    CStatic   m_stGameMapArea;
    CListBox  m_lbMessageBox;
    CSplitterControl m_spSplitBar;

    afx_msg void OnSize(UINT nType, int cx, int cy);
    
};
