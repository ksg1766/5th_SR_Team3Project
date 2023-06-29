#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidBody :
	public CComponent
{
private:
	explicit CRigidBody();
	explicit CRigidBody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRigidBody(const CRigidBody& rhs);
	virtual ~CRigidBody();

	// CComponent��(��) ���� ��ӵ�
	virtual CComponent * Clone(void) override;

private:
	virtual void			Free();
};

END