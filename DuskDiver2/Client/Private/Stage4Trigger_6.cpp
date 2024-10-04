#include "stdafx.h"
#include "..\Public\Stage4Trigger_6.h"
#include "GameInstance.h"

#include "Camera_Manager.h"
#include "UI_Manager.h"

CStage4Trigger_6::CStage4Trigger_6(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCreture(pDevice, pContext)
{
}

CStage4Trigger_6::CStage4Trigger_6(const CStage4Trigger_6 & rhs)
	: CCreture(rhs)
{
}

HRESULT CStage4Trigger_6::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStage4Trigger_6::Initialize(void * pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_sTag = "Tag_Boss";
	Make_ImGui();


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-50.8f, 16.5f, 108.1f, 1.f));


	return S_OK;
}

void CStage4Trigger_6::Tick(_float fTimeDelta)
{
	UM->Set_Trigger6(m_bOpen);

	if (!m_bOpen)
		return;

	m_fTimeAcc += fTimeDelta;
	if (5.f  < m_fTimeAcc)
	{
		Set_Dead();
		CRM->Get_Cam()->End_Target();
		UM->Set_FrontUI(true);
		UM->Set_BattleTalkIndex(19);
	}


}

void CStage4Trigger_6::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	Tick_Col(m_pTransformCom->Get_WorldMatrix());
	Choose_Col(nullptr);
	Render_Col(m_pRendererCom);

}

HRESULT CStage4Trigger_6::Render()
{


	return S_OK;
}

void CStage4Trigger_6::OnCollision(CCollider::OTHERTOMECOLDESC Desc)
{

	if ("Tag_Player" == Desc.pOther->Get_Tag())
	{
		// 내 손 Sphere 둘중 하나와 상대편의 OBB와 충돌 했다면
		if (!strcmp("Monster_Body1", Desc.pMyDesc->sTag) && !strcmp("Player_Body", Desc.pOtherDesc->sTag))
		{

		}
	}
}

void CStage4Trigger_6::On_Hit(CCreture * pOther)
{
	if (m_bHurt)
	{
		m_bHurt = false;

	}
}

HRESULT CStage4Trigger_6::Ready_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	CCollider::COLLIDERDESC ColDesc;
	ZeroMemory(&ColDesc, sizeof(CCollider::COLLIDERDESC));

	ColDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColDesc.vSize = _float3(3.f, 2.f, 2.f);
	ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColDesc.pOwner = this;
	ColDesc.iFlag = 2;
	strcpy(ColDesc.sTag, "Monster_Push_Range"); // 밀어내기 위함

	if (FAILED(AddCollider(CCollider::TYPE_SPHERE, ColDesc)))
		return E_FAIL;


	return S_OK;
}

CStage4Trigger_6 * CStage4Trigger_6::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CStage4Trigger_6*		pInstance = new CStage4Trigger_6(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CStage4Trigger_6"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCreture * CStage4Trigger_6::Clone(void * pArg)
{
	CStage4Trigger_6*		pInstance = new CStage4Trigger_6(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CStage4Trigger_6"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage4Trigger_6::Free()
{
	__super::Free();


	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}




void CStage4Trigger_6::Render_Debug()
{
	ImGui::Begin(m_sImGuiTag.data());

	IG->Debug_Transform(m_pTransformCom);

	_float3 vSize = Get_Colliders("Monster_Push_Range")->Get_Desc()->vSize;
	_float3 vCenter = Get_Colliders("Monster_Push_Range")->Get_Desc()->vCenter;

	ImGui::DragFloat3("Size", (float*)&vSize, 0.1f);
	ImGui::DragFloat3("Center", (float*)&vCenter, 0.1f);
	Get_Colliders("Monster_Push_Range")->Set_Size(vSize);
	Get_Colliders("Monster_Push_Range")->Set_Center(vCenter);



	ImGui::End();
}

void CStage4Trigger_6::Open()
{
	m_bOpen = true;

	CCamera_Main::CUTDESC CamDesc;
	CamDesc.vPos = _float3(-39.571f, 21.f, 106.198f);
	CamDesc.vLook = _float3(-42.778f, 20.166f, 106.545f);
	CamDesc.bLerp = true;
	CamDesc.bLookLerp = true;
	CamDesc.fLerpSpeed = 10.f;
	CamDesc.fLookLerpSpeed = 10.f;
	CamDesc.vPivotLookPos = _float3(0.f, 0.f, 0.f);
	CamDesc.pTran = nullptr;
	CamDesc.vPosDis = _float3(0.f, 0.f, 0.f);
	CamDesc.bPixLocal = false;
	CRM->Get_Cam()->Show_Target(CamDesc);

}
