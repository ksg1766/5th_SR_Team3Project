#include "Export_Utility.h"
#include "../Client/Header/Player.h"

IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager() : m_pBeforScene(nullptr), m_pCurrentScene(nullptr), m_pNextScene(nullptr), m_bGameStop(false), m_bSceneChange(false)
{
	for (_uint i = 0; (SCENETAG)i < SCENETAG::SCENE_END; ++i) {
		m_bStageVisit[i] = false;
	}
}

CSceneManager::~CSceneManager()
{
	Free();
}

CLayer * CSceneManager::Get_Layer(LAYERTAG eLayerTag)
{
	if (nullptr == m_pCurrentScene)
		return nullptr;

	return m_pCurrentScene->Get_Layer(eLayerTag);
}

HRESULT CSceneManager::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pCurrentScene);
	Renderer()->Clear_RenderGroup();

	m_pCurrentScene = pScene;

	return S_OK;
}

HRESULT CSceneManager::Change_Scene(CScene* pScene)
{
	m_bSceneChange = true;
		
	m_pBeforScene = m_pCurrentScene;
	m_pNextScene  = pScene;

	// ���� ���� �湮 ���θ� ���
	m_bStageVisit[(unsigned long long)pScene->Get_SceneTag() - 1] = true;

 	return S_OK;
}

_int CSceneManager::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	if (m_bSceneChange) {
		Octree()->Ready_Octree();
		m_bSceneChange = false;
	}
	
	Octree()->Update_Octree();

	return m_pCurrentScene->Update_Scene(fTimeDelta);
}

void CSceneManager::LateUpdate_Scene()
{
	if (nullptr == m_pCurrentScene)
		return;

	m_pCurrentScene->LateUpdate_Scene();
}

void CSceneManager::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
#ifdef _DEBUG
	//if(SCENETAG::STAGE == m_pScene->Get_SceneTag())	Octree()->Render_Octree(pGraphicDev);
#endif

	if (SCENETAG::VILLAGE == m_pCurrentScene->Get_SceneTag() || SCENETAG::STAGE == m_pCurrentScene->Get_SceneTag() || SCENETAG::BOSSSTAGE == m_pCurrentScene->Get_SceneTag())
		Octree()->Render_Octree(pGraphicDev);

	Renderer()->Render_GameObject(pGraphicDev);

	if (m_pCurrentScene)
		m_pCurrentScene->Render_Scene();	

	if (m_bSceneChange)
	{
		CLayer* pEnvironmentObj = Get_Layer(LAYERTAG::ENVIRONMENT);
		CLayer* pGameLogic      = Get_Layer(LAYERTAG::GAMELOGIC);

		if (pEnvironmentObj != nullptr && pGameLogic != nullptr)
		{
			CGameObject* m_pPlayer         = pGameLogic->Get_ObjectList(OBJECTTAG::PLAYER).front();
			CGameObject* m_pCamera         = pEnvironmentObj->Get_ObjectList(OBJECTTAG::CAMERA).front();
			CGameObject* m_pPlayerRay      = pGameLogic->Get_ObjectList(OBJECTTAG::RAY).front();
			vector<CGameObject*> m_vecItem = pGameLogic->Get_ObjectList(OBJECTTAG::ITEM);

			Safe_Release(m_pBeforScene);
			Renderer()->Clear_RenderGroup();
			m_pCurrentScene = m_pNextScene;

			switch (m_pCurrentScene->Get_SceneTag())
			{
			case SCENETAG::VILLAGE:
				m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 0.f, -70.f);
				break;
			case SCENETAG::STAGE:
				m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 11.f, 0.f);  // ���� ��ġ �������ּ���
				break;
			case SCENETAG::BOSSSTAGE:
				m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(100.f, 20.f, 0.f); // ���� ��ġ �������ּ���
				break;
			}

			m_pCurrentScene->Set_MainPlayer(dynamic_cast<CPlayer*>(m_pPlayer));
			Engine::EventManager()->CreateObject(m_pCamera,    LAYERTAG::ENVIRONMENT);
			Engine::EventManager()->CreateObject(m_pPlayer,    LAYERTAG::GAMELOGIC);
			Engine::EventManager()->CreateObject(m_pPlayerRay, LAYERTAG::GAMELOGIC);
			for (auto& iter : m_vecItem)
			{
				Engine::EventManager()->CreateObject(iter, LAYERTAG::GAMELOGIC);
			}

			Octree()->DestroyInstance();
			Load_Data();
		}
	}
}

HRESULT CSceneManager::Load_Data()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Load_Data();

	return S_OK;
}

void CSceneManager::Free()
{
	Safe_Release(m_pCurrentScene);
}
