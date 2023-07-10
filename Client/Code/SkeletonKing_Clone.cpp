#include "stdafx.h"
#include "..\Header\SkeletonKing_Clone.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "BossProjectile.h"
#include "BossExplosion.h"

CSkeletonKing_Clone::CSkeletonKing_Clone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bSkill(false)
{

}

CSkeletonKing_Clone::CSkeletonKing_Clone(const CSkeletonKing_Clone& rhs)
	: Engine::CGameObject(rhs), m_bSkill(rhs.m_bSkill)
{
}

CSkeletonKing_Clone::~CSkeletonKing_Clone()
{
	Free();
}

HRESULT CSkeletonKing_Clone::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::BOSS;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->Scale(_vec3(1.f, 3.f, 1.f));
	return S_OK;
}

_int CSkeletonKing_Clone::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this);

	if (SceneManager()->Get_GameStop()) { return 0; }

	_int iExit = __super::Update_Object(fTimeDelta);
	
	m_fFrame += 8.f * fTimeDelta;
	m_fSkillCool += fTimeDelta;
	if (8.f < m_fFrame)
		m_fFrame = 0.f;

	if (!m_bSkill)
		Clone_Pattern();
	/*else if (m_bSkill)
		Engine::EventManager()->DeleteObject(this);*/

	ForceHeight(m_pTransform->m_vInfo[INFO_POS]);

	return iExit;
}

void CSkeletonKing_Clone::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	m_pBillBoard->LateUpdate_Component();
	__super::LateUpdate_Object();
}

void CSkeletonKing_Clone::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTexture->Render_Texture((_uint)m_fFrame);

	m_pBuffer->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSkeletonKing_Clone::ForceHeight(_vec3 _vPos)
{
	_float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
	_float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

	x /= (_float)VTXITV;
	z /= (_float)VTXITV;

	_int col = ::floorf(x);
	_int row = ::floorf(z);

	_vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
	_vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
	_vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
	_vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

	_float dx = x - col;
	_float dz = z - row;

	_float height;

	if (dz < 1.0f - dx)
	{

		_vec3 uy = B - A;
		_vec3 vy = C - A;

		height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height;
	}
	else
	{
		_vec3 uy = C - D;
		_vec3 vy = B - D;

		height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
		m_pTransform->m_vInfo[INFO_POS].y = height+2.f;
	}
}

HRESULT CSkeletonKing_Clone::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Boss"));
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

//�׽�Ʈ �� �ڵ��Դϴ�.
void CSkeletonKing_Clone::Clone_Pattern()
{
	if (3.f < m_fSkillCool)
	{
		m_fSkillCool = 0.f;
		Engine::CGameObject* pGameObject = nullptr;
		srand(unsigned(time(NULL)));
		int _iTemp = rand() % 2;
		switch (_iTemp)
		{
		case 0:
			m_bSkill = true;
			pGameObject = CBossProjectile::Create(m_pGraphicDev);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossProjectile*>(pGameObject)->Set_Terrain(m_pTerrain);
			dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pTransform->m_vInfo[INFO_POS]);
			Engine::EventManager()->DeleteObject(this);
			break;
		case 1:
			m_bSkill = true;
			pGameObject = CBossExplosion::Create(m_pGraphicDev);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pTransform->m_vInfo[INFO_POS]);
			dynamic_cast<CBossExplosion*>(pGameObject)->Set_StartPosY(-2.f);
			Engine::EventManager()->DeleteObject(this);
			break;
		}
	}
}
//�׽�Ʈ �� �ڵ��Դϴ�.


CSkeletonKing_Clone* CSkeletonKing_Clone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkeletonKing_Clone* pInstance = new CSkeletonKing_Clone(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SkeletonKing Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSkeletonKing_Clone::Free()
{
	__super::Free();
}
