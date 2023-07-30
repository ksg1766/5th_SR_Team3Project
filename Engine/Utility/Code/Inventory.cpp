#include "..\..\Header\Inventory.h"
#include "Export_Function.h"

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
	//CInventory::Free();
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

void CInventory::Add_ItemObject(CGameObject* pGameObject)
{
	// ���� ������ ������Ʈ ������ Ÿ�� �� ���̵�, ������ ������.
	ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObject)->Get_ItemTag();

    // �̹� �����ϰ� �ִ� ���������� �˻��Ѵ�.
	//
	// Ű���� �˻�
	for (auto& iter : m_mapKeySlot) {
		if (iter.second != nullptr) {
			// �ش� ���Կ� �����ϴ� ������ Ÿ���� �����´�.
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			// ���Կ� �ִ� ������ Ÿ�԰� ���� ������ Ÿ���� ���ٸ�
			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				// �ش� ������ ������ ī��Ʈ�� �÷��ְ�
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				dynamic_cast<CItem*>(pGameObject)->m_pTransform;

				CTransform* pPlayerTransform        = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				ChildTransform.erase(remove_if(ChildTransform.begin(), ChildTransform.end(),
					[&](CTransform* pTramsform)
					{
						if (pTramsform == dynamic_cast<CItem*>(pGameObject)->m_pTransform)
							return true;

						return false;
					}
				),
					ChildTransform.end());

				dynamic_cast<CItem*>(pGameObject)->m_pTransform = nullptr;
				EventManager()->DeleteObject(pGameObject);
				return;
			}
		}
	}

	// ���� ���� �˻�
	for (auto& iter : m_mapItemSlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				dynamic_cast<CItem*>(iter.second)->Add_ItemCount(ItemType.iCount);
				dynamic_cast<CItem*>(pGameObject)->m_pTransform;

				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				ChildTransform.erase(remove_if(ChildTransform.begin(), ChildTransform.end(),
					[&](CTransform* pTramsform)
					{
						if (pTramsform == dynamic_cast<CItem*>(pGameObject)->m_pTransform)
							return true;

						return false;
					}
				),
					ChildTransform.end());

				dynamic_cast<CItem*>(pGameObject)->m_pTransform = nullptr;
				EventManager()->DeleteObject(pGameObject);
				return;
			}
		}
	}

	// ���� �κ��丮 �˻�
	for (auto& iter = m_vecInventory.begin(); iter != m_vecInventory.end();) {
		if (*iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(*iter)->Get_ItemTag();

			if (SlotItemType.eItemID == ItemType.eItemID)
			{
				dynamic_cast<CItem*>(*iter)->Add_ItemCount(ItemType.iCount);
				dynamic_cast<CItem*>(pGameObject)->m_pTransform;

				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				ChildTransform.erase(remove_if(ChildTransform.begin(), ChildTransform.end(),
					[&](CTransform* pTramsform)
					{
						if (pTramsform == dynamic_cast<CItem*>(pGameObject)->m_pTransform)
							return true;

						return false;
					}
				),
					ChildTransform.end());

				dynamic_cast<CItem*>(pGameObject)->m_pTransform = nullptr;
				EventManager()->DeleteObject(pGameObject);
				return;
			}
			else
				++iter;
		}
		else
			++iter;
	}

	// �����ϰ� ���� ���� �������̱⿡
	// Ű ������ ��������� �ش� ���Կ� ����
	if (m_bKeySlotEmpty[KEYSLOT_ONE] == false)
	{
		m_mapKeySlot[KEYSLOT_ONE] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_ONE] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_TWO] == false)
	{
		m_mapKeySlot[KEYSLOT_TWO] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_TWO] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_THREE] == false)
	{
		m_mapKeySlot[KEYSLOT_THREE] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_THREE] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FOUR] == false)
	{
		m_mapKeySlot[KEYSLOT_FOUR] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_FOUR] = true;
		return;
	}
	else if (m_bKeySlotEmpty[KEYSLOT_FIVE] == false)
	{
		m_mapKeySlot[KEYSLOT_FIVE] = pGameObject;
		m_bKeySlotEmpty[KEYSLOT_FIVE] = true;
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
	
		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_ONE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{	
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				for (auto& iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;
	

				m_bKeySlotEmpty[KEYSLOT_ONE] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_TWO] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_TWO])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				for (auto& iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}


				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bKeySlotEmpty[KEYSLOT_TWO] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_THREE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_THREE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;
				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				for (auto& iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;



				m_bKeySlotEmpty[KEYSLOT_THREE] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_FOUR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FOUR])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();


				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							iter = ChildTransform.erase(iter);
						}
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bKeySlotEmpty[KEYSLOT_FOUR] = false;
				return;
			}
		}
	}
	if (m_bKeySlotEmpty[KEYSLOT_FIVE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapKeySlot[KEYSLOT_FIVE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}


				for (auto& iter = m_mapKeySlot.begin(); iter != m_mapKeySlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapKeySlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
				return;
			}
		}
	}

	// ���������� ������ ���� �˻�
	if (m_bItemSlotEmpty[ITEMSLOT_HELMET] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HELMET])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HELMET])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;



				m_bItemSlotEmpty[ITEMSLOT_HELMET] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_HAND] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HAND])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_HAND])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bItemSlotEmpty[ITEMSLOT_HAND] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_ARMOR] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_ARMOR])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_ARMOR])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_bItemSlotEmpty[ITEMSLOT_ARMOR] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_RING] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_RING])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_RING])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
				{
					if (iter->second != nullptr)
					{
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					}
					else
						++iter;
				}


				m_bItemSlotEmpty[ITEMSLOT_RING] = false;
				return;
			}
		}
	}
	if (m_bItemSlotEmpty[ITEMSLOT_PANTS] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_PANTS])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_PANTS])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{

				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;

				m_bItemSlotEmpty[ITEMSLOT_PANTS] = false;
				return;
			}
		}
	}
	else if (m_bItemSlotEmpty[ITEMSLOT_NECKLACE] == true) {
		ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_NECKLACE])->Get_ItemTag();

		if (SlotItemType.eItemID == _itemId.eItemID)
		{
			// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
			if (SlotItemType.iCount > _itemId.iCount)
			{
				dynamic_cast<CItem*>(m_mapItemSlot[ITEMSLOT_NECKLACE])->Remove_ItemCount(_itemId.iCount);
				return;
			}
			else
			{
				CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

				vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
				vector<CTransform*>::iterator iter = ChildTransform.begin();

				for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
				{
					if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
					{
						if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
							iter = ChildTransform.erase(iter);
						else
							++iter;
					}
					else
						++iter;
				}

				for (auto& iter = m_mapItemSlot.begin(); iter != m_mapItemSlot.end();)
					if (iter->second != nullptr)
						if (dynamic_cast<CItem*>(iter->second)->Get_ItemTag().eItemID == SlotItemType.eItemID)
						{
							m_vDead.push_back(iter->second);
							iter = m_mapItemSlot.erase(iter);
						}
						else
							++iter;
					else
						++iter;


				m_mapItemSlot[ITEMSLOT_NECKLACE] = false;
				return;
			}
		}
	}

	// ���� �κ��丮 �˻�
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == _itemId.eItemID)
			{
				// ���� �������� �����ϰ� ���� �������� ���� �� ī��Ʈ�� ���� / ���� ������ �� ����
				if (SlotItemType.iCount > _itemId.iCount)
				{
					dynamic_cast<CItem*>(iter)->Remove_ItemCount(_itemId.iCount);
					return;
				}
				else
				{
					CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;

					vector<CTransform*>& ChildTransform = pPlayerTransform->Get_Child();
					vector<CTransform*>::iterator iter = ChildTransform.begin();

					for (auto iter = ChildTransform.begin(); iter != ChildTransform.end();)
					{
						if (OBJECTTAG::ITEM == (*iter)->Get_ObjectTag())
						{
							if (dynamic_cast<CItem*>((*iter)->Get_Host())->Get_ItemTag().eItemID == SlotItemType.eItemID)
								iter = ChildTransform.erase(iter);
							else
								++iter;
						}
						else
							++iter;
					}

					for (auto& iter = m_vecInventory.begin(); iter != m_vecInventory.end();)
					{
						if (*iter != nullptr)
						{
							if (dynamic_cast<CItem*>(*iter)->Get_ItemTag().eItemID == SlotItemType.eItemID)
							{
								m_vDead.push_back(*iter);
								iter = m_vecInventory.erase(iter);
							}
							else
								++iter;
						}
						else
							++iter;
					}

					return;
				}
			}
		}
	}
}

CGameObject* CInventory::Get_IDItem(ITEMID _eID)
{
	// Ű ���� �˻�
	for (auto& iter : m_mapKeySlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == _eID)
			{
				return iter.second;
			}
		}
	}

	// ���� ���� �˻�
	for (auto& iter : m_mapItemSlot) {
		if (iter.second != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter.second)->Get_ItemTag();

			if (SlotItemType.eItemID == _eID)
			{
				return iter.second;
			}
		}
	}

	// ���� �κ��丮 �˻�
	for (auto& iter : m_vecInventory) {
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();

			if (SlotItemType.eItemID == _eID)
			{
				return iter;
			}
		}
	}
}

void CInventory::GoSwitch_InvenItem(ITEMTYPEID  _ItemID, UIOBJECTTTAG _StartslotId, _uint _StartUINumber, UIOBJECTTTAG _EndslotId, _uint _EndUINumber)
{
	// ���� ��ġ���� �ش� �������� ã�ƿ��� �ش� ������ �����.
	CGameObject* pFindItemObject = nullptr;

	switch (_StartslotId)
	{
	case Engine::UIID_SLOTBASIC:
		pFindItemObject = Find_KeySlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		pFindItemObject = Find_ItemSlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEMPTY:    
		pFindItemObject = Find_InvenSlotItem(_ItemID);
		break;
	}

	if (pFindItemObject == nullptr)
		return;

	// ã�� �������� �̵��Ѵ�.
	switch (_EndslotId)
	{
	case Engine::UIID_SLOTBASIC:
		m_mapKeySlot[(INVENKEYSLOT)_EndUINumber] = pFindItemObject;
		m_bKeySlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		m_mapItemSlot[(INVENITEMSLOT)_EndUINumber] = pFindItemObject;
		m_bItemSlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEMPTY:
		m_vecInventory.push_back(pFindItemObject);
		break;
	}
}

void CInventory::ExSwitch_InvenItem(ITEMTYPEID  _StartItemID, UIOBJECTTTAG _StartslotId, _uint _StartUINumber, ITEMTYPEID  _EndItemID, UIOBJECTTTAG _EndslotId, _uint _EndUINumber)
{
	// ���� ����Ī�� �����۵��� ã�ƿ´�.
	CGameObject* pStartItemObject = nullptr;
	CGameObject* pEndItemObject = nullptr;

	switch (_StartslotId)
	{
	case Engine::UIID_SLOTBASIC:
		pStartItemObject = Find_KeySlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		pStartItemObject = Find_ItemSlotItem(_StartUINumber);
		break;

	case Engine::UIID_SLOTEMPTY:
		pStartItemObject = Find_InvenSlotItem(_StartItemID);
		break;
	}

	if (pStartItemObject == nullptr)
		return;

	switch (_EndslotId)
	{
	case Engine::UIID_SLOTBASIC:
		pEndItemObject = Find_KeySlotItem(_EndUINumber);
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		pEndItemObject = Find_ItemSlotItem(_EndUINumber);
		break;

	case Engine::UIID_SLOTEMPTY:
		pEndItemObject = Find_InvenSlotItem(_EndItemID);
		break;
	}

	if (pEndItemObject == nullptr)
		return;

	// ������ ��ġ�� �־��ش�.
	switch (_EndslotId)
	{
	case Engine::UIID_SLOTBASIC:
		m_mapKeySlot[(INVENKEYSLOT)_EndUINumber] = pStartItemObject;
		m_bKeySlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		m_mapItemSlot[(INVENITEMSLOT)_EndUINumber] = pStartItemObject;
		m_bItemSlotEmpty[_EndUINumber] = true;
		break;

	case Engine::UIID_SLOTEMPTY:
		m_vecInventory.push_back(pStartItemObject);
		break;
	}

	switch (_StartslotId)
	{
	case Engine::UIID_SLOTBASIC:
		m_mapKeySlot[(INVENKEYSLOT)_StartUINumber] = pEndItemObject;
		m_bKeySlotEmpty[_StartUINumber] = true;
		break;

	case Engine::UIID_SLOTEQUIPMENT:
		m_mapItemSlot[(INVENITEMSLOT)_StartUINumber] = pEndItemObject;
		m_bItemSlotEmpty[_StartUINumber] = true;
		break;

	case Engine::UIID_SLOTEMPTY:
		m_vecInventory.push_back(pEndItemObject);
		break;
	}
}

CGameObject* CInventory::Find_KeySlotItem(_uint _StartUINumber)
{
	CGameObject* pFindKeyItem = nullptr;

	switch (_StartUINumber) 
	{
	case 0:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_ONE];
		m_mapKeySlot.erase(KEYSLOT_ONE);
		m_bKeySlotEmpty[KEYSLOT_ONE] = false;
		break;

	case 1:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_TWO];
		m_mapKeySlot.erase(KEYSLOT_TWO);
		m_bKeySlotEmpty[KEYSLOT_TWO] = false;
		break;

	case 2:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_THREE];
		m_mapKeySlot.erase(KEYSLOT_THREE);
		m_bKeySlotEmpty[KEYSLOT_THREE] = false;
		break;

	case 3:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_FOUR];
		m_mapKeySlot.erase(KEYSLOT_FOUR);
		m_bKeySlotEmpty[KEYSLOT_FOUR] = false;
		break;

	case 4:
		pFindKeyItem = m_mapKeySlot[KEYSLOT_FIVE];
		m_mapKeySlot.erase(KEYSLOT_FIVE);
		m_bKeySlotEmpty[KEYSLOT_FIVE] = false;
		break;
	}

	return pFindKeyItem;
}

CGameObject* CInventory::Find_ItemSlotItem(_uint _StartUINumber)
{
	CGameObject* pFindItemSlotItem = nullptr;

	switch (_StartUINumber)
	{
	case 0:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_HELMET];
		m_mapItemSlot.erase(ITEMSLOT_HELMET);
		m_bItemSlotEmpty[ITEMSLOT_HELMET] = false;
		break;

	case 1:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_HAND];
		m_mapItemSlot.erase(ITEMSLOT_HAND);
		m_bItemSlotEmpty[ITEMSLOT_HAND] = false;
		break;

	case 2:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_ARMOR];
		m_mapItemSlot.erase(ITEMSLOT_ARMOR);
		m_bItemSlotEmpty[ITEMSLOT_ARMOR] = false;
		break;

	case 3:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_RING];
		m_mapItemSlot.erase(ITEMSLOT_RING);
		m_bItemSlotEmpty[ITEMSLOT_RING] = false;
		break;

	case 4:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_PANTS];
		m_mapItemSlot.erase(ITEMSLOT_PANTS);
		m_bItemSlotEmpty[ITEMSLOT_PANTS] = false;
		break;

	case 5:
		pFindItemSlotItem = m_mapItemSlot[ITEMSLOT_NECKLACE];
		m_mapItemSlot.erase(ITEMSLOT_NECKLACE);
		m_bItemSlotEmpty[ITEMSLOT_NECKLACE] = false;
		break;
	}

	return pFindItemSlotItem;
}

CGameObject* CInventory::Find_InvenSlotItem(ITEMTYPEID  _ItemID)
{
	for (auto&iter : m_vecInventory)
	{
		if (iter != nullptr) {
			ITEMTYPEID SlotItemType = dynamic_cast<CItem*>(iter)->Get_ItemTag();
			if (SlotItemType.eItemID == _ItemID.eItemID) {
				return iter;
			}
		}
	}

	return nullptr;
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

	for_each(m_vDead.begin(), m_vDead.end(), CDeleteObj());
	m_vDead.clear();

	CComponent::Free();
}