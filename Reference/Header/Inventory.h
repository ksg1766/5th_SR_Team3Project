#pragma once
#include "Component.h"

#include "Item.h" // ������ �θ� Ŭ����

BEGIN(Engine)

class ENGINE_DLL CInventory : public CComponent
{
private:
	explicit CInventory();
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory(const CInventory& rhs);
	virtual ~CInventory();

public:
	void            Add_ItemObject(CGameObject* pGameObject); // �κ��丮�� ������ �߰�
	void            delete_FindItem(ITEMTYPEID _itemId);      // �ش� ���̵��� ������ �κ��丮���� ���� �Ǵ� ī��Ʈ ����

	CGameObject*    Get_KeySlotObject(INVENKEYSLOT _key) { return m_mapKeySlot[_key]; }
	ITEMTYPEID      Get_KeySlotObjID(INVENKEYSLOT _key)  { return static_cast<CItem*>(m_mapKeySlot[_key])->Get_ItemTag(); }


	CGameObject*	Get_ItemSlot(INVENITEMSLOT _Key)	 { return m_mapItemSlot[_Key]; }
	CGameObject*	Get_IDItem(ITEMID _eID);

	void            GoSwitch_InvenItem(ITEMTYPEID  _ItemID, UIOBJECTTTAG _StartslotId, _uint _StartUINumber, UIOBJECTTTAG _EndslotId, _uint _EndUINumber);
	void            ExSwitch_InvenItem(ITEMTYPEID  _StartItemID, UIOBJECTTTAG _StartslotId, _uint _StartUINumber, ITEMTYPEID  _EndItemID, UIOBJECTTTAG _EndslotId, _uint _EndUINumber);

	vector<CGameObject*>& Get_DropInven() { return m_vDead; }
public:
	HRESULT			Ready_Inventory();

private:
	CGameObject*    Find_KeySlotItem(_uint _StartUINumber);
	CGameObject*    Find_ItemSlotItem(_uint _StartUINumber);
	CGameObject*    Find_InvenSlotItem(ITEMTYPEID  _ItemID);

private:
	map<INVENKEYSLOT,  CGameObject*> m_mapKeySlot;       // Ű ���� 5ĭ
	map<INVENITEMSLOT, CGameObject*> m_mapItemSlot;      // ������ ���� ���� 6ĭ
	vector<CGameObject*>             m_vecInventory;     // ���� �κ��丮 18ĭ

	vector<CGameObject*>			 m_vDead;

	_bool                            m_bKeySlotEmpty[KEYSLOT_END];  // Ű ���� ������ ���� ����
	_bool                            m_bItemSlotEmpty[ITEMSLOT_END];// ������ ���� ������ ���� ����

public:
	static	CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual void			Free();
};

END