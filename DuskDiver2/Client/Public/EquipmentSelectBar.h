#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CEquipmentSelectBar final : public CUI
{
private:
	CEquipmentSelectBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEquipmentSelectBar(const CEquipmentSelectBar& rhs);
	virtual ~CEquipmentSelectBar() = default;

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

	_bool m_bRender = true;
	// 그리고 안그리고
	_bool m_bRend = false;

	_uint m_iCurNum = 1;

	_float m_fCountRender = 0.f;
	_float m_fBlend = 0.f;

	_float m_fYPos = 0.f;

public:
	static CEquipmentSelectBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END