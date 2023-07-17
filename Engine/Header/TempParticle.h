#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTempParticle : public CGameObject
{
protected:
	explicit CTempParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempParticle(const CTempParticle& rhs);
	virtual ~CTempParticle();

public:
	bool isEmpty() { return m_ParticleList.empty(); }
	bool isDead() {
		// ��ƼŬ�� ��� ���� ��� True�� ����.
		for (const auto& attribute : m_ParticleList)
		{
			if (attribute.bAlive)
				return false;
		}
		return true;
	}

protected:
	virtual void    Initial_Particle(ParticleAttribute* _attribute)PURE; // ��ƼŬ���� ���µǴ� ����� �޶�����.
	virtual void    Add_Particle() {
		ParticleAttribute attribute;
		Initial_Particle(&attribute);
		m_ParticleList.push_back(attribute);
	}


protected:
	HRESULT			Ready_Object(const _tchar* pPath);
	_int			Update_Object(const _float& fTimeDelta);
	void		    LateUpdate_Object(void);
	void		    Render_Object(void);


protected:
	void Get_RandomVector(_vec3* out, _vec3* min, _vec3* max)
	{
		out->x = Get_RandomFloat(min->x, max->x);
		out->y = Get_RandomFloat(min->y, max->y);
		out->z = Get_RandomFloat(min->z, max->z);
	}

	float Get_RandomFloat(float lowBound, float hightBound)
	{
		if (lowBound >= hightBound)
			return lowBound;

		float f = (rand() % 10000) * 0.0001f;

		return (f * (hightBound - lowBound)) + lowBound;
	}

private:
	virtual void Reset_List()
	{
		// ����Ʈ ���� ��� ��ƼŬ �ý��� ����
		for (auto& iter : m_ParticleList) {
			Initial_Particle(&iter);
		}
	}
	virtual	void PreRender_Object();
	virtual	void PostRender_Object();
	virtual void Remove_DeadParticle();

protected:
	EFFECTCOLOR m_ParticleColor = EFFECTCOLOR::ECOLOR_END;

	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DTEXTURE9              m_pTexture;

	_vec3 m_vOrigin;      // ��ƼŬ�� ���۵Ǵ� ��
	float m_fSize;        // ��� ��ƼŬ�� ũ��
	float m_fEmitRate;    // ���ο� ��ƼŬ�� �߰��Ǵ� ����
	int   m_maxParticles; // �־��� �ð� ���� ���� �� �ִ� �ִ� ��ƼŬ�� ��

	DWORD m_vbSize;       // ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ�� �� (���� �����ʹ� ������)
	DWORD m_vbOffset;     // ���ؽ� ���ۿ��� ���縦 ������ ��ƼŬ �� ���� �ܰ���� ������
	DWORD m_vbBatchSize;  // �ϳ��� �ܰ迡 ���ǵ� ��ƼŬ�� ��

	ParticleBoundingBox     m_BoundingBox;      // ��ƼŬ ��� ����
	list<ParticleAttribute> m_ParticleList; // ��ƼŬ �Ӽ��� ����Ʈ : ��ƼŬ�� ����� �����ϰ� �����ϴµ� �̿�.

protected:
	virtual void	Free(void);
};

END
