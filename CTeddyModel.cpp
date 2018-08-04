#include "stdafx.h"

#include "CTeddyModel.h"

int CStringCompare(const void* a, const void* b)
{
    return ((CString*)a)->Compare(*(CString*)b);
}

int CTeddyModel::LoadTemplateImges(const CString& strTemplateImageFileDir, CString& error)
{
    //  扫描所有的文件
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
        error.Format(_T("在模板文件目录 '%s' 下未找到任何 *.png 的文件，可能是文件或者目录不存在或者无法访问"), (LPCTSTR)strTemplateImageFileDir);
        return -4;
    }

    qsort(vTemplateImageFileNames.GetData(), vTemplateImageFileNames.GetSize(), sizeof(CString), CStringCompare);

    for (int i = 0; i < vTemplateImageFileNames.GetCount(); i++)
    {
        //  生成图片标签
        CString strIndex;
        strIndex.Format(_T("%d"), i);

        //  加载图片对象
        CString fileName = vTemplateImageFileNames.GetAt(i);
        CImage img;
        HRESULT hResult = img.Load(fileName);
        if (FAILED(hResult)) 
        {
            error.Format(_T("加载图片失败(%s)"), (LPCTSTR)(fileName));
            continue;
        }
        HBITMAP hbitmap = img.Detach();

        //  将图片标签和图片对象保存起来
        this->m_bmTemplateImages.Add(hbitmap);
    }

    return 0;
}

int CTeddyModel::Create(const CString& strTemplateImageFileDir, int width, int height, CString& error)
{
    if ((width < MAP_WIDTH_MIN) || (width  > MAP_WIDTH_MAX))
    {
        error.Format(_T("地图宽度太窄或者太宽（当前版本的 Teddy 地图编辑器的宽度范围为 %d ~ %d，请照此设置）"), MAP_WIDTH_MIN, MAP_WIDTH_MIN);
        return -1;
    }

    if ((height < MAP_HEIGHT_MIN) || (height > MAP_WIDTH_MAX))
    {
        error.Format(_T("地图宽度太窄或者太宽（当前版本的 Teddy 地图编辑器的宽度范围为 %d ~ %d，请照此设置）"), MAP_HEIGHT_MIN, MAP_HEIGHT_MAX);
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
