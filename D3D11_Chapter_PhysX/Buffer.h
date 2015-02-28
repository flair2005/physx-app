#pragma once

#include "DefHeaders.h"

/*
	��������������� �����, ���������� �������� ������� ��� DirectX11
	From Direct11 Tutorial @ zeswarchief
*/

class Buffer {
public:
	static ID3D11Buffer* createVertexBuffer(ID3D11Device* device, int size, bool dynamic, const void* mem);
	static ID3D11Buffer* createIndexBuffer(ID3D11Device* device, int size, bool dynamic, const void* mem);
	static ID3D11Buffer* createConstantBuffer(ID3D11Device* device, int size, bool dynamic);
};