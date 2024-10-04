#include "stdafx.h"
#include "..\Public\EquipmentSelectBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "ToolManager.h"



CEquipmentSelectBar::CEquipmentSelectBar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CEquipmentSelectBar::CEquipmentSelectBar(const CEquipmentSelectBar & rhs)
	: CUI(rhs)
{
}

HRESULT CEquipmentSelectBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEquipmentSelectBar::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CEquipmentSelectBar::Tick(_float fTimeDelta)
{
	m_iCurNum = UM->Get_EquipmentNum();


	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CEquipmentSelectBar::LateTick(_float fTimeDelta)
{
	if (m_iCurNum != UM->Get_EquipmentNum())
	{
		if (m_iCurNum > UM->Get_EquipmentNum())
		{
			for (int i = 1; i < 7; ++i)
			{
				if (i == UM->Get_EquipmentNum())
				{
					_float Pos = -203.3f + (float)(i - 1) * 53.f;
					m_fYPos = Pos;
				}
			}
		}
		else
		{
			for (int i = 2; i < 8; ++i)
			{
				if (i == UM->Get_EquipmentNum())
				{
					_float Pos = -203.3f + (float)(i - 1) * 53.f;
					m_fYPos = Pos;
				}
			}
		}

		m_bRender = false;
	}


	if (m_fCountRender > 0.2f)
	{
		m_UiInfo.fY = m_fYPos;
	}

	if (!m_bRender)
	{
		m_fCountRender += fTimeDelta;
		if (m_fCountRender > 0 && m_fCountRender <= 0.2f)
		{
			m_fBlend += fTimeDelta;
		}
		else
		{
			m_fBlend -= fTimeDelta;
		}
	}


	if (m_fCountRender > 0.4f)
	{
		m_bRender = true;
		m_fCountRender = 0.f;
		m_fBlend = 0.f;
	}



	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CEquipmentSelectBar::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_TimeDelta", &m_fBlend, sizeof(_float));


	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	
	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;

	if (UM->Get_EquipmentState() == 1)
	{
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}




	//UI_RenderDebug();

	// 자식들의 Render 호출
	__super::Render();

	return S_OK;
}






HRESULT CEquipmentSelectBar::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EquipmentSelectBar"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CEquipmentSelectBar::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CEquipmentSelectBar * CEquipmentSelectBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEquipmentSelectBar*		pInstance = new CEquipmentSelectBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CEquipmentSelectBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEquipmentSelectBar::Clone(void * pArg)
{
	CEquipmentSelectBar*		pInstance = new CEquipmentSelectBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CEquipmentSelectBar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CEquipmentSelectBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
