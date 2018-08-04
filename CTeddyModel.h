#ifndef __CTeddyModel_H_
#define __CTeddyModel_H_

#define MAP_WIDTH_MAX   200
#define MAP_WIDTH_MIN   4
#define MAP_WIDTH_DEF   12

#define MAP_HEIGHT_MAX  200
#define MAP_HEIGHT_MIN  4
#define MAP_HEIGHT_DEF  12

class CTeddyModel
{
public:
    CArray<HBITMAP> m_bmTemplateImages;
    CSize           m_siTemplateImageSize;
    
    CArray<int>     m_vGameMap;
    int             m_iGameMapWidth;
    int             m_iGameMapHeight;

    CArray<CString, CString>    m_arMessages;

    int     Create(const CString& pictureDir, int width, int height, CString& error);
    void    Clear();

private:
    int  LoadTemplateImges(const CString& strTemplateImageFileDir, CString& error);
};



#endif//__CTeddyModel_H_
