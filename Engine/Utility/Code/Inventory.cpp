#include "..\..\Header\Inventory.h"

#include <Export_Utility.h>

CInventory::CInventory()
{

}

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CInventory::CInventory(const CInventory& rhs)
	: CComponent(rhs), m_mapKeySlot(rhs.m_mapKeySlot), m_mapItemSlot(rhs.m_mapItemSlot), m_vecInventory(rhs.m_vecInventory)
{
	for (_uint i = 0; i < KEYSLOT_END; i++) {
		m_bKeySlotEmpty[i] = rhs.m_bKeySlotEmpty[i];
	}

	for (_uint i = 0; i < ITEMSLOT_END; i++) {
		m_bItemSlotEmpty[i] = rhs.m_bItemSlotEmpty[i];
	}
}

CInventory::~CInventory()
{
	CInventory::Free();
}

void CInventory::Add_ItemObject(CGameObject* pGameObject)
{
	// ���� ������ ������Ʈ ������ Ÿ�� �� ���̵�, ������ ������.
	ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObject)->Get_ItemTag();

    // �̹� �����ϰ� �ִ� ���������� �˻��Ѵ�.
	// Ű���� �˻�
	for (auto& iter : m_mapKeySlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// ���� �������� ������ �� �ش� ������ŭ ī��Ʈ ���� �� ���� ������ ����
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}

	// ���� �κ��丮 �˻�
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// ���� �������� ������ �� �ش� ������ŭ ī��Ʈ ���� �� ���� ������ ����
				dynamic_cast<CItem*>(iter)->Add_ItemCount(ItemType.iCount);
				Safe_Release<CGameObject*>(pGameObject);
				return;
			}
		}
	}

	// �����ϰ� ���� ���� �������̱⿡
	// Ű ������ ��������� �ش� ���Կ� ����
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_ONE] = true;
		m_mapKeySlot[KEYSLOT_ONE] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_TWO] = true;
		m_mapKeySlot[KEYSLOT_TWO] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_THREE] = true;
		m_mapKeySlot[KEYSLOT_THREE] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
		m_mapKeySlot[KEYSLOT_FOUR] = pGameObject;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == false)
	{
		m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
		m_mapKeySlot[KEYSLOT_FIVE] = pGameObject;
		return;
	}

	// Ű ������ ���� ������ �� ���� �κ��� �ִ´�.
	m_vecInventory.push_back(pGameObject);
}

void CInventory::delete_FindItem(ITEMTYPEID _itemId)
{
	// �ش� �������� ���̵� ã�Ƽ� ����.
	// ���������� Ű���� �˻�
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_ONE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_ONE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_ONE]);
				m_bKeySlotEmpty[KEYSLOT_ONE] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_TWO]);
				m_bKeySlotEmpty[KEYSLOT_TWO] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_THREE]);
				m_bKeySlotEmpty[KEYSLOT_THREE] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_FOUR]);
				m_bKeySlotEmpty[KEYSLOT_FOUR] = false;
				return;
			}
		}
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Get_ItemTag();

		if (SlotItemType.eItemType == _itemId.eItemType)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_FIVE]);
				m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
				return;
			}
		}
	}

	// ���� �κ��丮 �˻�
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemType == _itemId.eItemType)
			{
				// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					Safe_Release<CGameObject*>(m_mapKeySlot[KEYSLOT_FIVE]);
					return;
				}
			}
		}
	}
}

HRESULT CInventory::Ready_Inventory()
{
	// m_bKeySlotEmpty �ʱ�ȭ
	for (_uint i = 0; i < KEYSLOT_END; i++) {
		m_bKeySlotEmpty[i] = false;
	}

	// m_bItemSlotEmpty �ʱ�ȭ
	for (_uint i = 0; i < ITEMSLOT_END; i++) {
		m_bItemSlotEmpty[i] = false;
	}

	return S_OK;
}

void CInventory::LateUpdate_Component()
{
	
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory* pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_Inventory()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Inventory Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CInventory::Clone(void)
{
	return new CInventory(*this);
}

void CInventory::Free()
{
	if (false == m_bClone)
	{

		for (auto& Mapiter : m_mapKeySlot) {
			Safe_Release(Mapiter.second);
		}

		for (auto& Mapiter : m_mapItemSlot) {
			Safe_Release(Mapiter.second);
		}

		for_each(m_vecInventory.begin(), m_vecInventory.end(), CDeleteObj());
		m_vecInventory.clear();
	}

	CComponent::Free();
}
