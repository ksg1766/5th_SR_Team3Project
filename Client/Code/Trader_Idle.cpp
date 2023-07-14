#include "Trader_Idle.h"
#include "Export_Function.h"
//�÷��̾ �κ��丮, ��Ÿ �����۵� ��Ŭ��� �� ����

CNpc_Trader_Idle::CNpc_Trader_Idle()
{
}

CNpc_Trader_Idle::CNpc_Trader_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CNpc_Trader_Idle::~CNpc_Trader_Idle()
{
}

HRESULT CNpc_Trader_Idle::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CNpc_Trader_Idle::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CNpc_Trader_Idle::LateUpdate_State()
{
}

void CNpc_Trader_Idle::Render_State()
{
}

CNpc_Trader_Idle* CNpc_Trader_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CNpc_Trader_Idle* pState = new CNpc_Trader_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CNpc_Trader_Idle*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CNpc_Trader_Idle::Free()
{
	__super::Free();
}
