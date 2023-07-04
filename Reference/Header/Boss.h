#pragma once
#include "Monster.h"

BEGIN(Engine)

class ENGINE_DLL CBoss :
    public CMonster
{
protected:
    explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBoss(const CBoss& rhs);
    virtual ~CBoss();

public:
    HRESULT     Ready_Object();
    _int        Update_Object(const _float& fTimeDelta);

protected:
    // BOSS_ID m_eBossID;    // �ʿ��ϴٸ� �߰�
    // MonsterAI Component �߰��ؼ� ���
    // 
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