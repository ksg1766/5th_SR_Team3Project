#include "stdafx.h"
#include "..\Header\BossStage.h"
#include "Export_Function.h"
#include "PoolManager.h"
#include "Monstergroup.h"
#include "Itemgroup.h"
#include "UIUseShop_Trander.h"
#include "UIShop.h"
#include "SpawningPool.h"
#include "Blade_Trap_Body.h"
#include "StrikeDown_Trap_Body.h"
#include "Plate_Trap_Body.h"
#include "SoundManager.h"
#include "Jump_Plate.h"
#include "BlackIn.h"
#include "Lava.h"
#include "GameManager.h"

CBossStage::CBossStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CBossStage::~CBossStage()
{
}

HRESULT CBossStage::Ready_Scene()
{
	Engine::CGameObject* pGameObject = CBlackIn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 0);

	Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_UP, pGameObject);

	m_eSceneTag = SCENETAG::BOSSSTAGE;
	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	CSoundManager::GetInstance()->StopAll();
	CSoundManager::GetInstance()->PlayBGM(L"DK-7.mp3", 0.5f);
	return S_OK;
}

Engine::_int CBossStage::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	CGameManager::GetInstance()->Update_Game(fTimeDelta);

	UIManager()->Update_UI(fTimeDelta);
	if (10.f >= Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS].y)
	{
		switch (dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
		{
		case BOSSPHASE::PHASE1:
			Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS] = _vec3(100.f, 22.f, 0.f);
			dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Set_State(STATE::BOSS_SLEEP);
			break;
		case BOSSPHASE::PHASE2:
			Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS] = _vec3(-72.5f, 36.f, 75.5f);
			dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Set_State(STATE::BOSS_SLEEP);
			break;
		case BOSSPHASE::PHASE3:
			Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS] = _vec3(-72.f, 36.f, -75.f);
			dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Set_State(STATE::BOSS_SLEEP);
			break;
		case BOSSPHASE::LASTPHASE:
			Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS] = _vec3(100.f, 22.f, 0.f);
			dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Set_State(STATE::BOSS_SLEEP);
			break;
		}
	}
	return 0;
}

void CBossStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	CollisionManager()->LateUpdate_Collision();
	UIManager()->LateUpdate_UI();
}

void CBossStage::Render_Scene()
{
	// DEBUG �� ����
}

void CBossStage::Free()
{
	CPoolManager::DestroyInstance();

	__super::Free();
}

CBossStage* CBossStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossStage*	pInstance = new CBossStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CBossStage Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CBossStage::Ready_Prototype()
{

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

#pragma region ù ��° ������ �޾ƿ��� ������Ʈ
	// DynamicCamera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
	//										&_vec3(0.f, 0.f, 0.f),
	//										&_vec3(0.f, 0.f, 1.f),
	//										&_vec3(0.f, 1.f, 0.f),
	//										D3DXToRadian(90.f), 
	//										(_float)WINCX / WINCY,
	//										0.1f, 
	//										1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
#pragma endregion ù ��° ������ �޾ƿ��� ������Ʈ

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_GameLogic(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	CPoolManager::GetInstance()->Ready_Pool();

	Engine::CGameObject*		pGameObject = nullptr;

	// Boss
	pGameObject = CSkeletonKing::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-80.f, 35.f, 0.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CDimensionGate::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(95.f, 22.f, 0.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	return S_OK;
}

HRESULT CBossStage::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

#pragma region ù ��° ������ �޾ƿ��� ������Ʈ
	// �⺻ �κ��丮 5ĭ
	for (_uint i = 0; i < 5; ++i)
	{
		pGameObject = CUIbasicslot::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 520.f + (60.f * i);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 30.f;
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTBASIC, i);
		dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
		Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_DOWN, pGameObject);
	}

	_uint Index = 0;
	// ��񽽷�
	for (_uint iy = 0; iy < 3; ++iy)
	{
		for (_uint ix = 0; ix < 2; ++ix)
		{

			Index = iy * 2 + ix;
			pGameObject = CUIequipmentslot::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 350.f + (60.f * ix);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 135.f - (60.f * iy);
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTEQUIPMENT, Index);
			dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
		}
	}

	// ���� �κ��丮
	for (_uint iy = 0; iy < 3; ++iy)
	{
		for (_uint ix = 0; ix < 6; ++ix)
		{

			pGameObject = CUIemptyslot::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 490.f + (60.f * ix);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 135.f - (60.f * iy);
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTEMPTY, iy * 6 + ix);
			dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_INVEN, Engine::UILAYER::UI_DOWN, pGameObject);
		}
	}

	// �κ��丮 �� ���� ��ư
	for (_uint iy = 0; iy < 2; ++iy)
	{
		pGameObject = CUIbutton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 850.f;
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = 595.f - (40 * iy);
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_INVENBUTTON, iy);
		if (iy == 0)
		{
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIImage(1);
		}
		else
		{
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIImage(3);
		}

		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
	}

	pGameObject = CUIplayerstat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_STAT, Engine::UILAYER::UI_DOWN, pGameObject);

	// ������
	HCURSOR Cursor = nullptr;
	SetCursor(Cursor);
	pGameObject = CUIaimpoint::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_MOUSE, Engine::UILAYER::UI_DOWN, pGameObject);

	// �÷��̾� hp bar
	pGameObject = CUIplayerhp::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_DOWN, pGameObject);

	// ū ����
	pGameObject = CUIbigmap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_MAP, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc ���
	pGameObject = CUIEscBackground::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Resum ��ư
	pGameObject = CUIResumeButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Option ��ư
	pGameObject = CUIOptionButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Quition ��ư
	pGameObject = CUIQuitButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Shop
	pGameObject = CUIShop::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SHOP, Engine::UILAYER::UI_DOWN, pGameObject);


	// Speech Bubble Test
	pGameObject = CUIspeech_OldMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

	pGameObject = CUIUseShop_Trander::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_MAP);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_ESC);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SPEECH);	// Speech Bubble Test
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SHOP);	// Speech Bubble Test
#pragma endregion ù ��° ������ �޾ƿ��� ������Ʈ
	
	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CBossStage::Load_Data()
{
	CLayer* pLayer = m_mapLayer[LAYERTAG::GAMELOGIC];
	for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
	{
		// �ϴ� ��ϸ�
		if (OBJECTTAG::BLOCK != (OBJECTTAG)i)
			continue;

		vector<CGameObject*>& refObjectList = pLayer->Get_ObjectList((OBJECTTAG)i);
		for_each(refObjectList.begin(), refObjectList.end(), [&](CGameObject* pObj) { EventManager()->DeleteObject(pObj); });
		refObjectList.clear();
	}
	//HANDLE hFile = CreateFile(L"../Bin/Data/Sewer_TrapTest.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/Sewer.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(L"../Bin/Data/TempData.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/BossStage.dat", GENERIC_READ,	0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/BossStage_3rd.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	OBJECTTAG eTag = OBJECTTAG::OBJECT_END;

	DWORD		dwByte = 0;
	_float		fX, fY, fZ;
	_ubyte		byTextureNumber = 0;

	MONSTERTAG	eSpawnerTag = MONSTERTAG::MONSTER_END;  //
	_int		iPoolCapacity = 5;
	_float		fSpawnRadius = 10.0f;
	_float		fSpawnTime = 10.0f;

	while (true)
	{
		// key �� ����
		ReadFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		// if�� �߰�
		if (OBJECTTAG::BLOCK == eTag)
		{
			// value�� ����
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			ReadFile(hFile, &byTextureNumber, sizeof(_ubyte), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = nullptr;

			if (40 == byTextureNumber)
			{
				pGameObject = CLava::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				dynamic_cast<CLava*>(pGameObject)->Set_TextureNumber(0);
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
				//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
			else
			{
				pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(byTextureNumber);
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
				//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
		}
		else if (OBJECTTAG::SPAWNINGPOOL == eTag)
		{
			// value�� ����
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			ReadFile(hFile, &eSpawnerTag, sizeof(MONSTERTAG), &dwByte, nullptr);
			ReadFile(hFile, &iPoolCapacity, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &fSpawnRadius, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fSpawnTime, sizeof(_float), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = CSpawningPool::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(eSpawnerTag);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(iPoolCapacity);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(fSpawnRadius);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(fSpawnTime);
			pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		else if (OBJECTTAG::MONSTER == eTag)
		{
			// value�� ����
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			TRAPTAG eTrapTag;
			ReadFile(hFile, &eTrapTag, sizeof(TRAPTAG), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = nullptr;

			switch (eTrapTag)
			{
			case TRAPTAG::BLADE:
				pGameObject = CBlade_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;

			case TRAPTAG::STRIKEDOWN:
				pGameObject = CStrikeDown_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_InitailHeight(15.f);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;

			case TRAPTAG::JUMP:
				pGameObject = CJump_Plate::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;
			}

			pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(fX, fY + 1.f, fZ);
			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		else if (OBJECTTAG::TRAP == eTag)
		{
			// value�� ����
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			TRAPTAG eTrapTag;
			ReadFile(hFile, &eTrapTag, sizeof(TRAPTAG), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = nullptr;

			switch (eTrapTag)
			{
			case TRAPTAG::BLADE:
				pGameObject = CBlade_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;

			case TRAPTAG::STRIKEDOWN:
				pGameObject = CStrikeDown_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_InitailHeight(10.f);
				dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_MinHeight(10.f);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;

			case TRAPTAG::JUMP:
				pGameObject = CJump_Plate::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;
			}

			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
	}
	CloseHandle(hFile);
	return S_OK;
}
