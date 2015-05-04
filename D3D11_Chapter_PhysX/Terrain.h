#pragma once

#include <algorithm>

#include "DefHeaders.h"
#include "Log.h"
#include "Buffer.h"
#include "DataStructures.h"

class Terrain {
private:
	HeightMapInfo m_hminfo;
	float m_heightFactor;

	int m_numVertices;
	int m_numFaces;

	std::vector<Vertex> m_vertices;
	std::vector<DWORD> m_indices;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	bool m_loadHeightMap(const std::string* fileName);
	bool m_createVertices();
	bool m_createIndices();
	bool m_computeNormals();
	bool m_loadNormalsFromFile(const std::string* fileName);
	bool m_saveNormalsToFile(const std::string* fileName);
public:
	Terrain();
	~Terrain();
	
	ID3D11Buffer* const* getVertexBuffer()		{ return &m_pVertexBuffer;	}
	ID3D11Buffer* getIndexBuffer()				{ return m_pIndexBuffer;	}
	UINT getIndexCount()						{ return m_numFaces * 3;	}

	bool create(const std::string* fileName, ID3D11Device* pDevice);
};