#include "stdafx.h"
#include "..\Header\UIitem.h"
#include "UIequipmentslot.h"
#include "Player.h"
#include <UIbasicslot.h>
#include <UIemptyslot.h>

CUIitem::CUIitem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}


CUIitem::~CUIitem()
{

}

HRESULT CUIitem::Ready_Object()
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // �ʱ�ȭ

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vLocalScale.x = 23.f;
	m_pTransform->m_vLocalScale.y = 23.f;

	return S_OK;
}

_int CUIitem::Update_Object(const _float & fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIitem::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();

	Update_NumverUI();
}

void CUIitem::Render_Object()
{
	if(m_bUIDelete)
		Engine::UIManager()->Delete_FindItemUI(m_ItemID);

	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();

	// ���� ���� ���
	if (m_ItemID.iCount > 1 && !m_bMove)
	{
		_matrix      matWorld;

		if (m_iCurrentOneNum != 0)
		{
			D3DXMatrixIdentity(&matWorld);
			matWorld._11 = 4.2f;
			matWorld._22 = 5.1f;
			matWorld._41 = m_matWorld._41 + 8.f;
			matWorld._42 = m_matWorld._42 + 15.f;

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

			m_pNumberTextureCom->Render_Texture(m_iCurrentOneNum);
			m_pBufferCom->Render_Buffer();
		}

		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 4.2f;
		matWorld._22 = 5.1f;
		matWorld._41 = m_matWorld._41 + 17.f;
		matWorld._42 = m_matWorld._42 + 15.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pNumberTextureCom->Render_Texture(m_iCurrentTwoNum);
		m_pBufferCom->Render_Buffer();
	}

	if (m_bTooltipRender)
	{
		_matrix      matWorld;

	
		D3DXMatrixIdentity(&matWorld);
		matWorld._11 = 44.f;
		matWorld._22 = 24.f;
		matWorld._41 = m_fTooltipPosX + 41.f;
		matWorld._42 = m_fTooltipPosY + 21.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

		m_pTooltipCom->Render_Texture(m_fCurrentImage);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUIitem::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_itemUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	pComponent = m_pNumberTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_NumberUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	pComponent = m_pTooltipCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_tooltipUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE1, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CUIitem::Key_Input(void)
{
	// ���콺�� UI�� �����̴� ���̸� �����̴� UI�� �ƴ� �� ����
	if (Engine::UIManager()->m_bMouse && !m_bOnlyMove)
		return;

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// ���콺�� �ش� UI�� �����̰� �ִٸ� ���콺 ��ǥ ������ ����
	if (m_bOnlyMove) 
	{
		m_pTransform->m_vInfo[INFO_POS].x = pt.x;
		m_pTransform->m_vInfo[INFO_POS].y = WINCY - pt.y;
		WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	}

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y)) {
		if (Engine::InputDev()->Mouse_Pressing(DIM_LB)) {
			// ���콺�� UI�� �����̴� ���°� �ƴ� �� 
			if (!Engine::UIManager()->m_bMouse) {
				m_bMove = true;
				m_bOnlyMove = true;
				m_bTooltipRender = false;
				Engine::UIManager()->m_bMouse = true;
				Engine::UIManager()->Set_PickingItemUI(this);

				// ������ ���� ������ Ÿ���� ��� �ش� UINumber ������ ã�� �ش� ���� �̹��� ����
				if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM) {
					CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
					dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(true);
				}
			}
		}
		else {
			if (m_bMove) {
				m_bMove = false;

				// �̵��� ���� �ּҸ� ������ �Ҵ�
				CGameObject* ColliderSlotObj = Engine::UIManager()->Find_ColliderSlot();
				UIOBJECTTTAG UIColliderSlotObjID;
				_uint        UIColliderSlotNumber;

				CPlayer*    pPlayer    = dynamic_cast<CPlayer*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());
				CInventory* pInventory = dynamic_cast<CInventory*>(pPlayer->Get_Component(COMPONENTTAG::INVENTORY, ID_DYNAMIC));

				if (pPlayer == nullptr || pInventory == nullptr)
					return;

#pragma region ������ ������
				if (ColliderSlotObj == nullptr) { // �̵��� ������ ã�� ���� �ش� ������ ���� 
					m_bOnlyMove = false;
					m_bTooltipRender = false;
					Engine::UIManager()->m_bMouse = false;
					Engine::UIManager()->Set_PickingItemUI(nullptr);

					if(m_ItemID.iCount == 1) { // �������� �������� 1���� �� ������ �����ϰ� �ִ� �������� �� ���� ����
						if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM && pPlayer->Get_CurrentEquipRight() != nullptr && m_ItemID.eItemID == dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipRight())->Get_ItemTag().eItemID)
						{ // ���� �� ���� ������ �� 
							pPlayer->Set_PrevEquipRight(nullptr);
							pPlayer->Set_CurrentEquipRight(nullptr);
							pPlayer->Set_ItemEquipRight(false);
						}
						else if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM && pPlayer->Get_CurrentEquipLeft() != nullptr && m_ItemID.eItemID == dynamic_cast<CItem*>(pPlayer->Get_CurrentEquipLeft())->Get_ItemTag().eItemID)
						{ // �� �� ���� ������ �� 
							pPlayer->Set_PrevEquipLeft(nullptr);
							pPlayer->Set_CurrentEquipLeft(nullptr);
							pPlayer->Set_ItemEquipLeft(false);
						}

						UIOBJECTTTAG UIParentSlotObjID;
						_uint        UIParentSlotNumber;
						CGameObject* pSlotObj = Get_Parent();

						dynamic_cast<CTempUI*>(pSlotObj)->Get_UIObjID(UIParentSlotObjID, UIParentSlotNumber);
						dynamic_cast<CTempUI*>(pSlotObj)->Set_BeforeChild(dynamic_cast<CTempUI*>(Get_Parent())->Get_Child());
						dynamic_cast<CTempUI*>(pSlotObj)->Set_Child(nullptr);
						dynamic_cast<CTempUI*>(pSlotObj)->Set_EmptyBool(true);

						if (m_ItemID.eItemType != ITEMTYPE_GENERALITEM && m_ItemID.eItemType != ITEMTYPE_EQUIPITEM || UIParentSlotObjID != UIID_SLOTEQUIPMENT) {
							pInventory->delete_FindItem(m_ItemID);
						}
						else {
							dynamic_cast<CUIequipmentslot*>(pSlotObj)->Set_ThrowItem(m_ItemID.eItemID);
						}

						m_bUIDelete = true;
					}
				    else // �������� �� �Ѱ��� ����
				    {
						m_ItemID.iCount -= 1;
					   	pInventory->delete_FindItem(m_ItemID);
						m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
					}
				}
#pragma endregion ������ ������
#pragma region ������ �̵� �� ����
				else if (ColliderSlotObj != nullptr) // �̵��� ������ ã���� ��
				{
					_bool bSucess = false;
					dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_UIObjID(UIColliderSlotObjID, UIColliderSlotNumber);

					// �̵��� �� �ִ� �������� ���� �˻�
					switch (UIColliderSlotObjID)
					{
					case Engine::UIID_SLOTBASIC:
						bSucess = true;
						break;
					case Engine::UIID_SLOTEMPTY:
						bSucess = true;
						break;
					case Engine::UIID_SLOTEQUIPMENT:
						if (m_UINumber == UIColliderSlotNumber) { bSucess = true; }
						else                                    { bSucess = false; }
						break;
					}
					
					if (dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_EmptyBool() && bSucess) // �̵��� ������ ����ְ� �����ص� �� ������ ��쿡 �̵�
					{
						UIOBJECTTTAG UIStartObjID;
						_uint        UIStartNumber;
						dynamic_cast<CTempUI*>(Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(Get_Parent())->Get_Child());
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);

						// ���� ���� �� �ش� ���� �κ��丮�� ������Ʈ // ��ġ �̵� �� ���� ��ġ���� ���� �ʱ�ȭ (���� X)
						pInventory->GoSwitch_InvenItem(m_ItemID, UIStartObjID, UIStartNumber, UIColliderSlotObjID, UIColliderSlotNumber);

						m_pTransform->m_vInfo[INFO_POS].x = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

						Set_Parent(ColliderSlotObj);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

						// �����ϰ� �ִ� �������� �̵��� ��쿡 ����ϰ� �ִٴ� ���� ǥ�� �̵�
						if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM) {
							CGameObject* pRightItem = pPlayer->Get_CurrentEquipRight();
							if (pRightItem != nullptr) {

								ITEMTYPEID ItemId = dynamic_cast<CItem*>(pRightItem)->Get_ItemTag();
								if (ItemId.eItemID == m_ItemID.eItemID) {
									UIOBJECTTTAG UIFindObjID{};
									_uint        UIFindNumber;
									dynamic_cast<CTempUI*>(Get_Parent())->Get_UIObjID(UIFindObjID, UIFindNumber);

									if (UIFindObjID == UIID_SLOTBASIC)
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									else if (UIFindObjID == UIID_SLOTEMPTY)
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
								}
							}
						}
					}
					else if (!dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_EmptyBool() && bSucess) // �̵��� ������ ������� �ʰ� �����ص� �� ������ ��쿡 ���� ����Ī
					{
						// �̵��� ������ UI �� �θ� ���� �ּ�
						ITEMTYPEID   UIStartItemID = m_ItemID;
						CGameObject* UIStartParentSlot = Get_Parent();
						UIOBJECTTTAG UIStartObjID;
						_uint        UIStartNumber;
						dynamic_cast<CTempUI*>(UIStartParentSlot)->Get_UIObjID(UIStartObjID, UIStartNumber);

						// �̵��� ������ �ִ� ������ UI�� �������� ���� �ڽ����� ���
						CGameObject* pGoItemUI = dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_Child();
						ITEMTYPEID   UIGoItemID = dynamic_cast<CUIitem*>(pGoItemUI)->Get_ItemTag();

						// �̵����ϴ� �������� �ش� ���Կ� ���� �Ǵ� �� �˻�
						_bool bChangeSucess = false;
						switch (UIStartObjID)
						{
						case Engine::UIID_SLOTBASIC:
							bChangeSucess = true;
							break;
						case Engine::UIID_SLOTEMPTY:
							bChangeSucess = true;
							break;
						case Engine::UIID_SLOTEQUIPMENT:
							UIOBJECTTTAG UIEndObjID;
							_uint        UIEndNumber;
							dynamic_cast<CTempUI*>(pGoItemUI)->Get_UIObjID(UIEndObjID, UIEndNumber);
							if (UIEndNumber == UIStartNumber) { bChangeSucess = true; }
							else { bChangeSucess = false; }
							break;
						}

						if (bChangeSucess)
						{
							dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_BeforeChild(dynamic_cast<CTempUI*>(UIStartParentSlot)->Get_Child());
							dynamic_cast<CTempUI*>(ColliderSlotObj)->Set_BeforeChild(dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_Child());

							// ��ġ �̵�
							m_pTransform->m_vInfo[INFO_POS].x = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].x;
							m_pTransform->m_vInfo[INFO_POS].y = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].y;
							WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

							pGoItemUI->m_pTransform->m_vInfo[INFO_POS].x = UIStartParentSlot->m_pTransform->m_vInfo[INFO_POS].x;
							pGoItemUI->m_pTransform->m_vInfo[INFO_POS].y = UIStartParentSlot->m_pTransform->m_vInfo[INFO_POS].y;
							dynamic_cast<CUIitem*>(pGoItemUI)->WorldMatrix(pGoItemUI->m_pTransform->m_vInfo[INFO_POS].x, pGoItemUI->m_pTransform->m_vInfo[INFO_POS].y, pGoItemUI->m_pTransform->m_vLocalScale.x, pGoItemUI->m_pTransform->m_vLocalScale.y);

							// ���� �θ� �ڽ� ����
							if (UIColliderSlotObjID == UIID_SLOTEQUIPMENT) // �̵��ϴ� ���� ���� ������ �� ���� ���� ���� �� �ش� ���� ������Ʈ �ʿ�
							{
								// 2���� ���� ���� ���� ����
								// �� ���� ����

								Set_Parent(ColliderSlotObj);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_NextChild(this);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

								dynamic_cast<CUIitem*>(pGoItemUI)->Set_Parent(UIStartParentSlot);

								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(pGoItemUI);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(false);

							}
							else if (UIStartObjID == UIID_SLOTEQUIPMENT)
							{
								Set_Parent(ColliderSlotObj);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

								dynamic_cast<CUIitem*>(pGoItemUI)->Set_Parent(UIStartParentSlot);

								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_NextChild(pGoItemUI);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(false);
							}
							else
							{
								Set_Parent(ColliderSlotObj);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
								dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);

								dynamic_cast<CUIitem*>(pGoItemUI)->Set_Parent(UIStartParentSlot);

								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(true);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_Child(pGoItemUI);
								dynamic_cast<CTempUI*>(UIStartParentSlot)->Set_EmptyBool(false);
							}

							// ���� ���� �� �ش� ���� �κ��丮�� ������Ʈ
							pInventory->ExSwitch_InvenItem(UIStartItemID, UIStartObjID, UIStartNumber, UIGoItemID, UIColliderSlotObjID, UIColliderSlotNumber);

							// �����ϰ� �ִ� �������� �̵��� ��� �Ǵ� �̵� ���� ��쿡 �Һ� ����
							CGameObject* pRightItem = pPlayer->Get_CurrentEquipRight();
							if (pRightItem != nullptr) {
								ITEMTYPEID ItemId = dynamic_cast<CItem*>(pRightItem)->Get_ItemTag();

								// ���� �������� �̵��� ��� ���� ���� ���� ���� �̵��� ���� ���� Ų��.
								if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM && ItemId.eItemID == m_ItemID.eItemID)
								{
									// ���� ������ ã�� ���� ����.
									if (UIStartObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(false);
									}
									else if (UIStartObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(false);
									}

									// �̵��� ������ ã�� ���� Ų��.
									if (UIColliderSlotObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									}
									else if (UIColliderSlotObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									}
								}
								// ���� �������� �̵� ���� ��� ���� ���� ������ ���� ���� �̵����� ������ ���� Ų��.
								else if (UIGoItemID.eItemType == ITEMTYPE_WEAPONITEM && ItemId.eItemID == UIGoItemID.eItemID)
								{
									// ���� ������ ã�� ���� ����.
									if (UIColliderSlotObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(false);
									}
									else if (UIColliderSlotObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(false);
									}

									// �̵��� ������ ã�� ���� Ų��.
									if (UIStartObjID == UIID_SLOTBASIC) {
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(true);
									}
									else if (UIStartObjID == UIID_SLOTEMPTY) {
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(UIStartParentSlot))->Set_FindSlot(true);
									}
								}
							}
						}
						else // ���ڸ��� �̵�
						{
							m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
							m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
							WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
						}
						
					}
					else // �̵� ������ �� �������� �̵�
					{
						m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
					}

					m_bOnlyMove = false;
					Engine::UIManager()->m_bMouse = false;
					Engine::UIManager()->Set_PickingItemUI(nullptr);
				}

				// �ʷϺ� ���� ���� �ʱ�ȭ
				if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM) {
					CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
					dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(false);
				}
			}
		}
#pragma endregion ������ �̵� �� ����

		// �������� �ʰ� �ش� UI�� �浹 ���� �� ƫ�� ���
		if(!m_bMove) {
			m_fTooltipPosX = pt.x;
			m_fTooltipPosY = pt.y;
			m_bTooltipRender = true;
			Engine::UIManager()->Set_PickingItemUI(this);
		}
	}
	else { // �ش� UI�� �浹 ���°� �ƴ� �� ƫ�� �� ����
		m_bTooltipRender = false;
		if (Engine::UIManager()->Get_PickingItemUI() == this) {
			Engine::UIManager()->Set_PickingItemUI(nullptr);
		}
	}
}

void CUIitem::Update_NumverUI()
{
	_int iCurrentOneNum = m_ItemID.iCount / 10;
	_int iCurrentTwoNum = m_ItemID.iCount % 10;

	switch (iCurrentOneNum)
	{
	case 0:
		m_iCurrentOneNum = 0;
		break;
	case 1:
		m_iCurrentOneNum = 1;
		break;
	case 2:
		m_iCurrentOneNum = 2;
		break;
	case 3:
		m_iCurrentOneNum = 3;
		break;
	case 4:
		m_iCurrentOneNum = 4;
		break;
	case 5:
		m_iCurrentOneNum = 5;
		break;
	case 6:
		m_iCurrentOneNum = 6;
		break;
	case 7:
		m_iCurrentOneNum = 7;
		break;
	case 8:
		m_iCurrentOneNum = 8;
		break;
	case 9:
		m_iCurrentOneNum = 9;
		break;
	}

	switch (iCurrentTwoNum)
	{
	case 0:
		m_iCurrentTwoNum = 0;
		break;
	case 1:
		m_iCurrentTwoNum = 1;
		break;
	case 2:
		m_iCurrentTwoNum = 2;
		break;
	case 3:
		m_iCurrentTwoNum = 3;
		break;
	case 4:
		m_iCurrentTwoNum = 4;
		break;
	case 5:
		m_iCurrentTwoNum = 5;
		break;
	case 6:
		m_iCurrentTwoNum = 6;
		break;
	case 7:
		m_iCurrentTwoNum = 7;
		break;
	case 8:
		m_iCurrentTwoNum = 8;
		break;
	case 9:
		m_iCurrentTwoNum = 9;
		break;
	}
}

CUIitem* CUIitem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIitem*	pInstance = new CUIitem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CUIitem::Free()
{
	CTempUI::Free();
}
