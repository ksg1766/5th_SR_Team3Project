#include "..\..\Header\DynamicBf.h"

CDynamicBf::CDynamicBf()
{
}

CDynamicBf::CDynamicBf(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CDynamicBf::CDynamicBf(const CDynamicBf& rhs)
	:CVIBuffer(rhs)
{
}

CDynamicBf::~CDynamicBf()
{
}

HRESULT CDynamicBf::Ready_Buffer(void)
{
	//m_dwFVF = VTXCUBE::format;
	m_dwFVF = VTXNTX::format;
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	//m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxSize = sizeof(VTXNTX);
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // ������ ������ ũ��
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, // �����ϰ��� ���ؽ� ������ ����(0�� ��� ��������, D3DUSAGE_DYNAMIC ���� �� ���� ����)
		m_dwFVF,		// ���ؽ��� �Ӽ� �ɼ�
		D3DPOOL_DEFAULT, // �޸� ���� ���(���� ������ ��� MANAGED)
		&m_pVB,			// ��� �� �İ�ü
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, // ������ ������ ũ��
		D3DUSAGE_WRITEONLY, // �����ϰ��� ���ؽ� ������ ����(0�� ��� ��������, D3DUSAGE_DYNAMIC ���� �� ���� ����)
		m_IdxFmt,		// ���ؽ��� �Ӽ� �ɼ�
		D3DPOOL_DEFAULT, // �޸� ���� ���(���� ������ ��� MANAGED)
		&m_pIB,			// ��� �� �İ�ü
		NULL),
		E_FAIL);

	//VTXCUBE* pVertex = nullptr;
	VTXNTX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);

	// ����
	pVertex[0].vPosition = { -1.f, 1.f, -1.f };
	pVertex[0].vNormal = { -0.577350259f, 0.577350259f, 0.577350259f };
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = { 1.f, 1.f, -1.f };
	pVertex[0].vNormal = { 0.577350259f, 0.577350259f, 0.577350259f };
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = { 1.f, -1.f, -1.f };
	pVertex[0].vNormal = { 0.577350259f, 0.577350259f, -0.577350259f };
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = { -1.f, -1.f, -1.f };
	pVertex[0].vNormal = { -0.577350259f, 0.577350259f, -0.577350259f };
	pVertex[3].vTexUV = pVertex[3].vPosition;

	// �ĸ�
	pVertex[4].vPosition = { -1.f, 1.f, 1.f };
	pVertex[0].vNormal = { -0.577350259f, -0.577350259f, 0.577350259f };
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = { 1.f, 1.f, 1.f };
	pVertex[0].vNormal = { 0.577350259f, -0.577350259f, 0.577350259f };
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = { 1.f, -1.f, 1.f };
	pVertex[0].vNormal = { 0.577350259f, -0.577350259f, -0.577350259f };
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = { -1.f, -1.f, 1.f };
	pVertex[0].vNormal = { -0.577350259f, -0.577350259f, -0.577350259f };
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CDynamicBf::Render_Buffer(void)
{
	// ����̽� ��ġ���� ���� �׸����� �޸� ���۸� ��ũ ��Ű�� �Լ�
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 1. �� �� ��Ʈ���� ���۸� ������ ���ΰ�
	// 2. � ����� ��ġ�� ������ ���ΰ�
	// 3. ��𼭺��� ������ ���ΰ�
	// 4. � ũ�� ������ ������ ���ΰ�

	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);


	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0,m_dwTriCnt);
	// 1. �ﰢ���� �׸��� ���
	// 2. ���ؽ��� �ε��� ��(�� �� �ε������� �׸� ���ΰ�, ���� ���� ���� ������ ����� �ε��� ���� ���ؼ� �׷���)
	// 3. ���ؽ� ���ۿ� �� �ִ� �ε��� �� �� ��°���� �׸� ���ΰ�
	// 4. ���� ����ϰ��� �ϴ� ���ؽ��� ����
	// 5. �ε��� ���ۿ� �� �ִ� �ε��� �� �� ��°���� �׸� ���ΰ�
	// 6. �׸����� �ﰢ�� ����


	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	// 1. �ﰢ���� �׸��� ���
	// 2. �� ��° ���ؽ����� �׸� ���ΰ�
	// 3. �׸����� �ﰢ�� ����
}

CDynamicBf* CDynamicBf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicBf* pInstance = new CDynamicBf(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("DynamicBf Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CDynamicBf::Clone(void)
{
	return new CDynamicBf(*this);
}

void CDynamicBf::Free(void)
{
	CComponent::Free();
}
