#include "stdafx.h"
#include "..\Public\BeastBreath2.h"
#include "GameInstance.h"
#include "Time_Manager.h"

CBeastBreath2::CBeastBreath2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CBeastBreath2::CBeastBreath2(const CBeastBreath2& rhs)
	: CGameObject(rhs)
{
}

HRESULT CBeastBreath2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBeastBreath2::Initialize(void * pArg)
{
	m_pInfo = (*(BREATHINFO**)pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_bDead = false;
	
	_matrix Matrix = XMMatrixIdentity();
	_matrix Rotation = XMMatrixRotationAxis(_vector{ 1.f,0.f,0.f }, XMConvertToRadians(250.f));
	XMStoreFloat4x4(&m_pInfo->vWorldMatrix, Rotation * Matrix);
	m_pTransformCom->Set_WorldMatrix(m_pInfo->vWorldMatrix);
	m_pTransformCom->Set_Scale(XMLoadFloat3(&m_pInfo->vScale));
	m_vUVFixAcc = { 0.f,0.f };
	m_vUVFixSpeed = m_pInfo->vUvFixSpeed;

	return S_OK;
}

void CBeastBreath2::Tick(_float fTimeDelta)
{
	TIM->Add_TimeObj(CTime_Manager::OBJ_EFFECT, this);
	fTimeDelta *= m_fTMRatio;

	//m_pTransformCom->Set_Scale(XMLoadFloat3(&m_pTransformCom->Get_Scale()) + (XMLoadFloat3(&m_pInfo->vScaleSpeed) * fTimeDelta));

	m_vUVFixAcc.x += m_pInfo->vUvFixSpeed.x * fTimeDelta;
	m_vUVFixAcc.y += m_pInfo->vUvFixSpeed.y * fTimeDelta;

	/*if (m_pInfo->vRatio.x > 0.f)
	{
		m_pInfo->vRatio.x -= m_pInfo->vRatioSpeed.x * fTimeDelta;
		m_pInfo->vRatio.y -= m_pInfo->vRatioSpeed.y * fTimeDelta;
	}

	if (m_pInfo->vRatio.x <= 0.f || m_pInfo->vRatio.y <= 0.f)
	{
		Set_Dead();
	}*/
	
	
}

void CBeastBreath2::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	Compute_CamZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CBeastBreath2::Render()
{
	if (nullptr == m_pModelCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_pInfo->vColor, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor2", &m_pInfo->vColor2, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fRatio1", &m_pInfo->vRatio.x, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fRatio2", &m_pInfo->vRatio.y, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBlurPower", &m_pInfo->fBlurPower, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fUVFixX", &m_vUVFixAcc.x, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fUVFixY", &m_vUVFixAcc.y, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom,i, 3)))
			return E_FAIL;
	}
	return S_OK;
}

void CBeastBreath2::Set_Breath2Info(BREATHINFO Info)
{
}

void CBeastBreath2::Update_Breath2Matrix(_float4x4 vMatrix)
{
	m_pTransformCom->Set_WorldMatrix(vMatrix);
	m_pTransformCom->Set_Scale(XMLoadFloat3(&m_pInfo->vScale));
}

void CBeastBreath2::Fix_Info(FIXINFO Info)
{
	m_pInfo->vColor = Info.vColor;
	m_pInfo->vColor2 = Info.vColor2;
	m_pInfo->fBlurPower = Info.fBlurPower;
	m_pInfo->vRatio = Info.vRatio;
	m_pInfo->vRatioSpeed = Info.vRatioSpeed;
	m_pInfo->vScale = Info.vScale;
	m_pInfo->vUvFixSpeed = Info.vUvFixSpeed;
	m_pTransformCom->Set_Scale(XMLoadFloat3(&m_pInfo->vScale));
	
}

HRESULT CBeastBreath2::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Player01MeshEffect"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Effect_FireCylinder1"), TEXT("Effect_FireCylinder1"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

CGameObject * CBeastBreath2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBeastBreath2*		pInstance = new CBeastBreath2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CBeastBreath2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBeastBreath2::Clone(void * pArg)
{
	CBeastBreath2*		pInstance = new CBeastBreath2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CBeastBreath2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBeastBreath2::Free()
{
	__super::Free();
	if(m_pInfo != nullptr)
		Safe_Delete(m_pInfo);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	

}