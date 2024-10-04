#include "stdafx.h"
#include "..\Public\InvenQuestItemBackground.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "ToolManager.h"



CInvenQuestItemBackground::CInvenQuestItemBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CInvenQuestItemBackground::CInvenQuestItemBackground(const CInvenQuestItemBackground & rhs)
	: CUI(rhs)
{
}

HRESULT CInvenQuestItemBackground::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenQuestItemBackground::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CInvenQuestItemBackground::Tick(_float fTimeDelta)
{



	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CInvenQuestItemBackground::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CInvenQuestItemBackground::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;


	if (UM->Get_InvenNum() == 4)
	{
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}




	//UI_RenderDebug();

	// �ڽĵ��� Render ȣ��
	__super::Render();

	return S_OK;
}






HRESULT CInvenQuestItemBackground::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InvenQuestItemBackground"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CInvenQuestItemBackground::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CInvenQuestItemBackground * CInvenQuestItemBackground::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CInvenQuestItemBackground*		pInstance = new CInvenQuestItemBackground(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CInvenQuestItemBackground"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInvenQuestItemBackground::Clone(void * pArg)
{
	CInvenQuestItemBackground*		pInstance = new CInvenQuestItemBackground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CInvenQuestItemBackground"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CInvenQuestItemBackground::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}