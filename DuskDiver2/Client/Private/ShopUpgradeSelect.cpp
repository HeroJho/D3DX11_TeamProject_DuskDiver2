#include "stdafx.h"
#include "..\Public\ShopUpgradeSelect.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "ToolManager.h"



CShopUpgradeSelect::CShopUpgradeSelect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CShopUpgradeSelect::CShopUpgradeSelect(const CShopUpgradeSelect & rhs)
	: CUI(rhs)
{
}

HRESULT CShopUpgradeSelect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShopUpgradeSelect::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CShopUpgradeSelect::Tick(_float fTimeDelta)
{
	m_iCurstate = UM->Get_ShopSelect();

	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CShopUpgradeSelect::LateTick(_float fTimeDelta)
{
	if (m_iCurstate != 0 && UM->Get_ShopSelect() == 2)
	{
		m_bSizeUp = true;
	}
	else if (UM->Get_ShopSelect() != 2)
	{
		m_UiInfo.fSizeX = 375.f * 0.8f;
		m_UiInfo.fSizeY = 127.f * 0.8f;
		m_bSizeUp = false;
		m_fSizeUp = 0.f;
	}

	if (m_bSizeUp)
	{
		m_fSizeUp += fTimeDelta;
		if (m_fSizeUp < 0.2f)
		{
			m_UiInfo.fSizeX += 1.4f;
			m_UiInfo.fSizeY += 1.4f;
		}
		else if (0.2f < m_fSizeUp && m_fSizeUp < 0.35f)
		{
			m_UiInfo.fSizeX -= 1.4f;
			m_UiInfo.fSizeY -= 1.4f;
		}
	}

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CShopUpgradeSelect::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;


	if (UM->Get_ShopSelect() == 2)
	{
		if (FAILED(m_pShaderCom->Begin(4)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;
	}

	if (!UM->Get_ShopSelectBar())
	{
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}




	//UI_RenderDebug();

	// �ڽĵ��� Render ȣ��
	__super::Render();

	return S_OK;
}






HRESULT CShopUpgradeSelect::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ShopUpgradeSelect"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CShopUpgradeSelect::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CShopUpgradeSelect * CShopUpgradeSelect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CShopUpgradeSelect*		pInstance = new CShopUpgradeSelect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CShopUpgradeSelect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShopUpgradeSelect::Clone(void * pArg)
{
	CShopUpgradeSelect*		pInstance = new CShopUpgradeSelect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CShopUpgradeSelect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CShopUpgradeSelect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}