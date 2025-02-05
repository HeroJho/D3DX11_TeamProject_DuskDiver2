#pragma once
#include "OBB.h"
#include "AABB.h"
#include "Model.h"
#include "Rigid.h"
#include "Shader.h"
#include "Sphere.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "Rigid_Sphere.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_ZRect.h"
#include "VIBuffer_Cube.h"
#include "Model_Instance.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Particle.h"
#include "VIBuffer_ColorCube.h"
#include "VIBuffer_Rect_Instance.h"
#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Particle_Instance.h"




BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Release_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);
private:
	_uint												m_iNumLevels = 0;

	map<const _tchar*, class CComponent*>*				m_pPrototypes = nullptr;
	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	class CComponent* Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END