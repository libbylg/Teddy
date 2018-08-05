// ImageStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "Teddy.h"
#include "ImageStatic.h"



// CImageStatic

IMPLEMENT_DYNAMIC(CImageStatic, CStatic)

CImageStatic::CImageStatic()
{
    this->m_eStatus = ISS_INACTIVE;
    this->m_hBitmap = NULL;
    this->m_iBorderWidth = 0;
}

CImageStatic::~CImageStatic()
{
}


BEGIN_MESSAGE_MAP(CImageStatic, CStatic)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
//     ON_WM_NCPAINT()
//     ON_WM_NCCALCSIZE()
//     ON_WM_DESTROY()
//     ON_WM_CREATE()
//     ON_WM_PAINT()
END_MESSAGE_MAP()


void CImageStatic::SetupBitmap(HBITMAP h)
{
    this->m_hBitmap = h;
}

void CImageStatic::SetupBorderWidth(int w)
{
    this->m_iBorderWidth = w;
}

// CImageStatic 消息处理程序
BOOL CImageStatic::Create(const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    BOOL bRet = CStatic::Create(_T(""), (/*SS_BITMAP |*/ SS_CENTERIMAGE | SS_NOTIFY), rect, pParentWnd, nID);
    if (!bRet)
    {
        return bRet;
    }

    this->m_eStatus = ISS_INACTIVE;

    return bRet;
}

COLORREF CImageStatic::GetStatusColor()
{
    return RGB(0xFF,0x00,0x00);
    switch (this->m_eStatus)
    {
    case ISS_HOVER:     return  COLOR_3DSHADOW;
    case ISS_ACTIVE:    return  COLOR_ACTIVEBORDER;
    case ISS_INACTIVE:  return  COLOR_BTNFACE;
    default:            return  COLOR_BTNFACE;
    }
}


void CImageStatic::OnMouseHover(UINT, CPoint)
{
    AfxTrace(_T("%p OnMouseHover \r\n"), this->GetSafeHwnd());
}

void CImageStatic::OnMouseLeave()
{
    AfxTrace(_T("%p OnMouseLeave \r\n"), this->GetSafeHwnd());
}

void CImageStatic::OnMouseMove(UINT nFlags, CPoint point)
{
    CStatic::OnMouseMove(nFlags, point);

    TRACKMOUSEEVENT MouseBehaviour;
    MouseBehaviour.cbSize = sizeof(TRACKMOUSEEVENT);
    MouseBehaviour.dwFlags = TME_HOVER | TME_LEAVE;
    MouseBehaviour.hwndTrack = GetSafeHwnd();
    MouseBehaviour.dwHoverTime = HOVER_DEFAULT;

    TrackMouseEvent(&MouseBehaviour);
}

// void CImageStatic::OnNcPaint()
// {
//     CRect rcWindow;
//     this->GetWindowRect(&rcWindow);
//     this->ScreenToClient(&rcWindow);
// 
//     CPen pnPen(PS_SOLID, IMAGE_STATIC_BORDER_WIDTH, this->GetStatusColor());
//     CDC* pDC = this->GetWindowDC();
//     CPen* oldPen = pDC->SelectObject(&pnPen);
//     pDC->Rectangle(&rcWindow);
//     pDC->SelectObject(oldPen);
// }
//
// void CImageStatic::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
// {
// //     if (bCalcValidRects)
// //     {
// //         CRect& rc = (CRect&)lpncsp->rgrc[0];
// //         rc.top    += IMAGE_STATIC_BORDER_WIDTH;   
// //         rc.left   += IMAGE_STATIC_BORDER_WIDTH;   
// //         rc.bottom -= IMAGE_STATIC_BORDER_WIDTH;   
// //         rc.right  -= IMAGE_STATIC_BORDER_WIDTH;
// //         return;
// //     }
// 
//     CStatic::OnNcCalcSize(bCalcValidRects, lpncsp);
// }
// 
// int CImageStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
//     return CStatic::OnCreate(lpCreateStruct);
// }
// 
// void CImageStatic::OnDestroy()
// {
//     CStatic::OnDestroy();
// }

IMAGE_STATIC_STATUS CImageStatic::GetStatus()
{
    return this->m_eStatus;
}

void CImageStatic::ChangeStatus(IMAGE_STATIC_STATUS newStatus)
{
    if (newStatus == this->m_eStatus)
    {
        return;
    }

    this->m_eStatus = newStatus;

    this->Invalidate(FALSE);
}

// void CImageStatic::OnPaint()
// {
//     CRect rect;
//     this->GetClientRect(&rect);
// 
//     CDC* pDC = this->GetDC();
// 
//     //  先放入内存DC
//     CDC dcMemory;
//     dcMemory.CreateCompatibleDC(pDC);
//     dcMemory.SelectObject(this->GetBitmap());
// 
//     BITMAP bmInfo;
//     ::GetObject(this->GetBitmap(), sizeof(bmInfo), &bmInfo);
//     pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMemory, 0, 0, bmInfo.bmWidth,bmInfo.bmHeight, SRCCOPY);
// 
//     ReleaseDC(pDC);
// 
//     ///CStatic::OnPaint();
// }
// 

void CImageStatic::PreSubclassWindow()
{
    DWORD dwStyle = GetStyle();  
    SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_OWNERDRAW); 

    CStatic::PreSubclassWindow();  
}

void CImageStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//     lpDrawItemStruct->rcItem.left += IMAGE_STATIC_BORDER_WIDTH;
//     lpDrawItemStruct->rcItem.right -= IMAGE_STATIC_BORDER_WIDTH;
//     lpDrawItemStruct->rcItem.top += IMAGE_STATIC_BORDER_WIDTH;
//     lpDrawItemStruct->rcItem.bottom -= IMAGE_STATIC_BORDER_WIDTH;
//     CStatic::DrawItem(lpDrawItemStruct);

    CRect rClientRect;
    this->GetClientRect(&rClientRect);
    CDC* pDC = this->GetDC();
  


    //  先画图片
    CRect rImageRect = rClientRect;
    rImageRect.left   += this->m_iBorderWidth;
    rImageRect.right  -= this->m_iBorderWidth;
    rImageRect.top    += this->m_iBorderWidth;
    rImageRect.bottom -= this->m_iBorderWidth;
    HBITMAP hImageBitmap = this->m_hBitmap;//this->GetBitmap();
    BITMAP bmInfo = {0};
    ::GetObject(hImageBitmap, sizeof(bmInfo), &bmInfo);
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);
    dcMemory.SelectObject(hImageBitmap);
    pDC->StretchBlt(rImageRect.left, rImageRect.top, 
        rImageRect.Width(), rImageRect.Height(),
        &dcMemory, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);


    //  再画外围框
    if (this->m_iBorderWidth > 0)
    {
       DrawBorder(pDC, rClientRect);
    }

    ReleaseDC(pDC);
}

void CImageStatic::DrawBorder(CDC* pDC, const CRect& rClientRect)
{
    CPen pnPen(PS_SOLID, this->m_iBorderWidth, this->GetStatusColor());
    CPen* oldPen = pDC->SelectObject(&pnPen);
    pDC->MoveTo(rClientRect.TopLeft());
    pDC->LineTo(rClientRect.right, rClientRect.top);
    pDC->LineTo(rClientRect.BottomRight());
    pDC->LineTo(rClientRect.left, rClientRect.bottom);
    pDC->LineTo(rClientRect.TopLeft());
    pDC->SelectObject(oldPen);
}