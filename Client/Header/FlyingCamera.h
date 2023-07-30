#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCamera;

END

class CFlyingCamera :
    public CGameObject
{
private:
    explicit        CFlyingCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit        CFlyingCamera(const CFlyingCamera& rhs);
    virtual         ~CFlyingCamera();

public:
    virtual HRESULT Ready_Object(void)                      override;
    virtual _int    Update_Object(const _float& fTimeDelta) override;
    virtual void    LateUpdate_Object(void)                 override;
    virtual void    Render_Object(void) {};

public:
    void		    Shake_Camera();
    void		    Drunk_Camera();

    void		    Set_ShakeForce(_float _ShakeTime, _float _Amplitude, _float _Duration, _float _Frequency);
    void		    Reset_ShakeForce();

public:
    CCamera*        Get_Camera() { return m_pCamera; }
    _int            Get_CameraMode() { return (_int)m_eCameraMode; }
    _bool           Is_MouseFree() { return m_isMouseFree; }

    void            Change_Mode();
    void            Set_MouseFix(_bool _isMouseFree) { m_isMouseFree = _isMouseFree; }

private:
    HRESULT		    Add_Component(void);
    void		    Key_Input(const _float& fTimeDelta);

    void			Mouse_Fix();

private:
    enum CAMERA_MODE { FIRST_PERSON, THIRD_PERSON };

    _bool		    m_bCheck = false;   // ���� �𸣴� ����

    _float			m_fSpeed = 30.f;

//  CTransform*     m_pTransform;	// Transform Attached to this GameObject
    CCamera*        m_pCamera = nullptr;

    CAMERA_MODE		m_eCameraMode;
    _bool			m_isMouseFree = false;

    // Camera Shake
    struct CameraShaking
    {
        _bool		m_bShaking; // ��鸲 ����
        _float		m_fShakeElipsedTime; // ��ø�ð�

        _float		m_fAmplitude; // ����
        _float		m_fDuration;  // ���� �ð�
        _float		m_fFrequency; // ���� �ӵ�

        _vec3		m_vOriginPos; // ���� �ִ� ��ġ
    }m_tCameraShaking;

public:
    static CFlyingCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;
};

