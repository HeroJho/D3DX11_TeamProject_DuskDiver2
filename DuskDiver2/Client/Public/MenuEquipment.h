#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CMenuEquipMent final : public CUI
{
private:
	CMenuEquipMent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMenuEquipMent(const CMenuEquipMent& rhs);
	virtual ~CMenuEquipMent() = default;

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

	_uint m_iCurstate = 0;
	_bool m_bSizeUp = false;
	_float m_fSizeUp = 0.f;

public:
	static CMenuEquipMent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END