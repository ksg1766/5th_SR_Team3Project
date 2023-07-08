#pragma once
#include "Component.h"

BEGIN(Engine)

class CState;
class CGameObject;


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
	// ���¸� �� �����̳ʿ� �߰�
	HRESULT		Add_State(STATE _eState, CState* pState);

public:
	STATE		Get_State() { return m_eCurState; }

protected:
	// ���� ���� ��� �� �����̳�
	map<STATE, CState*>		m_StateMap;

	// � �������� ��� ���ϴ� ����
	CState* m_pCurState = nullptr;
	STATE	m_eCurState = STATE::STATE_END;
	
	// AI ���ο��� STATE ��ȯ �� ��Ÿ������ ����� ����

	
	// � ��ü�� Ŭ�� �ߴ��� Ȯ��

	// �ִϸ��̼� 

public:
	static	CStateMachine* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual	void	Free();
};

END

