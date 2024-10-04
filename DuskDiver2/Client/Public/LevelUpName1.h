#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CLevelUpName1 final : public CUI
{
private:
	CLevelUpName1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLevelUpName1(const CLevelUpName1& rhs);
	virtual ~CLevelUpName1() = default;

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

	_bool m_bAppear = false;
	_float m_fCut = 0.f;
	_uint m_iCurrentNum = 0;

	_bool m_bCut = false;
public:
	static CLevelUpName1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END