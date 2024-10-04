#include "stdafx.h"
#include "..\Public\Task_Contents.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "ToolManager.h"



CTask_Contents::CTask_Contents(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CTask_Contents::CTask_Contents(const CTask_Contents & rhs)
	: CUI(rhs)
{
}

HRESULT CTask_Contents::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTask_Contents::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();

	return S_OK;
}

void CTask_Contents::Tick(_float fTimeDelta)
{
	if (UM->Get_NextQuest())
	{
		UM->Plus_QuestNum();
		UM->Set_NextQuest(false);
	}

	//UI_InputDebug(fTimeDelta);



	__super::Tick(fTimeDelta);
}

void CTask_Contents::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CTask_Contents::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", UM->Get_QuestNum())))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	//UI_RenderDebug();


	// �ڽĵ��� Render ȣ��
	__super::Render();

	return S_OK;
}






HRESULT CTask_Contents::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Task_Contents"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CTask_Contents::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CTask_Contents * CTask_Contents::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTask_Contents*		pInstance = new CTask_Contents(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTask_Contents"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTask_Contents::Clone(void * pArg)
{
	CTask_Contents*		pInstance = new CTask_Contents(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTask_Contents"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CTask_Contents::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}