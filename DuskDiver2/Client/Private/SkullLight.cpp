#include "stdafx.h"
#include "..\Public\SkullLight.h"
#include "GameInstance.h"


#include "ToolManager.h"



CSkullLight::CSkullLight(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CSkullLight::CSkullLight(const CSkullLight & rhs)
	: CUI(rhs)
{
}

HRESULT CSkullLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkullLight::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CSkullLight::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
	m_fTime2 += fTimeDelta;
	if (m_fTime2 > 1.5f)
	{
		if (m_fTime > 0.03f)
		{
			m_UiInfo.fSizeX += 20.f;
			m_UiInfo.fSizeY += 20.f;
			m_fTime = 0.f;
		}
	}

	if (m_fTime2 > 1.5f)
	{
		m_fTime3 += fTimeDelta * 0.08f;
	}


	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CSkullLight::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CSkullLight::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_TimeDelta", &m_fTime3, sizeof(_float));



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






HRESULT CSkullLight::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkullLight"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CSkullLight::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CSkullLight * CSkullLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSkullLight*		pInstance = new CSkullLight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CSkullLight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkullLight::Clone(void * pArg)
{
	CSkullLight*		pInstance = new CSkullLight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CSkullLight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CSkullLight::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}