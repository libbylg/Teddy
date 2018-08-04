#include "stdafx.h"

#include "CTeddyModel.h"

int CStringCompare(const void* a, const void* b)
{
    return ((CString*)a)->Compare(*(CString*)b);
}

int CTeddyModel::LoadTemplateImges(const CString& strTemplateImageFileDir, CString& error)
{
    //  ɨ�����е��ļ�
    CArray<CString,CString> vTemplateImageFileNames;
    CFileFind fd;
    BOOL bFounded = fd.FindFile(strTemplateImageFileDir + "\\*.png");
    while (bFounded)
    {
        bFounded = fd.FindNextFile();
        vTemplateImageFileNames.Add(fd.GetFilePath());
    }

    if (vTemplateImageFileNames.GetCount() <= 0)
    {
        error.Format(_T("��ģ���ļ�Ŀ¼ '%s' ��δ�ҵ��κ� *.png ���ļ����������ļ�����Ŀ¼�����ڻ����޷�����"), (LPCTSTR)strTemplateImageFileDir);
        return -4;
    }

    qsort(vTemplateImageFileNames.GetData(), vTemplateImageFileNames.GetSize(), sizeof(CString), CStringCompare);

    for (int i = 0; i < vTemplateImageFileNames.GetCount(); i++)
    {
        //  ����ͼƬ��ǩ
        CString strIndex;
        strIndex.Format(_T("%d"), i);

        //  ����ͼƬ����
        CString fileName = vTemplateImageFileNames.GetAt(i);
        CImage img;
        HRESULT hResult = img.Load(fileName);
        if (FAILED(hResult)) 
        {
            error.Format(_T("����ͼƬʧ��(%s)"), (LPCTSTR)(fileName));
            continue;
        }
        HBITMAP hbitmap = img.Detach();

        //  ��ͼƬ��ǩ��ͼƬ���󱣴�����
        this->m_bmTemplateImages.Add(hbitmap);
    }

    return 0;
}

int CTeddyModel::Create(const CString& strTemplateImageFileDir, int width, int height, CString& error)
{
    if ((width < MAP_WIDTH_MIN) || (width  > MAP_WIDTH_MAX))
    {
        error.Format(_T("��ͼ���̫խ����̫����ǰ�汾�� Teddy ��ͼ�༭���Ŀ�ȷ�ΧΪ %d ~ %d�����մ����ã�"), MAP_WIDTH_MIN, MAP_WIDTH_MIN);
        return -1;
    }

    if ((height < MAP_HEIGHT_MIN) || (height > MAP_WIDTH_MAX))
    {
        error.Format(_T("��ͼ���̫խ����̫����ǰ�汾�� Teddy ��ͼ�༭���Ŀ�ȷ�ΧΪ %d ~ %d�����մ����ã�"), MAP_HEIGHT_MIN, MAP_HEIGHT_MAX);
        return -2;
    }

    int ret = this->LoadTemplateImges(strTemplateImageFileDir, error);
    if (0 != ret)
    {
        return -3;
    }

    this->m_siTemplateImageSize = CSize(50, 50);

    this->m_iGameMapHeight = height;
    this->m_iGameMapWidth  = width;

    for (int i = 0; i < height * width; i++)
    {
        this->m_vGameMap.Add(0);
    }

    return 0;
}


void CTeddyModel::Clear()
{

}
