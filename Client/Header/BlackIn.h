#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
// ��ο��ٰ� ������� ���̵� ��
class CBlackIn : public CTempUI
{
private:
	explicit CBlackIn(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBlackIn();

public:
	HRESULT Ready_Object() override;
	_int Update_Object(const _float& fTimeDelta) override;
	void LateUpdate_Object(void) override;
	void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	CRcTex*			m_pBufferCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	_float          m_fFrame = 19.f;

public:
	static CBlackIn*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

