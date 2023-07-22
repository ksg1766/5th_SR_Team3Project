#pragma once
#include "State.h"
class CGrapPattern :
    public CState
{
private:
    explicit CGrapPattern();
    explicit CGrapPattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CGrapPattern();


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
    _float  m_fPatternDelay;
    _bool  m_bCool;
public:
    static CGrapPattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

