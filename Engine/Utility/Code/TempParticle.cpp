#include "..\..\Header\TempParticle.h"

CTempParticle::CTempParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTempParticle::CTempParticle(const CTempParticle& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CTempParticle::~CTempParticle()
{
}

HRESULT CTempParticle::Ready_Object(const _tchar* pPath)
{
	TCHAR	szFileName[128] = L"";
	wsprintf(szFileName, pPath);

	FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&m_pTexture), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(
		m_vbSize * sizeof(ParticleVTX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		FVF_Particle,
		D3DPOOL_DEFAULT,
		&m_pVB,
		0),
		E_FAIL);

	return S_OK;
}

_int CTempParticle::Update_Object(const _float& fTimeDelta)
{
	for (auto& iter : m_ParticleList)
	{
		if (iter.bAlive)
		{
			iter.vPosition += iter.vVelocity * fTimeDelta;

			iter.fAge += fTimeDelta;

			if (iter.fAge > iter.fLifeTime)
				iter.bAlive = false;

			// ��� ������ ����°�?
			if (m_BoundingBox.vMax.x < iter.vPosition.x || m_BoundingBox.vMin.x > iter.vPosition.x ||
				m_BoundingBox.vMax.y < iter.vPosition.y || m_BoundingBox.vMin.y > iter.vPosition.y ||
				m_BoundingBox.vMax.z < iter.vPosition.z || m_BoundingBox.vMin.z > iter.vPosition.z)
			{
				// ��Ȱ��
				Initial_Particle(&iter);
			}
		}
	}

	return 0;
}

void CTempParticle::LateUpdate_Object(void)
{
	Remove_DeadParticle();
}

void CTempParticle::Render_Object(void)
{
	if (!m_ParticleList.empty())
	{
		// ���� ���¸� �����Ѵ�.
		PreRender_Object();

		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_pGraphicDev->SetFVF(FVF_Particle);
		m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(ParticleVTX));

		// ���ؽ� ���۸� ��� ��� ó������ �����Ѵ�.
		if (m_vbOffset >= m_vbSize)
			m_vbOffset = 0;

		ParticleVTX* v = 0;

		m_pVB->Lock(
			m_vbOffset * sizeof(ParticleVTX),
			m_vbBatchSize * sizeof(ParticleVTX),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		// ��� ��ƼŬ�� �������� ������
		for (auto& iter : m_ParticleList)
		{
			if (iter.bAlive)
			{
				// �� �ܰ��� ������ ��ƼŬ�� ���� ���ؽ� ���׸�Ʈ�� �����Ѵ�.
				v->vPosition = iter.vPosition;
				v->Color = (D3DCOLOR)iter.Color;
				v++; // next element;
				numParticlesInBatch++; // �ܰ� ī���͸� ����

				// ���� �ܰ谡 ��� ä���� �ִ°�?
				if (numParticlesInBatch == m_vbBatchSize)
				{
					// ���ؽ� ���۷� ����� ������ �ܰ��� ��ƼŬ���� �׸���
					m_pVB->Unlock();

					m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);

					// �ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���.
					// ���� �ܰ��� ó�� ���������� �̵��Ѵ�.
					m_vbOffset += m_vbBatchSize;

					// ���ؽ� ������ ��踦 �Ѵ� �޸𸮷� �������� �������� �ʴ´�. ��踦 ���� ��� ó������ �����Ѵ�.
					if (m_vbOffset >= m_vbSize)
						m_vbOffset = 0;

					m_pVB->Lock(
						m_vbOffset * sizeof(ParticleVTX),
						m_vbBatchSize * sizeof(ParticleVTX),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // ���� �ܰ踦 ���� ����
				}
			}
		}

		m_pVB->Unlock();

		// (numParticlesInBatch == vbBatchSize) ������ �������� ���Ͽ� ������ �ܰ谡 ���������� �ʴ� ��찡 �߻��� �� �ִ�.
		// �Ϻθ� ä���� �ܰ�� �̰����� ������ �ȴ�.
		if (numParticlesInBatch)
		{
			m_pGraphicDev->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
		}

		// ���� ���
		m_vbOffset += m_vbBatchSize;

		PostRender_Object();
	}
}

void CTempParticle::PreRender_Object(void)
{
	_matrix WorldMatrix;
	D3DXMatrixIdentity(&WorldMatrix);

	WorldMatrix._41 = m_vOrigin.x;
	WorldMatrix._42 = m_vOrigin.y;
	WorldMatrix._43 = m_vOrigin.z;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &WorldMatrix);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&m_fSize));
	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE_MIN, (DWORD)0.0f);

	// �Ÿ��� ���� ��ƼŬ�� ũ�⸦ ����.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_A, (DWORD)0.0f); // �ش� ������ �⺻ ũ�� ������.
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_B, (DWORD)0.0f);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&m_fSize));

	// �ؽ�ó�� ���ĸ� �̿�.
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// z
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

void CTempParticle::PostRender_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CTempParticle::Remove_DeadParticle() // �����ϱ⺸�ٴ� ��Ȱ���ϴ� ���� ȿ������ ���� �ִ�.
{
	for (auto iter = m_ParticleList.begin(); iter != m_ParticleList.end();)
	{
		if ((iter)->bAlive == false)
		{
			iter = m_ParticleList.erase(iter);
		}
		else
			++iter;
	}
}

void CTempParticle::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pTexture);

	CGameObject::Free();
}