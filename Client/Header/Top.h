#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBasicStat;
class CBillBoard;

END

class CTop : public Engine::CItem
{
private:
	explicit CTop(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTop(const CTop& rhs);
	virtual ~CTop();

public:
	virtual HRESULT Ready_Object(_bool _Item);
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object(void) override;
	virtual void	Render_Object(void) override;

private:
	HRESULT			Add_Component(void);

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:

private:
	CRcTex* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	CBillBoard* m_pBillBoard = nullptr;

public:
	static CTop* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool _Item);

private:
	virtual void Free() override;
};

