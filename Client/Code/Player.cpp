#include "stdafx.h"
#include "..\Header\Player.h"

#include "Export_Function.h"
#include "SoundManager.h"
// �ӽ� ������
#include "DynamicCamera.h"
#include "Itemgroup.h"

// State
#include "PlayerState_Walk.h"
#include "PlayerState_Idle.h"
#include "Player_Attack.h"

#include "UIitem.h"
#include "UIbasicslot.h"
#include "UIemptyslot.h"
#include "UIequipmentslot.h"

// �ӽ� ���� ������
#include "PoolManager.h"

// ���� �׽�Ʈ
#include "GameManager.h"
#include "CameraManager.h"
#include "FlyingCamera.h"
#include "EffectSwordTrail.h"
#include "EffectSwordLightning.h"
#include "EffectSwordParticles.h"
#include "EffectWater.h"

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
	m_bThrowShield = false;
	m_bIsAddiction = false;
	m_bIsTalk = false;
	m_iDrunkCount = 0.f;
	m_iAddictionCount = 0.f;
	m_iRootCount = 0;
	m_fAddictionTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_iPuzzleCount = 0;

	m_pMainCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
	m_pMainCamera->m_pTransform->Copy_RUL_AddPos(m_pTransform->m_vInfo);
	//CCameraManager::GetInstance()->Get_CurrentCam()->m_pTransform->Translate(_vec3(0.f, 0.5f, 0.f));
	m_pMainCamera->m_pTransform->Set_Parent(m_pTransform);

	m_pTransform->Scale(_vec3(0.9f, 0.9f, 0.9f));
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

	D3DXMatrixIdentity(&m_matPlayerWorld);
	m_matPlayerWorld = m_pTransform->WorldMatrix();
	m_fDrunkTime = 0.f;
	m_vOriginLook = m_pTransform->m_vInfo[INFO_LOOK];
	m_vOriginUp = m_pTransform->m_vInfo[INFO_UP];
	m_vOriginRight = m_pTransform->m_vInfo[INFO_RIGHT];

#pragma region PlayerStat
	// ���� ����
	m_pStat->Get_Stat()->fMaxHP = 12.f;
	m_pStat->Get_Stat()->fHP = 12.f;
	m_pStat->Get_Stat()->iDamageMin = 1;
	m_pStat->Get_Stat()->iDamageMax = 2;
	m_pStat->Get_Stat()->iArmorMin = 1;
	m_pStat->Get_Stat()->iArmorMax = 2;

	// ����
	m_pStat->Get_Stat()->fSpeed = 4.f;
	m_pStat->Get_Stat()->fAgility = 4.f;
	m_pStat->Get_Stat()->fDeffense = 4.f;
	m_pStat->Get_Stat()->fMagic = 4.f;
	m_pStat->Get_Stat()->fAttack = 4.f;

	m_pStat->Get_Stat()->fHealth = 4.f;
	m_pStat->Get_Stat()->iExp = 0.f;
	m_pStat->Get_Stat()->iExpMax = 8.f;
	m_pStat->Get_Stat()->iGold = 100;
	m_pStat->Get_Stat()->iLevel = 1;

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
	if (!m_pMainCamera->Get_CameraMode())
		Key_Input(fTimeDelta);
	
	if (SceneManager()->Get_GameStop()) { return 0; }
	m_pRigidBody->Update_RigidBody(fTimeDelta);

	_int iExit = __super::Update_Object(fTimeDelta);
	m_pStateMachine->Update_StateMachine(fTimeDelta);

	Foot_Sound(fTimeDelta);


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
		if (!m_bIsAttack)
		{
			CTransform* pLeftTrans = m_pCurrentEquipItemLeft->m_pTransform;
			_vec3 vLocalScale = pLeftTrans->LocalScale();

			pLeftTrans->Copy_RUL(m_pTransform->m_vInfo);

			for (_int i = 0; i < INFO_POS; ++i)
				pLeftTrans->m_vInfo[i] *= *(((_float*)&vLocalScale) + i);
		}
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
		Equip_Weapon(m_pCurrentEquipItemRight);
	else
	{
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax;
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin;

		m_bEquipStat = false;
	}
	PoisonDamage(fTimeDelta);
	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	if (SceneManager()->Get_GameStop()) { return; }

	__super::LateUpdate_Object();

	if (Get_Drunk())
		IsDrunk();

	m_pStateMachine->LateUpdate_StateMachine();

	/*cout << m_pTransform->m_vInfo[INFO_POS].x << endl;
	cout << m_pTransform->m_vInfo[INFO_POS].y << endl;
	cout << m_pTransform->m_vInfo[INFO_POS].z << endl;*/
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
	//CGameObject* pGameObject = SceneManager()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front();
	CGameObject* pGameObject = CCameraManager::GetInstance()->Get_CurrentCam();

	_long dwMouseMove;

	_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];
	_vec3 vRight = m_pTransform->m_vInfo[INFO_RIGHT];
	_vec3 vUp = m_pTransform->m_vInfo[INFO_UP];

	_bool bCameraOn = static_cast<CFlyingCamera*>(pGameObject)->Is_MouseFree();

#pragma region ���� �׽�Ʈ

	if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_X))
	{
		SceneManager()->SlowMode(3.f, 3);
	}

	if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_H))
	{
		CGameManager::GetInstance()->PlayMode(PD::HekirekiIssen);
	}

	if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_J))
	{
		CGameManager::GetInstance()->PlayMode(PD::ClearGame);
	}

#pragma endregion ���� �׽�Ʈ

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

	if (!IsTalk())
	{
		if (Engine::InputDev()->Key_Down(DIK_SPACE))
		{
			if (!Get_UseUI())
			{
				if (!m_IsJump)
				{
					m_pRigidBody->Add_Force(_vec3(0.f, 1.1f * m_fSpeed, 0.f));
					m_pRigidBody->UseGravity(true);

					m_bTestJump = true;
					//m_IsJump = true;
				}
			}
		}

		// UI ����Ű �߰�
		if (Engine::InputDev()->Key_Down(DIK_I))
		{
			if (Engine::UIManager()->Set_InvenUse())
			{
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
				SceneManager()->Set_GameStop(false);
				Set_UseUI(true);
				// ����� ���콺 �� �ް�.
			}
			else
			{
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);
				Set_UseUI(false);
			}
		}
		else if (Engine::InputDev()->Key_Down(DIK_C))
		{
			if (Engine::UIManager()->Set_StatUse())
			{
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
				SceneManager()->Set_GameStop(false);
				Set_UseUI(true);
			}
			else
			{
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);
				Set_UseUI(false);
			}

		}
		else if (Engine::InputDev()->Key_Down(DIK_M))
		{
			if (Engine::UIManager()->Set_MapUse()) {
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
				SceneManager()->Set_GameStop(true);
				Set_UseUI(true);
			}
			else
			{
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);
				SceneManager()->Set_GameStop(false);
				Set_UseUI(false);
			}
		}
		else if (Engine::InputDev()->Key_Down(DIK_ESCAPE))
		{
			if (Engine::UIManager()->Set_EscUse()) {
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(true);
				SceneManager()->Set_GameStop(true);
				Set_UseUI(true);
			}
			else
			{
				static_cast<CFlyingCamera*>(pGameObject)->Set_MouseFix(false);
				SceneManager()->Set_GameStop(false);
				Set_UseUI(false);
			}
		}
#pragma region ������ ������
		if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_Q) && m_bItemEquipRight == true)
		{
			// �����տ� ��� �ִ� �����۸� QŰ�� ������ ����
			ITEMTYPEID ItemType = dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemTag();
			if (ItemType.iCount == 1)
			{
				// �Ѱ� ������ �־��� ��
				m_pInventory->delete_FindItem(ItemType); // �κ��丮 ������ �ش� �������� ã�� ����.

				// ���� ���� ���� ǥ�� ����
				Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
				dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(false);

				// ������ UI ���ο����� �ش� ������ UI�� ã�� ����.
				Engine::UIManager()->Delete_FindItemUI(ItemType);

				// 1���� ������ �־��⿡ �������� ���� ���� X, ���� �������� ��� ���� �ʱ�ȭ
				vector<CGameObject*>&vecWorldItem = m_pInventory->Get_DropInven();

				auto FindDropItem = [&ItemType](CGameObject* Item)
				{
					if (CItem* ItemCast = dynamic_cast<CItem*>(Item))
						return ItemCast->Get_ItemTag().eItemID == ItemType.eItemID;

					return false;
				};

				auto Itemiter = find_if(vecWorldItem.begin(), vecWorldItem.end(), FindDropItem);

				if (Itemiter != vecWorldItem.end())
				{
					dynamic_cast<CItem*>(*Itemiter)->Set_WorldItem(true);
					dynamic_cast<CItem*>(*Itemiter)->Set_DropItem(true);
					dynamic_cast<CItem*>(*Itemiter)->Set_BillBoard();
					vecWorldItem.erase(Itemiter);
				}


				m_pCurrentEquipItemRight = nullptr;
				m_pPrevEquipItemRight    = nullptr;
				m_bItemEquipRight        = false;
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
#pragma endregion ������ ������

		// 1 2 3 4 5 ���Կ� �ִ� ������ ���(�Ҹ� �Ǵ� ��) �� ���� + �����ϱ�
		if (Engine::InputDev()->Key_Down(DIK_1))
		{
			Use_SlotItem(KEYSLOT_ONE);
		}
		else if (Engine::InputDev()->Key_Down(DIK_2))
		{
			Use_SlotItem(KEYSLOT_TWO);
		}
		else if (Engine::InputDev()->Key_Down(DIK_3))
		{
			Use_SlotItem(KEYSLOT_THREE);
		}
		else if (Engine::InputDev()->Key_Down(DIK_4))
		{
			Use_SlotItem(KEYSLOT_FOUR);
		}
		else if (Engine::InputDev()->Key_Down(DIK_5))
		{
			Use_SlotItem(KEYSLOT_FIVE);
		}
	}
}

void CPlayer::Use_SlotItem(INVENKEYSLOT _SlotNum)
{
	CGameObject* SlotItemObj = m_pInventory->Get_KeySlotObject(_SlotNum);

	if (SlotItemObj != nullptr) {
		ITEMTYPEID ItemType = dynamic_cast<CItem*>(SlotItemObj)->Get_ItemTag();

#pragma region �ش� ������ ���� ��ȣ ã��
		// �ش� ������ ���� ��ȣ ã��
		_uint ItemSlotNumber = 0;
		switch (ItemType.eItemID)
		{
		case GENERAL_SHIELD:
			ItemSlotNumber = 1;
			break;
		case GENERAL_LAMP:
			ItemSlotNumber = 1;
			break;
		case GENERAL_BEER:
			ItemSlotNumber = 1;
			break;
		case EQUIP_OLDHELMET:
			ItemSlotNumber = 0;
			break;
		case EQUIP_OLDARMOR:
			ItemSlotNumber = 2;
			break;
		case EQUIP_OLDTROUSERS:
			ItemSlotNumber = 4;
			break;
		case EQUIP_IRONHELMET:
			ItemSlotNumber = 0;
			break;
		case EQUIP_IRONARMOR:
			ItemSlotNumber = 2;
			break;
		case EQUIP_IRONTROUSERS:
			ItemSlotNumber = 4;
			break;
		case EQUIP_SMALLSILVERRING:
			ItemSlotNumber = 3;
			break;
		case EQUIP_BIGSILVERRING:
			ItemSlotNumber = 3;
			break;
		case EQUIP_SILVERNECKLACE:
			ItemSlotNumber = 5;
			break;
		case EQUIP_SMALLGOLDRING:
			ItemSlotNumber = 3;
			break;
		case EQUIP_BIGGOLDRING:
			ItemSlotNumber = 3;
			break;
		case EQUIP_GOLDNECKLACE:
			ItemSlotNumber = 5;
			break;
		}
#pragma endregion �ش� ������ ���� ��ȣ ã��

#pragma region �ش� ������ �������� ���� �տ� �����ϴ� Ÿ���� ���
		if (ItemType.eItemType == ITEMTYPE_WEAPONITEM) // ���� �� ���� ������ Ÿ��
		{
			if (!m_bItemEquipRight) // ���� �տ� �����ϰ� �ִ� ���°� �ƴ� �� 
			{
				// �����ϴ� ���� ������ ��� Ÿ���� ������ ��� �޼� ���� ���� ����
				if (ItemType.eItemID == WEAPON_BOW || ItemType.eItemID == WEAPON_EPICBOW) {
					if (m_bItemEquipLeft) {
						// �̵� ��Ű���� �޼� ������ UI�� �����´�.
						CGameObject* pItemUIObject = Engine::UIManager()->Get_ItemUI(dynamic_cast<CItem*>(Get_CurrentEquipLeft())->Get_ItemTag().eItemID);
						if (pItemUIObject != nullptr) {

							UIOBJECTTTAG UIStartObjID;
							_uint        UIStartNumber;
							dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
							dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_Child());
							dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_Child(nullptr);
							dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_EmptyBool(true);

							// ����ִ� �������� ���� �̵�
							Engine::UIManager()->ReplayAdd_ItemGameobject(pItemUIObject);

							// �κ��丮�� ��ġ �̵�
							UIOBJECTTTAG UIGoObjID;
							_uint        UIGoNumber;
							dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_UIObjID(UIGoObjID, UIGoNumber);
							m_pInventory->GoSwitch_InvenItem(ItemType, UIStartObjID, UIStartNumber, UIGoObjID, UIGoNumber);

							if (UIGoObjID == UIID_SLOTBASIC || UIGoObjID == UIID_SLOTEMPTY && Engine::UIManager()->m_bInven) {
								pItemUIObject->Set_Dead(false);
							}
							else {
								pItemUIObject->Set_Dead(true);
							}
						}

						Set_PrevEquipLeft(Get_CurrentEquipLeft());
						Set_CurrentEquipLeft(nullptr);
						Set_ItemEquipLeft(false);
					}
				}

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
					// �����ϴ� ���� ������ ��� Ÿ���� ������ ��� �޼� ���� ���� ����
					if (ItemType.eItemID == WEAPON_BOW || ItemType.eItemID == WEAPON_EPICBOW) {
						if (m_bItemEquipLeft) {
							// �̵� ��Ű���� �޼� ������ UI�� �����´�.
							CGameObject* pItemUIObject = Engine::UIManager()->Get_ItemUI(dynamic_cast<CItem*>(Get_CurrentEquipLeft())->Get_ItemTag().eItemID);
							if (pItemUIObject != nullptr) {

								UIOBJECTTTAG UIStartObjID;
								_uint        UIStartNumber;
								dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
								dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_Child());
								dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_Child(nullptr);
								dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_EmptyBool(true);

								// ����ִ� �������� ���� �̵�
								Engine::UIManager()->ReplayAdd_ItemGameobject(pItemUIObject);

								// �κ��丮�� ��ġ �̵�
								UIOBJECTTTAG UIGoObjID;
								_uint        UIGoNumber;
								dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_UIObjID(UIGoObjID, UIGoNumber);
								m_pInventory->GoSwitch_InvenItem(ItemType, UIStartObjID, UIStartNumber, UIGoObjID, UIGoNumber);

								if (UIGoObjID == UIID_SLOTBASIC || UIGoObjID == UIID_SLOTEMPTY && Engine::UIManager()->m_bInven) {
									pItemUIObject->Set_Dead(false);
								}
								else {
									pItemUIObject->Set_Dead(true);
								}
							}

							Set_PrevEquipLeft(Get_CurrentEquipLeft());
							Set_CurrentEquipLeft(nullptr);
							Set_ItemEquipLeft(false);
						}
					}

					// �����Ϸ��� Ÿ�԰� �ٸ� Ÿ���� ��� ���� �� ���� ������ ���� ���� �� �ش� ���������� �� ���� + UI ���� ���� �������� ����
					m_bItemEquipRight = true;
					Set_CurrentEquipRight(SlotItemObj);

					ITEMTYPEID PrevItemType = dynamic_cast<CItem*>(Get_PrevEquipRight())->Get_ItemTag(); // ���� ������ ������ �±װ����.
					Engine::CGameObject* PrevFindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(PrevItemType);

					UIOBJECTTTAG UIPrevObjID;
					_uint        UIPrevNumber;
					dynamic_cast<CTempUI*>(PrevFindSlotObj)->Get_UIObjID(UIPrevObjID, UIPrevNumber);

					if (UIPrevObjID == UIID_SLOTBASIC) {
						dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(PrevFindSlotObj))->Set_FindSlot(false);
					}
					else if (UIPrevObjID == UIID_SLOTEMPTY) {
						dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(PrevFindSlotObj))->Set_FindSlot(false);
					}


					Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
					UIOBJECTTTAG UIFindObjID;
					_uint        UIFindNumber;
					dynamic_cast<CTempUI*>(FindSlotObj)->Get_UIObjID(UIFindObjID, UIFindNumber);

					if (UIFindObjID == UIID_SLOTBASIC) {
						dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
					}
					else if (UIFindObjID == UIID_SLOTEMPTY) {
						dynamic_cast<CUIemptyslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
					}

					Set_PrevEquipRight(SlotItemObj);
				}
			}
		}
#pragma endregion �ش� ������ �������� ���� �տ� �����ϴ� Ÿ���� ���

#pragma region �ش� ������ �������� �� �տ� �����ϴ� Ÿ���� ���
		else if (ItemType.eItemType == ITEMTYPE_GENERALITEM) 
		{
			if (!m_bItemEquipLeft) // �� �տ� �����ϰ� �ִ� ���°� �ƴ� �� 
			{
				CGameObject* pItemUIObject = Engine::UIManager()->Get_ItemUI(ItemType.eItemID);
				CGameObject* GoSlotObject  = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, ItemSlotNumber);
				
				if (dynamic_cast<CTempUI*>(GoSlotObject)->Get_EmptyBool()) { // �ش� ������ ��������� �ش� �������� ���� �̵�
					if (pItemUIObject != nullptr) {

						UIOBJECTTTAG UIStartObjID;
						_uint        UIStartNumber;
						dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
						dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_Child());
						dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_Child(nullptr);
						dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_EmptyBool(true);

						m_pInventory->GoSwitch_InvenItem(ItemType, UIStartObjID, UIStartNumber, UIID_SLOTEQUIPMENT, ItemSlotNumber);

						pItemUIObject->m_pTransform->m_vInfo[INFO_POS].x = GoSlotObject->m_pTransform->m_vInfo[INFO_POS].x;
						pItemUIObject->m_pTransform->m_vInfo[INFO_POS].y = GoSlotObject->m_pTransform->m_vInfo[INFO_POS].y;
						dynamic_cast<CTempUI*>(pItemUIObject)->WorldMatrix(pItemUIObject->m_pTransform->m_vInfo[INFO_POS].x, pItemUIObject->m_pTransform->m_vInfo[INFO_POS].y, pItemUIObject->m_pTransform->m_vLocalScale.x, pItemUIObject->m_pTransform->m_vLocalScale.y);

						dynamic_cast<CTempUI*>(pItemUIObject)->Set_Parent(GoSlotObject);
						dynamic_cast<CTempUI*>(GoSlotObject)->Set_Child(pItemUIObject);
						dynamic_cast<CTempUI*>(GoSlotObject)->Set_EmptyBool(false);

					    if (!Engine::UIManager()->m_bInven && !Engine::UIManager()->m_bStat) {
							Engine::UIManager()->Hide_InvenItem(0); // �⺻ ���� ���� ��� ��
					    }
						else if (Engine::UIManager()->m_bStat) {
							Engine::UIManager()->Hide_InvenItem(1); // ���� ���Ը� ��
						}
					}
				}
			}
			else // �� �տ� �����ϰ� �ִ� ������ �� ���� ����Ī
			{
				// ���� ������ ������ ���� ������ ������ ����
				Set_PrevEquipLeft(Get_CurrentEquipLeft());
				ITEMTYPEID PrevItemType = dynamic_cast<CItem*>(Get_PrevEquipLeft())->Get_ItemTag();
				
				// ������ �ִ� ������ UI �� ���� ã�ƿ�.
				ITEMTYPEID   UIPrevItemID     = PrevItemType;
				CGameObject* UIPrevUIItem     = Engine::UIManager()->Get_ItemUI(UIPrevItemID.eItemID);
				CGameObject* UIPrevParentSlot = dynamic_cast<CTempUI*>(UIPrevUIItem)->Get_Parent();
				UIOBJECTTTAG UIPrevSlotObjID;
				_uint        UIPrevSlotNumber;
				dynamic_cast<CTempUI*>(UIPrevParentSlot)->Get_UIObjID(UIPrevSlotObjID, UIPrevSlotNumber);

				// ���� ���� ������ ������ UI �� ���� ã�ƿ�.
				ITEMTYPEID   UINewItemID     = ItemType;
				CGameObject* UINewUIItem     = Engine::UIManager()->Get_ItemUI(ItemType.eItemID);
				CGameObject* UINewParentSlot = dynamic_cast<CTempUI*>(UINewUIItem)->Get_Parent();
				UIOBJECTTTAG UINewSlotObjID;
				_uint        UINewSlotNumber;
				dynamic_cast<CTempUI*>(UINewParentSlot)->Get_UIObjID(UINewSlotObjID, UINewSlotNumber);

				// ���Կ� ���� �ڽ��� ������ ���� �ڽ� ������ ����
				dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_BeforeChild(dynamic_cast<CTempUI*>(UIPrevParentSlot)->Get_Child());
				dynamic_cast<CTempUI*>(UINewParentSlot)->Set_BeforeChild(dynamic_cast<CTempUI*>(UINewParentSlot)->Get_Child());

				// ���� ��ġ �̵�
				UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].x = UINewParentSlot->m_pTransform->m_vInfo[INFO_POS].x;
				UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].y = UINewParentSlot->m_pTransform->m_vInfo[INFO_POS].y;
				dynamic_cast<CUIitem*>(UIPrevUIItem)->WorldMatrix(UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].x, UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].y, UIPrevUIItem->m_pTransform->m_vLocalScale.x, UIPrevUIItem->m_pTransform->m_vLocalScale.y);

				UINewUIItem->m_pTransform->m_vInfo[INFO_POS].x = UIPrevParentSlot->m_pTransform->m_vInfo[INFO_POS].x;
				UINewUIItem->m_pTransform->m_vInfo[INFO_POS].y = UIPrevParentSlot->m_pTransform->m_vInfo[INFO_POS].y;
				dynamic_cast<CUIitem*>(UINewUIItem)->WorldMatrix(UINewUIItem->m_pTransform->m_vInfo[INFO_POS].x, UINewUIItem->m_pTransform->m_vInfo[INFO_POS].y, UINewUIItem->m_pTransform->m_vLocalScale.x, UINewUIItem->m_pTransform->m_vLocalScale.y);

				// ���� �θ� �ڽ� ����
				if (UINewSlotObjID == UIID_SLOTEQUIPMENT)
				{
					dynamic_cast<CTempUI*>(UIPrevUIItem)->Set_Parent(UINewParentSlot);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_NextChild(UIPrevUIItem);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(false);

					dynamic_cast<CUIitem*>(UINewUIItem)->Set_Parent(UIPrevParentSlot);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(UINewUIItem);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(false);
				}
				else if (UIPrevSlotObjID == UIID_SLOTEQUIPMENT)
				{
					dynamic_cast<CTempUI*>(UIPrevUIItem)->Set_Parent(UINewParentSlot);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(UIPrevUIItem);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(false);

					dynamic_cast<CUIitem*>(UINewUIItem)->Set_Parent(UIPrevParentSlot);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_NextChild(UINewUIItem);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(false);
				}
				else
				{
					dynamic_cast<CTempUI*>(UIPrevUIItem)->Set_Parent(UINewParentSlot);;
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(UIPrevUIItem);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(false);

					dynamic_cast<CUIitem*>(UINewUIItem)->Set_Parent(UIPrevParentSlot);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(UINewUIItem);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(false);
				}

				// �̵��� ���� ������ UI�� ���̴� ������ �Ⱥ����� �ϴ� ������ �˻�
				if (!Engine::UIManager()->m_bInven && !Engine::UIManager()->m_bStat) {
					Engine::UIManager()->Show_InvenItem(2);
					Engine::UIManager()->Hide_InvenItem(0); // �⺻ ���� ���� ��� ��
				}
				else if (Engine::UIManager()->m_bStat) {
					Engine::UIManager()->Show_InvenItem(2);
					Engine::UIManager()->Hide_InvenItem(1); // ���� ���Ը� ��
				}

				// ����Ī ���� �� �ش� ���� �κ��丮�� ������Ʈ
				m_pInventory->ExSwitch_InvenItem(UIPrevItemID, UIPrevSlotObjID, UIPrevSlotNumber, UINewItemID, UINewSlotObjID, UINewSlotNumber);
			}
		}
#pragma endregion �ش� ������ �������� �� �տ� �����ϴ� Ÿ���� ���

#pragma region �ش� ������ �������� ������ ���Կ� �����ϴ� Ÿ���� ���
		else if (ItemType.eItemType == ITEMTYPE_EQUIPITEM)   // ������ ���� ���� ������ Ÿ��
		{
			// �ش� ������ ���Կ� ���� �� ��ġ �̵�
			CGameObject* pItemUIObject = Engine::UIManager()->Get_ItemUI(ItemType.eItemID);
			CGameObject* GoSlotObject  = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, ItemSlotNumber);

			// �ش� ������ ��������� �ش� �������� ���� �̵�
			if (dynamic_cast<CTempUI*>(GoSlotObject)->Get_EmptyBool()) { 
				if (pItemUIObject != nullptr) {

					UIOBJECTTTAG UIStartObjID;
					_uint        UIStartNumber;
					dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_UIObjID(UIStartObjID, UIStartNumber);
					dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_BeforeChild(dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Get_Child());
					dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(dynamic_cast<CTempUI*>(pItemUIObject)->Get_Parent())->Set_EmptyBool(true);

					m_pInventory->GoSwitch_InvenItem(ItemType, UIStartObjID, UIStartNumber, UIID_SLOTEQUIPMENT, ItemSlotNumber);

					pItemUIObject->m_pTransform->m_vInfo[INFO_POS].x = GoSlotObject->m_pTransform->m_vInfo[INFO_POS].x;
					pItemUIObject->m_pTransform->m_vInfo[INFO_POS].y = GoSlotObject->m_pTransform->m_vInfo[INFO_POS].y;
					dynamic_cast<CTempUI*>(pItemUIObject)->WorldMatrix(pItemUIObject->m_pTransform->m_vInfo[INFO_POS].x, pItemUIObject->m_pTransform->m_vInfo[INFO_POS].y, pItemUIObject->m_pTransform->m_vLocalScale.x, pItemUIObject->m_pTransform->m_vLocalScale.y);

					dynamic_cast<CTempUI*>(pItemUIObject)->Set_Parent(GoSlotObject);
					dynamic_cast<CTempUI*>(GoSlotObject)->Set_Child(pItemUIObject);
					dynamic_cast<CTempUI*>(GoSlotObject)->Set_EmptyBool(false);

					if (!Engine::UIManager()->m_bInven && !Engine::UIManager()->m_bStat) {
						Engine::UIManager()->Hide_InvenItem(0); // �⺻ ���� ���� ��� ��
					}
					else if (Engine::UIManager()->m_bStat) {
						Engine::UIManager()->Hide_InvenItem(1); // ���� ���Ը� ��
					}
				}
			}
			else // �ش� ���Կ� �������� �ִ� ��� ���� ����Ī
			{
				// ������ �ִ� ������ UI �� ���� ã�ƿ�.
				ITEMTYPEID   UIPrevItemID = dynamic_cast<CUIitem*>(dynamic_cast<CTempUI*>(GoSlotObject)->Get_Child())->Get_ItemTag();
				CGameObject* UIPrevUIItem = Engine::UIManager()->Get_ItemUI(UIPrevItemID.eItemID);
				CGameObject* UIPrevParentSlot = dynamic_cast<CTempUI*>(UIPrevUIItem)->Get_Parent();
				UIOBJECTTTAG UIPrevSlotObjID;
				_uint        UIPrevSlotNumber;
				dynamic_cast<CTempUI*>(UIPrevParentSlot)->Get_UIObjID(UIPrevSlotObjID, UIPrevSlotNumber);

				// ���� ���� ������ ������ UI �� ���� ã�ƿ�.
				ITEMTYPEID   UINewItemID = ItemType;
				CGameObject* UINewUIItem = Engine::UIManager()->Get_ItemUI(ItemType.eItemID);
				CGameObject* UINewParentSlot = dynamic_cast<CTempUI*>(UINewUIItem)->Get_Parent();
				UIOBJECTTTAG UINewSlotObjID;
				_uint        UINewSlotNumber;
				dynamic_cast<CTempUI*>(UINewParentSlot)->Get_UIObjID(UINewSlotObjID, UINewSlotNumber);

				// ���Կ� ���� �ڽ��� ������ ���� �ڽ� ������ ����
				dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_BeforeChild(dynamic_cast<CTempUI*>(UIPrevParentSlot)->Get_Child());
				dynamic_cast<CTempUI*>(UINewParentSlot)->Set_BeforeChild(dynamic_cast<CTempUI*>(UINewParentSlot)->Get_Child());

				// ���� ��ġ �̵�
				UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].x = UINewParentSlot->m_pTransform->m_vInfo[INFO_POS].x;
				UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].y = UINewParentSlot->m_pTransform->m_vInfo[INFO_POS].y;
				dynamic_cast<CUIitem*>(UIPrevUIItem)->WorldMatrix(UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].x, UIPrevUIItem->m_pTransform->m_vInfo[INFO_POS].y, UIPrevUIItem->m_pTransform->m_vLocalScale.x, UIPrevUIItem->m_pTransform->m_vLocalScale.y);

				UINewUIItem->m_pTransform->m_vInfo[INFO_POS].x = UIPrevParentSlot->m_pTransform->m_vInfo[INFO_POS].x;
				UINewUIItem->m_pTransform->m_vInfo[INFO_POS].y = UIPrevParentSlot->m_pTransform->m_vInfo[INFO_POS].y;
				dynamic_cast<CUIitem*>(UINewUIItem)->WorldMatrix(UINewUIItem->m_pTransform->m_vInfo[INFO_POS].x, UINewUIItem->m_pTransform->m_vInfo[INFO_POS].y, UINewUIItem->m_pTransform->m_vLocalScale.x, UINewUIItem->m_pTransform->m_vLocalScale.y);

				// ���� �θ� �ڽ� ����
				if (UINewSlotObjID == UIID_SLOTEQUIPMENT)
				{
					dynamic_cast<CTempUI*>(UIPrevUIItem)->Set_Parent(UINewParentSlot);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_NextChild(UIPrevUIItem);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(false);

					dynamic_cast<CUIitem*>(UINewUIItem)->Set_Parent(UIPrevParentSlot);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(UINewUIItem);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(false);
				}
				else if (UIPrevSlotObjID == UIID_SLOTEQUIPMENT)
				{
					dynamic_cast<CTempUI*>(UIPrevUIItem)->Set_Parent(UINewParentSlot);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(UIPrevUIItem);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(false);

					dynamic_cast<CUIitem*>(UINewUIItem)->Set_Parent(UIPrevParentSlot);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_NextChild(UINewUIItem);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(false);
				}
				else
				{
					dynamic_cast<CTempUI*>(UIPrevUIItem)->Set_Parent(UINewParentSlot);;
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_Child(UIPrevUIItem);
					dynamic_cast<CTempUI*>(UINewParentSlot)->Set_EmptyBool(false);

					dynamic_cast<CUIitem*>(UINewUIItem)->Set_Parent(UIPrevParentSlot);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(nullptr);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(true);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_Child(UINewUIItem);
					dynamic_cast<CTempUI*>(UIPrevParentSlot)->Set_EmptyBool(false);
				}

				// �̵��� ���� ������ UI�� ���̴� ������ �Ⱥ����� �ϴ� ������ �˻�
				if (!Engine::UIManager()->m_bInven && !Engine::UIManager()->m_bStat) {
					Engine::UIManager()->Show_InvenItem(2);
					Engine::UIManager()->Hide_InvenItem(0); // �⺻ ���� ���� ��� ��
				}
				else if (Engine::UIManager()->m_bStat) {
					Engine::UIManager()->Show_InvenItem(2);
					Engine::UIManager()->Hide_InvenItem(1); // ���� ���Ը� ��
				}

				// ����Ī ���� �� �ش� ���� �κ��丮�� ������Ʈ
				m_pInventory->ExSwitch_InvenItem(UIPrevItemID, UIPrevSlotObjID, UIPrevSlotNumber, UINewItemID, UINewSlotObjID, UINewSlotNumber);
			}
		}
#pragma endregion �ش� ������ �������� ������ ���Կ� �����ϴ� Ÿ���� ���

#pragma region �ش� ������ �������� ȿ�� ���� �� �Ҹ��ϴ� �������� ���
		else if (ItemType.eItemType == ITEMTYPE_EATITEM)    // HP ȸ�� ������ Ÿ��
		{
			// �ش� �������� ȸ�� ���� ���� HP ȸ�� �� ������ �Ҹ�
			// ������ ȿ�� ����
			Eating(dynamic_cast<CItem*>(SlotItemObj)->Get_ItemStat());

			if (ItemType.iCount == 1)
			{
				m_pInventory->delete_FindItem(ItemType);
				Engine::UIManager()->Delete_FindItemUI(ItemType);
			}
			else if (ItemType.iCount > 1)
			{
				ItemType.iCount = 1;
				m_pInventory->delete_FindItem(ItemType);
				Engine::UIManager()->Delete_FindItemUI(ItemType);
			}
		}
		else if (ItemType.eItemType == ITEMTYPE_POTIONITEM) // �پ��� ���� ������ Ÿ��
		{
			// �ش� ������ �ɷ� �� ȿ�� ���� �� ������ �Ҹ�
			// ������ ȿ�� ����
			switch (ItemType.eItemID)
			{
			case ITEMID::EAT_POTION2:
				dynamic_cast<CHpPotion*>(SlotItemObj)->Set_Heal(true);
				dynamic_cast<CHpPotion*>(SlotItemObj)->Set_Use(true);
				break;
			case ITEMID::EAT_POTION5:
				dynamic_cast<CRandomPotion*>(SlotItemObj)->Set_Use(true);
				dynamic_cast<CRandomPotion*>(SlotItemObj)->Set_RandomUse(true);
				break;
			case ITEMID::EAT_POTION7:
				dynamic_cast<CHolyWater*>(SlotItemObj)->Set_Heal(true);
				break;
			}
		}
#pragma endregion �ش� ������ �������� ȿ�� ���� �� �Ҹ��ϴ� �������� ���
	}
}

void CPlayer::PoisonDamage(const _float& fTimeDelta)
{
	if (!m_bPoisoning) { return; }
	m_fPoisoningTime += fTimeDelta;
	if (1.5f < m_fPoisoningTime)
	{
		m_fPoisoningTime = 0.f;
		m_pStat->Get_Stat()->fHP -= 1.f;
		m_fPoisoningTime = 0.f;
		++m_iPosingingCount;
	}
	if (2 < m_iPosingingCount)
	{
		m_iPosingingCount = 0;
		m_bPoisoning = false;
	}
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTER
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::TRAP
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::TRIGERBLOCK)
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
			if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
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

	if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::ITEM)
	{
		if (dynamic_cast<CItem*>(_pOther->Get_Host())->Get_WorldItem())
		{
			if(dynamic_cast<CItem*>(_pOther->Get_Host())->Get_ItemTag().eItemType != ITEMTYPE::ITEMTYPE_QUEST)
				Create_Item(_pOther);
		}
	}
}

void CPlayer::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTER
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::MONSTERBULLET
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::ITEM
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::TRAP
		&& _pOther->Get_Host()->Get_ObjectTag() != OBJECTTAG::TRIGERBLOCK)
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
			if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::MONSTER)
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
	//if (_pOther->Get_Host()->Get_ObjectTag() == OBJECTTAG::BLOCK)
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

void CPlayer::IsDrunk()
{

	m_fDrunkTime += Engine::Get_TimeDelta(L"Timer_FPS60");

	if (m_fDrunkTime <= 0.5)
	{
		_matrix matRot;

		_vec3 vUp = *D3DXVec3Cross(&vUp, &m_pTransform->m_vInfo[INFO_LOOK], &m_pTransform->m_vInfo[INFO_RIGHT]);
		_vec3 vRight = *D3DXVec3Cross(&vRight, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);
		_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];

		D3DXMatrixRotationAxis(&matRot, &vLook, 0.1 * Engine::Get_TimeDelta(L"Timer_FPS60"));
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_UP], &matRot);
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_RIGHT], &m_pTransform->m_vInfo[INFO_RIGHT], &matRot);
	}
	else if (m_fDrunkTime <= 1.5)
	{
		_matrix matRot;


		_vec3 vUp = *D3DXVec3Cross(&vUp, &m_pTransform->m_vInfo[INFO_LOOK], &m_pTransform->m_vInfo[INFO_RIGHT]);
		_vec3 vRight = *D3DXVec3Cross(&vRight, &m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_LOOK]);
		_vec3 vLook = m_pTransform->m_vInfo[INFO_LOOK];

		_vec3 vNewUp, vNewLook;

		D3DXMatrixRotationAxis(&matRot, &vLook, -(0.1 * Engine::Get_TimeDelta(L"Timer_FPS60")));
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_UP], &m_pTransform->m_vInfo[INFO_UP], &matRot);
		D3DXVec3TransformCoord(&m_pTransform->m_vInfo[INFO_RIGHT], &m_pTransform->m_vInfo[INFO_RIGHT], &matRot);
	}
	else
	{
		_vec3 vDirUp = m_vOriginUp - m_pTransform->m_vInfo[INFO_UP];
		_vec3 vDirRight = m_vOriginRight - m_pTransform->m_vInfo[INFO_RIGHT];

		m_pTransform->m_vInfo[INFO_UP] += vDirUp * 0.5 * Engine::Get_TimeDelta(L"Timer_FPS60");
		m_pTransform->m_vInfo[INFO_RIGHT] += vDirRight * 0.5 * Engine::Get_TimeDelta(L"Timer_FPS60");

		_float fRightDistance = D3DXVec3Length(&vDirRight);
		_float fUpDistance = D3DXVec3Length(&vDirUp);


		if (m_iDrunkCount < 10)
		{
			if (fRightDistance <= 0.05f && fUpDistance <= 0.05f)
			{
				m_fDrunkTime = 0.f;
				++m_iDrunkCount;
			}
		}
		else
		{
			if (fRightDistance <= 0.0025f && fUpDistance <= 0.0025f)
			{
				m_fDrunkTime = 0.f;
				m_bDrunk = false;
			}
		}
	}
}

void CPlayer::Add_Exp(CGameObject* pExp)
{
	// ���͸� ����� �� Exp �߰� -> ���ʹ� exp 6.

	BASICSTAT eStat = *dynamic_cast<CMonster*>(pExp)->Get_BasicStat()->Get_Stat();
	
	m_pStat->Get_Stat()->iExp += eStat.iExp;

	if (m_pStat->Get_Stat()->iExp >= m_pStat->Get_Stat()->iExpMax)
	{
		++m_pStat->Get_Stat()->iLevel;
		// ������ ��Ű�� �ʰ��� �����������.

		_uint iResultExp = m_pStat->Get_Stat()->iExp - m_pStat->Get_Stat()->iExpMax;
		
		m_pStat->Get_Stat()->iExp = iResultExp;

		m_pStat->Get_Stat()->iExpMax *= 4;
		m_pStat->Get_Stat()->fHP = m_pStat->Get_Stat()->fMaxHP;

		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_PLAYER);
		CSoundManager::GetInstance()->PlaySound(L"levelup.mp3", CHANNELID::SOUND_PLAYER, 0.7f);

		Engine::UIManager()->Show_PopupUI(Engine::UIPOPUPLAYER::POPUP_LEVELUP);
	}
}

void CPlayer::Equip_Weapon(CGameObject* pWeapon)
{
	ITEMTYPEID eItemType = dynamic_cast<CItem*>(pWeapon)->Get_ItemTag();

	switch (eItemType.eItemID)
	{
	case ITEMID::WEAPON_SWORD:
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMax + (static_cast<_int>(m_pStat->Get_Stat()->fAttack) % 4);
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMin + (static_cast<_int>(m_pStat->Get_Stat()->fAttack) % 4);
		break;
	case ITEMID::WEAPON_BOW:
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMax + (static_cast<_int>(m_pStat->Get_Stat()->fAgility) % 4);
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMin + (static_cast<_int>(m_pStat->Get_Stat()->fAgility) % 4);
		break;
	case ITEMID::WEAPON_WAND3:
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMax + (static_cast<_int>(m_pStat->Get_Stat()->fMagic) % 4);
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMin + (static_cast<_int>(m_pStat->Get_Stat()->fMagic) % 4);
		break;
	case ITEMID::WEAPON_EPICBOW:
		m_pStat->Get_Stat()->iDamageMax = iDefalutDamageMax + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMax + (static_cast<_int>(m_pStat->Get_Stat()->fAgility) % 4);
		m_pStat->Get_Stat()->iDamageMin = iDefalutDamageMin + dynamic_cast<CItem*>(m_pCurrentEquipItemRight)->Get_ItemStat()->Get_Stat()->iDamageMin + (static_cast<_int>(m_pStat->Get_Stat()->fAgility) % 4);
		break;
	}

	m_bEquipStat = true;
}

void CPlayer::IsAttack(CBasicStat* _MonsterStat)
{
	_int iDamage = 1 + rand() % (m_pStat->Get_Stat()->iDamageMin +
		(m_pStat->Get_Stat()->iDamageMax - m_pStat->Get_Stat()->iDamageMin));

	_int iDeffence = 1 + rand() % (_MonsterStat->Get_Stat()->iArmorMin + (_MonsterStat->Get_Stat()->iArmorMax - _MonsterStat->Get_Stat()->iArmorMin));

	_int iResultDamage = iDamage - iDeffence;

	if (iResultDamage <= 0)
		iResultDamage = 1;

	//cout << iResultDamage << " ������" << endl;

	Set_AttackTick(true);
	_MonsterStat->Take_Damage(iResultDamage);
}

void CPlayer::IsAddiction(const _float& fTimeDelta)
{
	if (m_bIsAddiction)
	{

		m_fAddictionTime += fTimeDelta;

		m_iAddictionCount++;

		if (m_fAddictionTime > 1.5 && m_iAddictionCount < 5)
		{
			this->Get_Stat()->Get_Stat()->fHP -= 1;

			dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Shake_Camera();

			m_fAddictionTime = 0.f;
			m_iAddictionCount = 0;
			m_bIsAddiction = false;
		}
	}
}

void CPlayer::Eating(CBasicStat* _foodStat)
{
	m_pStat->Get_Stat()->fHP += _foodStat->Get_Stat()->fHP;

	if (m_pStat->Get_Stat()->fHP > m_pStat->Get_Stat()->fMaxHP)
		m_pStat->Get_Stat()->fHP = m_pStat->Get_Stat()->fMaxHP;
}

void CPlayer::Create_Item(CCollider* _pOther)
{
	ITEMTYPEID ItemType = dynamic_cast<CItem*>(_pOther->Get_Host())->Get_ItemTag();

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
	case ITEMID::EQUIP_OLDARMOR:
		pItem = CTop::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EQUIP_OLDTROUSERS:
		pItem = CPants::Create(m_pGraphicDev, false);
		break;
	case ITEMID::GENERAL_SHIELD:
		pItem = CShield::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EQUIP_GOLDNECKLACE:
		pItem = CNecklace::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EQUIP_BIGGOLDRING:
		pItem = CRing::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_SLICEDBREAD:
		pItem = CBread::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_APPLE:
		pItem = CApple::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_CHEESE:
		pItem = CCheese::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_RAWMEAT:
		pItem = CMeat::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_COOKEDMEAT:
		pItem = CRoastmeat::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_POTION2:
		pItem = CHpPotion::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_POTION5:
		pItem = CRandomPotion::Create(m_pGraphicDev, false);
		break;
	case ITEMID::EAT_POTION7:
		pItem = CHolyWater::Create(m_pGraphicDev, false);
		break;
	case ITEMID::GENERAL_LAMP:
		pItem = CLamp::Create(m_pGraphicDev, false);
		break;
	case ITEMID::WEAPON_EPICBOW:
		pItem = CEpicBow::Create(m_pGraphicDev, false);
		break;
	case ITEMID::GENERAL_BEER:
		pItem = CBeer::Create(m_pGraphicDev, false);
		break;
	case ITEMID::QUEST_ORB:
		pItem = COrb::Create(m_pGraphicDev, false);
		break;
	}

	m_pInventory->Add_ItemObject(pItem);

	Engine::CGameObject* pGameObjectUI = CUIitem::Create(m_pGraphicDev);
	dynamic_cast<CUIitem*>(pGameObjectUI)->Set_ItemTag(ItemType.eItemType, ItemType.eItemID, ItemType.iCount);

	if (ItemType.eItemType == ITEMTYPE_WEAPONITEM && !m_bItemEquipRight)
	{
		m_bItemEquipRight = true;

		Set_CurrentEquipRight(pItem);
		Set_PrevEquipRight(pItem);
		Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);

		Engine::UIManager()->Add_ItemGameobject(pGameObjectUI);

		Engine::CGameObject* FindSlotObj = Engine::UIManager()->Get_PopupObjectBasicSlot(ItemType);
		dynamic_cast<CUIbasicslot*>(dynamic_cast<CTempUI*>(FindSlotObj))->Set_FindSlot(true);
	}
//	else if (ItemType.eItemType == ITEMTYPE::ITEMTYPE_GENERALITEM && !m_bItemEquipLeft)
//	{
//		m_bItemEquipLeft = true;
//
//		Set_CurrentEquipLeft(pItem);
//		Set_PrevEquipLeft(pItem);
//		Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
//
//#pragma region �ش� ������ ã�Ƽ� ����� �� �ش� ���Կ� �Ҵ� �� ����
//		_uint ItemSlotNumber = 0;
//		switch (ItemType.eItemID)
//		{
//		case GENERAL_SHIELD:
//			ItemSlotNumber = 1;
//			break;
//		case GENERAL_LAMP:
//			ItemSlotNumber = 1;
//			break;
//		case GENERAL_BEER:
//			ItemSlotNumber = 1;
//			break;
//		case EQUIP_OLDHELMET:
//			ItemSlotNumber = 0;
//			break;
//		case EQUIP_OLDARMOR:
//			ItemSlotNumber = 2;
//			break;
//		case EQUIP_OLDTROUSERS:
//			ItemSlotNumber = 4;
//			break;
//		case EQUIP_IRONHELMET:
//			ItemSlotNumber = 0;
//			break;
//		case EQUIP_IRONARMOR:
//			ItemSlotNumber = 2;
//			break;
//		case EQUIP_IRONTROUSERS:
//			ItemSlotNumber = 4;
//			break;
//		case EQUIP_SMALLSILVERRING:
//			ItemSlotNumber = 3;
//			break;
//		case EQUIP_BIGSILVERRING:
//			ItemSlotNumber = 3;
//			break;
//		case EQUIP_SILVERNECKLACE:
//			ItemSlotNumber = 5;
//			break;
//		case EQUIP_SMALLGOLDRING:
//			ItemSlotNumber = 3;
//			break;
//		case EQUIP_BIGGOLDRING:
//			ItemSlotNumber = 3;
//			break;
//		case EQUIP_GOLDNECKLACE:
//			ItemSlotNumber = 5;
//			break;
//		}
//
//		CGameObject* SlotObject = Engine::UIManager()->Get_PopupObject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, UIID_SLOTEQUIPMENT, ItemSlotNumber);
//
//		if (dynamic_cast<CTempUI*>(SlotObject)->Get_EmptyBool()) // �ش� ������ ���������
//		{
//			// ���� ������ ui �������� ����ִ� �ش� ���� ������ ������ ���� �� ���� ��� ����
//			pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].x = SlotObject->m_pTransform->m_vInfo[INFO_POS].x;
//			pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].y = SlotObject->m_pTransform->m_vInfo[INFO_POS].y;
//			dynamic_cast<CTempUI*>(pGameObjectUI)->WorldMatrix(pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].x, pGameObjectUI->m_pTransform->m_vInfo[INFO_POS].y, pGameObjectUI->m_pTransform->m_vLocalScale.x, pGameObjectUI->m_pTransform->m_vLocalScale.y);
//
//			// �������� �θ� ������Ʈ�� �ش� ���� ���
//			dynamic_cast<CTempUI*>(pGameObjectUI)->Set_Parent(SlotObject);
//			// ���� �ڽ� ������Ʈ�� �ش� ������ ��� �� ������� �ʴٴ� ���·� ����
//			dynamic_cast<CTempUI*>(SlotObject)->Set_Child(pGameObjectUI);
//			dynamic_cast<CTempUI*>(SlotObject)->Set_EmptyBool(false);
//
//			Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ITEM, Engine::UILAYER::UI_DOWN, pGameObjectUI);
//			Engine::UIManager()->Hide_InvenItem(0);
//		}
//		else // ��� ���� ������ �׳� �߰�
//		{
//			Engine::UIManager()->AddItemGameobject_UI(pGameObjectUI);
//		}
//#pragma endregion 
//	}
	else
	{
		Engine::EventManager()->CreateObject(pItem, LAYERTAG::GAMELOGIC);
		Engine::UIManager()->Add_ItemGameobject(pGameObjectUI);
	}

	// ���� ������ �����
	EventManager()->DeleteObject(dynamic_cast<CItem*>(_pOther->Get_Host()));
}

void CPlayer::Foot_Sound(const _float& fTimeDelta)
{
	m_fWaterEffectTime += 1.f * fTimeDelta;

	if (SceneManager()->Get_Scene()->Get_SceneTag() == SCENETAG::VILLAGE)
	{
		if (!IsJump() && (m_pStateMachine->Get_State() == STATE::ROMIMG || m_pStateMachine->Get_State() == STATE::ATTACK))
			CSoundManager::GetInstance()->PlaySound(L"feet_default_01.mp3", CHANNELID::SOUND_PLAYER, 1.f);

		if (IsJump() && m_bTestJump)
		{
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_PLAYER);
			m_bTestJump = false;
		}
	}
	else if (SceneManager()->Get_Scene()->Get_SceneTag() == SCENETAG::STAGE)
	{
		if (!IsJump() && (m_pStateMachine->Get_State() == STATE::ROMIMG || m_pStateMachine->Get_State() == STATE::ATTACK) && InWater())
		{
			CSoundManager::GetInstance()->PlaySound(L"splash2.mp3", CHANNELID::SOUND_PLAYER, 1.f);

			if (m_fWaterEffectTime > 2.f) {
				m_fWaterEffectTime = 0.f;
				CGameObject* pGameObject = CEffectWater::Create(m_pGraphicDev, _vec3(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z), 5, 0.05f, EFFECTCOLOR::ECOLOR_WHITE);
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
		}
		
		if (IsJump() && m_bTestJump)
		{
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_PLAYER);
			m_bTestJump = false;
		}
	}
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