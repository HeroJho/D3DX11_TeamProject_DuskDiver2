#include "..\Public\VIBuffer_ColorCube.h"

#include "GameInstance.h"


CVIBuffer_ColorCube::CVIBuffer_ColorCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_ColorCube::CVIBuffer_ColorCube(const CVIBuffer_ColorCube & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_ColorCube::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 8;
	m_iStride = sizeof(VTX);


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTX*		pVertices = new VTX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[7].vPosition = _float3(-0.5f, -0.5, 0.5f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitives = 12;
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_iNumIndicesofPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitives);

	/* +x */
	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	/* -x */
	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	/* +y */
	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	/* -y */
	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	/* +z */
	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	/* -z */
	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

#pragma endregion

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_ColorCube::Initialize(void * pArg)
{
	return S_OK;
}









CVIBuffer_ColorCube * CVIBuffer_ColorCube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_ColorCube*			pInstance = new CVIBuffer_ColorCube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CVIBuffer_ColorCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_ColorCube::Clone(void * pArg)
{
	CVIBuffer_ColorCube*			pInstance = new CVIBuffer_ColorCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CVIBuffer_ColorCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_ColorCube::Free()
{
	__super::Free();
}