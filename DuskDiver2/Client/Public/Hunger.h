#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CHunger final : public CUI
{
private:
	CHunger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHunger(const CHunger& rhs);
	virtual ~CHunger() = default;

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

public:
	static CHunger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END