#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CMonster
	:CGameObject
{
protected:
    explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMonster(const CMonster& rhs);
    virtual ~CMonster();

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

protected:
    // MONSTER_ID m_eMonserID;
    // MonsterAI Component �߰��ؼ� ���

// CGameObject�κ��� ���
    //map<COMPONENTTAG, CComponent*>	m_mapComponent[ID_END];
    //LPDIRECT3DDEVICE9					m_pGraphicDev;
    //_bool								m_IsDead;

    //OBJECTTAG							m_eObjectTag;
    //CTransform*                       m_pTransform;	// Transform Attached to this GameObject

protected:
    virtual void	Free(void);
};

END