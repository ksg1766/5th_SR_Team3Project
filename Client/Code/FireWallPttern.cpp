#include "FireWallPttern.h"
#include "Export_Function.h"
#include "BossFireWall.h"
#include "SkeletonKing.h"

CFireWallPttern::CFireWallPttern()
{
}

CFireWallPttern::CFireWallPttern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CFireWallPttern::~CFireWallPttern()
{
}

HRESULT CFireWallPttern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_vCenterDistance[0] = _vec3(0.f, 0.f, 3.f);
    m_vCenterDistance[1] = _vec3(0.f, 0.f, -3.f);
    m_vCenterDistance[2] = _vec3(3.f, 0.f, 0.f);
    m_vCenterDistance[3] = _vec3(-3.f, 0.f, 0.f);
    m_bCool = false;
    m_fDuration = 0.f;
	return S_OK;
}

STATE CFireWallPttern::Update_State(const _float& fTimeDelta)
{
    m_fDuration += fTimeDelta;
    Engine::CGameObject* pGameObject = nullptr;
    if (!m_bCool)
    {
        for (int i = 0; i < 5; ++i)
        {
            pGameObject = CBossFireWall::Create(m_pGraphicDev);
            dynamic_cast<CBossFireWall*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + _vec3(0.f, 0.f, 3.f)) + (m_vCenterDistance[0] * (i * 2));
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Angle(3.f);
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Speed(20.f* fTimeDelta);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        for (int i = 0; i < 5; ++i)
        {
            pGameObject = CBossFireWall::Create(m_pGraphicDev);
            dynamic_cast<CBossFireWall*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + _vec3(0.f, 0.f, -3.f)) + (m_vCenterDistance[1] * (i * 2));
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Angle(20.f);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        for (int i = 0; i < 5; ++i)
        {
            pGameObject = CBossFireWall::Create(m_pGraphicDev);
            dynamic_cast<CBossFireWall*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_pOwner->Get_Transform()->m_vInfo[INFO_POS]+_vec3(6.f, 0.f, 0.f)) + (m_vCenterDistance[2] * (i * 2));
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Angle(20.f);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        for (int i = 0; i < 5; ++i)
        {
            pGameObject = CBossFireWall::Create(m_pGraphicDev);
            dynamic_cast<CBossFireWall*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_pOwner->Get_Transform()->m_vInfo[INFO_POS]+_vec3(-6.f, 0.f, 0.f)) + (m_vCenterDistance[3] * (i * 2));
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
            dynamic_cast<CBossFireWall*>(pGameObject)->Set_Angle(20.f);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        m_bCool = true;
    }
    if (20.f < m_fDuration)
    {
        m_bCool = false;
        m_fDuration = 0.f;
    }
    else if (10.f < m_fDuration)
    {
        return STATE::BOSS_PH1SKILL5;
    }      
}

void CFireWallPttern::LateUpdate_State()
{

}

void CFireWallPttern::Render_State()
{

}

CFireWallPttern* CFireWallPttern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CFireWallPttern* pState = new CFireWallPttern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CFireWallPttern*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CFireWallPttern::Free()
{
    __super::Free();
}
