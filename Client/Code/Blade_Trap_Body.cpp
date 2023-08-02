#include "Blade_Trap_Body.h"
#include "Export_Function.h"
#include "Blade_Trap_Blade.h"
#include "Player.h"

CBlade_Trap::CBlade_Trap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CTrap(pGraphicDev)
{
}

CBlade_Trap::CBlade_Trap(const CBlade_Trap& rhs)
	:Engine::CTrap(rhs)
{
}

CBlade_Trap::~CBlade_Trap()
{
}

HRESULT CBlade_Trap::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::TRAP;
	m_eTrapTag = TRAPTAG::BLADE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 0.2f, 1.f));

	m_vBladePos[0] = _vec3(0.6f,	0.f,	0.6f);//�������
	m_vBladePos[1] = _vec3(0.6f,	0.f,	-0.6f);//�����ϴ�
	m_vBladePos[2] = _vec3(-0.6f,	0.f,	-0.6f);//�����ϴ�
	m_vBladePos[3] = _vec3(-0.6f,	0.f,	0.6f);//�������
	m_vBladePos[4] = _vec3(0.7f,	0.f,	0.f);//������
	m_vBladePos[5] = _vec3(-0.7f,	0.f,	0.f);//����
	m_vBladePos[6] = _vec3(0.f,		0.f,	0.f);//���
	m_vBladePos[7] = _vec3(0.f,		0.f,	0.8f);//��
	m_vBladePos[8] = _vec3(0.f,		0.f,	-0.8f);//�Ʒ�

	m_vBladeDir[0] = _vec3(0.f, -10.f, 0.f);//�������
	m_vBladeDir[1] = _vec3(0.f, 10.f, 0.f);//�����ϴ�
	m_vBladeDir[2] = _vec3(0.f, -10.f, 0.f);//�����ϴ�
	m_vBladeDir[3] = _vec3(0.f, 10.f, 0.f);//�������
	m_vBladeDir[4] = _vec3(0.f, 30.f, 0.f);//������
	m_vBladeDir[5] = _vec3(0.f, 30.f, 0.f);//����
	m_vBladeDir[6] = _vec3(0.f, 30.f, 0.f);//���

	m_bSpawnBlade = false;

	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		__super::Ready_Object();

	//m_pTransform->Translate(_vec3(0.f, -0.8f, 0.f));

	return S_OK;
}

_int CBlade_Trap::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = 0;
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
		return iExit;

	if (SceneManager()->Get_GameStop()) { return 0; }
	iExit = __super::Update_Object(fTimeDelta);
	
	CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();

	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS];
	_float fDistance = D3DXVec3Length(&vDir);


	if (fDistance > 20.f)
		return iExit;

	for (_uint i = 0; i < 9; ++i)
		static_cast<CBlade_Trap_Blade*>(m_vecTrapBlade[i])->Update_Object(fTimeDelta);


	return iExit;
}

void CBlade_Trap::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CBlade_Trap::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();

	for (int i = 0; i < 9; ++i)
		m_vecTrapBlade[i]->Render_Object();
}

void CBlade_Trap::Create_Blade()
{
	Engine::CGameObject* pGameObject = nullptr;
	for (int i = 0; i < 9; ++i)
	{
		pGameObject = CBlade_Trap_Blade::Create(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS]);
		dynamic_cast<CBlade_Trap_Blade*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_vInfo[INFO_POS];
		dynamic_cast<CBlade_Trap_Blade*>(pGameObject)->m_pTransform->Translate(m_vBladePos[i] - _vec3(0.f, 1.f, 0.f));
		if (i == 6)
			dynamic_cast<CBlade_Trap_Blade*>(pGameObject)->Set_Collider();
		if (i < 7)
			dynamic_cast<CBlade_Trap_Blade*>(pGameObject)->m_pTransform->Rotate(m_vBladeDir[i]);
		m_vecTrapBlade.push_back(dynamic_cast<CBlade_Trap_Blade*>(pGameObject));
	}
}

void CBlade_Trap::OnCollisionEnter(CCollider* _pOther)
{
	CGameObject* pOtherObj = _pOther->Get_Host();
	if (OBJECTTAG::PLAYER == pOtherObj->Get_ObjectTag())
	{

		if (OBJECTTAG::PLAYER == pOtherObj->Get_ObjectTag())
		{
			CPlayerStat& PlayerState = *static_cast<CPlayer*>(pOtherObj)->Get_Stat();
			PlayerState.Take_Damage(4.f);
		}
	}
}

void CBlade_Trap::OnCollisionStay(CCollider* _pOther)
{
}

void CBlade_Trap::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CBlade_Trap::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BladeTrapBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBlade_Trap* CBlade_Trap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlade_Trap* pInstance = new CBlade_Trap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Blade_Trap_Body Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBlade_Trap::Free()
{
	__super::Free();
}
