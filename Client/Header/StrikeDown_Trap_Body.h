#pragma once
#include "Trap.h"

BEGIN(Engine)

class CCubeBf;
class CTexture;
class CStateMachine;

END

class CStrikeDown_Trap_Blade;

class CStrikeDown_Trap :
	public CTrap
{
private:
	explicit	CStrikeDown_Trap(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit	CStrikeDown_Trap(const CStrikeDown_Trap& rhs);
	virtual		~CStrikeDown_Trap();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

	void		Ground_Pounding(const _float& fTimeDelta);

	void		Set_InitailHeight(_float _Height);
	void		Set_MinHeight(_float _Height);

public:
	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	CCubeBf*		m_pCubeBf = nullptr;
	CTexture*		m_pTexture = nullptr;
	CGameObject*	m_pOtherObj = nullptr;
	_float			m_fInitialHeight;
	_float			m_fMinHeight;
	_float			m_fDelayTime;

	_bool			m_bAttack;//�浹���� 
	_bool			m_bMinHeight;//�������� Ȯ�ο�
	_bool			m_bUp;		//�ö󰡱� ��������
	_bool			m_bMaxHeight;//�ְ���� Ȯ�ο�
	_bool			m_bPlayerHit;//�÷��̾�  �¾��� ��.

private:
	HRESULT		Add_Component(void);

public:
	static CStrikeDown_Trap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

