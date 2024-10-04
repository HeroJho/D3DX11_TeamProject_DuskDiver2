#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CCreture;

class CPunchScoreText : public CGameObject
{
private:
	CPunchScoreText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPunchScoreText(const CPunchScoreText& rhs);
	virtual ~CPunchScoreText() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Render_Debug() override;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CCreture*				m_pMonster = nullptr;

protected:
	HRESULT Ready_Components();

private:
	_float4 m_vMonsterPos;
	_bool m_bRender = false;

	_float m_fSizeX, m_fSizeY = 0.f;
	_float m_fNowHp = 0.f;

	_bool m_bLight = false;

	_float m_fTime = 0.f;
	_float m_fAlpha = 1.f;
public:
	static CPunchScoreText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END