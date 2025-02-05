#include "stdafx.h"
#include "..\Public\Team_Player03.h"
#include "GameInstance.h"
#include "Player_Manager.h"
#include "ToolManager.h"
#include "UI_Manager.h"


CTeam_Player03::CTeam_Player03(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CTeam_Player03::CTeam_Player03(const CTeam_Player03 & rhs)
	: CUI(rhs)
{
}

HRESULT CTeam_Player03::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTeam_Player03::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();

	if (FAILED(Make_ChildUI(0.f, 0.f, 89.6f, 93.1f, TEXT("Prototype_GameObject_Team_bg"))))
		return E_FAIL;

	if (FAILED(Make_ChildUI(2.480506f, 21.43639f, 56.7f, 12.6f, TEXT("Prototype_GameObject_Team_Player03_HP"))))
		return E_FAIL;

	if (FAILED(Make_ChildUI(-11.771793f, 31.771963f, 23.1f, 7.7f, TEXT("Prototype_GameObject_Team_Player03_SP"))))
		return E_FAIL;

	//UM->LEVIADA_On();

	return S_OK;
}

void CTeam_Player03::Tick(_float fTimeDelta)
{
	if (!m_bPop && CPlayer::PLAYER_03 == PM->Get_SelectPlayer())
	{
		m_bPop = true;
	}

	if (m_bPop && m_bCheck)
	{
		//스케일 조정
		m_fPop += fTimeDelta;

		if (0.2f > m_fPop)
		{
			m_UiInfo.fSizeX += 1.f;
			m_UiInfo.fSizeY += 1.f;
		}

		else if (0.2f <= m_fPop && 0.4f >= m_fPop)
		{
			m_UiInfo.fSizeX -= 1.f;
			m_UiInfo.fSizeY -= 1.f;
		}

		else if (0.4f >= m_fPop)
		{
			m_UiInfo.fSizeX = 89.6f;
			m_UiInfo.fSizeY = 93.1f;
			m_fPop = 0;
			m_bCheck = false;
		}
	}

	if (1 != PM->Get_SelectPlayer())
	{
		m_UiInfo.fSizeX = 89.6f;
		m_UiInfo.fSizeY = 93.1f;
		m_bPop = false;
		m_bCheck = true;
		m_fPop = 0;
	}


	
	//UI_InputDebug(fTimeDelta);

	if (UM->LEVIADA_State() && !PM->Get_PlayerDie(CPlayer::PLAYER_03))
	{
		__super::Tick(fTimeDelta);
	}
}

void CTeam_Player03::LateTick(_float fTimeDelta)
{
	if (UM->LEVIADA_State() && !PM->Get_PlayerDie(CPlayer::PLAYER_03))
	{
		__super::LateTick(fTimeDelta);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CTeam_Player03::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;



	if (PM->Get_PlayerDie(CPlayer::PLAYER_03))
	{
		if (FAILED(m_pShaderCom->Begin(39)))
			return E_FAIL;
	}

	else
	{
		if (CPlayer::PLAYER_03 == PM->Get_SelectPlayer())
		{
			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;
		}

		else
		{
			if (FAILED(m_pShaderCom->Begin(5)))
				return E_FAIL;
		}
	}

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;





	//UI_RenderDebug();
	if (!PM->Get_PlayerDie(CPlayer::PLAYER_03) && UM->LEVIADA_State())
	{
		// 자식들의 Render 호출
		__super::Render();
	}
	
	return S_OK;
}






HRESULT CTeam_Player03::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Team_Player03"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}


void CTeam_Player03::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CTeam_Player03 * CTeam_Player03::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTeam_Player03*		pInstance = new CTeam_Player03(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CTeam_Player03"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTeam_Player03::Clone(void * pArg)
{
	CTeam_Player03*		pInstance = new CTeam_Player03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CTeam_Player03"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CTeam_Player03::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
