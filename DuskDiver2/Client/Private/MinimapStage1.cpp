#include "stdafx.h"
#include "..\Public\MinimapStage1.h"
#include "GameInstance.h"
#include "ToolManager.h"



CMinimapStage1::CMinimapStage1(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CMinimapStage1::CMinimapStage1(const CMinimapStage1 & rhs)
	: CUI(rhs)
{
}

HRESULT CMinimapStage1::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMinimapStage1::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();

	


	return S_OK;
}

void CMinimapStage1::Tick(_float fTimeDelta)
{


	UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CMinimapStage1::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CMinimapStage1::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(23)))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;




	UI_RenderDebug();

	// 자식들의 Render 호출
	__super::Render();

	return S_OK;
}






HRESULT CMinimapStage1::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MinimapStage1"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;




	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CMinimapStage1::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CMinimapStage1 * CMinimapStage1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMinimapStage1*		pInstance = new CMinimapStage1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CMinimapStage1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMinimapStage1::Clone(void * pArg)
{
	CMinimapStage1*		pInstance = new CMinimapStage1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CMinimapStage1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMinimapStage1::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
