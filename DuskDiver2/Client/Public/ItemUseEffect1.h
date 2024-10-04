#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CItemUseEffect1 final : public CUI
{
private:
	CItemUseEffect1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemUseEffect1(const CItemUseEffect1& rhs);
	virtual ~CItemUseEffect1() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Render_Debug() override;


private:
	HRESULT Ready_Components();
	void Use_Sound();
	void Cancel_Sound();


private:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;

	_bool m_bDebug = false;

	_float m_fTime = 1.29f;

	_bool m_bRender = false;

	_bool m_bRed = false;

	_bool m_bSoundOn = false;

public:
	static CItemUseEffect1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END