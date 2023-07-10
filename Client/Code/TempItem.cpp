#include "stdafx.h"
#include "..\Header\Player.h"

#include "..\Header\TempItem.h"
#include "Export_Function.h"

CTempItem::CTempItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CItem(pGraphicDev)
{
}

CTempItem::CTempItem(const CTempItem& rhs)
	: Engine::CItem(rhs)
{
}

CTempItem::~CTempItem()
{
}

HRESULT CTempItem::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::ITEM;
	m_eItemTag = ITEMTAG::WEAPON;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(0.3f, 0.3f, 0.3f));
	m_pCollider->
		InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], 2 * m_pTransform->LocalScale());

	m_pBasicStat->Get_Stat()->fAttack = 1.f;
	m_pBasicStat->Get_Stat()->fHealth = 3.f;

	CGameObject* pGamePlayer = SceneManager()->GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front();
	m_pTransform->Translate(pGamePlayer->m_pTransform->m_vInfo[INFO_POS] + *dynamic_cast<CPlayer*>(pGamePlayer)->Get_Offset());

	m_AttackInfo.fWeaponSpeed = 0.08f;
	m_AttackInfo.fReturnSpeed = 0.08f;
	m_AttackInfo.fMaxDistance = 0.8f;

	return S_OK;
}

_int CTempItem::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (pPlayer.Get_Attack())
	{
		if (!m_AttackInfo.bReturn)
		{
		
			m_pTransform->Translate(Get_RotationDir(pPlayer, fTimeDelta) * m_AttackInfo.fReturnSpeed);


		}

		else
		{
			Get_ReturnTrans(pPlayer, fTimeDelta);
		}

		_vec3 vWeaponToPlayer = pPlayer.m_pTransform->m_vInfo[INFO_POS] + *pPlayer.Get_Offset() - m_pTransform->m_vInfo[INFO_POS];
		_float fDistanceToPlayer = D3DXVec3Length(&vWeaponToPlayer);

		if (fDistanceToPlayer >= m_AttackInfo.fMaxDistance) 
			// ������ ������ ������ �ִ� ��Ÿ��� �´ٸ� �ٽ� ���ư�����.
		{
			// ���Ⱑ �÷��̾� ��ġ�� ���ƿ����� ����
			m_AttackInfo.bReturn = true;
		}
	}


	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CTempItem::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}


void CTempItem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture();
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTempItem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);
	
	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	m_pTransform->Set_Parent(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform);
	m_pTransform->Copy_RUL(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo);


	return S_OK;
}

void CTempItem::OnCollisionEnter(CCollider* _pOther)
{
	if (!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER) && 
		!(_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::PLAYER))
		__super::OnCollisionEnter(_pOther);
	// ���Ͱų� �÷��̾�� �о���ʴ´�.

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
	// �÷��̾��� ������ ���۷����� ����.

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER)
		// ���� �ݸ����� ���� Ÿ���� �����̸鼭, �÷��̾��� ������Ʈ�� �����̶��
	{
		if (!pPlayer.Get_AttackTick())
			// ���� ���� ���� ���¶��.
		{
			_pOther->Get_Host()->Get_BasicStat()->Take_Damage(1.f);
			// ������ ���ݿ� ������ 1�� �ش�.
			pPlayer.Set_AttackTick(true);
			// �׸��� �ٽ� ƽ�� true�� �ؼ� ���ȴٴ� ���� �˷���.

			cout << "������" << endl;
		}
	}



}

void CTempItem::OnCollisionStay(CCollider* _pOther)
{
}

void CTempItem::OnCollisionExit(CCollider* _pOther)
{
}

_vec3 CTempItem::Get_RotationDir(const CPlayer& pPlayer, const _float& fTimeDelta)
{
	m_AttackInfo.vFoward = pPlayer.m_pTransform->m_vInfo[INFO_LOOK];
	// ������ ���� ���͸� �����Ѵ�. -> �÷��̾ �ٶ󺸰� �ִ� ����

	m_AttackInfo.vRotation = D3DXVECTOR3(0.0f, atan2(m_AttackInfo.vFoward.x, m_AttackInfo.vFoward.z), 0.0f);
	// ���� ȸ���Կ� ���� ȸ�� ���� ����ؾ��ϴµ� 
	// Ʈ�������� �ޱ۰��� �ҷ� �� �� �����Ƿ�, �÷��̾ �ٶ󺸴� ���� ���ͷ� ���ﰢ�Լ��� �̿��ؼ� ������ ����
	// ��ȯ �� Radian�̰� atan2�� ����ؼ� x-z ������ ����

	D3DXMatrixRotationYawPitchRoll(&m_AttackInfo.matRot, m_AttackInfo.vRotation.y, m_AttackInfo.vRotation.x, m_AttackInfo.vRotation.z);
	// ��Ʈ������ y�� ȸ������ ���� Rotation���� ���� �ٶ󺸴� ���⿡�� y,x,z���� ȸ���� �����ϰ� 
	// ȸ����Ŀ� ����Ѵ�.

	_vec3 vFowardDir = _vec3(0.0f, 0.0f, 1.0f); 
	// ������ �̵� ����

	D3DXVec3TransformNormal(&vFowardDir, &vFowardDir, &m_AttackInfo.matRot);
	// �� ���溤�� ���� y�� ȸ���� �������� �� ��Ʈ������ ���溤���� �������� �ٲ�

	D3DXVec3Normalize(&vFowardDir, &vFowardDir);
	// ��ֶ������ؼ� �������ͷ� �� ��

	return vFowardDir; 
}

void CTempItem::Get_ReturnTrans(CPlayer& pPlayer, const _float& fTimeDelta)
{
	_vec3 vWeaponToPlayer = (pPlayer.m_pTransform->m_vInfo[INFO_POS] + *pPlayer.Get_Offset()) - m_pTransform->m_vInfo[INFO_POS];
	// ���� �������� ��� ������ ��ġ���� �� ��ġ�� ���� ���⺤�Ͱ� ����.

	// ���Ⱑ �÷��̾� ��ġ�� ������ ���
	// ���⺤�ͺ��� ���ƿ��� �ӵ��� ũ�ٸ� 
	if (D3DXVec3Length(&vWeaponToPlayer) <= m_AttackInfo.fReturnSpeed)
	{
	
		m_AttackInfo.bReturn = false;
		m_pTransform->m_vInfo[INFO_POS] = pPlayer.m_pTransform->m_vInfo[INFO_POS] + *pPlayer.Get_Offset();
		// ���Ⱑ ���ƿ� ���¸� ǥ���ϰ�, ��ġ�� �÷��̾� ��ġ�� ����

		//m_pTransform->Rotate(_vec3(0.0f, -interpolatedAngle * fTimeDelta, 0.f));
		pPlayer.Set_Attack(false);
		// ���ƿ����Ƿ� ���ݻ��¸� false�� ���� �ٽ� ������ �� �ְ� �����.
	}
	else
	{
		// ���⸦ �÷��̾� ��ġ�� �̵�
		D3DXVec3Normalize(&vWeaponToPlayer, &vWeaponToPlayer);
		// ����ȭ ���Ѽ� �������ͷ� �����.
		m_pTransform->Translate(vWeaponToPlayer * m_AttackInfo.fReturnSpeed);

		// �߰� ������ ȸ���� ����

		//m_pTransform->Rotate(_vec3(0.0f, -interpolatedAngle * fTimeDelta, 0.f));
	}

	
}

CTempItem* CTempItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTempItem* pInstance = new CTempItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TempItem Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTempItem::Free()
{
	__super::Free();
}
