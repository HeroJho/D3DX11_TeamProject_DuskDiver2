#include "stdafx.h"
#include "..\Public\Holo_1.h"
#include "GameInstance.h"
#include "ToolManager.h"
#include "MonsterBullet.h"
#include "Player_Manager.h"
#include "Time_Manager.h"

CHolo_1::CHolo_1(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CHolo_1::CHolo_1(const CHolo_1 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHolo_1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHolo_1::Initialize(void * pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	HOLODESC* pDesc = (HOLODESC*)pArg;


	Make_ImGui();

	m_sTag = "Tag_Holo";

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vPos), 1.f));
	m_pTransformCom->RotationThree(_float3(1.f, 0.f, 0.f), pDesc->vRot.x,
		_float3(0.f, 1.f, 0.f), pDesc->vRot.y,
		_float3(0.f, 0.f, 1.f), pDesc->vRot.z);
	m_pTransformCom->Set_Scale(XMVectorSet(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z, 1.f));

	m_fStartTime = 2.f;

	return S_OK;
}

void CHolo_1::Tick(_float fTimeDelta)
{
	m_fTimeDelta += fTimeDelta;
	if (10.f < m_fTimeDelta)
		m_fTimeDelta = 0.f;

	if (m_fStartTime < m_fTimeDelta)
	{
		m_fStartAlpa += fTimeDelta;
		if (1.f < m_fStartAlpa)
			m_fStartAlpa = 1.f;
	}
}

void CHolo_1::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	// Compute_CamZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CHolo_1::Render()
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



	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_NoiseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_DeltaTime", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	_float fEditUV = 25.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_EditUV", &fEditUV, sizeof(_float))))
		return E_FAIL;
	_float fEditMaskUV = 5.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_EditMaskUV", &fEditMaskUV, sizeof(_float))))
		return E_FAIL;

	fEditUV = 0.1f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_EditAlpa", &fEditUV, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_StartAlpa", &m_fStartAlpa, sizeof(_float))))
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

	return S_OK;
}



void CHolo_1::OnCollision(CCollider::OTHERTOMECOLDESC Desc)
{

}


HRESULT CHolo_1::Ready_Components()
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

	

	//CCollider::COLLIDERDESC ColDesc;
	//ZeroMemory(&ColDesc, sizeof(CCollider::COLLIDERDESC));
	//ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	//ColDesc.vRotation = _float3(0.f, 0.f, 0.f);
	//ColDesc.vSize = _float3(0.3f, 0.3f, 0.3f);
	//ColDesc.pOwner = this;
	//strcpy(ColDesc.sTag, "Monster_Attack");
	//if (FAILED(AddCollider(CCollider::TYPE_SPHERE, ColDesc)))
	//	return E_FAIL;


	return S_OK;
}





CHolo_1 * CHolo_1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHolo_1*		pInstance = new CHolo_1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CHolo_1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHolo_1::Clone(void * pArg)
{
	CHolo_1*		pInstance = new CHolo_1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CHolo_1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CHolo_1::Free()
{
	__super::Free();


	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}


void CHolo_1::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());

	IG->Debug_Transform(m_pTransformCom);



	ImGui::End();
}