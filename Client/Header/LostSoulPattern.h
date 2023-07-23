#pragma once
#include "State.h"
class CLostSoulPattern :
    public CState
{
    explicit CLostSoulPattern();
    explicit CLostSoulPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLostSoulPattern();


public:
    virtual HRESULT	    Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }
public:
    static CLostSoulPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);
private:
    //Ŭ���� �������� �� �Լ�
private:
    //Ŭ���� �������� �� �Լ�
    _float m_fDelay;
    _float  m_fPatternDelay;

private:
    virtual void Free();
};

