#include "stdafx.h"
#include "..\Header\Village.h"

#include "Export_Function.h"
#include "PoolManager.h"

#include "Monstergroup.h"
#include "Itemgroup.h"

#include "SpawningPool.h"
#include "Box_Cube.h"

#include "Blade_Trap_Body.h"
#include "StrikeDown_Trap_Body.h"
#include "Plate_Trap_Body.h"
#include "Pot.h"

#include "SkyBoxVillage.h"
#include "Moon.h"
#include "Tree.h"

CVillage::CVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CVillage::~CVillage()
{
}

HRESULT CVillage::Ready_Scene()
{
	m_eSceneTag = SCENETAG::VILLAGE;

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	return S_OK;
}

Engine::_int CVillage::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	UIManager()->Update_UI(fTimeDelta);

	return 0;
}

void CVillage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	CollisionManager()->LateUpdate_Collision();

	UIManager()->LateUpdate_UI();
}

void CVillage::Render_Scene()
{
	// DEBUG �� ����
}

HRESULT CVillage::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
											&_vec3(0.f, 0.f, 0.f),
											&_vec3(0.f, 0.f, 1.f),
											&_vec3(0.f, 1.f, 0.f),
											D3DXToRadian(90.f), 
											(_float)WINCX / WINCY,
											0.1f, 
											1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	
	// SkyBox
	pGameObject = CSkyBoxVillage::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Moon
	pGameObject = CMoon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

#pragma region TREE
	// Tree
	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(0.f, 0.f, 50.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
#pragma endregion TREE

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CVillage::Ready_Layer_GameLogic(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	CPoolManager::GetInstance()->Ready_Pool();

	Engine::CGameObject*		pGameObject = nullptr;

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(0.f, 0.f,-70.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	m_pPlayer = dynamic_cast<CPlayer*>(pGameObject);

	// FootRay
	pGameObject = CFootRay::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CFootRay*>(pGameObject)->Set_Host(m_pPlayer);
	//pGameObject->m_pTransform->Translate(m_pPlayer->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.25f, 0.f));

	pGameObject = CBox_Cube::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-30.f, 3.5f, -30.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Npc Test
	pGameObject = CNpc_OldMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-40.f, 1.f, -40.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Bard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-55.f, 1.f, -55.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	return S_OK;
}

HRESULT CVillage::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

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
		Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);
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
			Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
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
			Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_INVEN, Engine::UILAYER::UI_DOWN, pGameObject);
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

		Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
	}

	pGameObject = CUIplayerstat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_STAT, Engine::UILAYER::UI_DOWN, pGameObject);

	// ������
	HCURSOR Cursor = nullptr;
	SetCursor(Cursor);
	pGameObject = CUIaimpoint::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_MOUSE, Engine::UILAYER::UI_DOWN, pGameObject);

	// �÷��̾� hp bar
	pGameObject = CUIplayerhp::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);

	// ū ����
	pGameObject = CUIbigmap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_MAP, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc ���
	pGameObject = CUIEscBackground::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Resum ��ư
	pGameObject = CUIResumeButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Option ��ư
	pGameObject = CUIOptionButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Quition ��ư
	pGameObject = CUIQuitButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Speech Bubble Test
	pGameObject = CUIspeech_OldMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_MAP);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_ESC);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SPEECH);	// Speech Bubble Test

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CVillage::Load_Data()
{
	CLayer* pLayer = m_mapLayer[LAYERTAG::GAMELOGIC];
	for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
	{
		// �ϴ� ���ϸ�
		if (OBJECTTAG::BLOCK != (OBJECTTAG)i)
			continue;

		vector<CGameObject*>& refObjectList = pLayer->Get_ObjectList((OBJECTTAG)i);
		for_each(refObjectList.begin(), refObjectList.end(), [&](CGameObject* pObj) { EventManager()->DeleteObject(pObj); });
		refObjectList.clear();
	}
	//HANDLE hFile = CreateFile(L"../Bin/Data/Sewer.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/TempData.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(L"../Bin/Data/TerrainGiantTree10.dat", GENERIC_READ,	0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

			CGameObject* pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(byTextureNumber);
			pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
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
	}
	CloseHandle(hFile);
	return S_OK;
}

CVillage* CVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVillage* pInstance = new CVillage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CVillage::Free()
{
	CPoolManager::DestroyInstance();

	__super::Free();
}