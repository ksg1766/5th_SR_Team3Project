#include "stdafx.h"
#include "..\Header\UIOptionsButton.h"

CUIOptionButton::CUIOptionButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempUI(pGraphicDev)
{
}

CUIOptionButton::~CUIOptionButton()
{
}

HRESULT CUIOptionButton::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::UI;
	FAILED_CHECK_RETURN(CTempUI::Ready_Object(), E_FAIL); // �ʱ�ȭ

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = WINCX / 2;
	m_pTransform->m_vInfo[INFO_POS].y = 338.f;
	m_pTransform->m_vLocalScale.x = 95.f;
	m_pTransform->m_vLocalScale.y = 25.f;

	WorldMatrix(m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y);
	
	m_fCurrentImage = 2;

	return S_OK;
}

Engine::_int CUIOptionButton::Update_Object(const _float& fTimeDelta)
{
	if (m_IsDead)
		return 0;

	_int iExit = CTempUI::Update_Object(fTimeDelta);

	return iExit;
}

void CUIOptionButton::LateUpdate_Object(void)
{
	if (m_IsDead)
		return;

	CTempUI::LateUpdate_Object();
}

void CUIOptionButton::Render_Object(void)
{
	if (m_IsDead)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pTextureCom->Render_Texture(m_fCurrentImage);
	m_pBufferCom->Render_Buffer();
}

HRESULT CUIOptionButton::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_EscButtonUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}


void CUIOptionButton::Key_Input(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (OnCollision(pt, m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vLocalScale.x, m_pTransform->m_vLocalScale.y))
	{
		m_fCurrentImage = 3;
		if (Engine::InputDev()->Mouse_Down(DIM_LB)) {
			//DestroyWindow(g_hWnd);
		}
	}
	else
	{
		m_fCurrentImage = 2;
	}
}

void CUIOptionButton::Free()
{
	CTempUI::Free();
}

CUIOptionButton* CUIOptionButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUIOptionButton*	pInstance = new CUIOptionButton(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pInstance;
}
