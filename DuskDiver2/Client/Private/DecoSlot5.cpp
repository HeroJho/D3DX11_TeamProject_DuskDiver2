#include "stdafx.h"
#include "..\Public\DecoSlot5.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "ToolManager.h"



CDecoSlot5::CDecoSlot5(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CDecoSlot5::CDecoSlot5(const CDecoSlot5 & rhs)
	: CUI(rhs)
{
}

HRESULT CDecoSlot5::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDecoSlot5::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CDecoSlot5::Tick(_float fTimeDelta)
{

	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CDecoSlot5::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CDecoSlot5::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));


	if (UM->Get_Deco(4) != 6)
	{
		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", UM->Get_Deco(4))))
			return E_FAIL;
	}

	if (UM->Get_Deco(4) != 6)
	{
		if (FAILED(m_pShaderCom->Begin(2)))
			return E_FAIL;
	}


	if (UM->Get_EquipmentClassSelect() == 4 && UM->Get_Deco(0) != 6)
	{
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}




	//UI_RenderDebug();

	// 자식들의 Render 호출
	__super::Render();

	return S_OK;
}






HRESULT CDecoSlot5::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DecoSlot"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CDecoSlot5::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CDecoSlot5 * CDecoSlot5::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDecoSlot5*		pInstance = new CDecoSlot5(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CDecoSlot5"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDecoSlot5::Clone(void * pArg)
{
	CDecoSlot5*		pInstance = new CDecoSlot5(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CDecoSlot5"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CDecoSlot5::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
