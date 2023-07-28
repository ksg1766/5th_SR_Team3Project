#include "Export_Utility.h"

#include "../../../Client/Header/UIitem.h"

IMPLEMENT_SINGLETON(CUIManager)

CUIManager::CUIManager()
{
	for (_uint i = 0; i < 5; ++i) {
		m_bSlotBasicCollider[i] = false;
	}

	for (_uint i = 0; i < 6; ++i) {
		m_bSlotEquipmentCollider[i] = false;
	}

	for (_uint i = 0; i < 18; ++i) {
		UIID_SLOTEMPTYCollider[i] = false;
	}
}

CUIManager::~CUIManager()
{
	Free();
}

void CUIManager::Show_PopupUI(UIPOPUPLAYER _PopupID)
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto& iter : m_mapPpopupUI[_PopupID][i])
		{
			iter->Set_Dead(false);
		}
	}
}

void CUIManager::Hide_PopupUI(UIPOPUPLAYER _PopupID)
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto& iter : m_mapPpopupUI[_PopupID][i])
		{
			iter->Set_Dead(true);
		}
	}
}

void CUIManager::Delete_BasicObject(UILAYER eType)
{
	for (auto& obj : m_vecUIbasic[eType]){
		m_vecDead.push_back(obj);
	}
	m_vecUIbasic[eType].clear();
}

void CUIManager::Delete_FindItemUI(ITEMTYPEID _itemId)
{
#pragma region �κ��丮 UI ����
	for (auto iter = m_mapPpopupUI[POPUP_ITEM][UI_DOWN].begin(); iter != m_mapPpopupUI[POPUP_ITEM][UI_DOWN].end();)
	{
		if (*iter != nullptr)
		{
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(*iter)->Get_ItemTag();

			if (SlotItemType.eItemID == _itemId.eItemID)
			{
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CUIitem*>(*iter)->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					(*iter)->Set_Dead(true);
					dynamic_cast<CTempUI*>(dynamic_cast<CUIitem*>(*iter)->Get_Parent())->Set_EmptyBool(true);
					m_vecItemDead.push_back(*iter);
					iter = m_mapPpopupUI[POPUP_ITEM][UI_DOWN].erase(iter);
					return;
				}
			}
			else
				++iter;
		}
		else
			++iter;
	}
#pragma endregion �κ��丮 UI ���� 
}

void CUIManager::Show_InvenItem(_uint iType)
{
	UIOBJECTTTAG _UIObjID;
	_uint _UINumber;

	for (auto& iter : m_mapPpopupUI[POPUP_ITEM][UI_DOWN]) {
		if (iter != nullptr) {

			CGameObject* pSlotObj = dynamic_cast<CTempUI*>(iter)->Get_Parent();
			dynamic_cast<CTempUI*>(pSlotObj)->Get_UIObjID(_UIObjID, _UINumber);

			if (iType == 0)
			{
				if (_UIObjID != UIID_SLOTBASIC)
				{
					iter->Set_Dead(false);
				}
			}
			else if (iType == 1)
			{
				if (_UIObjID != UIID_SLOTBASIC && _UIObjID != UIID_SLOTEQUIPMENT)
				{
					iter->Set_Dead(false);
				}
			}

		}
	}
}

void CUIManager::Hide_InvenItem(_uint iType)
{
	UIOBJECTTTAG _UIObjID;
	_uint        _UINumber;

	for (auto& iter : m_mapPpopupUI[POPUP_ITEM][UI_DOWN]) {
		if (iter != nullptr) {

			CGameObject* pSlotObj = dynamic_cast<CTempUI*>(iter)->Get_Parent();
			dynamic_cast<CTempUI*>(pSlotObj)->Get_UIObjID(_UIObjID, _UINumber);

			if (iType == 0)	{
				if (_UIObjID != UIID_SLOTBASIC) {
					iter->Set_Dead(true);
				}
			}
			else if (iType == 1) {
				if (_UIObjID != UIID_SLOTBASIC && _UIObjID != UIID_SLOTEQUIPMENT) {
					iter->Set_Dead(true);
				}
			}
		}
	}
}

void CUIManager::Add_BasicGameobject(UILAYER eType, CGameObject* pGameObject)
{
	if (UILAYER::UI_END <= eType || nullptr == pGameObject)
		return;

	m_vecUIbasic[eType].push_back(pGameObject);
	//pGameObject->AddRef();
}

CGameObject* CUIManager::Get_PopupObjectBasicSlot(ITEMTYPEID ItemType)
{
	for (auto iter : m_vecUIbasic[UI_DOWN])
	{
		if (iter != nullptr)
		{
			CGameObject* pChildObj = dynamic_cast<CTempUI*>(iter)->Get_Child();
			
			if (pChildObj != nullptr)
			{
				ITEMTYPEID ItemId = dynamic_cast<CUIitem*>(pChildObj)->Get_ItemTag();

				if (ItemId.eItemID == ItemType.eItemID)
				{
					return iter;
				}
			}
		}
	}

	for (auto iter : m_mapPpopupUI[POPUP_INVEN][UI_DOWN])
	{
		if (iter != nullptr)
		{
			CGameObject* pChilidObj = dynamic_cast<CTempUI*>(iter)->Get_Child();

			if (pChilidObj != nullptr)
			{
				ITEMTYPEID Itemid = dynamic_cast<CUIitem*>(pChilidObj)->Get_ItemTag();

				if (Itemid.eItemID == ItemType.eItemID)
					return iter;
			}
		}
	}


	return nullptr;
}

void CUIManager::Add_PopupGameobject(UIPOPUPLAYER ePopupLayer, UILAYER eType, CGameObject* pGameObject)
{
	if (UIPOPUPLAYER::POPUP_END <= ePopupLayer || UILAYER::UI_END <= eType || nullptr == pGameObject)
		return;

	m_mapPpopupUI[ePopupLayer][eType].push_back(pGameObject);
}

void CUIManager::Add_ItemGameobject(CGameObject* pGameObject)
{
	// ���� ������ ������Ʈ ������ Ÿ�� �� ���̵�, ������ ������.
	ITEMTYPEID ItemType = dynamic_cast<CUIitem*>(pGameObject)->Get_ItemTag();

	// �̹� �����ϰ� �ִ� ���������� �˻��Ѵ�.
	// �⺻ ���� 5�� + ���� ���Կ� �����ϰ� �ִ� ���������� ���� �˻�
	for (auto iter : m_mapPpopupUI[POPUP_ITEM][UI_DOWN]){
		if (iter != nullptr){
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// ���� �������� ������ �� �ش� ������ŭ ī��Ʈ ���� �� ���� ������ ����
				dynamic_cast<CUIitem*>(iter)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}
	
	// �� ������ �ִ��� �˻��� �Ҵ� �� ����
	for (auto iter : m_vecUIbasic[UI_DOWN])
	{
		if (dynamic_cast<CTempUI*>(iter)->Get_EmptyBool())
		{
			// ���� ������ ui �������� ����ִ� �ش� ���� ������ ������ ���� �� ���� ��� ����
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = iter->m_pTransform->m_vInfo[INFO_POS].x;
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = iter->m_pTransform->m_vInfo[INFO_POS].y;
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);

			// �������� �θ� ������Ʈ�� �ش� ���� ���
			dynamic_cast<CTempUI*>(pGameObject)->Set_Parent(iter);
			// ���� �ڽ� ������Ʈ�� �ش� ������ ��� �� ������� �ʴٴ� ���·� ����
			dynamic_cast<CTempUI*>(iter)->Set_Child(pGameObject);
			dynamic_cast<CTempUI*>(iter)->Set_EmptyBool(false);

			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ITEM, Engine::UILAYER::UI_DOWN, pGameObject);
			return;
		}
	}

	for (auto iter : m_mapPpopupUI[POPUP_INVEN][UI_DOWN])
	{
		if (dynamic_cast<CTempUI*>(iter)->Get_EmptyBool())
		{
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = iter->m_pTransform->m_vInfo[INFO_POS].x;
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = iter->m_pTransform->m_vInfo[INFO_POS].y;
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);

			dynamic_cast<CTempUI*>(pGameObject)->Set_Parent(iter);
			dynamic_cast<CTempUI*>(iter)->Set_Child(pGameObject);
			dynamic_cast<CTempUI*>(iter)->Set_EmptyBool(false);

			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ITEM, Engine::UILAYER::UI_DOWN, pGameObject);
			Hide_InvenItem(0); // ���� ������ ����
			return;
		}
	}
}

void CUIManager::ReplayAdd_ItemGameobject(CGameObject* pGameObject)
{
	// �� ������ �ִ��� �˻��� �Ҵ� �� ����
	for (auto iter : m_vecUIbasic[UI_DOWN])
	{
		if (dynamic_cast<CTempUI*>(iter)->Get_EmptyBool())
		{
			// ���� ������ ui �������� ����ִ� �ش� ���� ������ ������ ���� �� ���� ��� ����
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = iter->m_pTransform->m_vInfo[INFO_POS].x;
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = iter->m_pTransform->m_vInfo[INFO_POS].y;
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);

			// �������� �θ� ������Ʈ�� �ش� ���� ���
			dynamic_cast<CTempUI*>(pGameObject)->Set_Parent(iter);
			// ���� �ڽ� ������Ʈ�� �ش� ������ ��� �� ������� �ʴٴ� ���·� ����
			dynamic_cast<CTempUI*>(iter)->Set_Child(pGameObject);
			dynamic_cast<CTempUI*>(iter)->Set_EmptyBool(false);

			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ITEM, Engine::UILAYER::UI_DOWN, pGameObject);
			return;
		}
	}

	for (auto iter : m_mapPpopupUI[POPUP_INVEN][UI_DOWN])
	{
		if (dynamic_cast<CTempUI*>(iter)->Get_EmptyBool())
		{
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = iter->m_pTransform->m_vInfo[INFO_POS].x;
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = iter->m_pTransform->m_vInfo[INFO_POS].y;
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);

			dynamic_cast<CTempUI*>(pGameObject)->Set_Parent(iter);
			dynamic_cast<CTempUI*>(iter)->Set_Child(pGameObject);
			dynamic_cast<CTempUI*>(iter)->Set_EmptyBool(false);

			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ITEM, Engine::UILAYER::UI_DOWN, pGameObject);
			//Hide_InvenItem(0); // ���� ������ ����
			return;
		}
	}
}

CGameObject* CUIManager::Get_ItemUI(ITEMID _eItemID)
{
	for (auto iter : m_mapPpopupUI[POPUP_ITEM][UI_DOWN]) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CUIitem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == _eItemID){
				return iter;
			}
		}
	}

	return nullptr;
}

void CUIManager::Set_ColliderSlot(UIOBJECTTTAG _SlotTag, _uint _UINumber, _bool _Setbool)
{
	switch (_SlotTag)
	{
	case Engine::UIID_SLOTBASIC:
		m_bSlotBasicCollider[_UINumber] = _Setbool;
		break;
	case Engine::UIID_SLOTEMPTY:
		UIID_SLOTEMPTYCollider[_UINumber] = _Setbool;
		break;
	case Engine::UIID_SLOTEQUIPMENT:
		m_bSlotEquipmentCollider[_UINumber] = _Setbool;
		break;
	}
}

CGameObject* CUIManager::Find_ColliderSlot()
{
	UIOBJECTTTAG _UIObjID;
	_uint        _UINumber;

	for (_uint i = 0; i < 5; ++i) {
		if (m_bSlotBasicCollider[i] == true)
		{
			for (auto iter : m_vecUIbasic[UI_DOWN])
			{
				dynamic_cast<CTempUI*>(iter)->Get_UIObjID(_UIObjID, _UINumber);
				if (i == _UINumber)
				{
					return iter;
				}
			}
		}
	}

	for (_uint i = 0; i < 6; ++i) {
		if (m_bSlotEquipmentCollider[i] == true)
		{
			for (auto iter : m_mapPpopupUI[POPUP_EQUIPMENT][UI_DOWN])
			{
				dynamic_cast<CTempUI*>(iter)->Get_UIObjID(_UIObjID, _UINumber);
				if (i == _UINumber)
				{
					return iter;
				}
			}
		}
	}

	for (_uint i = 0; i < 18; ++i) {
		if (UIID_SLOTEMPTYCollider[i] == true)
		{
			for (auto iter : m_mapPpopupUI[POPUP_INVEN][UI_DOWN])
			{
				dynamic_cast<CTempUI*>(iter)->Get_UIObjID(_UIObjID, _UINumber);
				if (i == _UINumber)
				{
					return iter;
				}
			}
		}
	}

	return nullptr;
}

_int CUIManager::Update_UI(const _float& fTimeDelta)
{
    //for_each(m_vecDead.begin(), m_vecDead.end(), CDeleteObj());
	//m_vecDead.clear();

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
		{
			if(iter != nullptr)
				iter->Update_Object(fTimeDelta);
		}
	}

	for (auto& Mapiter : m_mapPpopupUI)
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
				if (iter != nullptr)
					iter->Update_Object(fTimeDelta);
		}
	}

	return _int();
}

void CUIManager::LateUpdate_UI()
{
	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
			if (iter != nullptr && !iter->IsDead())
				iter->LateUpdate_Object();
	}

	for (auto& Mapiter : m_mapPpopupUI)
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
				if (iter != nullptr && !iter->IsDead())
					iter->LateUpdate_Object();
		}
	}
}

void CUIManager::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9 m_BackupViewPort;
	pGraphicDev->GetViewport(&m_BackupViewPort); // ���� �� ��Ʈ ���

	// UI�� ���ο� �� ��Ʈ ���� �� ����
	D3DVIEWPORT9 UiViewPort;
	UiViewPort.X = 0;
	UiViewPort.Y = 0;
	UiViewPort.Width = WINCX;
	UiViewPort.Height = WINCY;
	UiViewPort.MinZ = 0;
	UiViewPort.MaxZ = 0;
	pGraphicDev->SetViewport(&UiViewPort);

	_matrix matView;
	pGraphicDev->SetTransform(D3DTS_VIEW, D3DXMatrixIdentity(&matView)); // �׵���ķ� ����� �� ��� �ʱ�ȭ.

	_matrix	m_matProj;
	D3DXMatrixOrthoOffCenterLH(&m_matProj, 0, WINCX, 0, WINCY, -1.f, 1.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);   // �������� ��� ����.

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);    // Z���� OFF

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // ���ķ����� ON
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
			if (iter != nullptr && !iter->IsDead())
				iter->Render_Object();
	}

	for (auto& Mapiter : m_mapPpopupUI)
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
			{
				if (iter != nullptr && !iter->IsDead())
				{
					if (Mapiter.first == UIPOPUPLAYER::POPUP_ITEM && i == UILAYER::UI_DOWN && m_pPickingObject != nullptr && iter == m_pPickingObject) { continue; }
					iter->Render_Object();
				}
			}	
		}
	}

	if (m_pPickingObject != nullptr)
		m_pPickingObject->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // ���ķ����� OFF

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      // Z���� ON


	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);    // UI ��ü ��� �� �ٽ� �������� ��� ����.

	pGraphicDev->SetViewport(&m_BackupViewPort);                // UI ��ü ��� �� ����ص� ���� ����Ʈ�� �ǵ���.

	for_each(m_vecDead.begin(), m_vecDead.end(), CDeleteObj());
	m_vecDead.clear();
}

void CUIManager::Free()
{
	for_each(m_vecItemDead.begin(), m_vecItemDead.end(), CDeleteObj());
	m_vecItemDead.clear();

	for (size_t i = 0; i < UILAYER::UI_END; ++i)
	{
		for (auto iter : m_vecUIbasic[i])
			if (iter != nullptr)
				Safe_Release(iter);
	}

	for (auto& Mapiter : m_mapPpopupUI) 
	{
		for (size_t i = 0; i < UILAYER::UI_END; ++i)
		{
			for (auto iter : Mapiter.second[i])
				if (iter != nullptr)
					Safe_Release(iter);

			Mapiter.second[i].clear();
		}
	}
}