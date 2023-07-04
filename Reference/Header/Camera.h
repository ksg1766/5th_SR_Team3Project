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

public:
	// TODO : �ܺο��� ȣ���� ���� ���ٸ� protected���� �ΰ� Update������ ȣ���ص� ��. ����� ����� �� ��.
	virtual void	SetViewSpcae();
	virtual void	SetProjection();

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