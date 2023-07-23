#include "Jump_Plate.h"
#include "Export_Function.h"
#include "Player.h"

CJump_Plate::CJump_Plate(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CJump_Plate::CJump_Plate(const CJump_Plate& rhs)
	:Engine::CGameObject(rhs)
{
}

CJump_Plate::~CJump_Plate()
{
}

HRESULT CJump_Plate::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::MONSTER;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(0.4f, 0.15f, 0.4f));
	m_fTime = 0.f;
	m_bAttack = false;
	m_fInitialHeight = false;
	return S_OK;
}

_int CJump_Plate::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	if (SceneManager()->Get_GameStop()) { return 0; }
	_uint iExit = __super::Update_Object(fTimeDelta);
	m_fTime += fTimeDelta;
	if (m_bAttack)
		Trap_On();
	return iExit;
}

void CJump_Plate::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }
	__super::LateUpdate_Object();
}

void CJump_Plate::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture();
	m_pCubeBf->Render_Buffer();
}

void CJump_Plate::Trap_On()
{
	//����Ʈ �������� �߰��� ����
}

void CJump_Plate::OnCollisionEnter(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
	if (m_bAttack) { return; }
	m_pOtherObj = _pOther->GetHost();
	if (OBJECTTAG::PLAYER == m_pOtherObj->Get_ObjectTag())
	{
		_vec3 JumpDir = m_pOtherObj->m_pTransform->m_vInfo[INFO_LOOK] * 20.f;//(dynamic_cast<CPlayer*>(m_pOtherObj)->Get_Speed());
		(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_RigidBody()->Add_Force(_vec3(JumpDir.x, 1.1f * 15.f, JumpDir.z)));
		(dynamic_cast<CPlayer*>(_pOther->GetHost())->Get_RigidBody()->UseGravity(true));
		(dynamic_cast<CPlayer*>(_pOther->GetHost())->Set_JumpState(true));
	}
}

void CJump_Plate::OnCollisionStay(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }

}

void CJump_Plate::OnCollisionExit(CCollider* _pOther)
{
	if (SceneManager()->Get_GameStop()) { return; }
}

HRESULT CJump_Plate::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_PlateTrapBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CJump_Plate* CJump_Plate::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJump_Plate* pInstance = new CJump_Plate(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Jump_Plate_Body Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CJump_Plate::Free()
{
	__super::Free();
}