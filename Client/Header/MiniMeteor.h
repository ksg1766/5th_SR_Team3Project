#pragma once
#include "Monster.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CState;
END

class CMiniMeteor :
    public CMonster
{
private:
    explicit CMiniMeteor(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit	CMiniMeteor(const CMiniMeteor& rhs);
    virtual ~CMiniMeteor();

public:
    virtual HRESULT Ready_Object()                          override;
    virtual _int Update_Object(const _float& fTimeDelta)    override;
    virtual void LateUpdate_Object(void)                    override;
    virtual void Render_Object(void)                        override;
public:
    virtual void    Init_Stat()                             override;

    virtual void		OnCollisionEnter(CCollider* _pOther);
    virtual void		OnCollisionStay(CCollider* _pOther);
    virtual void		OnCollisionExit(CCollider* _pOther);
private:
    //�Լ�
private:
    //����
    CCubeBf* m_pCubeBf;
    CTexture* m_pTexture;
public:
    HRESULT		Add_Component();
public:
    static CMiniMeteor* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    virtual void Free();
};
