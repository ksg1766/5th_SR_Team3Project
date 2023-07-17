#include "..\..\Header\Animation.h"
#include "Texture.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), 
	m_pCurAnimation(nullptr), 
	m_fFrameSpeed(.0f), 
	m_fCurFrame(0.f),
	m_bIsEnd(false),
	m_bIsLoop(false)
{
	m_pGraphicDev->AddRef();
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev),
	m_pOwner(rhs.m_pOwner),
	m_pCurAnimation(rhs.m_pCurAnimation),
	m_fFrameSpeed(rhs.m_fFrameSpeed),
	m_fCurFrame(rhs.m_fCurFrame),
	m_bIsEnd(rhs.m_bIsEnd),
	m_bIsLoop(rhs.m_bIsLoop),
	m_pPrevAnmation(rhs.m_pPrevAnmation),
	m_pGhostingRenderTarget(rhs.m_pGhostingRenderTarget),
	m_pOriginalRenderTarget(rhs.m_pOriginalRenderTarget)
{
}

CAnimation::~CAnimation()
{
	Free();
}

HRESULT CAnimation::Ready_Animation(CTexture* pTexture,
	STATE _eState, _float _fFrame, _bool Loop)
{

	if (pTexture == nullptr)
		return E_FAIL;

	m_pCurAnimation = pTexture;
	// ���� �ؽ��ĸ� �޾ƿ´�
	m_eState = _eState;
	// ������Ʈ �ӽŰ� �Բ� ����ϱ� ���� State�� Ű �� ����.
	m_fFrameSpeed = _fFrame;
	// ��������Ʈ ���� ������ �����ð�
	m_bIsLoop = Loop;
	// Loop�� �������� False�� �ٲٸ� ��������Ʈ�� ���� �� �ְ� ����. End �� ���� �ִ�.


	return S_OK;
}

STATE CAnimation::Update_Animation(const _float& fTimeDelta)
{
	if (!m_bIsEnd)
	m_fCurFrame += m_fFrameSpeed * fTimeDelta;
	// ���⼭ ���� �������� �����ǰ�

	if (m_fCurFrame >= m_pCurAnimation->Get_TextureSize() && !m_bIsEnd) // ���� ������� ������ ����� Ŀ�� ���ǹ� ������ ����.
	{
		if (m_bIsLoop)
			m_fCurFrame = 0.f;
		// ���Դµ� Loop ���¶�°� �ٽ� ��������Ʈ �̹��� �����ڴٴ� ����.

		else
		{
			m_fCurFrame = static_cast<float>(m_pCurAnimation->Get_TextureSize() - 1);
			m_bIsEnd = true;
			// Loop�� �ƴϸ� ����� ���ͼ� ������ End�� ���� ��.
		}
	}

	if (m_pCurAnimation != nullptr)
		m_pPrevAnmation = m_pCurAnimation;

	return STATE::STATE_END; // �Ű澲���� �� ��ȯ �ƹ��� �� ����.
}

void CAnimation::LateUpdate_Animation()
{
}

void CAnimation::Render_Animation()
{
	m_pCurAnimation->Render_Texture((_uint)m_fCurFrame);
	// ���������� ������ ���⼭ �Ϸ� ��.
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTexture* pTexture, STATE _eState, _float _fFrame, _bool Loop)
{
	CAnimation* pAnimation = new CAnimation(pGraphicDev);

	if (FAILED(pAnimation->Ready_Animation(pTexture, _eState, _fFrame, Loop)))
	{
		Safe_Release<CAnimation*>(pAnimation);

		MSG_BOX("Create Animation Failed");
		return nullptr;
	}

	// �ִϸ��̼��� ������ ��, 
	// 1. ����̽� ���� -> ����̽� ������ �����ڿ� �Ѱ��ְ� ������ Ready�� �Ѱ��ּ���
	
	// 2. ���� �ؽ�������(Add Component �ܰ迡�� �ؽ��� �迭�� STATE ���·� �����ϸ� ��.)
	// 3. ���� STATE ���� 
	// 4. �������� ���� �ӵ�
	// 5. Loop ��ų����, ���Ұ��� ���� 

	return pAnimation;
}

void CAnimation::Free()
{
	Safe_Release<LPDIRECT3DDEVICE9>(m_pGraphicDev);
}
