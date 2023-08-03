#include "..\Header\Arrow.h"
#include "Export_Function.h"
#include "Player.h"

#include "Itemgroup.h"

#include "EffectSquare.h"
#include "EffectDamageStar.h"
#include "EffectDamage.h"
#include "EffectPastTrace.h"

static _int g_iCount = 0;

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev)
{
}

CArrow::CArrow(const CArrow& rhs)
	: CItem(rhs)
{

}

CArrow::~CArrow()
{
	Free();
}

HRESULT CArrow::Ready_Object(CTransform* Weapon, CTransform* pOwner, _float _fSpeed)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_eObjectTag = OBJECTTAG::PLAYERBULLET;
	m_eState = STATE::ATTACK;
	m_fSpeed = _fSpeed;

	//m_pTransform->Set_Parent(pOwner);

	if (Weapon != nullptr && pOwner != nullptr)
	{
		m_pTransform->Copy_RUL(pOwner->Get_Transform()->m_vInfo);
		m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
		m_pTransform->Rotate(ROT_Y, -30.f);

		m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], 0.5f * m_pTransform->LocalScale());
		m_pTransform->m_vInfo[INFO_POS] = Weapon->m_vInfo[INFO_POS];

		m_vDir = pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
	}

	BASICSTAT* pOwnerStat = {};
	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	pOwnerStat = rPlayer.Get_Stat()->Get_Stat();

	if (pOwnerStat != nullptr)
	{
		m_pBasicStat->Get_Stat()->iDamageMin = pOwnerStat->iDamageMin;
		m_pBasicStat->Get_Stat()->iDamageMax = pOwnerStat->iDamageMax;
	}

	m_bIsAttack = false;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);

	// ����ü ���� ����Ʈ �߰�
	CGameObject* pGameObject = m_pEffect = CEffectPastTrace::Create(m_pGraphicDev);
	pGameObject->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

	return S_OK;
}

_int CArrow::Update_Object(const _float& fTimeDelta)
{
	if (IsDead())
		return 0;

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);
	
	if (SceneManager()->Get_GameStop()) { return 0; }

	//m_pRigidBody->Update_RigidBody(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);		

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!m_bIsAttack)
	{
		m_vPrevPos = m_pTransform->m_vInfo[INFO_POS];
		m_bIsAttack = true;
	}

	_float fDistance = D3DXVec3Length(&(m_pTransform->m_vInfo[INFO_POS] - m_vPrevPos));

	if (fDistance > 60.f && Get_State() != STATE::DEAD)
	{
		Set_State(STATE::DEAD);
		EventManager()->DeleteObject(this);
	}


	m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS] + m_vDir * m_fSpeed * fTimeDelta;

	return iExit;
}

void CArrow::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();
}

void CArrow::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();
}

HRESULT CArrow::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CArrow::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

	// ���Ͱų� �÷��̾�� �о���ʴ´�.

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	// �÷��̾��� ������ ���۷����� ����.

	if ((_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER &&
		dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_StateMachine()->Get_State() != STATE::DEAD)|| (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BOSS))
	{
		pPlayer.IsAttack(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_BasicStat());

		dynamic_cast<CMonster*>(_pOther->Get_Host())->Set_KnockBack(true);

		//////////////////////////////////////////////////////////////////////////////// ����Ʈ 
		CGameObject* pGameObject = CEffectDamageStar::Create(m_pGraphicDev);
		dynamic_cast<CTempEffect*>(pGameObject)->Set_TargetObject(_pOther->Get_Host());
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		//CPoolManager::GetInstance()->Create_Effect(pGameObject, );
		//////////////////////////////////////////////////////////////////////////////// ����Ʈ 
		

		//////////////////////////////////////////////////////////////////////////////// ����Ʈ 
		_matrix      matMonsterWorld = _pOther->Get_Host()->m_pTransform->WorldMatrix();
		_vec3        vecMonsterPos = _vec3(matMonsterWorld._41, matMonsterWorld._42 + .5f, matMonsterWorld._43);
		pGameObject = CEffectSquare::Create(m_pGraphicDev, vecMonsterPos, 50, EFFECTCOLOR::ECOLOR_NONE);
		dynamic_cast<CEffectSquare*>(pGameObject)->Set_MonsterEffectColor(dynamic_cast<CMonster*>(_pOther->Get_Host())->Get_MonsterTag());
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		//////////////////////////////////////////////////////////////////////////////// ����Ʈ 

	}

	Set_State(STATE::DEAD);
	Engine::EventManager()->DeleteObject(this);

}

void CArrow::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

void CArrow::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

CArrow* CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* Weapon,
	CTransform* pOwner, _float _fSpeed)
{
	CArrow* pInstance = new CArrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(Weapon, pOwner, _fSpeed)))
	{
		Safe_Release<CArrow*>(pInstance);

		MSG_BOX("Create Arrow Failed");
		return nullptr;
	}

	return pInstance;
}

void CArrow::Free()
{
	Safe_Release(m_pEffect);

	__super::Free();
}
