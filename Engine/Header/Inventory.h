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
	ITEMTYPEID      Get_KeySlotObjID(INVENKEYSLOT _key)  { return dynamic_cast<CItem*>(m_mapKeySlot[_key])->Get_ItemTag(); }

	void            Switch_InvenItem(ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber); // ������ ���̵�� �ű���� ���� ��ġ ���̵�

public:
	HRESULT			Ready_Inventory();
	virtual void	LateUpdate_Component() override;

private:
	void            Switch_Keyslot(INVENKEYSLOT _key, ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber);
	void            Switch_Itemslot(INVENITEMSLOT _key, ITEMTYPEID _itemId, UIOBJECTTTAG _slotId, _uint _UINumber);

private:
	map<INVENKEYSLOT, CGameObject*>  m_mapKeySlot;       // Ű ���� 5ĭ
	map<INVENITEMSLOT, CGameObject*> m_mapItemSlot;      // ������ ���� ���� 6ĭ
	vector<CGameObject*>             m_vecInventory;     // ���� �κ��丮 18ĭ

	_bool                            m_bKeySlotEmpty[KEYSLOT_END];  // Ű ���� ������ ���� ����
	_bool                            m_bItemSlotEmpty[ITEMSLOT_END];// ������ ���� ������ ���� ����

	_bool                            m_bSwitch;

public:
	static	CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);

private:
	virtual void			Free();
};

END