#include "stdafx.h"
#include "..\Header\MainApp.h"

#include "Engine_Define.h"
#include "Export_Function.h"
#include "Logo.h"
#include "Stage.h"
#include "Village.h"
#include "BossStage.h"
#include "GameManager.h"

float g_fSound = 1.f;

CMainApp::CMainApp() : m_pDeviceClass(nullptr), m_pSceneManager(nullptr), m_pSoundManager(nullptr)
{
	
}

CMainApp::~CMainApp()
{
	
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(CSoundManager::GetInstance()->ReadySound(&m_pSoundManager), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pSceneManager), E_FAIL);

	return S_OK;
}

int CMainApp::Update_MainApp(const float & fTimeDelta)
{
	Engine::InputDev()->Update_InputDev();

	NULL_CHECK_RETURN(m_pSceneManager, -1);
	m_pSceneManager->Update_Scene(fTimeDelta);

	// �������� �̵� �׽�Ʈ
	if (Engine::InputDev()->Key_Pressing(DIK_LSHIFT) && Engine::InputDev()->Key_Down(DIK_F6))
	{
		CScene* pScene = CBossStage::Create(m_pGraphicDev);
		Engine::SceneManager()->Change_Scene(pScene);
		CGameManager::GetInstance()->PlayMode(PD::ShowBoss);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F6))
	{
		CScene* pScene = CVillage::Create(m_pGraphicDev);
		Engine::SceneManager()->Change_Scene(pScene);
		CGameManager::GetInstance()->PlayMode(PD::ShowSewer);
	}
	 if (Engine::InputDev()->Key_Down(DIK_F7))
	 {
		 CScene* pScene = CBossStage::Create(m_pGraphicDev);
		 Engine::SceneManager()->Change_Scene(pScene);
	 }
	return 0;
}

void CMainApp::LateUpdate_MainApp()
{
	NULL_CHECK(m_pSceneManager);
	m_pSceneManager->LateUpdate_Scene();
}

void CMainApp::Render_MainApp()
{
	++m_iFps;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFps);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFps = 0;
		m_dwTime = GetTickCount();
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
if (!m_pSceneManager->Is_TickPassed())
	{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));
	 
	m_pSceneManager->Render_Scene(m_pGraphicDev);
	
	
		Engine::Render_End();
	}
	Engine::EventManager()->Update();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, 1280, 720, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	// �ؽ�ó ������ȭ �����ڵ�
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	(*ppGraphicDev)->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	FAILED_CHECK_RETURN(Engine::InputDev()->Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CSceneManager ** ppSceneManager)
{
	Engine::CScene*		pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_SceneManager(pGraphicDev, ppSceneManager), E_FAIL);
	(*ppSceneManager)->AddRef();

	FAILED_CHECK_RETURN((*ppSceneManager)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp *	pInstance = new	CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance;
		pInstance = nullptr;

		return pInstance;
	}
	
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pSceneManager);
		
	Engine::Release_Utility();
	Engine::Release_System();
}
