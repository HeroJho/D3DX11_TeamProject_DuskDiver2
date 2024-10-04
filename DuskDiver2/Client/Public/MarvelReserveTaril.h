#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_ZRect;
END

BEGIN(Client)

class CMarvelReserveTaril : public CGameObject
{
public:
	typedef struct tagReserveTrailInfo
	{
		_bool	bFixColor;
		_float	fBlurPower;
		_float	fLifeTime;
		_float	fStaticY;
		_float	fScaleX;
		_float2 vRatio;
		_float2 vRatioSpeed;
		_float4 vColor;
		_float4 vColor2;
		_float4 vPosition;
		_float4 vTargetPos;

	}RESERVETRAILINFO;
private:
	CMarvelReserveTaril(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMarvelReserveTaril(const CMarvelReserveTaril& rhs);
	virtual ~CMarvelReserveTaril() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Render_Debug() override;

public:
	void SetUp_ReserveTrail(_float4 vPos, _float4 vTargetPos);
	void Render_OnOff(_bool bOnOff) { m_bRender = bOnOff; }

private:
	RESERVETRAILINFO		m_ReserveTrailInfo;
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_ZRect*		m_pVIBufferCom = nullptr;

private:
	HRESULT Ready_Components();

private:
	_bool		m_bDeadStart = false;
	_float		m_fStartAcc = 0.f;
	_float		m_fDeadAcc = 0.f;
	_float		m_fDist = 0.f;
	_float3		m_vSclae = { 0.f, 0.f, 0.f };
	_float4		m_vTargetLook;
	_bool		m_bOneCreate = true;
	_bool		m_bRender = false;
public:
	static CMarvelReserveTaril* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END