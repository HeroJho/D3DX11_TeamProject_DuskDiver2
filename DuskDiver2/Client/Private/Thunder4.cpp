#include "stdafx.h"
#include "..\Public\Thunder4.h"
#include "GameInstance.h"


#include "ToolManager.h"



CThunder4::CThunder4(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CThunder4::CThunder4(const CThunder4 & rhs)
	: CUI(rhs)
{
}

HRESULT CThunder4::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CThunder4::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CThunder4::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;


	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CThunder4::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CThunder4::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 4)))
		return E_FAIL;

	if(m_fTime > 0.2f)
	{ 
		if (FAILED(m_pShaderCom->Begin(17)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Begin(2)))
			return E_FAIL;
	}


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;




	//UI_RenderDebug();

	// 자식들의 Render 호출
	__super::Render();

	return S_OK;
}






HRESULT CThunder4::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Thunder"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CThunder4::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CThunder4 * CThunder4::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CThunder4*		pInstance = new CThunder4(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CThunder4"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CThunder4::Clone(void * pArg)
{
	CThunder4*		pInstance = new CThunder4(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CThunder4"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CThunder4::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
