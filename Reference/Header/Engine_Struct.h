#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexDefault
	{
		_vec3		vPosition;

		static const _ulong	format = D3DFVF_XYZ;
	}VTXDEF;
	
	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

		static const _ulong	format = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;
	}VTXCOL;
	
	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec2		vTexUV;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_TEX1;
	}VTXTEX;
	
	typedef struct tagVertexCubeBfture
	{
		_vec3		vPosition;
		_vec3		vTexUV;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	}VTXCUBE;

	typedef struct tagVertexNormal
	{
		_vec3		vPosition;
		_vec3		vNormal;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_NORMAL;
	}VTXNOM;

	typedef struct tagVertexNormalColor
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_ulong		dwColor;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	}VTXNCO;

	typedef struct tagVertexNormalTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexture;
		static const _ulong	format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	}VTXNTX;

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;
	
	union COLLIDER_ID
	{
		struct
		{
			_uint Left_id;
			_uint Right_id;
		};
		ULONGLONG ID;
	};

	struct tagEvent
	{
		EVENT_TYPE	eEvent;
		DWORD_PTR	lParam;
		DWORD_PTR	wParam;
	};

	typedef struct tagBasicStat {
		_float	fHealth = 4.f;
		_float	fMagic = 4.f;
		_float	fSpeed = 4.f;
		_float	fAttack = 4.f;
		_float	fDeffense = 4.f;
		_float	fAgility = 4.f;
		_int	iExp = 0.f;
		_int	iExpMax = 8.f;
		//STATE	eState		= STATE::IDLE;
	}BASICSTAT;

	typedef struct tagAttack
	{
		_bool	bReturn = false;
		_bool	bAttacking = false;


		_float fMaxDistance;	// ���� ��Ÿ�
		_float fWeaponSpeed;	// ���� �ӵ� 
		_float fReturnSpeed;	// ������ ������ ���ƿ��� �ӵ�

		_vec3	vFoward = _vec3(0.f,0.f,1.f);		// ���溤��
		_vec3	vRotation;		// ȸ�� ���� ���� ���� (Ʈ�������� angle�� �����ϱ� ���ﰢ�Լ��� ���� �� ����)

		_matrix	matRot;		// ���� ��ǥ�迡 ȸ���� ������ ���


		//tagAttack(_float A, _float B, _float _fAlpha)
		//	: fA(A), fB(B), fAlpha(_fAlpha)
		//{
		//	
		//}

		static _float	Interpolate(_float A, _float B, _float fAlpha)
		{
			return A * (1 - fAlpha) + B * fAlpha;
		}

	}MYATTACKINFO;

	struct ITEMTYPEID
	{
		ITEMTYPE eItemType;
		ITEMID   eItemID;
		_uint    iCount;
	};

	typedef struct tagParticleVertex
	{
		D3DXVECTOR3	vPosition;
		D3DCOLOR    Color;

	}ParticleVTX;
	const _ulong	FVF_Particle = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagParticleAttribute
	{
		D3DXVECTOR3	vPosition;
		D3DXVECTOR3	vVelocity;
		D3DXVECTOR3	vAcceleration;
		float       fLifeTime;
		float       fAge;
		D3DCOLOR    Color;
		D3DCOLOR    ColorFade;
		bool        bAlive;

	}ParticleAttribute;

	typedef struct tagBoundingBox
	{
		D3DXVECTOR3	vMin;
		D3DXVECTOR3	vMax;

	}ParticleBoundingBox;
}


#endif // Engine_Struct_h__
