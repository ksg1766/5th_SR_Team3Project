#pragma once
#include "Component.h"

BEGIN(Engine)

class CTexture;
class CAnimation;

class ENGINE_DLL CAnimator :
    public CComponent
{
private:
	explicit CAnimator();
	explicit CAnimator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	HRESULT			Ready_Animator();
	virtual _int	Update_Animator(const _float& fTimeDelta);
	virtual void	LateUpdate_Component()	override;
	virtual void	Render_Animator();

public:
	void	Set_Animation(STATE	eState);

public:
	CAnimation* Get_Animation() { return m_pCurAnimation; }

public:
	HRESULT	Add_Animation(STATE _eState, CAnimation* pAnimation);


private:
	map<STATE, CAnimation*> m_mapAnimation;
	// �ִϸ��̼��� �ʿ� STATE�� Ű������ �����Ѵ�.

	_float	m_fFrame;
	// ������� ����(�ִϸ��̼��� ��� ����)
	STATE	m_eCurState;
	// ���� �ִϸ������� ������Ʈ

	CAnimation* m_pCurAnimation;
	// ���� �ִϸ��̼��� ����Ʈ.
	
	

public:
	static	CAnimator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END