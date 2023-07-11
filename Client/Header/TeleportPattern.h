#pragma once
#include "State.h"

class CTeleportPattern :
    public CState
{
private:
    explicit CTeleportPattern();
    explicit CTeleportPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CTeleportPattern();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //Ŭ���� �������� �� �Լ�

private:
    //Ŭ���� �������� �� �Լ�
    _float m_fDelay;
    _int    m_iSkillCount;

public:
    static CTeleportPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};



