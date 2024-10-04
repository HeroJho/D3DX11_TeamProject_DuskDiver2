#include "stdafx.h"
#include "..\Public\ReserveEffect.h"
#include "GameInstance.h"


#include "ToolManager.h"



CReserveEffect::CReserveEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CReserveEffect::CReserveEffect(const CReserveEffect & rhs)
	: CUI(rhs)
{
}

HRESULT CReserveEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CReserveEffect::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CReserveEffect::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta * 0.3f;

	if (m_fTime > 0.9f)
		Set_Dead();


	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CReserveEffect::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CReserveEffect::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_TimeDelta", &m_fTime, sizeof(_float));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;




	//UI_RenderDebug();

	// �ڽĵ��� Render ȣ��
	__super::Render();

	return S_OK;
}






HRESULT CReserveEffect::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ReserveEffect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CReserveEffect::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CReserveEffect * CReserveEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CReserveEffect*		pInstance = new CReserveEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CReserveEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CReserveEffect::Clone(void * pArg)
{
	CReserveEffect*		pInstance = new CReserveEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CReserveEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CReserveEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}