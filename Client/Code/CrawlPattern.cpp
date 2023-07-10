#include "CrawlPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"


CCrawlPattern::CCrawlPattern()
{
}

CCrawlPattern::CCrawlPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CCrawlPattern::~CCrawlPattern()
{
}

HRESULT CCrawlPattern::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fSpeed = 2.f;
    m_iSkillCount = 0;
	return S_OK;
}

STATE CCrawlPattern::Update_State(const _float& fTimeDelta)
{
    m_iSkillCount += fTimeDelta;
    m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
    m_vDir = m_vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

    dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->m_pTransform->Translate(m_vDir * fTimeDelta * m_fSpeed);
	return STATE::BOSS_CRAWL;
}

void CCrawlPattern::LateUpdate_State()
{
  
}

void CCrawlPattern::Render_State()
{
}

void CCrawlPattern::Dash(const _float& fTimeDelta)
{
    if (5.f < m_iSkillCount)
    {
        m_iSkillCount = 0.f;
        m_fSpeed = 2.f;
    }
    else if (3.f < m_iSkillCount)
        m_fSpeed = 100.f;
    dynamic_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->m_pTransform->Translate(m_vDir * fTimeDelta);
}

CCrawlPattern* CCrawlPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CCrawlPattern* pState = new CCrawlPattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CCrawlPattern*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CCrawlPattern::Free()
{
}