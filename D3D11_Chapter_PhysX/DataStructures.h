#pragma once


struct cbMatrixData {
	XMMATRIX WVP;
	XMMATRIX world;
};

struct Light {
	Light() {
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct ProjectorLight {
	ProjectorLight() {
		ZeroMemory(this, sizeof(ProjectorLight));
	}
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 dir;
	float cone;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct cbLightData {
	Light light;
};

struct HeightMapInfo {
	int terrainWidth;
	int terrainHeight;
	XMFLOAT3* heightMap;
};

struct Vertex {
	Vertex() : pos(0.0f, 0.0f, 0.0f), tex(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f) {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos(x, y, z), 
																						tex(u, v),
																						normal(nx, ny, nz) {}
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 normal;
};

struct VertexFont {
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};
