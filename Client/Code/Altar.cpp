#include "..\Header\Altar.h"
#include "Export_Function.h"
#include "Orb.h"
#include "Player.h"
#include "SkyBoxVillage.h"
#include "TempEffect.h"
#include "EffectFallingleaves.h"
#include "EffectFirefly.h"
#include "EffectTwinkle.h"
#include "FlyingCamera.h"
#include "CameraManager.h"
#include "SoundManager.h"
#include <Orb.h>
#include <OrbBlockLight.h>

CAltar::CAltar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCubeBlock(pGraphicDev)
{
}

CAltar::CAltar(const CAltar& rhs)
	: CCubeBlock(rhs)
{
}

CAltar::~CAltar()
{
	
}

HRESULT CAltar::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::BLOCK;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Scale(_vec3(1.f, 0.3f, 1.f));
	//m_pTransform->Translate(_vec3(0.f, 4.f, -60.f));
	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT],
		m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(0.f, 3.f, -10.f));

	return S_OK;
}

_int CAltar::Update_Object(const _float& fTimeDelta)
{
	Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);

	_uint iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CAltar::LateUpdate_Object()
{
	__super::LateUpdate_Object();
}

void CAltar::Render_Object()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

	m_pTexture->Render_Texture(m_iCurrentImage);
	m_pCubeBf->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif // _DEBUG
}

void CAltar::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM) {
		if (dynamic_cast<CItem*>(_pOther->Get_Host())->Get_ItemTag().eItemID == ITEMID::QUEST_ORB)
		{
			_vec3 vPos = m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, 1.f, 0.f);
			_pOther->Get_Host()->m_pTransform->m_vInfo[INFO_POS] = vPos;

			dynamic_cast<COrb*>(_pOther->Get_Host())->Set_Altar(true);

			CPlayer& rPlayer = *SceneManager()->Get_Scene()->Get_MainPlayer();
			rPlayer.Set_Orb(true);

			if (m_bOrbCollision)
				return;

			// ��� ���� ����
			CSoundManager::GetInstance()->StopAll();
			CSoundManager::GetInstance()->PlayBGM(L"win.mp3", 0.5f);

			// ���ʰ��� ī�޶� ����ŷ
			CFlyingCamera* pCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
			if (pCamera != nullptr) {
				pCamera->Set_ShakeForce(0.f, 0.05, 4.f, 5.f);
				pCamera->Shake_Camera();
			}

            // ���꿡 ��¦�̴� ����Ʈ ����
			CGameObject* pGameObject = nullptr;
			for (_uint i = 0; i < 3; ++i) {
				pGameObject = CEffectTwinkle::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + 1.f, m_pTransform->m_vInfo[INFO_POS].z);
				dynamic_cast<CEffectTwinkle*>(pGameObject)->Set_Distance(.1f);
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
			
			// ���� ���� ��� ����
			dynamic_cast<COrb*>(_pOther->Get_Host())->Set_CurrentImage(43);
			// ���� ���� ���� ��� ����
			m_iCurrentImage = 52;

			// ���� �ֺ� �� ���� ��� ���� : �� �̹��� ���
			CGameObject* pAltarLight = COrbBlockLight::Create(m_pGraphicDev);
			pAltarLight->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + 0.0001f, m_pTransform->m_vInfo[INFO_POS].z);
			Engine::EventManager()->CreateObject(pAltarLight, LAYERTAG::GAMELOGIC);

			// UI �� �̹��� ��� �� �Ҹ�
			// 


			// ���� ��ü �ּҸ� �Ѱܼ� nullptr != �� ���� ��ǥ�� ����Ʈ �� ��ǥ�� �ٲ㼭 �ش� ��ġ�� ��� �� �ִϸ��̼� ���/ ��� �� ���� �� �Ҹ�


			// ���� �� �̹��� ��ü
			vector<CGameObject*> pEffectBlock = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BLOCK);
			for (auto& iter : pEffectBlock) {
				if (dynamic_cast<CCubeBlock*>(iter)->Get_TextureNumber() == 53) {
					dynamic_cast<CCubeBlock*>(iter)->Set_TextureNumber(51);
				}
				else if (iter->m_pTransform->m_vInfo[INFO_POS].y > 50.f) {
					dynamic_cast<CCubeBlock*>(iter)->Set_TextureNumber(15);
				}
			}

            // ��ī�� �ڽ� ���� �̹��� ��ü
			CGameObject* pSkyObject = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::SKYBOX).front();
			if (pSkyObject != nullptr)
				dynamic_cast<CSkyBoxVillage*>(pSkyObject)->Set_SkyMode(1);

			// �Ȱ� ���� ��ü
			Engine::Renderer()->Set_FogColor(100, 255, 170, 150);
			Engine::Renderer()->Set_FogDistance(1.f, 200.0f);

			vector<CGameObject*> pEffectList = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::EFFECT);
			for (auto& iter : pEffectList) {
				// ������ ����Ʈ -> �� �̹����� ��ü
				if (dynamic_cast<CTempEffect*>(iter)->Get_EffectTag() == EFFECTTAG::EFFECT_LEAVES) {
					dynamic_cast<CEffectFallingleaves*>(iter)->Set_ChangeMode(true);
				}
				// �ݵ����� ����Ʈ -> ���� �̹����� ��ü
				else if (dynamic_cast<CTempEffect*>(iter)->Get_EffectTag() == EFFECTTAG::EFFECT_FIREFLY) {
					dynamic_cast<CEffectFirefly*>(iter)->Set_ChangeMode(true);
				}
			}

			// �ش� �� Ŭ������·� ����
			// ���ʸ��� �ϴÿ� ���� ����Ʈ �߻�

			// Ŭ���� �� ���� �� ���� ũ���� ������ ��ȯ

			m_bOrbCollision = true;
		}
	}
}

void CAltar::OnCollisionStay(CCollider* _pOther)
{
}

void CAltar::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CAltar::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCubeBf = dynamic_cast<CCubeBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTexture = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Cube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);


	for (_uint i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

CAltar* CAltar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAltar* pInstance = new CAltar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release<CAltar*>(pInstance);

		MSG_BOX("Create Altar FAILED");
		return nullptr;
	}

	return pInstance;
}

void CAltar::Free()
{
	__super::Free();
}
