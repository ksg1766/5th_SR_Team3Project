#pragma once
#include "State.h"
class CFIreWave2 :
    public CState
{
private:
    explicit CFIreWave2();
    explicit CFIreWave2(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CFIreWave2();


public:
    virtual HRESULT	    Ready_State(CStateMachine* pOwner);
    virtual STATE		Update_State(const _float& fTimeDelta);
    virtual  void		LateUpdate_State();
    virtual  void		Render_State();

public:
    virtual STATE	Key_Input(const _float& fTimeDelta) { return STATE(); }

private:
    //Ŭ���� �������� �� �Լ�
    void    Create_Clone(_int _iNum);
private:
    //Ŭ���� �������� �� �Լ�
    _float m_fDelay;
    _float  m_fPatternDelay;
    _bool  m_bSkill;
    _bool   m_bWarning;
    _int    m_iCount;

public:
    static CFIreWave2* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

