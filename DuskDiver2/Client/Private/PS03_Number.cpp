#include "stdafx.h"
#include "..\Public\PS03_Number.h"
#include "GameInstance.h"
#include "UI_Manager.h"


CPS03_Number::CPS03_Number(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{

}

CPS03_Number::CPS03_Number(const CPS03_Number & rhs)
	: CUI(rhs)
{

}

HRESULT CPS03_Number::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPS03_Number::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Make_ImGui();


	return S_OK;
}

void CPS03_Number::Tick(_float fTimeDelta)
{
	//UI_InputDebug(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CPS03_Number::LateTick(_float fTimeDelta)
{


	__super::LateTick(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CPS03_Number::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));


	_float4x4 mStoreMax;
	
	_float fX = m_TotalInfo.fX;

	//이거 넣으면 뒷숫자로 정렬
	//fX -= m_UiInfo.fSizeX * 0.67f *  m_sNumber.size();

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
void CPS03_Number::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	if (ImGui::Button("Test Int"))
	{
		++m_iTestNum;
		Set_Number(m_iTestNum);
	}


	ImGui::End();
}


void CPS03_Number::Set_Number(_uint iNum)
{
	m_iNumber = iNum;
	m_sNumber = to_string(m_iNumber);
}



HRESULT CPS03_Number::Ready_Components()
{

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CPS03_Number * CPS03_Number::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPS03_Number*		pInstance = new CPS03_Number(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPS03_Number"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPS03_Number::Clone(void * pArg)
{
	CPS03_Number*		pInstance = new CPS03_Number(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPS03_Number"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPS03_Number::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
