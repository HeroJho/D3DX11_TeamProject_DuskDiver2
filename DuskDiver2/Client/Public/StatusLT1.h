#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Client)

class CStatusLT1 final : public CUI
{
private:
	CStatusLT1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStatusLT1(const CStatusLT1& rhs);
	virtual ~CStatusLT1() = default;

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

	class CLT1Level* m_pUINum = nullptr;
	class CLT1HP* m_pUINum2 = nullptr;

	_bool m_bDebug = false;

public:
	static CStatusLT1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END