#pragma once

#include "Client_Defines.h"
#include "Creture.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
class CTexture;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CCapsuleScene : public CCreture
{
private:
	CCapsuleScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCapsuleScene(const CCapsuleScene& rhs);
	virtual ~CCapsuleScene() = default;

private:

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void OnCollision(CCollider::OTHERTOMECOLDESC Desc) override;

	virtual void Render_Debug() override;


private:
	CShader*				m_pShaderCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	

protected:
	HRESULT Ready_Components();

private:
	XMVECTOR m_OriginPos;
	XMVECTOR m_Pos;

	_int m_iNum = 0;

	_float x = 0, y = 0, z = 0;

	_float m_fAracadeTime = 0.f;
	_float m_fTickTime = 0.f;

	_float m_SceneTime = 0.f;
public:
	static CCapsuleScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CCreture* Clone(void* pArg);
	virtual void Free() override;
};

END