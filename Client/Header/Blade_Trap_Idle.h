#pragma once
#include "State.h"

class CBlade_Trap_Idle :
    public CState
{
private:
    explicit CBlade_Trap_Idle();
    explicit CBlade_Trap_Idle(LPDIRECT3DDEVICE9 pGrapicDev);
    virtual ~CBlade_Trap_Idle();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

    void		Set_TrapCenter(_vec3 _vCenter) { m_vTrapCenter = _vCenter; }

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //Ŭ���� �������� �� �Լ�
    //STATE 
private:
    //����
    _vec3			m_vTrapCenter;

public:
    static CBlade_Trap_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

