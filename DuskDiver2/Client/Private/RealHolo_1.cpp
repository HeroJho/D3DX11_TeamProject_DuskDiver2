#include "stdafx.h"
#include "..\Public\RealHolo_1.h"
#include "GameInstance.h"
#include "ToolManager.h"
#include "MonsterBullet.h"
#include "Player_Manager.h"
#include "Time_Manager.h"
#include "Holo_3.h"
#include "Holo_4.h"
#include "Holo_5.h"
#include "Holo_6.h"
#include "Holo_7.h"

#include "Holo_1.h"


CRealHolo_1::CRealHolo_1(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CRealHolo_1::CRealHolo_1(const CRealHolo_1 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRealHolo_1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRealHolo_1::Initialize(void * pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CHolo_1::HOLODESC* pDesc = (CHolo_1::HOLODESC*)pArg;
	pDesc->pParentObj = this;

	Make_ImGui();

	m_sTag = "Tag_Holo";

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vPos), 1.f));
	m_pTransformCom->RotationThree(_float3(1.f, 0.f, 0.f), pDesc->vRot.x,
		_float3(0.f, 1.f, 0.f), pDesc->vRot.y,
		_float3(0.f, 0.f, 1.f), pDesc->vRot.z);

	m_pTransformCom->Set_Scale(XMVectorSet(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z, 1.f));


	XMStoreFloat3(&pDesc->vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION) 
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)) * -2.f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)) * -0.1f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * -1.f);
	pDesc->vScale = { 3.f, 2.f, 2.f };
	m_pHolo_3_1 = (CHolo_3*)GI->Clone_GameObject(TEXT("Holo_3"), pDesc);

	XMStoreFloat3(&pDesc->vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION)
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)) * 0.f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)) * -0.1f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * -1.f);
	pDesc->vScale = { 3.f, 2.f, 2.f };	
	m_pHolo_3_2 = (CHolo_3*)GI->Clone_GameObject(TEXT("Holo_3"), pDesc);

	XMStoreFloat3(&pDesc->vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION)
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)) * -1.f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)) * -0.1f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 0.3f);
	pDesc->vScale = { 5.5f, 2.f, 1.f };	
	m_pHolo_5 = (CHolo_5*)GI->Clone_GameObject(TEXT("Holo_5"), pDesc);

	XMStoreFloat3(&pDesc->vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION)
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)) * -1.f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)) * -0.1f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * 1.16f);
	pDesc->vScale = { 5.5f, 2.f, 0.5f };	
	m_pHolo_6 = (CHolo_6*)GI->Clone_GameObject(TEXT("Holo_6"), pDesc);

	XMStoreFloat3(&pDesc->vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION)
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT)) * 2.f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP)) * -0.1f
		+ XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) * -0.15f);
	pDesc->vScale = { 3.f, 2.f, 4.f };	
	m_pHolo_7 = (CHolo_7*)GI->Clone_GameObject(TEXT("Holo_7"), pDesc);


	m_fStartTime = (_float)GI->Get_Random(0, 1);

	return S_OK;
}

void CRealHolo_1::Tick(_float fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;

	if (20.f < m_fTimeDelta)
		m_fTimeDelta = 0.f;

	if (m_fStartTime < m_fTimeDelta)
	{
		m_fStartAlpa += fTimeDelta;
		if (1.f < m_fStartAlpa)
			m_fStartAlpa = 1.f;
	}

	m_pHolo_3_1->Tick(fTimeDelta);
	m_pHolo_3_2->Tick(fTimeDelta);
	m_pHolo_5->Tick(fTimeDelta);
	m_pHolo_6->Tick(fTimeDelta);
	m_pHolo_7->Tick(fTimeDelta);
}

void CRealHolo_1::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	//Compute_CamZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	// m_pHolo_3->LateTick(fTimeDelta);
}

HRESULT CRealHolo_1::Render()
{
	if (nullptr == m_pModelCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_StartAlpa", &m_fStartAlpa, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_NoiseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_DeltaTime", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	_float fEditUV = 3.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_EditUV", &fEditUV, sizeof(_float))))
		return E_FAIL;
	fEditUV = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_EditMaskUV", &fEditUV, sizeof(_float))))
		return E_FAIL;

	fEditUV = 0.15f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_EditAlpa", &fEditUV, sizeof(_float))))
		return E_FAIL;
	
	_float fGama = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Gama", &fGama, sizeof(_float))))
		return E_FAIL;

	// For.Blur
	_float fBlurPower = 0.2f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_BlurPower", &fBlurPower, sizeof(_float))))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 5)))
			return E_FAIL;
	}



	m_pHolo_3_1->Render();
	m_pHolo_3_2->Render();
	m_pHolo_5->Render();
	m_pHolo_6->Render();
	m_pHolo_7->Render();

	return S_OK;
}



void CRealHolo_1::OnCollision(CCollider::OTHERTOMECOLDESC Desc)
{

}


HRESULT CRealHolo_1::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Holo_1"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeNoise"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}





CRealHolo_1 * CRealHolo_1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRealHolo_1*		pInstance = new CRealHolo_1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CRealHolo_1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRealHolo_1::Clone(void * pArg)
{
	CRealHolo_1*		pInstance = new CRealHolo_1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CRealHolo_1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CRealHolo_1::Free()
{
	__super::Free();

	Safe_Release(m_pHolo_3_1);
	Safe_Release(m_pHolo_3_2);
	Safe_Release(m_pHolo_5);
	Safe_Release(m_pHolo_6);
	Safe_Release(m_pHolo_7);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}


void CRealHolo_1::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());

	IG->Debug_Transform(m_pTransformCom);

	ImGui::End();

	m_pHolo_3_1->Render_Debug();
	m_pHolo_3_2->Render_Debug();
	m_pHolo_5->Render_Debug();
	m_pHolo_6->Render_Debug();
	m_pHolo_7->Render_Debug();
}