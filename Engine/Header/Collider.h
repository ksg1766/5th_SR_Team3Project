#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider :
	public CComponent
{
private:
	explicit CCollider();
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

public:
	HRESULT			Ready_Collider();
	virtual void	LateUpdate_Component()	override;
	void			Render_Collider();

	HRESULT			InitOBB(_vec3& _vCenter, _vec3* _vAxisDir, _float* _fAxisLen);

	_vec3&			GetCenterPos() { return m_vCenterPos; }
	_vec3*			GetAxisDir() { return m_vAxisDir; }
	float*			GetAxisLen() { return m_fAxisLen; }

	UINT			GetID() { return m_iID; }

	void			SetCenterPos(_vec3& _vCenterPos) { m_vCenterPos = _vCenterPos; }

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	_vec3			m_vCenterPos;	// ���� �߾��� ��ǥ
	_vec3			m_vAxisDir[3];	//���ڿ� ������ �� ���� ��������
	_float			m_fAxisLen[3];	// ������ ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.

	_uint			m_iID;
	static _uint	g_iNextID;

	int				m_iCol;

	// Only Used in Debug mode
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

public:
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);

private:
	virtual void			Free();
};

END