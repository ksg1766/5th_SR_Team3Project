#include "..\Header\Test_Move.h"
#include "Export_Function.h"
#include "Player.h"

CTest_Move::CTest_Move()
{
}

CTest_Move::CTest_Move(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CTest_Move::~CTest_Move()
{
}

HRESULT CTest_Move::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	m_bJumCoolDown = false;
	m_fJumpCoolTimer = 0.f;

	m_vRandomPos = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

STATE CTest_Move::Update_State(const _float& fTimeDelta)
{
	if (m_pOwner->Get_PrevState() == STATE::ATTACK || m_pOwner->Get_PrevState() == STATE::HIT)
		m_bJumCoolDown = true;

	CPlayer& rPlayer = *dynamic_cast<CPlayer*>(SceneManager()->
		GetInstance()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (m_bJumCoolDown) // �� ���� State�� �����̳� ��Ʈ���ٸ� ��Ÿ���� ����.
	{
		m_fJumpCoolTimer += fTimeDelta; // ��Ÿ�� �ð��� ������� �����ٸ�

		if (m_fJumpCoolTimer >= m_fJumpCoolDuration) // 2�ʺ��� �� ũ�ٸ� �ٽ� ���ݻ��·� ����.
		{
			m_bJumCoolDown = false; // ��ٿ� ����
			m_fJumpCoolTimer = 0.f; // ��Ÿ�� �ð� ���� 
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			return STATE::ATTACK;
		}
		else // �۴ٸ� �÷��̾� �������� ������ ��ġ�� �̵�.
		{
			_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
			// ���� ������ġ.
			_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - vCurrentPos;
			// ���� �÷��̾���� ����.
			
			_float fAngle = (_float)rand() / 2.f * D3DX_PI;
			m_vSavePos.x = (10 * cosf(fAngle)) / 100.f;
			m_vSavePos.z = (10 * -sinf(fAngle)) / 100.f;
			
			
			D3DXVec3Normalize(&m_vSavePos, &m_vSavePos); // ����ȭ ��Ų��. �ٵ� �÷��̾��� ���⿡�� �����ϰ�.
			
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSavePos * 5.f * fTimeDelta;
		}
	}

	// �÷��̾� ���� �ҷ���.

	_vec3 vPlayerPos = rPlayer.m_pTransform->m_vInfo[INFO_POS]; // �÷��̾� ��ġ
	_vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS]; // �÷��̾���� ����

	_float fLength = D3DXVec3Length(&vDistance); // ���� ���⺤���� ��Į�� ����.
	_float fSight = 10.f; // �ϴ� ��� ������ 

	if (fLength < fSight) // �÷��̾�� �Ÿ��� �þߺ��� ������� �����ϵ���.
	{
		return STATE::ATTACK;
	}

	_vec3 vCurrentPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	// ���� ������ġ.
	_vec3 vDir = rPlayer.m_pTransform->m_vInfo[INFO_POS] - vCurrentPos;
	// ���� �÷��̾���� ����.

	_float fAngle = (_float)rand() / 2.f * D3DX_PI;
	_float X = m_vSavePos.x + (10 * cosf(fAngle)) / 100.f;
	_float Z = m_vSavePos.z + (10 * -sinf(fAngle)) / 100.f;

	m_vSavePos = _vec3(X, 0, Z);

	D3DXVec3Normalize(&m_vSavePos, &m_vSavePos); // ����ȭ ��Ų��. �ٵ� �÷��̾��� ���⿡�� �����ϰ�.

	m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSavePos * 5.f * fTimeDelta;

	return STATE::ROMIMG;
}

void CTest_Move::LateUpdate_State()
{
}

void CTest_Move::Render_State()
{
}

CTest_Move* CTest_Move::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CTest_Move* pState = new CTest_Move(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CTest_Move*>(pState);

		MSG_BOX("Create Test Move Failed");
		return nullptr;
	}

	return pState;
}

void CTest_Move::Free()
{
	__super::Free();
}
