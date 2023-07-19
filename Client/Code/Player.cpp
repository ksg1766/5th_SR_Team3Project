#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_Function.h"

// �ӽ� ������
#include "TempItem.h"
#include "Bow.h"
#include "FireWands.h"
#include "Beer.h"
#include "Helmet.h"
#include "DynamicCamera.h"

// State
#include "PlayerState_Walk.h"
#include "PlayerState_Idle.h"
#include "Player_Attack.h"

#include "UIitem.h"
#include <UIbasicslot.h>

// �ӽ� ���� ������
#include "PoolManager.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
{

}

CPlayer::~CPlayer()
{
	Free();
}

HRESULT CPlayer::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::PLAYER;
	m_bItemEquipRight = false;
	m_bItemEquipLeft = false;
	m_bIsAttack = false;
	m_bAttackTick = true;
	m_bDrunk = false;
	m_bLeftRot = false;
	m_bRightRot = false;
	m_bStartRot = true;
	m_iDrunkCount = 0.f;
	m_iRootCount = 0;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

	m_pTransform->Translate(_vec3(0.f, 10.f, 0.f));
	//m_vOffset	  =	_vec3(0.7f, -0.6f, 1.5f);
	//m_vLeftOffset = _vec3(-0.7, -0.6f, 1.5f);

	// �ȱ� ���� �߰�
	CState* pState = CPlayerState_Walk::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ROMIMG, pState);
	pState = CPlayerState_Idle::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::IDLE, pState);
	pState = CPlayer_Attack::Create(m_pGraphicDev, m_pStateMachine);
	m_pStateMachine->Add_State(STATE::ATTACK, pState);

	m_pStateMachine->Set_State(STATE::IDLE);


#pragma region PlayerStat
	// ���� ����
	m_pStat->Get_Stat()->fMaxHP			= 12.f;
	m_pStat->Get_Stat()->fHP			= 12.f;
	m_pStat->Get_Stat()->iDamageMin		= 1;
	m_pStat->Get_Stat()->iDamageMax		= 2;
	m_pStat->Get_Stat()->iArmorMin		= 1;
	m_pStat->Get_Stat()->iArmorMax		= 2;

	// ����
	m_pStat->Get_Stat()->fSpeed			= 4.f;
	m_pStat->Get_Stat()->fAgility		= 4.f;
	m_pStat->Get_Stat()->fDeffense		= 4.f;
	m_pStat->Get_Stat()->fMagic			= 4.f;
	m_pStat->Get_Stat()->fAttack		= 4.f;

	m_pStat->Get_Stat()->fHealth		= 4.f;
	m_pStat->Get_Stat()->iExp			= 0.f;
	m_pStat->Get_Stat()->iExpMax		= 8.f;
	m_pStat->Get_Stat()->iGold			= 100;
	m_pStat->Get_Stat()->iLevel			= 1;

	iDefalutDamageMax = 0;
	iDefalutDamageMin = 0;
	iArmorMax = 0;
	iArmorMin = 0;
	m_bEquipStat = false;

#pragma endregion

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	//Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
	// 
	//
	Key_Input(fTimeDelta);
	if (SceneManager()->Get_GameStop()) { return 0; }
	m_pRigidBody->Update_RigidBody(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);

#pragma region ksg

	if (m_pCurrentEquipItemRight)
	{
		if (!m_bIsAttack)
		{
			CTransform* pRightTrans = m_pCurrentEquipItemRight->m_pTransform;
			_vec3 vLocalScale = pRightTrans->LocalScale();

			pRightTrans->Copy_RUL(m_pTransform->m_vInfo);

			for (_int i = 0; i < INFO_POS; ++i)
				pRightTrans->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
		}
	}

	if (m_pCurrentEquipItemLeft)
	{
		CTransform* pLeftTrans = m_pCurrentEquipItemLeft->m_pTransform;
		_vec3 vLocalScale = pLeftTrans->LocalScale();

		pLeftTrans->Copy_RUL(m_pTransform->m_vInfo);

		for (_int i = 0; i < INFO_POS; ++i)
			pLeftTrans->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
	}

#pragma endregion ksg

	if (!m_bEquipStat)
	{
		iDefalutDamageMax = m_pStat->Get_Stat()->iDamageMax;
		iDefalutDamageMin = m_pStat->Get_Stat()->iDamageMin;
		iArmorMax = m_pStat->Get_Stat()->iArmorMax;
		iArmorMin = m_pStat->Get_Stat()->iArmorMin;
	}

	if (m_pCurrentEquipItemRight)
	{
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMax;
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMin;

		m_bEquipStat = true;
	}
	else
	{
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax;
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin;

		m_bEquipStat = false;
	}


	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	if (Get_Drunk())
	{
		if (m_bStartRot)
		{
			m_bStartRot = false;
			D3DXMatrixIdentity(&m_matPlayerWorld);
			m_matPlayerWorld = m_pTransform->WorldMatrix();
		}

		if (m_iRootCount < 4)
		{
			if (m_iDrunkCount < 30 && !m_bRightRot)
			{
				++m_iDrunkCount;
				m_pTransform->Rotate(ROT_Z, 0.05 *  Engine::Get_TimeDelta(L"Timer_FPS60"));
			}
			else if (m_iDrunkCount > -30 && !m_bLeftRot)
			{
				--m_iDrunkCount;
				m_pTransform->Rotate(ROT_Z, -0.05  * Engine::Get_TimeDelta(L"Timer_FPS60"));
				m_bRightRot = true;
			}
			else
			{
				m_bRightRot = false;
				++m_iRootCount;
			}
		}
		else if (m_iDrunkCount == -20 && !m_bRightRot)
		{
			m_pTransform->Set_WorldMatrix(m_matPlayerWorld);
			Set_Drunk(false);
			m_bStartRot = true;
			m_bLeftRot = false;
			m_bRightRot = false;
		}
	}

	m_pStateMachine->LateUpdate_StateMachine();
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pBuffer->Render_Buffer();

#if _DEBUG
	m_pCollider->Render_Collider();
#endif
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBuffer = dynamic_cast<CPlayerBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_PlayerBf"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Player_State"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

	pComponent = m_pRigidBody = dynamic_cast<CRigidBody*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::RIGIDBODY, pComponent);

	pComponent = m_pStat = dynamic_cast<CPlayerStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Player_Stat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

	pComponent = m_pInventory = dynamic_cast<CInventory*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::INVENTORY, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	CGameObject* pGameObject = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();

	_long dwMouseMove;

	_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];
	_vec3 vRight = m_pTransform->m_vInfo[INFO_RIGHT];
	_vec3 vUp = m_pTransform->m_vInfo[INFO_UP];

	_bool bCameraOn = static_cast<CDynamicCamera*>(pGameObject)->Get_MouseFix();

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X)) && !bCameraOn)
	{
		_matrix matRotX;

		m_pTransform->Rotate(ROT_Y, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

		D3DXMatrixRotationAxis(&matRotX, &m_pTransform->m_vInfo[INFO_UP], D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);
		D3DXVec3TransformCoord(&m_vOffset, &m_vOffset, &matRotX);
		D3DXVec3TransformCoord(&m_vLeftOffset, &m_vLeftOffset, &matRotX);
	}

	if (0 != (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y)) && !bCameraOn)
	{
		m_pTransform->Rotate(ROT_X, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

		_vec3 vRigh2t;
		D3DXVec3Cross(&vRigh2t, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);

		_matrix matRotY;

		D3DXMatrixRotationAxis(&matRotY, &m_pTransform->m_vInfo[INFO_RIGHT], D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);
		D3DXVec3TransformCoord(&m_vOffset, &m_vOffset, &matRotY);
		D3DXVec3TransformCoord(&m_vLeftOffset, &m_vLeftOffset, &matRotY);
	}

	if (Engine::InputDev()->Key_Down(DIK_SPACE))
	{
		if (!m_IsJump)
		{
			m_pRigidBody->Add_Force(_vec3(0.f, 1.1f * m_fSpeed, 0.f));
			m_pRigidBody->UseGravity(true);
			//m_IsJump = true;
		}
	}

	// UI ����Ű �߰�
	if (Engine::InputDev()->Key_Down(DIK_I))
	{
		if (Engine::UIManager()->Set_InvenUse())
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(true);
			// ����� ���콺 �� �ް�.
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			Set_UseUI(false);
		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_C))
	{
		if (Engine::UIManager()->Set_StatUse())
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(true);
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			Set_UseUI(false);
		}
		
	}
	else if (Engine::InputDev()->Key_Down(DIK_M))
	{
		if (Engine::UIManager()->Set_MapUse()) {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(true);
			Set_UseUI(true);
		}
		else {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(false);
		}
	}
	else if (Engine::InputDev()->Key_Down(DIK_ESCAPE))
	{
		if (Engine::UIManager()->Set_EscUse()) {
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(true);
			SceneManager()->Set_GameStop(true);
			Set_UseUI(true);
		}
		else
		{
			static_cast<CDynamicCamera*>(pGameObject)->Set_Fix(false);
			SceneManager()->Set_GameStop(false);
			Set_UseUI(false);
		}
	}

	// ������ �ݱ� �� ������
	//if (Engine::InputDev()->Key_Down(DIK_E))
	//{
	//	//  �ٶ󺸰� �ִ� ������ �ݱ� / E Ű�� �տ� �ִ� ������ ȹ���� �׽�Ʈ ������ �ӽ� ����
	//	Engine::CGameObject* pGameObjectItem = CBeer::Create(m_pGraphicDev, false);
	//	//Engine::CGameObject* pGameObjectItem = CTempItem::Create(m_pGraphicDev, false);
	//	// ȹ���� ������ Ÿ�� �� ������ �޾ƿ�.
	//	ITEMTYPEID ItemType = dynamic_cast<CItem*>(pGameObjectItem)->Get_ItemTag();

	//	// ȹ���� �������� �κ��丮�� �ִ´�.	
	//	m_pInventory->Add_ItemObject(pGameObjectItem);

	//	//// ITEM UI ��ü ���� �� ���� ������ Ÿ�� �� ���̵�� �� ����.
	//	Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
	//	dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);
	//	// ���� �� UI �Ŵ����� ������UI �߰�.
	//	Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);

	//	// �����ϴ� ������ Ÿ���̰� �������� �����ϰ� ���� �ʴٸ� ����(����)
	//	if (ItemType.eItemType == ITEMTYPE::ITEMTYPE_GENERALITEM && !m_bItemEquipLeft) // ������ ���� ������ Ÿ��
	//	{
	//		m_bItemEquipLeft = true;
	//		Set_CurrentEquipLeft(pGameObjectItem);
	//		Set_PrevEquipLeft(pGameObjectItem);
	//		Engine::EventManager()->CreateObject(pGameObjectItem, LAYERTAG::GAMELOGIC);

	//		// ���� ���� // �ش� ������ ã�Ƽ� �ش� ���� ���� ���� ǥ��
	//		Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
	//		dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
	//	}
	//}
	if (Engine::InputDev()->Key_Down(DIK_Q))
	{
		// �����տ� ��� �ִ� �����۸� QŰ�� ������ ����

		if (m_bItemEquipRight == true) // �����տ� �������� �����ϰ� �ִ� ������ �� 
		{
			// �����տ� �����ϰ� �ִ� ������ Ÿ���� ������.
			ITEMTYPEID ItemType = dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemTag();

			// �ش� �������� ������ 1���Ͻÿ� �� �̻��� �ø� ���� : 1���� �������� �����鼭 ���� �տ� �����ϰ� �ִ� ���� �ʱ�ȭ, 1�� �̻��̸� �ش� ������ ī��Ʈ 1����(���� ����)
			if (ItemType.iCount == 1)
			{
				// �κ��丮 ������ �ش� �������� ã�� ����.
				m_pInventory->delete_FindItem(ItemType);

				// ���� ���� ����
				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);

				// ������ UI ���ο����� �ش� ������ UI�� ã�� ����.
				Engine::UIManager()->Delete_FindItemUI(ItemType);

				// 1���� ������ �־��⿡ �������� ���� ���� X, ���� �������� ��� ���� �ʱ�ȭ
				m_pCurrentEquipItemRight = nullptr;
				m_pPrevEquipItemRight = nullptr;
				m_bItemEquipRight = false;
			}
			else if (ItemType.iCount > 1)
			{
				ItemType.iCount = 1; // �������� �����ϰ� �ֱ⿡ �տ� ����ִ� 1���� ������.

				// �κ��丮 ������ �ش� �������� ã�� ����.
				m_pInventory->delete_FindItem(ItemType);

				//������ UI ���ο����� �ش� �������� ã�� ����.
				Engine::UIManager()->Delete_FindItemUI(ItemType);
			}
		}
	}

	// 1 2 3 4 5 ���Կ� �ִ� ������ ���(�Ҹ� �Ǵ� ��) �� ���� + �����ϱ�
	if (Engine::InputDev()->Key_Down(DIK_F1))
	{
		Use_SlotItem(KEYSLOT_ONE);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F2))
	{
		Use_SlotItem(KEYSLOT_TWO);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F3))
	{
		Use_SlotItem(KEYSLOT_THREE);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F4))
	{
		Use_SlotItem(KEYSLOT_FOUR);
	}
	else if (Engine::InputDev()->Key_Down(DIK_F5))
	{
		Use_SlotItem(KEYSLOT_FIVE);
	}

	if (Engine::InputDev()->Key_Down(DIK_DELETE))
	{
		vector<CGameObject*>& vecMonsterList = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::MONSTER);
		if (!vecMonsterList.empty())
			CPoolManager::GetInstance()->Delete_Object(vecMonsterList.back());
	}
}

void CPlayer::Use_SlotItem(INVENKEYSLOT _SlotNum)
{
	CGameObject* SlotItemObj = m_pInventory->Get_KeySlotObject(_SlotNum);

	if (SlotItemObj != nullptr) {
		ITEMTYPEID ItemType = dynamic_cast<CItem*>(SlotItemObj)->Get_ItemTag();
		if (ItemType.eItemType == ITEMTYPE_WEAPONITEM)        // ���� �� ���� ������ Ÿ��
		{
			if (!m_bItemEquipRight) // ���� �տ� �����ϰ� �ִ� ���°� �ƴ� �� 
			{
				// �����տ� ���� �� �ش� ��ġ ���� UI ��� ���� �������� ����
				m_bItemEquipRight = true;
				
				Set_CurrentEquipRight(SlotItemObj);
				Set_PrevEquipRight(SlotItemObj);

				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
			}
			else // ���� �տ� �����ϰ� �ִ� ������ �� 
			{
				if (dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemTag().eItemID == ItemType.eItemID)
				{
					// �����Ϸ��� Ÿ�԰� ���� Ÿ���� ��� ���� �� ���� ���� �� ���� UI ���� �������� ����
					m_bItemEquipRight = false;
					Set_CurrentEquipRight(nullptr);
					Set_PrevEquipRight(SlotItemObj);
					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);
				}
				else
				{
					// �����Ϸ��� Ÿ�԰� �ٸ� Ÿ���� ��� ���� �� ���� ������ ���� ���� �� �ش� ���������� �� ���� + UI ���� ���� �������� ����
					m_bItemEquipRight = true;
					Set_CurrentEquipRight(SlotItemObj);
					ITEMTYPEID PrevItemType = dynamic_cast<CItem*>(Get_PrevEquipRight())->Get_ItemTag(); // ���� ������ ������ �±װ����.
					Engine::CGameObject* PrevFindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(PrevItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(PrevFindSlotObj))->Set_FindSlot(false);

					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);

					Set_PrevEquipRight(SlotItemObj);
				}
			}
		}
		else if (ItemType.eItemType == ITEMTYPE_GENERALITEM) // �� �� ���� ������ Ÿ��
		{
			// �޼տ� ���� �� ������ ��ġ �� �������� ��ġ �̵�

			if (!m_bItemEquipLeft) // �� �տ� �����ϰ� �ִ� ���°� �ƴ� �� 
			{
				// �޼տ� ���� �� �ش� ��ġ �������� ��ġ �̵�
				m_bItemEquipLeft = true;

				Set_CurrentEquipLeft(SlotItemObj);
				Set_PrevEquipLeft(SlotItemObj);

				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);

			}
			else // �� �տ� �����ϰ� �ִ� ������ �� 
			{
				// �����Ϸ��� Ÿ�԰� ���� Ÿ���� ��� ���� �� ���� ����
				if (dynamic_cast<CItem*>(m_pCurrentEquipItemLeft)->Get_ItemTag().eItemID == ItemType.eItemID)
				{
					// �����Ϸ��� Ÿ�԰� ���� Ÿ���� ��� ���� �� ���� ���� �� ���� UI ���� �������� ����
					m_bItemEquipLeft = false;
					Set_CurrentEquipLeft(nullptr);
					Set_PrevEquipLeft(SlotItemObj);
					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);
				}
				else
				{
					// �����Ϸ��� Ÿ�԰� �ٸ� Ÿ���� ��� ���� �� ���� ������ ���� ���� �� �ش� ���������� �� ���� + UI ���� ���� �������� ����
					m_bItemEquipLeft = true;
					Set_CurrentEquipRight(SlotItemObj);
					ITEMTYPEID PrevItemType = dynamic_cast<CItem*>(Get_PrevEquipLeft())->Get_ItemTag(); // ���� ������ ������ �±װ����.
					Engine::CGameObject* PrevFindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(PrevItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(PrevFindSlotObj))->Set_FindSlot(false);

					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);

					Set_PrevEquipLeft(SlotItemObj);
				}
				// �����Ϸ��� Ÿ�԰� �ٸ� Ÿ���� ��� ���� �� ���� ���� �� �ش� ���������� �� ����


			}
		}
		else if (ItemType.eItemType == ITEMTYPE_EQUIPITEM)   // ������ ���� ���� ������ Ÿ��
		{
			// �ش� ������ ���Կ� ���� �� ��ġ �̵� + ���� �� ������ ���� ȿ�� ���� �� ����
		}
		else if (ItemType.eItemType == ITEMTYPE_EATITEM)    // HP ȸ�� ������ Ÿ��
		{
			// �ش� �������� ȸ�� ���� ���� HP ȸ�� �� ������ �Ҹ�

			// ������ ȿ�� ����

			// ������ �Ҹ�
		}
		else if (ItemType.eItemType == ITEMTYPE_POTIONITEM) // �پ��� ���� ������ Ÿ��
		{
			// �ش� ������ �ɷ� �� ȿ�� ���� �� ������ �Ҹ�

			// ������ ȿ�� ����

			// ������ �Ҹ�
		}
	}
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTER 
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM)
	{
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();

		// OBJECTTAG�� ���� ���� ó�� ���ɼ�
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// ���� ���� ���� ���� ��� �浹�� ��. �� ���� �о�� ��.

		if (fRadiusY == fMinAxis)
		{
			if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER)
				return;
			if (vOtherPos.y < vThisPos.y)
			{
				//m_IsJump = false;
				//m_pRigidBody->UseGravity(false);
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransform->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
			}
			else
			{
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
			}
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
	}

	if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::ITEM)
	{
		if (dynamic_cast<CItem*>(_pOther->GetHost())->Get_WorldItem())
		{
			ITEMTYPEID ItemType = dynamic_cast<CItem*>(_pOther->GetHost())->Get_ItemTag();

			CGameObject* pItem = nullptr;

			switch (ItemType.eItemID)
			{
			case ITEMID::WEAPON_SWORD:
				pItem = CTempItem::Create(m_pGraphicDev, false);
				break;
			case ITEMID::WEAPON_BOW:
				pItem = CBow::Create(m_pGraphicDev, false);
				break;
			case ITEMID::WEAPON_WAND3:
				pItem = CFireWands::Create(m_pGraphicDev, false);
				break;
			case ITEMID::EQUIP_OLDHELMET:
				pItem = CHelmet::Create(m_pGraphicDev, false);
				break;
			}

			m_pInventory->Add_ItemObject(pItem);

			Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
			dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);

			Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);

			if (ItemType.eItemType == ITEMTYPE_WEAPONITEM && !m_bItemEquipRight)
			{
				m_bItemEquipRight = true;
				
				Set_CurrentEquipRight(pItem);
				Set_PrevEquipRight(pItem);
				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
			}
			else
			{
				Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
				//Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				//dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);
			}


			EventManager()->GetInstance()->DeleteObject(dynamic_cast<CItem*>(_pOther->GetHost()));
		}
	}
}

void CPlayer::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTER 
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET
		&& _pOther->GetHost()->Get_ObjectTag() != OBJECTTAG::ITEM)
	{
		_vec3	vOtherPos = _pOther->GetCenterPos();
		_float* fOtherAxis = _pOther->GetAxisLen();

		_vec3	vThisPos = m_pCollider->GetCenterPos();
		_float* fThisAxis = m_pCollider->GetAxisLen();

		// OBJECTTAG�� ���� ���� ó�� ���ɼ�
		_float fWidth = fabs(vOtherPos.x - vThisPos.x);
		_float fHeight = fabs(vOtherPos.y - vThisPos.y);
		_float fDepth = fabs(vOtherPos.z - vThisPos.z);

		_float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
		_float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
		_float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

		_float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// ���� ���� ���� ���� ��� �浹�� ��. �� ���� �о�� ��.

		if (fRadiusY == fMinAxis)
		{
			if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::MONSTER)
				return;
			if (vOtherPos.y < vThisPos.y)
			{
				//m_IsJump = false;
				//m_pRigidBody->UseGravity(false);
				m_pRigidBody->Set_Force(_vec3(0.f, 0.f, 0.f));
				m_pTransform->Translate(_vec3(0.f, fRadiusY - 0.000001f, 0.f));
			}
			else
				m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
		}
		else if (fRadiusZ == fMinAxis)
		{
			if (vOtherPos.z < vThisPos.z)
				m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
			else
				m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
		}
		else //(fRadiusX == fMinAxis)
		{
			if (vOtherPos.x < vThisPos.x)
				m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
			else
				m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
		}
	}
	
}

void CPlayer::OnCollisionExit(CCollider* _pOther)
{
	//if (_pOther->GetHost()->Get_ObjectTag() == OBJECTTAG::BLOCK)
	//{
	//	_vec3 vThisPos = m_pTransform->m_vInfo[INFO_POS];
	//	_vec3 vOtherPos = _pOther->Get_Transform()->m_vInfo[INFO_POS];

	//	if (1.5f < vThisPos.y - vOtherPos.y && 2.5f > vThisPos.y - vOtherPos.y)
	//	{
	//		//m_IsJump = true;
	//		//m_pRigidBody->UseGravity(true);
	//	}
	//}
}

void CPlayer::IsAttack(CBasicStat* _MonsterStat)
{
	_int iDamage = 1 + rand() % (m_pStat->Get_Stat()->iDamageMin +
		(m_pStat->Get_Stat()->iDamageMax - m_pStat->Get_Stat()->iDamageMin));

	_int iDeffence = 1 + rand() % (_MonsterStat->Get_Stat()->iArmorMin + (_MonsterStat->Get_Stat()->iArmorMax - _MonsterStat->Get_Stat()->iArmorMin));

	_int iResultDamage = iDamage - iDeffence;

	if (iResultDamage <= 0)
		iResultDamage = 1;

	cout << iResultDamage << " ������" << endl;

	Set_AttackTick(true);
	_MonsterStat->Take_Damage(iResultDamage);
}

void CPlayer::Free()
{
	__super::Free();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pInstance;
}