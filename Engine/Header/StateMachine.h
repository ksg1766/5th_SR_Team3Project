#pragma once
#include "Component.h"

BEGIN(Engine)

class CState;
class CGameObject;
class CAnimator;

class ENGINE_DLL CStateMachine : public CComponent
{
private:
	explicit CStateMachine();
	explicit CStateMachine(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStateMachine(const CStateMachine& rhs);
	virtual	~CStateMachine();

public:
	HRESULT		Ready_StateMachine();
	// ���� ������ ������Ʈ ��ȯ ���� Ÿ���� �޾� ���Ͽ� ���°� ���ߴ��� �Ǵ�?
	void	Update_StateMachine(const _float& fTimeDelta);
	void	LateUpdate_StateMachine();
	void	Render_StateMachine();

public://Animation tap

public:

	// ���¸� ���� �� �������ִ� �Լ�(�ִϸ����� ���� �ٲ۴�.)
	void		Set_State(STATE _eState);
	void		Set_Animator(CAnimator* pAnimator) { m_pAnimator = pAnimator; }
	// ���¸� �� �����̳ʿ� �߰�
	HRESULT		Add_State(STATE _eState, CState* pState);

	virtual CAnimator* Get_Animator() { return m_pAnimator; }
public:
	STATE		Get_State()		{ return m_eCurState; }
	STATE		Get_PrevState() { return m_ePrevState; }

protected:
	// ���� ���� ��� �� �����̳�
	map<STATE, CState*>		m_StateMap;

	// � �������� ��� ���ϴ� ����
	CState* m_pCurState = nullptr;
	CState* m_pPrevState = nullptr;

	STATE	m_eCurState = STATE::STATE_END;
	STATE	m_ePrevState = STATE::STATE_END;
	// AI ���ο��� STATE ��ȯ �� ��Ÿ������ ����� ����

	_bool	m_FirstOn;
	// ���� �� �� Prev�� ���� ����
	
	// � ��ü�� Ŭ�� �ߴ��� Ȯ��

	// �ִϸ��̼� 
	CAnimator*			m_pAnimator = nullptr;

public:
	static	CStateMachine* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual	void	Free();
};

END

