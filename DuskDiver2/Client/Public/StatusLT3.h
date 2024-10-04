#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CStatusLT3 final : public CUI
{
private:
	CStatusLT3(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStatusLT3(const CStatusLT3& rhs);
	virtual ~CStatusLT3() = default;

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

	class CLT3Level* m_pUINum = nullptr;
	class CLT3HP* m_pUINum2 = nullptr;

	_bool m_bDebug = false;

public:
	static CStatusLT3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END