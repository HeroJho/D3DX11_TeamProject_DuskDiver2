#include "stdafx.h"
#include "..\Public\Coin_Icon.h"
#include "GameInstance.h"
#include "ToolManager.h"
#include "UI_Manager.h"


CCoin_Icon::CCoin_Icon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CCoin_Icon::CCoin_Icon(const CCoin_Icon & rhs)
	: CUI(rhs)
{
}

HRESULT CCoin_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCoin_Icon::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();

	return S_OK;
}

void CCoin_Icon::Tick(_float fTimeDelta)
{
	if (UM->Get_CoinUp())
	{
		if (m_bSizeUp)
		{
			m_UiInfo.fSizeX = 340.f;
			m_UiInfo.fSizeY = 188.f;
			m_bSizeUp = false;
			//UM->Set_CoinUp(false);
		}
	}

	if (!m_bSizeUp)

	{
		m_fSizeUpAcc += 60.f * fTimeDelta;

		if (5.f <= m_fSizeUpAcc)
		{
			m_bSizeUp = true;
			m_fSizeUpAcc = 0.f;
			m_UiInfo.fSizeX = 297.5f;
			m_UiInfo.fSizeY = 164.5f;
		}

		m_UiInfo.fSizeX -= m_fSizeUpAcc;
		m_UiInfo.fSizeY -= m_fSizeUpAcc;
	}
	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CCoin_Icon::LateTick(_float fTimeDelta)
{

	__super::LateTick(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CCoin_Icon::Render()
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


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;



	//UI_RenderDebug();


	// 자식들의 Render 호출
	__super::Render();


	return S_OK;
}






HRESULT CCoin_Icon::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Coin_Icon"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CCoin_Icon::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CCoin_Icon * CCoin_Icon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCoin_Icon*		pInstance = new CCoin_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CCoin_Icon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCoin_Icon::Clone(void * pArg)
{
	CCoin_Icon*		pInstance = new CCoin_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CCoin_Icon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CCoin_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
