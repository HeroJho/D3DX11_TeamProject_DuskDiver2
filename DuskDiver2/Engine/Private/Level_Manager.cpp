#include "..\Public\Level_Manager.h"
#include "Level.h"

#include "GameInstance.h"

#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}


HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr != m_pCurrentLevel)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		pGameInstance->Clear(m_iCurrentLevelIndex);

		Safe_Release(pGameInstance);
	}

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;

	m_iCurrentLevelIndex = iLevelIndex;

	CLight_Manager::Get_Instance()->Set_RenderLight(true);

	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
