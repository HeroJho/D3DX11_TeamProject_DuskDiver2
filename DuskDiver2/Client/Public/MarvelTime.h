#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CMarvelTime final : public CUI
{
private:
	CMarvelTime(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMarvelTime(const CMarvelTime& rhs);
	virtual ~CMarvelTime() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Render_Debug() override;


private:
	HRESULT Ready_Components();

private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	_bool m_bDebug = false;
	_bool m_bRender = false;

	_float m_fTime = 0.f;
	_float m_fAlpha = 0.3f;

	_bool m_bPattern = false;

	_float m_fTotalTime = 30.f;
	_float m_fOriginSize = 0.f;
	_float m_fOriginPos = 0.f;

	_float m_fPattern = 0.f;

	_float m_fTime2 = 0.f;
	_float m_fTime3 = 0.f;
public:
	static CMarvelTime* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END