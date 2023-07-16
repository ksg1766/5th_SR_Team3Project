#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFont :
    public CComponent
{
private:
    explicit    CFont();
    explicit    CFont(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit    CFont(const CComponent& rhs);
    virtual     ~CFont();

public:
    HRESULT Ready_Font();
    ID3DXFont* Create_3DXFont(_int _Height, _uint _Width, _uint _Weight,
        _bool _Italic, const wchar_t _wchar[], ID3DXFont* _pFont);//����(_int), �ʺ�(_uint),����0~1000(_uint), �����(_bool), �۲� ""
    void    Set_pFont(ID3DXFont* _pFont) { m_pFont = _pFont; }//�Ű������� �޾� ������Ʈ ������ ID3DXFont* pFont�� ����.
    void    Set_Rect(RECT _rect) { m_rect = _rect; }//���ڸ� ��� �簢���� ũ�� RECT
    void    Set_Anchor(DWORD anchor) { m_dwAnchor = anchor; }//���ڸ� ��� �簢���� ��ġ����. �ڼ��Ѱ� NODRAWTEXT�˻�
    void    Set_FontColor(_uint _FontColor) { m_FontColor = _FontColor;}//������ ���� ����. 0xff ??(����) ??(�ʷ�) ??(�Ķ�) ����. 0xffffffff == ���
   
    void    DrawText(const TCHAR* outputString, LONG x = 0, LONG y = 0);//����ϰ���� ����
public:
    static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent* Clone(void);
private:
    ID3DXFont*  m_pFont;
    DWORD       m_dwAnchor;
    RECT        m_rect;
    D3DXFONT_DESC m_lf;
    _uint       m_FontColor;//�⺻ 0xffffffff = ���
private:
    virtual void Free();
};
END