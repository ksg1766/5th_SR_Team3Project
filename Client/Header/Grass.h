#pragma once
#include "Environment.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CBillBoard;

END

class CGrass : public CEnvironment
{
private:
	explicit CGrass(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGrass(const CGrass& rhs);
	virtual ~CGrass();

public:
	_uint			Get_GrassNumber()            { return m_iGrassNumber; }
	void			Set_GrassNumber(_uint _iNum) { m_iGrassNumber = _iNum; }

public:
	virtual HRESULT Ready_Object(void)						override;
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void)					override;
	virtual void	Render_Object(void)						override;

private:
	HRESULT			Add_Component(void);

private:
	_uint           m_iGrassNumber = 0;

public:
	static CGrass*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free() override;

};

