#include "stdafx.h"
#include "..\Public\AzitProp.h"
#include "GameInstance.h"

#include "ToolManager.h"
#include "Player_Manager.h"

CAzitProp::CAzitProp(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CAzitProp::CAzitProp(const CAzitProp & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAzitProp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAzitProp::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_sTag = "Tag_Stage_Prop";

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CAzitProp::Tick(_float fTimeDelta)
{
	Check_RenderState();
}

void CAzitProp::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (true == m_bRenderState)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

}

HRESULT CAzitProp::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	//m_pTextureCom->Set_SRV(m_pShaderCom, "g_MaskTexture", 0);

	_float fGama = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Gama", &fGama, sizeof(_float))))
		return E_FAIL;

	// For.Blur
	_float fBlurPower = 0.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_BlurPower", &fBlurPower, sizeof(_float))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CAzitProp::Render_ShadowDepth()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	//m_pTextureCom->Set_SRV(m_pShaderCom, "g_MaskTexture", 0);

	// For.Blur
	_float fBlurPower = 0.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_BlurPower", &fBlurPower, sizeof(_float))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CAzitProp::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("AzitProp"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

void CAzitProp::Check_RenderState()
{
	if (140.f <= GI->Get_CamPosition().z)
	{
		m_bRenderState = true;
	}
	else
		m_bRenderState = false;
	
}

CAzitProp * CAzitProp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAzitProp*		pInstance = new CAzitProp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CAzitProp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAzitProp::Clone(void * pArg)
{
	CAzitProp*		pInstance = new CAzitProp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CAzitProp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAzitProp::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
