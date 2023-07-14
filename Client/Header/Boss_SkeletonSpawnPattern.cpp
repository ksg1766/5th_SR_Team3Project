#include "Boss_SkeletonSpawnPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "Boss_Skeleton.h"

CBoss_SkeletonSpawnPattern::CBoss_SkeletonSpawnPattern()
{
}

CBoss_SkeletonSpawnPattern::CBoss_SkeletonSpawnPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_SkeletonSpawnPattern::~CBoss_SkeletonSpawnPattern()
{
}

HRESULT CBoss_SkeletonSpawnPattern::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_bSkill = false;
    m_fSkillCool = 0.f;
    m_vSpawnPos[0] = _vec3(8.f, 0.f, 0.f);
    m_vSpawnPos[1] = _vec3(-8.f, 0.f, 8.f);
    m_vSpawnPos[2] = _vec3(0.f, 0.f, -8.f);
    m_vSpawnPos[3] = _vec3(0.f, 0.f, 8.f);
    return S_OK;
}

STATE CBoss_SkeletonSpawnPattern::Update_State(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
    m_fSkillCool += fTimeDelta;
    if ((1.5f < m_fSkillCool) && (!m_bSkill))
    {
        Spawn_Skeleton();
        m_bSkill = true;
    }
    if (3.f < m_fSkillCool)
    {
        m_bSkill = false;
        m_fSkillCool = 0.f;
        return STATE::BOSS_TELEPORT;
    }
}

void CBoss_SkeletonSpawnPattern::LateUpdate_State()
{
}

void CBoss_SkeletonSpawnPattern::Render_State()
{
}

void CBoss_SkeletonSpawnPattern::Spawn_Skeleton()
{
    Engine::CGameObject* pGameObject = nullptr;
 
        pGameObject = CBoss_Skeleton::Create(m_pGraphicDev);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        dynamic_cast<CBoss_Skeleton*>(pGameObject)->Set_Terrain(dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Terrain());
        dynamic_cast<CBoss_Skeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] =
            (Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front()->m_pTransform->m_vInfo[INFO_POS]);
        dynamic_cast<CBoss_Skeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS].y = -2.f;
        dynamic_cast<CBoss_Skeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS].y += 2.f;
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}

CBoss_SkeletonSpawnPattern* CBoss_SkeletonSpawnPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_SkeletonSpawnPattern* pState = new CBoss_SkeletonSpawnPattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_SkeletonSpawnPattern*>(pState);

        MSG_BOX("Boss SpawnPattern State Failed");
    }
    return pState;
}

void CBoss_SkeletonSpawnPattern::Free()
{
}
