#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CHit_Bar final : public CUI
{
private:
	CHit_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHit_Bar(const CHit_Bar& rhs);
	virtual ~CHit_Bar() = default;

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
	_bool m_bOnce = false;

	_float m_fTimerAcc = 5.f;
	_float m_fShaderTime = 0.f;

public:
	static CHit_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END