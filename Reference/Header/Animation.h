#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CAnimator;
class CTexture;

class ENGINE_DLL CAnimation : public CBase 
{
protected:
	explicit CAnimation();
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimation(const CAnimation& rhs);
	virtual	~CAnimation();

public:
	virtual	HRESULT	Ready_Animation(CTexture* pTexture,
		STATE _eState, _float _fFrame, _bool Loop);
	virtual STATE	Update_Animation(const _float& fTimeDelta);
	virtual void	LateUpdate_Animation();
	virtual void	Render_Animation();

	virtual _float	Get_Frame() { return m_fCurFrame; }
	void	Set_Frame(_float _fFrame) { m_fCurFrame = _fFrame; }

public:


public:
	void	Set_Loop(_bool _loop)		{ m_bIsLoop = _loop; }
	void	Set_End(_bool _end)			{ m_bIsEnd = _end; }
	_bool	Get_Loop()					{ return m_bIsLoop; }
	_bool	Get_End()					{ return m_bIsEnd; }

	STATE	  Get_State()					   { return m_eState; }
	CTexture* Get_CurTexture()				   { return m_pCurAnimation; }
	CTexture* Get_PrevTexture()				   { return m_pPrevAnmation; }
	LPDIRECT3DTEXTURE9* Get_GhostiongTexture() { return m_pGhostingRenderTarget; }
	IDirect3DSurface9* Get_OriginalTarget()	   { return m_pOriginalRenderTarget; }

protected:
	// �ִϸ��̼� ���� �Լ�
	CAnimator* m_pOwner = nullptr;
	// ���� ���� �ִ���, (���� ��� ���ϴ� ��)
	CTexture* m_pCurAnimation = nullptr;
	CTexture* m_pPrevAnmation = nullptr;
	LPDIRECT3DTEXTURE9* m_pGhostingRenderTarget = nullptr;
	IDirect3DSurface9* m_pOriginalRenderTarget = nullptr;
	// ���� � �ؽ��ĸ� ��� �ִ� ��
	_float	m_fFrameSpeed;
	// ������ �����ð� �ӵ�.
	_float	m_fCurFrame;
	// ���� ������.

	_bool	m_bIsLoop;
	// �ִϸ��̼��� �����Ұ���
	_bool	m_bIsEnd;
	// �ִϸ��̼��� �����Ұ���

	STATE	m_eState = STATE::STATE_END;

	LPDIRECT3DDEVICE9	m_pGraphicDev;
	// CBase�� ��� �ް� �־ �׷��� ����̽� ������ ���� �޾ƾ� ��.

public:
	static	CAnimation* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTexture* pTexture,
		STATE _eState, _float _fFrame, _bool Loop);
	// �ִϸ��̼��� ������ ��, 
	// 1. ����̽� ����
	// 2. ���� �ؽ�������(Add Component �ܰ迡�� �ؽ��� �迭�� STATE ���·� �����ϸ� ��.)
	// 3. �������� ���� �ӵ�
	// 4. Loop ��ų����, ���Ұ��� ���� 

protected:
	virtual void Free();
};


END
