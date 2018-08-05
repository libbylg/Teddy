#pragma once


// CImageStatic


enum IMAGE_STATIC_STATUS
{
    ISS_HOVER,
    ISS_ACTIVE,
    ISS_INACTIVE,
};


class CImageStatic : public CStatic
{
	DECLARE_DYNAMIC(CImageStatic)

public:
	CImageStatic();
	virtual ~CImageStatic();
    virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
    IMAGE_STATIC_STATUS GetStatus();
    void ChangeStatus(IMAGE_STATIC_STATUS newStatus);
    void SetupBitmap(HBITMAP h);
    void SetupBorderWidth(int w);

public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void PreSubclassWindow();  

private:
    IMAGE_STATIC_STATUS m_eStatus;
    HBITMAP             m_hBitmap;
    int                 m_iBorderWidth;

private:
    COLORREF GetStatusColor();
    void DrawBorder(CDC* pDC, const CRect& rClientRect);
protected:
    afx_msg void OnMouseHover(UINT, CPoint);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseMove(UINT, CPoint);
//     afx_msg void OnNcPaint();
//     afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
//     afx_msg void OnDestroy();
//     afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    //afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


