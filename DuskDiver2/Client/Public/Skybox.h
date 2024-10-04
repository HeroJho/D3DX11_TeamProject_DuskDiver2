#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CModel;
END

BEGIN(Client)

class CSkybox : public CGameObject
{
public:
	typedef struct tagSkyDesc
	{
		_uint iSkyIndex;
		_bool bDisolve = false;
	}SKYDESC;


private:
	CSkybox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkybox(const CSkybox& rhs);
	virtual ~CSkybox() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Render_Debug() override;
	virtual void OnCollision(CCollider::OTHERTOMECOLDESC Desc) override;


private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pDissolveTexture = nullptr;
	
	
protected:
	HRESULT Ready_Components();


private:
	SKYDESC m_Desc;

	_float m_fDissolveAcc = 0.f;
	_float m_fBlurPow = 0.8f;
	_uint test = 0;

public:
	static CSkybox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END