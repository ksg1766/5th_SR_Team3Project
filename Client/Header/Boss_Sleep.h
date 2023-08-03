#pragma once
#include "State.h"

class CBoss_Sleep :
    public CState
{
private:
    explicit CBoss_Sleep();
    explicit CBoss_Sleep(LPDIRECT3DDEVICE9 pGrapicDev);
    virtual ~CBoss_Sleep();

public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //Ŭ���� �������� �� �Լ�
    //STATE 
private:
    //����
    _bool   m_bMusic;
    _bool   m_bSummon;
    _vec3   m_vSummonPos[8];
public:
    static CBoss_Sleep* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

