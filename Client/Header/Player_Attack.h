#pragma once
#include "State.h"


BEGIN(Engine)

END

class CPlayer_Attack : public CState
{
private:
	explicit CPlayer_Attack();
	explicit CPlayer_Attack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer_Attack();

public:
	virtual HRESULT	Ready_State(CStateMachine* pOwner);
	virtual STATE	Update_State(const _float& fTimeDelta);
	virtual void	LateUpdate_State();
	virtual void	Render_State();

public:
	virtual STATE	Key_Input(const _float& fTimeDelta);

private:
	//_float fMaxDistance;	// �ٶ󺸴� ���������� �ִ� �Ÿ�
	//_float fMoveSpeed;		// ���� �ӵ� 
	//
	//_vec3	vStartPos;		// �÷��̾ �ٶ󺸴� ���� ����.
	//_vec3	vTargetPos;		// ���� ��ġ(Range)
	//_vec3	vCurrentPos;	// ���� ��ġ

public:
	static CPlayer_Attack* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
	virtual void Free();
};

