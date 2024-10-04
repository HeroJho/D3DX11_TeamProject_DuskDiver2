#pragma once
#include "Client_Defines.h"
#include "Creture.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)

class CPlayer04Slash final : public CCreture
{
public:
	typedef struct tagSlashInfo
	{
		_float3 vScale;
		_float4x4 Worldmatrix;		
	}SLASHINFO;		 

private:
	CPlayer04Slash(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CPlayer04Slash(const CPlayer04Slash& rhs);
	virtual ~CPlayer04Slash() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Render_Debug() override;
	virtual void OnCollision(CCollider::OTHERTOMECOLDESC Desc) override;

private:
	HRESULT Ready_Components();

private:
	_float4 m_vColor1 = { 0.6f,0.6f,1.f,1.f };
	_float4 m_vColor2 = { 0.f,0.f,1.f,1.f };
	_float2 m_vRatio = { 1.f,1.f };
	_float m_fBlurPower = 10.f;
	_float m_fLimitAcc = 0.f;
	_float m_fMaxUVIndexX;
	_float m_fMaxUVIndexY;
	_float2 m_vUVFixAcc;
	_float2 m_vUVFixSpeed;
	_float m_fLifeTimeAcc = 0.f;
	_float3 m_vTargetLook;
	_float m_fColAcc = 0.f;
	_bool m_bAttackCol = true;

private:
	CShader*				m_pShaderCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTransform* m_pTransformCom2 = nullptr;
	SLASHINFO* m_pSlashInfo = nullptr;
public:
	static CCreture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END