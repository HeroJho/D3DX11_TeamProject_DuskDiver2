#include "stdafx.h"
#include "..\Public\FoodSlot4.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Slot_Num.h"
#include "ToolManager.h"



CFoodSlot4::CFoodSlot4(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

CFoodSlot4::CFoodSlot4(const CFoodSlot4 & rhs)
	: CUI(rhs)
{
}

HRESULT CFoodSlot4::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoodSlot4::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();

	if (FAILED(Make_ChildUI(230.f, 0.f, 20.f, 20.f, TEXT("Prototype_GameObject_Slot_Num"))))
		return E_FAIL;

	vector<CUI*>::iterator iter = m_pChildUIs.begin();
	m_pUINum = (CSlot_Num*)(*iter);

	return S_OK;
}

void CFoodSlot4::Tick(_float fTimeDelta)
{
	m_pUINum->Set_Number(UM->Get_Food(4));


	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CFoodSlot4::LateTick(_float fTimeDelta)
{
	if (UM->Get_FoodZero(5) == 4)
	{
		m_UiInfo.fY = -206.f;
		if (UM->Get_Food(4) > 0 && UM->Get_InvenSelNum() == 1)
			UM->Set_FoodType(4);
	}
	else if (UM->Get_FoodZero(5) == 3)
	{
		m_UiInfo.fY = -171.f;
		if (UM->Get_Food(4) > 0 && UM->Get_InvenSelNum() == 2)
			UM->Set_FoodType(4);
	}
	else if (UM->Get_FoodZero(5) == 2)
	{
		m_UiInfo.fY = -136.f;
		if (UM->Get_Food(4) > 0 && UM->Get_InvenSelNum() == 3)
			UM->Set_FoodType(4);
	}
	else if(UM->Get_FoodZero(5) == 1)
	{
		m_UiInfo.fY = -101.f;
		if (UM->Get_Food(4) > 0 && UM->Get_InvenSelNum() == 4)
			UM->Set_FoodType(4);
	}
	else
	{
		m_UiInfo.fY = -66.f;
		if (UM->Get_Food(4) > 0 && UM->Get_InvenSelNum() == 5)
			UM->Set_FoodType(4);
	}

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CFoodSlot4::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;


	m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 4)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(10)))
		return E_FAIL;

	if (UM->Get_InvenNum() == 1)
	{
		if (UM->Get_Food(4) > 0)
		{
			if (FAILED(m_pVIBufferCom->Render()))
				return E_FAIL;

			__super::Render();
		}
	}



	//UI_RenderDebug();

	// 자식들의 Render 호출


	return S_OK;
}






HRESULT CFoodSlot4::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FoodType"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CFoodSlot4::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Debug"))
		m_bDebug = !m_bDebug;

	ImGui::End();
}



CFoodSlot4 * CFoodSlot4::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFoodSlot4*		pInstance = new CFoodSlot4(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CFoodSlot4"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFoodSlot4::Clone(void * pArg)
{
	CFoodSlot4*		pInstance = new CFoodSlot4(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CFoodSlot4"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CFoodSlot4::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
