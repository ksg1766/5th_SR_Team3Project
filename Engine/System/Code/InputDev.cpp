#include "InputDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInputDev)

Engine::CInputDev::CInputDev(void)
{

}

Engine::CInputDev::~CInputDev(void)
{
	Free();
}

bool CInputDev::Key_Pressing(_ubyte byKeyID)
{
	if (m_byKeyState[byKeyID] & 0x80)
		return true;

	return false;
}

bool CInputDev::Key_Down(_ubyte byKeyID)
{
	// ������ ���� ���� ����, ���� ������ ��

	if (!m_byKeyStateBF[byKeyID] && (m_byKeyState[byKeyID] & 0x80))
	{
		m_byKeyStateBF[byKeyID] = !m_byKeyStateBF[byKeyID];
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (m_byKeyStateBF[i] && !(m_byKeyState[i] & 0x80))
			m_byKeyStateBF[i] = !m_byKeyStateBF[i];
	}

	return false;
}

bool CInputDev::Key_Up(_ubyte byKeyID)
{
	// ������ ���� ���� �ְ�, ����� ������ ���� ���
	if (m_byKeyStateBF[byKeyID] && !(m_byKeyState[byKeyID] & 0x80))
	{
		m_byKeyStateBF[byKeyID] = !m_byKeyStateBF[byKeyID];
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (!m_byKeyStateBF[byKeyID] && (m_byKeyState[byKeyID] & 0x80))
			m_byKeyStateBF[i] = !m_byKeyStateBF[i];
	}

	return false;
}

_bool CInputDev::Mouse_Pressing(MOUSEKEYSTATE eMouseID)
{
	if (m_tMouseState.rgbButtons[eMouseID] & 0x80)
		return true;

	return false;
}

_bool CInputDev::Mouse_Down(MOUSEKEYSTATE eMouseID)
{
	// ������ ���� ���� ����, ���� ������ ��

	if (!m_ubyMouseStateBF[eMouseID] && (m_tMouseState.rgbButtons[eMouseID] & 0x80))
	{
		m_ubyMouseStateBF[eMouseID] = !m_ubyMouseStateBF[eMouseID];
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (m_ubyMouseStateBF[i] && !(m_tMouseState.rgbButtons[i] & 0x80))
			m_ubyMouseStateBF[i] = !m_ubyMouseStateBF[i];
	}

	return false;
}

_bool CInputDev::Mouse_Up(MOUSEKEYSTATE eMouseID)
{
	// ������ ���� ���� �ְ�, ����� ������ ���� ���
	if (m_ubyMouseStateBF[eMouseID] && !(m_tMouseState.rgbButtons[eMouseID] & 0x80))
	{
		m_ubyMouseStateBF[eMouseID] = !m_ubyMouseStateBF[eMouseID];
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (!m_ubyMouseStateBF[eMouseID] && (m_tMouseState.rgbButtons[eMouseID] & 0x80))
			m_ubyMouseStateBF[i] = !m_ubyMouseStateBF[i];
	}

	return false;
}

HRESULT Engine::CInputDev::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput �İ�ü�� �����ϴ� �Լ�
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// Ű���� ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();

	ZeroMemory(m_byKeyStateBF, sizeof(m_byKeyStateBF));

	// ���콺 ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();

	ZeroMemory(m_ubyMouseStateBF, sizeof(m_ubyMouseStateBF));

	return S_OK;
}

void Engine::CInputDev::Update_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

