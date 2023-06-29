#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGraphicDev;
class CSceneManager;

END
class CMainApp : public CBase 
{
private:		// ������, �Ҹ���
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp(void);
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp();
	void		Render_MainApp();

private:
	HRESULT				SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT				Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CSceneManager** ppManagementClass);

private:
	CGraphicDev*			m_pDeviceClass;
	CSceneManager*			m_pSceneManager;

	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	static CMainApp*		Create();

private:
	virtual void		Free();

};

// 1. �佺���̽� ��ȯ �Լ��� ���� ���� ��� ���� �Լ��� ���� �����Ͷ�
// 2. ���̸��� �̿��Ͽ� ������ �����̸� ������ �Ͷ�