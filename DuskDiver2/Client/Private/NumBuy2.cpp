#include "stdafx.h"
#include "..\Public\NumBuy2.h"
#include "GameInstance.h"
#include "UI_Manager.h"


CNumBuy2::CNumBuy2(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{

}

CNumBuy2::CNumBuy2(const CNumBuy2 & rhs)
	: CUI(rhs)
{

}

HRESULT CNumBuy2::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNumBuy2::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CNumBuy2::Tick(_float fTimeDelta)
{

	//UI_InputDebug(fTimeDelta);
	
	__super::Tick(fTimeDelta);
}

void CNumBuy2::LateTick(_float fTimeDelta)
{


	__super::LateTick(fTimeDelta);

	if (UM->Get_MoreCombo())
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

}

HRESULT CNumBuy2::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	

	_float4x4 mStoreMax;
	
	_float fX = m_TotalInfo.fX;
		

	for (_uint i = 0; i < m_sNumber.size(); ++i)
	{
		_uint iIndex = m_sNumber[i] - 48;

		_matrix mMax = m_pTransformCom->Get_WorldMatrix();
		mMax.r[3] = XMVectorSet(fX - g_iWinSizeX * 0.5f, -m_TotalInfo.fY + g_iWinSizeY * 0.5f, 0.0f, 1.f);


		mMax = XMMatrixTranspose(mMax);
		XMStoreFloat4x4(&mStoreMax, mMax);
		m_pShaderCom->Set_RawValue("g_WorldMatrix", &mStoreMax, sizeof(_float4x4));

		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", iIndex)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(10)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;


 		fX += m_UiInfo.fSizeX * 0.67f;
	}

	//UI_RenderDebug();

	// 자식들의 Render 호출
	__super::Render();


	return S_OK;
}



// *[HeroJho]: 디버깅 창입니다. ImGui로 작동됩니다.
void CNumBuy2::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Test Int"))
	{
		++m_iTestNum;
		Set_Number(m_iTestNum);
	}


	ImGui::End();
}


void CNumBuy2::Set_Number(_uint iNum)
{
	m_iNumber = iNum;
	m_sNumber = to_string(m_iNumber);
}



HRESULT CNumBuy2::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CNumBuy2 * CNumBuy2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNumBuy2*		pInstance = new CNumBuy2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CNumBuy2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CNumBuy2::Clone(void * pArg)
{
	CNumBuy2*		pInstance = new CNumBuy2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CNumBuy2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNumBuy2::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
