#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CStateMachine;

class ENGINE_DLL CState : public CBase
{
protected:
	explicit CState();
	explicit CState(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CState();

public:
	virtual HRESULT		Ready_State(CStateMachine* pOwner)		PURE;

	// ������ Ÿ���� ��ȯ��.
	virtual STATE		Update_State(const _float& fTimeDelta)	PURE;
	virtual	void		LateUpdate_State()						PURE;
	virtual void		Render_State()							PURE;


public:
	virtual	STATE		Key_Input(const _float& fTimeDelta)		PURE;
	
public:
	virtual STATE		Get_State() { return m_eState; }

protected:
	// � ���¸ӽ��� �ڽ��� ���ϰ� �ִ��� �˷���
	CStateMachine* m_pOwner = nullptr;
	// � �������� ������
	STATE		   m_eState = STATE::STATE_END;

	// ������Ʈ ���ο��� ���������� ����� ����
	_float		   m_fChase;
	_vec3		   m_vRandomPos;

	
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	

public:
	//static	CState* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	//virtual CComponent* Clone(void);

protected:
	virtual void			Free();
};

END