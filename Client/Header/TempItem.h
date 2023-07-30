#pragma once

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBasicStat;
class CBillBoard;
class CRigidBody;

END

class CTempItem :
    public Engine::CItem
{
private:
	explicit CTempItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempItem(const CTempItem& rhs);
	virtual ~CTempItem();

public:
	virtual HRESULT Ready_Object(_bool _Item);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

public:
private:
	CRcTex*		m_pBuffer = nullptr;
	CTexture*	m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;
	CRigidBody* m_pRigidBody = nullptr;

	_int		m_iAttackTick;
	_int		m_iMoveTick;
	_float		m_fChase;

	// ���� ���
	MYATTACKINFO m_AttackInfo;

public:
	static CTempItem* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

