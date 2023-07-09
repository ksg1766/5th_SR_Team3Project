#include "stdafx.h"
#include "CubeBlock.h"

#include "Export_Function.h"

CCubeBlock::CCubeBlock(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CCubeBlock::CCubeBlock(const CCubeBlock& rhs)
    : CGameObject(rhs)
{
}

CCubeBlock::~CCubeBlock()
{
}

HRESULT CCubeBlock::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(2.f, 2.f, 2.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(20.f, 2.f, 20.f));
	
	return S_OK;
}

_int CCubeBlock::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	
	return iExit;
}

void CCubeBlock::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CCubeBlock::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	
	m_pTexture->Render_Texture(1);
	m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
}

void CCubeBlock::OnCollisionEnter(CCollider* _pOther)
{
}

void CCubeBlock::OnCollisionStay(CCollider* _pOther)
{
}

void CCubeBlock::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CCubeBlock::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	/*pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);*/

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CCubeBlock* CCubeBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeBlock* pInstance = new CCubeBlock(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CubeBlock Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CCubeBlock::Free()
{
	__super::Free();
}
