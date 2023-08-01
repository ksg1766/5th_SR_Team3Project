#include "Phantom_Idle.h"
#include "Export_Function.h"
//�÷��̾ �κ��丮, ��Ÿ �����۵� ��Ŭ��� �� ����

CPhantom_Idle::CPhantom_Idle()
{
}

CPhantom_Idle::CPhantom_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CPhantom_Idle::~CPhantom_Idle()
{
}

HRESULT CPhantom_Idle::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CPhantom_Idle::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CPhantom_Idle::LateUpdate_State()
{
}

void CPhantom_Idle::Render_State()
{
}

CPhantom_Idle* CPhantom_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CPhantom_Idle* pState = new CPhantom_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CPhantom_Idle*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CPhantom_Idle::Free()
{
	__super::Free();
}
