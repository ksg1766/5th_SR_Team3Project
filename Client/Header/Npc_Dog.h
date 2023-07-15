#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CAnimator;
class CStateMachine;

END

class CTerrain;

class CNpc_Dog : public  Engine::CNpc
{
private:
	explicit CNpc_Dog(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Dog(const CNpc_Dog& rhs);
	virtual ~CNpc_Dog();

public:
	virtual HRESULT	Ready_Object() override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object() override;
	virtual void	Render_Object() override;

public:
	void		ForceHeight(_vec3 _vPos);
	void		Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

	virtual void		OnCollisionEnter(CCollider* _pOther);
	virtual void		OnCollisionStay(CCollider* _pOther);
	virtual void		OnCollisionExit(CCollider* _pOther);

private:
	HRESULT	Add_Component();

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture[(_uint)STATE::STATE_END] = {};
	CTerrain* m_pTerrain = nullptr;
	CAnimator* m_pAnimator = nullptr;

public:
	static CNpc_Dog* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};
