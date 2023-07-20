#include "Font.h"

CFont::CFont()
{
}

CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CFont::CFont(const CComponent& rhs)
	: CComponent(rhs)
{
}

CFont::~CFont()
{

}


HRESULT CFont::Ready_Font()
{
	m_pFont = 0;
	m_dwAnchor = 0;
	m_rect = {0,0, 0, 0};
	m_FontColor = 0xffffffff;
	return S_OK;
}

ID3DXFont* CFont::Create_3DXFont(_int _Height, _uint _Width, _uint _Weight, _bool _Italic, WCHAR _wchar[], ID3DXFont* _pFont)
{
	D3DXFONT_DESC	lf;
	ZeroMemory(&lf, sizeof(D3DXFONT_DESC));
	lf.Height = _Height;//���� ���� ����
	lf.Width = _Width;//�ʺ� ���� ����
	lf.Weight = _Weight;//������ ����	0~1000
	lf.Italic = _Italic;//����Ӳ�
	lf.MipLevels = 1;//��û�� �Ӽ���. ���� 1�� �� �ؽ�ó ������ ȭ�� ������ �����ϰ� ����
	lf.CharSet = DEFAULT_CHARSET;//��������
	lf.OutputPrecision = OUT_DEFAULT_PRECIS;//��� ���е�
	lf.Quality = DEFAULT_QUALITY;//ǰ��
	lf.PitchAndFamily = DEFAULT_PITCH;//��ġ �� �йи�
	lf.FaceName, _wchar;//���ڿ��� ���̰� 32�� �ʰ��ϸ� �ȵ�. �۲ü���.

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &lf, &_pFont)))
	{
		MessageBox(0, TEXT("D3DXCreateFontIndirect() - FAILED"), 0, 0);
	}
	return _pFont;
}



void CFont::DrawText(const TCHAR* outputString, LONG x, LONG y)
{
	m_rect;
#ifdef UNICODE
	m_pFont->DrawTextW(0, outputString, -1, &m_rect, m_dwAnchor, m_FontColor);//��¹��ڿ�, null���ڿ�, �ؽ�Ʈ �簢��, �簢����ġ, �۲û�
#else
	m_pFont->DrawTextA(0, outputString, -1, &m_rect, m_dwAnchor, m_FontColor);
#endif
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFont* pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Font_Component Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CFont::Clone(void)
{
	return new CFont(*this);
}

void CFont::Free()
{
	CComponent::Free();
}
