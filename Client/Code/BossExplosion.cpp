#include "stdafx.h"
#include "..\Header\BossExplosion.h"
#include "Export_Function.h"
#include "Terrain.h"

CBossExplosion::CBossExplosion(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fFrame(0.f)
	, m_iCount(0)
{
}

CBossExplosion::CBossExplosion(const CBossExplosion& rhs)
	:	Engine::CGameObject(rhs), m_fFrame(rhs.m_fFrame)
	, m_iCount(rhs.m_iCount)
{
}

CBossExplosion::~CBossExplosion()
{
	Free();
}

HRESULT CBossExplosion::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::PROJECTTILE;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CBossExplosion::Update_Object(const _float& fTimeDelta)
{

	_int iExit = __super::Update_Object(fTimeDelta);
	if (3 <= m_iCount)
		return iExit;//Test
	m_fFrame += 16.f * fTimeDelta;

	if (16.f < m_fFrame)
	{
		m_pTransform->Translate(_vec3 (3.f, 0.f, 3.f));
		++m_iCount;
		m_fFrame = 0.f;
	}

	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CBossExplosion::LateUpdate_Object(void)
{
	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
}

void CBossExplosion::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTexture->Render_Texture((_uint)m_fFrame);
	m_pBuffer->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBossExplosion::Set_StartPos(_vec3 _vec)
{
	m_pTransform->m_vInfo[INFO_POS] = _vec;
}

void CBossExplosion::Set_StartPosY(float _fY)
{
	m_pTransform->m_vInfo[INFO_POS].y += _fY;
}

HRESULT CBossExplosion::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_BossExplosion"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CBossExplosion* CBossExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossExplosion* pInstance = new CBossExplosion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BossProjectitle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBossExplosion::Free()
{
	__super::Free();
}