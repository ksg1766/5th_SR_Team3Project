#pragma once
#include "Monster.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CBillBoard;
END

class CKingSpiderWeb :
    public CMonster
{
private:
	explicit	CKingSpiderWeb(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CKingSpiderWeb(const CKingSpiderWeb& rhs);
	virtual		~CKingSpiderWeb();

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta)	override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

public:
	virtual void	Init_Stat()override;
public:

	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

	void			Set_Dir(_vec3 _vDir);
private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;

	_vec3	m_vDir;
	_vec3	m_vPlayerLook;
	_vec3	m_vPlayerRight;
	_bool	m_bPlayerHit;
	_bool	m_bHit;
	_bool	m_bDebuff;
	_bool	m_bFloor;
	_float	m_fDelay;
	_float	m_fDebuffDuration;
	_int	m_iHp;
private:
	HRESULT		Add_Component(void);

public:
	static CKingSpiderWeb* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

