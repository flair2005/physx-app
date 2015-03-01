#include "Box.h"

Box::Box(PxReal width, PxReal height, PxReal depth) {
	m_width = width;
	m_height = height;
	m_depth = depth;
}

Box::Box(PxVec3 size) {
	m_width = size.x;
	m_height = size.y;
	m_depth = size.z;
}

Box::~Box() {

}

PxGeometryHolder Box::createGeometry() {
	PxGeometryHolder geometry; 
	geometry.storeAny(PxBoxGeometry(m_width/2, m_height/2, m_depth/2));
	return geometry;
}

bool Box::create(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,	PxVec3 velocity) {
	if(!Primitive::create(pPhysics, pMaterial, position, density, velocity)) {
		Log::get()->err("Box Create Failed");
		return false;
	}

	createModel();
	return true;
}

void Box::createModel() {
	std::vector<Vertex> vertices;

	Vertex v[] = {
		Vertex(-(m_width/2), -(m_height/2), -(m_depth/2), 0.0f, 1.0f, -1.0f, -1.0f, -1.0f),
		Vertex(-(m_width/2),  (m_height/2), -(m_depth/2), 0.0f, 0.0f, -1.0f,  1.0f, -1.0f),
		Vertex( (m_width/2),  (m_height/2), -(m_depth/2), 1.0f, 0.0f,  1.0f,  1.0f, -1.0f),
		Vertex( (m_width/2), -(m_height/2), -(m_depth/2), 1.0f, 1.0f,  1.0f, -1.0f, -1.0f),

		Vertex(-(m_width/2), -(m_height/2), (m_depth/2), 1.0f, 1.0f, -1.0f, -1.0f, 1.0f),
		Vertex( (m_width/2), -(m_height/2), (m_depth/2), 0.0f, 1.0f,  1.0f, -1.0f, 1.0f),
		Vertex( (m_width/2),  (m_height/2), (m_depth/2), 0.0f, 0.0f,  1.0f,  1.0f, 1.0f),
		Vertex(-(m_width/2),  (m_height/2), (m_depth/2), 1.0f, 0.0f, -1.0f,  1.0f, 1.0f),

		Vertex(-(m_width/2), (m_height/2), -(m_depth/2), 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-(m_width/2), (m_height/2),  (m_depth/2), 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex( (m_width/2), (m_height/2),  (m_depth/2), 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex( (m_width/2), (m_height/2), -(m_depth/2), 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		Vertex(-(m_width/2), -(m_height/2), -(m_depth/2), 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex( (m_width/2), -(m_height/2), -(m_depth/2), 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex( (m_width/2), -(m_height/2),  (m_depth/2), 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-(m_width/2), -(m_height/2),  (m_depth/2), 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		Vertex(-(m_width/2), -(m_height/2),  (m_depth/2), 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-(m_width/2),  (m_height/2),  (m_depth/2), 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-(m_width/2),  (m_height/2), -(m_depth/2), 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-(m_width/2), -(m_height/2), -(m_depth/2), 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		Vertex( (m_width/2), -(m_height/2), -(m_depth/2), 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex( (m_width/2),  (m_height/2), -(m_depth/2), 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex( (m_width/2),  (m_height/2),  (m_depth/2), 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex( (m_width/2), -(m_height/2),  (m_depth/2), 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	for(int i = 0; i < ARRAYSIZE(v); i++) {
		vertices.push_back(v[i]);
	}

	std::vector<DWORD> indices;

	DWORD ind[] = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,
		
		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};

	for(int i = 0; i < ARRAYSIZE(ind); i++) {
		indices.push_back(ind[i]);
	}
	Object::getModel(vertices, indices);
}