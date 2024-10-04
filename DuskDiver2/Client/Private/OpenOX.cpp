#include "stdafx.h"
#include "..\Public\OpenOX.h"
#include "GameInstance.h"
#include "ToolManager.h"
#include "UI_Manager.h"
#include "Creture.h"
#include "Player_Manager.h"

COpenOX::COpenOX(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

COpenOX::COpenOX(const COpenOX & rhs)
	: CGameObject(rhs)
{
}

HRESULT COpenOX::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COpenOX::Initialize(void * pArg)
{

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_fSizeX = 3.f;
	m_fSizeY = 3.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_fSizeX, m_fSizeY, 0.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(106.7f, -3.6f, 137.5f, 1.f));

	return S_OK;
}

void COpenOX::Tick(_float fTimeDelta)
{
	if (UM->Get_Trigger1())
	{
		if (!m_bTriggerCheck)
		{
			m_fAlpha = 0.f;
			m_bTriggerCheck = true;
		}
		m_fO += fTimeDelta;
		if (m_fO > 2.f)
			m_bO = true;
		if (m_fO > 3.f)
			m_fAlpha += fTimeDelta;

		UM->Set_PunchTime(0);
	}
	else
	{
		m_fAlphaTime += fTimeDelta;
		if (m_fAlphaTime > 5.f)
			m_fAlphaTime = 0.f;
		else if (m_fAlphaTime > 4.f && m_fAlphaTime <= 5.f)
			m_fAlpha -= fTimeDelta;
		else if (m_fAlphaTime > 3.f)
			m_fAlpha += fTimeDelta;
	}

	m_fPlayerPos = PM->Get_PlayerPos();
	m_bNear = m_pTransformCom->Check_LimitDistance(XMLoadFloat4(&m_fPlayerPos), 5.f);

	if (m_bNear && !m_bOnce) //������ ������
	{
		GI->PlaySoundW(L"song054_unknown.wav", SD_NPC, 0.7f);
		m_bOnce = true;
	}

	else if (!m_bNear && m_bOnce && !m_bO) //�־�������
	{
		GI->StopSound(SD_NPC);
		m_bOnce = false;
	}

	else if (!m_bNear && !m_bOnce && m_bO) // �ְ� O�� �ٲ������
	{
		GI->PlaySoundW(L"song043_ojama_okuri_shou.wav", SD_NPC, 0.7f);
		m_bOnce = true;
	}


	m_pTransformCom->Rotation(XMVectorSet( 0,1.f,0,0), -30.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(106.5f, -5.6f, 137.7f, 1.f));
}

void COpenOX::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;


	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT COpenOX::Render()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (m_bO)
	{
		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 1)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(12)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin(11)))
			return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	
	return S_OK;
}

void COpenOX::Render_Debug()
{
}

HRESULT COpenOX::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_OpenOX"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}





COpenOX * COpenOX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	COpenOX*		pInstance = new COpenOX(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : COpenOX"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * COpenOX::Clone(void * pArg)
{
	COpenOX*		pInstance = new COpenOX(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : COpenOX"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void COpenOX::Free()
{
	__super::Free();


	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}