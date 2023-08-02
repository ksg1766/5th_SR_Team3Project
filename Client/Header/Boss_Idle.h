#pragma once
#include "State.h"
class CBoss_Idle :
    public CState
{
private:
    explicit CBoss_Idle();
    explicit CBoss_Idle(LPDIRECT3DDEVICE9 gGraphicDev);
    virtual ~CBoss_Idle();

public:
    virtual HRESULT	    Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	    Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //Ŭ���� �������� �� �Լ�
    //STATE 
private:
    //����
    _float  m_fSkillCool;
public:
    static CBoss_Idle* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();

};

