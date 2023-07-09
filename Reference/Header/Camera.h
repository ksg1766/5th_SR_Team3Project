#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :
    public CComponent
{
private:
	explicit CCamera();
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	

public:
	HRESULT			Ready_Camera();
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

public:
	_matrix&		Get_ViewMatrix() { return m_matView; }
	_matrix&		Get_ProjMatrix() { return m_matProj; }
	// TODO : �ܺο��� ȣ���� ���� ���ٸ� protected���� �ΰ� Update������ ȣ���ص� ��. ����� ����� �� ��. -> ???
	void			Set_ViewSpcae();
	virtual void	Set_Projection(const CAMERA_TYPE eMode = CAMERA_TYPE::PERSPECTIVE);

private:
	_matrix	m_matView;
	_matrix	m_matProj;

public:
	static CCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END