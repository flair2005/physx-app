#include "Terrain.h"

Terrain::Terrain() {
	m_heightFactor = 0;

	m_hminfo.heightMap = 0;
	m_hminfo.terrainHeight = 0;
	m_hminfo.terrainWidth = 0;

	m_vertices.clear();
	m_indices.clear();

	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}

Terrain::~Terrain() {
	delete[] m_hminfo.heightMap;
	m_hminfo.heightMap = nullptr;

	m_vertices.clear();
	m_indices.clear();


	if(m_pVertexBuffer) {
		m_pVertexBuffer->Release();
	}
	if(m_pIndexBuffer) {
		m_pIndexBuffer->Release();
	}
}

bool Terrain::create(const std::string* fileName, ID3D11Device* pDevice) {
	if(!m_loadHeightMap(fileName)) {
		Log::get()->err("Terrain::create() failed: unable to load HeightMap from file");
		return false;
	}

	if(!m_createVertices()) {
		Log::get()->err("Terrain::create() failed: unable to create vertices");
		return false;
	}

	if(!m_createIndices()) {
		Log::get()->err("Terrain::create() failed: unable to create indices");
		return false;
	}

	if(!m_loadNormalsFromFile(&std::string("D:\\RESTORE_FILES\\Projects\\D3D11Lessons\\D3D11_Chapter_PhysX\\D3D11_Chapter_PhysX\\Res\\heightmap3.hmn"))) {
		Log::get()->debug("Terrain::create(): файл с нормал€ми не найден, попытка вычислить нормали заново и сохранить в файл...");
		if(!m_computeNormals()) {
			Log::get()->err("Terrain::create() failed: unable to compute normals");
			return false;
		}
	}

	m_pVertexBuffer = Buffer::createVertexBuffer(pDevice, sizeof(Vertex) * m_vertices.size(), false, &m_vertices[0]);
	if(!m_pVertexBuffer) {
		Log::get()->err("Terrain::create() failed: unable to create Vertex Buffer");
		return false;
	}

	m_pIndexBuffer = Buffer::createIndexBuffer(pDevice, sizeof(DWORD) * m_indices.size(), false, &m_indices[0]);
	if(!m_pIndexBuffer) {
		Log::get()->err("Terrain::create() failed: unable to create Index Buffer");
		return false;
	}

	return true;
}

bool Terrain::m_loadHeightMap(const std::string* fileName) {
	FILE* pFile;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, index;
	unsigned char height;

	pFile = fopen(fileName->c_str(), "rb");
	if(!pFile) {
		Log::get()->err("Terrain::loadHeightMap() failed: unable to open file: \n\t%s", fileName->c_str());
		return false;
	}

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	m_hminfo.terrainWidth = bitmapInfoHeader.biWidth;
	m_hminfo.terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = m_hminfo.terrainWidth * m_hminfo.terrainHeight * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];

	fseek(pFile, bitmapFileHeader.bfOffBits, SEEK_SET);

	fread(bitmapImage, 1, imageSize, pFile);

	fclose(pFile);

	m_hminfo.heightMap = new XMFLOAT3[m_hminfo.terrainWidth * m_hminfo.terrainHeight];
	// ѕропускает следующие два компонента в наборе данных изображени€ (читает R, пропускает GB - они все одинаковые)
	int k = 0;
	m_heightFactor = 4.0f;

	for(UINT i = 0; i < m_hminfo.terrainHeight; i++) {
		for(UINT j = 0; j < m_hminfo.terrainWidth; j++) {
			height = bitmapImage[k];
			index = (m_hminfo.terrainHeight * i) + j;

			m_hminfo.heightMap[index].x = (float)j;
			m_hminfo.heightMap[index].y = (float)height / m_heightFactor;
			m_hminfo.heightMap[index].z = (float)i;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = nullptr;

	return true;
}

bool Terrain::m_createVertices() {
	int cols = m_hminfo.terrainWidth;
	int rows = m_hminfo.terrainHeight;

	if(cols == 0 || rows == 0) {
		Log::get()->err("Terrain::m_createVertices() failed: empty data");
		return false;
	}

	m_numVertices = rows * cols;
	m_numFaces = (rows - 1) * (cols - 1) * 2;

	m_vertices.resize(m_numVertices);
	for(DWORD i = 0; i < rows; i++) {
		for(DWORD j = 0; j < cols; j++) {
			m_vertices[i * cols + j].pos = m_hminfo.heightMap[i * cols + j];
			m_vertices[i * cols + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}

	return true;
}

bool Terrain::m_createIndices() {
	int cols = m_hminfo.terrainWidth;
	int rows = m_hminfo.terrainHeight;

	if(cols == 0 || rows == 0) {
		Log::get()->err("Terrain::m_createIndices() failed: empty data");
		return false;
	}

	m_indices.resize(m_numFaces * 3);

	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	
	for(DWORD i = 0; i < rows - 1; i++) {
		for(DWORD j = 0; j < cols - 1; j++) {
			m_indices[k]   = i*cols+j;		// Bottom left of quad
			m_vertices[i*cols+j].tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

			m_indices[k+1] = (i+1)*cols+j;	// Top left of quad
			m_vertices[(i+1)*cols+j].tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			m_indices[k+2] = i*cols+j+1;		// Bottom right of quad
			m_vertices[i*cols+j+1].tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);


			m_indices[k+3] = (i+1)*cols+j;	// Top left of quad
			m_vertices[(i+1)*cols+j].tex = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			m_indices[k+4] = (i+1)*cols+j+1;	// Top right of quad
			m_vertices[(i+1)*cols+j+1].tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

			m_indices[k+5] = i*cols+j+1;		// Bottom right of quad
			m_vertices[i*cols+j+1].tex = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	return true;
}

bool Terrain::m_computeNormals() {
	if(m_vertices.size() == 0) {
		Log::get()->err("Terrain::m_computeNormals() failed: empty data");
		return false;
	}

	std::vector<XMFLOAT3> tempNormal;

	XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float vecX, vecY, vecZ;

	XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	for(UINT i = 0; i < m_numFaces; i++) {
		vecX = m_vertices[m_indices[(i * 3)]].pos.x - m_vertices[m_indices[(i * 3) + 2]].pos.x;
		vecY = m_vertices[m_indices[(i * 3)]].pos.y - m_vertices[m_indices[(i * 3) + 2]].pos.y;
		vecZ = m_vertices[m_indices[(i * 3)]].pos.z - m_vertices[m_indices[(i * 3) + 2]].pos.z;

		edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

		vecX = m_vertices[m_indices[(i * 3) + 2]].pos.x - m_vertices[m_indices[(i * 3) + 1]].pos.x;
		vecY = m_vertices[m_indices[(i * 3) + 2]].pos.y - m_vertices[m_indices[(i * 3) + 1]].pos.y;
		vecZ = m_vertices[m_indices[(i * 3) + 2]].pos.z - m_vertices[m_indices[(i * 3) + 1]].pos.z;		
		edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

		XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
		tempNormal.push_back(unnormalized);
	}

	XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int facesUsing = 0;
	float tX;
	float tY;
	float tZ;

	for(UINT i = 0; i < m_numVertices; i++) {
		for(UINT j = 0; j < m_numFaces; j++) {
			if(m_indices[j * 3] == i || m_indices[(j * 3) + 1] == i || m_indices[(j * 3) + 2] == i) {
				tX = XMVectorGetX(normalSum) + tempNormal[j].x;
				tY = XMVectorGetY(normalSum) + tempNormal[j].y;
				tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

				normalSum = XMVectorSet(tX, tY, tZ, 0.0f);
				facesUsing++;
			}
		}
		normalSum = normalSum / facesUsing;
		normalSum = XMVector3Normalize(normalSum);

		m_vertices[i].normal.x = XMVectorGetX(normalSum);
		m_vertices[i].normal.y = XMVectorGetY(normalSum);
		m_vertices[i].normal.z = XMVectorGetZ(normalSum);

		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		facesUsing = 0;
	}

	if(!m_saveNormalsToFile(&std::string("D:\\RESTORE_FILES\\Projects\\D3D11Lessons\\D3D11_Chapter_PhysX\\D3D11_Chapter_PhysX\\Res\\heightmap3.hmn"))) {
		Log::get()->err("Terrain::create() failed: unable to save normals to file");
		return false;
	}

	return true;
}

bool Terrain::m_loadNormalsFromFile(const std::string* fileName) {
	std::ifstream ifs;
	ifs.clear();
	ifs.open(fileName->c_str(), std::ifstream::in | std::ifstream::binary);
	if(!ifs.good()) {
		Log::get()->err("Terrain::m_loadNormalsFromFile(): failed to load normals from file");
		return false;
	}
	int i = 0;
	float iX;
	float iY;
	float iZ;
	while(!ifs.eof() && i < m_vertices.size()) {
		ifs >> iX;
		ifs >> iY;
		ifs >> iZ;
		m_vertices[i].normal.x = iX;
		m_vertices[i].normal.y = iY;
		m_vertices[i].normal.z = iZ;
		i++;
	}
	ifs.close();

	return true;
}

bool Terrain::m_saveNormalsToFile(const std::string* fileName) {
	std::ofstream ofs(*fileName, std::ofstream::out | std::ofstream::binary);
	if(!ofs.good()) {
		Log::get()->err("Terrain::m_saveNormalsToFile(): failed to save normals to file");
		return false;
	}
	for(UINT i = 0; i < m_vertices.size(); i++) {
		ofs << m_vertices[i].normal.x << " " << m_vertices[i].normal.y << " " << m_vertices[i].normal.z << std::endl;
	}
	ofs.close();

	return true;
}