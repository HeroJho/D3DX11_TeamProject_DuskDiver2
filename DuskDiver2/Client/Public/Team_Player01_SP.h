#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CTeam_Player01_SP final : public CUI
{
private:
	CTeam_Player01_SP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTeam_Player01_SP(const CTeam_Player01_SP& rhs);
	virtual ~CTeam_Player01_SP() = default;

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
	_float m_fSPGauge = 0.f;
	_int m_iSP = 0;

	class CTeam_SP_Number* m_pUINum = nullptr;

public:
	static CTeam_Player01_SP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END