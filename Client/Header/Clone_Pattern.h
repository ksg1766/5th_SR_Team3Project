#pragma once
#include "State.h"
class CClone_Pattern :
    public CState
{
private:
    explicit CClone_Pattern();
    explicit CClone_Pattern(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CClone_Pattern();


public:
    virtual HRESULT	Ready_State(CStateMachine* pOwner);
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
    _vec3   m_vPillarPos[3];
public:
    static CClone_Pattern* Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner);

private:
    virtual void Free();
};

