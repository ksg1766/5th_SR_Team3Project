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
	if(m_bDelete)
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
						{// �� �� ���� ������ �� 
							pPlayer->Set_PrevEquipLeft(nullptr);
							pPlayer->Set_CurrentEquipLeft(nullptr);
							pPlayer->Set_ItemEquipLeft(false);
						}

						pInventory->delete_FindItem(m_ItemID);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);

						m_bDelete = true;
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
					
					if (dynamic_cast<CTempUI*>(ColliderSlotObj)->Get_EmptyBool()) // �̵��� ������ ������� ��
					{
						
					}
					else
					{
						bSucess = false;
					}

					if (bSucess)
					{
						UIOBJECTTTAG UIStartObjID;
						_uint        UIStartNumber;
						dynamic_cast<CTempUI*>(Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(Get_Parent())->Get_Child());
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(nullptr);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(true);

						// ���� ���� �� �ش� ���� �κ��丮�� ������Ʈ // ��ġ �̵� �� ���� ��ġ���� ���� �ʱ�ȭ (���� X)
						pInventory->Switch_InvenItem(m_ItemID, UIStartObjID, UIStartNumber, UIColliderSlotObjID, UIColliderSlotNumber); // : �̵��� ������ ���̵�, �̵��� ���� Ÿ�� �� ��ȣ

						m_pTransform->m_vInfo[INFO_POS].x = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = ColliderSlotObj->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);

						Set_Parent(ColliderSlotObj);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_Child(this);
						dynamic_cast<CTempUI*>(Get_Parent())->Set_EmptyBool(false);


						if (m_ItemID.eItemType == ITEMTYPE_WEAPONITEM)
						{
							CGameObject* pRightItem = pPlayer->Get_CurrentEquipRight();
							if (pRightItem != nullptr) {
								ITEMTYPEID ItemId = dynamic_cast<CItem*>(pRightItem)->Get_ItemTag();

								if (ItemId.eItemID == m_ItemID.eItemID)
								{
									UIOBJECTTTAG UIFindObjID{};
									_uint        UIFindNumber;
									dynamic_cast<CTempUI*>(Get_Parent())->Get_UIObjID(UIFindObjID, UIFindNumber);

									if (UIFindObjID == UIID_SLOTBASIC)
										dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
									else if (UIFindObjID == UIID_SLOTEMPTY)
										dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(Get_Parent()))->Set_FindSlot(true);
								}
								else
								{

								}
							}
						}
					}
					else
					{
						m_pTransform->m_vInfo[INFO_POS].x = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].x;
						m_pTransform->m_vInfo[INFO_POS].y = Get_Parent()->m_pTransform->m_vInfo[INFO_POS].y;
						WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
					}

					m_bOnlyMove = false;
					Engine::UIManager()->m_bMouse = false;
					Engine::UIManager()->Set_PickingItemUI(nullptr);
				}
#pragma endregion ������ �̵� �� ����

				if (m_ItemID.eItemType == ITEMTYPE_GENERALITEM || m_ItemID.eItemType == ITEMTYPE_EQUIPITEM)
				{
					CGameObject* SlotObj = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, m_UINumber);
					dynamic_cast<CUIequipmentslot*>(dynamic_cast<CTempUI*>(SlotObj))->Set_FindSlot(false);
				}
			}
		}

		if(!m_bMove)
		{
			m_bTooltipRender = true;

			m_fTooltipPosX = pt.x;
			m_fTooltipPosY = pt.y;

			Engine::UIManager()->Set_PickingItemUI(this);
		}
	}
	else
	{
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
