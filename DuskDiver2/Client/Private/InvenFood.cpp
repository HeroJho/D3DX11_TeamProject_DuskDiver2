#include "stdafx.h"
#include "..\Public\InvenFood.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "ToolManager.h"



CInvenFood::CInvenFood(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CInvenFood::CInvenFood(const CInvenFood & rhs)
	: CUI(rhs)
{
}

HRESULT CInvenFood::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenFood::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CInvenFood::Tick(_float fTimeDelta)
{
	if (m_iNum == 10)
	{
		m_Side = R;
	}

	m_iNum = UM->Get_InvenNum();


	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CInvenFood::LateTick(_float fTimeDelta)
{


	if (m_iNum != UM->Get_InvenNum() && (m_iNum == 10 || m_iNum == 0 || m_iNum == 2 || m_iNum == 1))
	{
		if (m_iNum > UM->Get_InvenNum())
			m_Side = R;
		else
			m_Side = L;

		if(m_iNum == 1 && UM->Get_InvenNum() == 0 || m_iNum == 0 || m_iNum == 10)
			m_Side = R;
	}


	if (m_iNum == 1)
	{
		if (m_UiInfo.fSizeX >= 201.6f)
		{
			m_UiInfo.fSizeX = 201.6f;
			m_UiInfo.fSizeY = 48.3f;
		}
		else if (m_UiInfo.fSizeX < 201.6f)
		{
			m_fTime += fTimeDelta;
			if (m_fTime > 0.01f)
			{
				m_UiInfo.fSizeX += 20.f;
				m_UiInfo.fSizeY += 4.f;
				m_bTexture = false;
				m_fTime = 0;
			}
		}

	}

	else
	{
		if (m_UiInfo.fSizeX <= 28.f)
		{
			m_UiInfo.fSizeX = 28.f;
			m_UiInfo.fSizeY = 28.f;
			m_bTexture = true;
		}
		else if (m_UiInfo.fSizeX > 28.f)
		{
			m_fTime += fTimeDelta;
			if (m_fTime > 0.01f)
			{
				m_UiInfo.fSizeX -= 20.f;
				m_UiInfo.fSizeY -= 4.f;
				m_fTime = 0;
			}

		}

	}

	if (m_iNum != 1)
	{
		if (m_Side == L)
		{
			if (m_UiInfo.fX <= -180.f)
				m_UiInfo.fX = -180.f;
			else
			{
				m_fTime2 += fTimeDelta;
				if (m_fTime2 > 0.01f)
				{
					m_UiInfo.fX -= 30.f;
				}
			}
		}
		if (m_Side == R)
		{
			if (m_UiInfo.fX >= -60.f)
				m_UiInfo.fX = -60.f;
			else
			{
				m_fTime2 += fTimeDelta;
				if (m_fTime2 > 0.01f)
				{
					m_UiInfo.fX += 30.f;
				}
			}
		}
	}
	else
	{
		if (m_UiInfo.fX >= -120.f)
			m_UiInfo.fX = -120.f;
		else
		{
			m_fTime2 += fTimeDelta;
			if (m_fTime2 > 0.01f)
			{
				m_UiInfo.fX += 30.f;
			}
		}
	}


	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CInvenFood::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (!m_bTexture)
	{
		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 1)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;
	}


	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;




	//UI_RenderDebug();

	// 자식들의 Render 호출
	__super::Render();

	return S_OK;
}






HRESULT CInvenFood::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InvenFood"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CInvenFood::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CInvenFood * CInvenFood::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CInvenFood*		pInstance = new CInvenFood(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CInvenFood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CInvenFood::Clone(void * pArg)
{
	CInvenFood*		pInstance = new CInvenFood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CInvenFood"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CInvenFood::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
