#include "stdafx.h"
#include "..\Public\Player04MotionTrail.h"
#include "GameInstance.h"
#include "HierarchyNode.h"


CPlayer04MotionTrail::CPlayer04MotionTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer04MotionTrail::CPlayer04MotionTrail(const CPlayer04MotionTrail & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer04MotionTrail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer04MotionTrail::Initialize(void * pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;
	Make_ImGui();

	return S_OK;
}

void CPlayer04MotionTrail::Tick(_float fTimeDelta)
{
	if (m_bOn)
	{
		fTimeDelta *= m_fTMRatio;

		if (!m_bUp)
		{
			m_fAlpha += m_fUpSpeed * fTimeDelta;
			if (m_fAlpha >= m_fAlphaLimit)
			{
				m_bUp = true;
				m_fAlpha = m_fAlphaLimit;
			}
		}
		else
		{
			m_fAlpha -= m_fDownSpeed * fTimeDelta;
			if (m_fAlpha <= 0.f)
			{
				m_bUp = false;
				m_bOn = false;
				m_fAlpha = 0.f;
			}
		}
	}
}

void CPlayer04MotionTrail::LateTick(_float fTimeDelta)
{
	if (m_bOn)
	{
		Compute_CamZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
	}
}

HRESULT CPlayer04MotionTrail::Render()
{
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (m_iShaderPass == 10)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_RimPower", &m_fRimPower, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_RimColor", &m_fRimColor, sizeof(_float4))))
			return E_FAIL;
	}

	if (m_iShaderPass == 8)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_vMotionTrailColor", &m_vMotionTrailColor, sizeof(_float4))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_BlurPower", &m_fBlurPower, sizeof(_float))))
		return E_FAIL;

		
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_iShaderPass)))
			return E_FAIL;
	}
	

	
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pHeadTransform->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	switch (m_eHeadType)
	{
	case CPlayer04MotionTrail::HEADA:
	{
		_uint		iNumMeshes = m_pHeadCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pHeadCom->SetUp_OnShader(m_pShaderCom, m_pHeadCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pHeadCom->Render(m_pShaderCom, i, m_iShaderPass)))
				return E_FAIL;
		}
	}
		break;
	case CPlayer04MotionTrail::HEADB:
	{
		_uint		iNumMeshes = m_pHeadCom2->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pHeadCom2->SetUp_OnShader(m_pShaderCom, m_pHeadCom2->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pHeadCom2->Render(m_pShaderCom, i, m_iShaderPass)))
				return E_FAIL;
		}
	}
		break;
	}
	return S_OK;
}

void CPlayer04MotionTrail::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());


	ImGui::DragFloat("Power", &m_fRimPower, 0.01f, 0.1f, 5.f, "%.2f");
	ImGui::DragFloat4("Color", (float*)&m_fRimColor, 0.01f, 0.f, 1.f, "%.2f");
	ImGui::DragFloat("UpSpeed", &m_fUpSpeed, 0.001f, 0.001f, 50.f, "%.3f");
	ImGui::DragFloat("DownSpeed", &m_fDownSpeed, 0.001f, 0.001f, 50.f, "%.3f");
	ImGui::End();
}

HRESULT CPlayer04MotionTrail::Ready_Components()
{


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Player04Body"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_HeadTransform"), (CComponent**)&m_pHeadTransform, &TransformDesc)))
		return E_FAIL;

	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Player04Hair", TEXT("Player04Hair"), (CComponent**)&m_pHeadCom)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Player04HairBattle", TEXT("Player04HairBattle"), (CComponent**)&m_pHeadCom2)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	return S_OK;
}

void CPlayer04MotionTrail::Set_OnTrail(PLAYER04MOTIONTRAILINFO Info)
{
	m_eHeadType = Info.eHeadType;
	
	vector<CHierarchyNode*>* OriginBody = m_pModelCom->Get_HierarchyPointer();
	vector<CHierarchyNode*>* OriginHead = m_pHeadCom->Get_HierarchyPointer();
	vector<CHierarchyNode*>* OriginHead2 = m_pHeadCom2->Get_HierarchyPointer();

	m_pTransformCom->Set_WorldMatrix(Info.WorldMatrix);
	
	for (int i = 0; i < Info.Bodys->size(); ++i)
	{
		(*OriginBody)[i]->Set_CombinedTransformation((*Info.Bodys)[i]->Get_CombinedTransformation());
	}

	switch (m_eHeadType)
	{
	case CPlayer04MotionTrail::HEADA:
		for (int i = 0; i < Info.Heads->size(); ++i)
		{
			(*OriginHead)[i]->Set_CombinedTransformation((*Info.Heads)[i]->Get_CombinedTransformation());
		}
		m_pHeadTransform->Set_WorldMatrix(Info.HeadAMatrix);
		break;
	case CPlayer04MotionTrail::HEADB:
		for (int i = 0; i < Info.Heads2->size(); ++i)
		{
			(*OriginHead2)[i]->Set_CombinedTransformation((*Info.Heads2)[i]->Get_CombinedTransformation());
		}
		m_pHeadTransform->Set_WorldMatrix(Info.HeadBMatrix);
		break;
	}
		
	m_bOn = true;
	m_bUp = false;
	m_fAlpha = 0.f;
}

CPlayer04MotionTrail * CPlayer04MotionTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer04MotionTrail*		pInstance = new CPlayer04MotionTrail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CPlayer04MotionTrail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer04MotionTrail::Clone(void * pArg)
{
	CPlayer04MotionTrail*		pInstance = new CPlayer04MotionTrail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CPlayer04MotionTrail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer04MotionTrail::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pHeadCom);
	Safe_Release(m_pHeadCom2);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pHeadTransform);
}
