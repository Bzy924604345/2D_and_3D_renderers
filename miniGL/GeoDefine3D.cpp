#include "GeoDefine3D.h"
#include "Vec3.h"

// 计算法向量
void Mesh:: calcNormals()
{
	if (vertexSet.size() == 0) return;
	if (faceSet.size() == 0) return;

	vector<Vec3> vertex_normals;
	vertex_normals.resize(vertexSet.size());
	vector<int> vertex_faceNumberLookupTable(vertexSet.size());//保存每个顶点关联的三角形数量
	faceNormals.reserve(faceSet.size());

	for (size_t i = 0, size = faceSet.size(); i < size; ++i)
	{
		Face& tri = faceSet[i];
		int p0 = tri[0];
		int p1 = tri[1];
		int p2 = tri[2];
		Vec4 p0p1 = vertexSet[p1].pos - vertexSet[p0].pos;
		Vec4 p0p2 = vertexSet[p2].pos - vertexSet[p0].pos;
		Vec4 normal2 = p0p1 ^ p0p2;
		double x = normal2[0];
		double y = normal2[1];
		double z = normal2[2];
		Vec3 normal;
		normal.v[0] = x;
		normal.v[1] = y;
		normal.v[2] = z;
		faceNormals.push_back(normal);

		vertex_normals[p0] += normal;
		vertex_normals[p1] += normal;
		vertex_normals[p2] += normal;
		++vertex_faceNumberLookupTable[p0];
		++vertex_faceNumberLookupTable[p1];
		++vertex_faceNumberLookupTable[p2];
	}
	// 法线单位化
	for (size_t i = 0, size = vertex_normals.size(); i < size; ++i)
	{
		vertexSet[i].attribute.normal.normalize();
	}
}


Vec3 Box3D::operator[](int index)
{
	switch (index)
	{
	case 0:
		return Vec3(xmin, ymin, zmin);
		break;
	case 1:
		return Vec3(xmax, ymin, zmin);
		break;
	case 2:
		return Vec3(xmax, ymax, zmin);
		break;
	case 3:
		return Vec3(xmin, ymax, zmin);
		break;
	case 4:
		return Vec3(xmin, ymin, zmax);
		break;
	case 5:
		return Vec3(xmax, ymin, zmax);
		break;
	case 6:
		return Vec3(xmax, ymax, zmax);
		break;
	case 7:
		return Vec3(xmin, ymax, zmax);
		break;
	}
	throw;
}

Mesh* Box3D::toMesh()
{
	Mesh* pMesh = new Mesh();
	pMesh->vertexSet.reserve(8);
	for (int i = 0; i < 8; ++i)
	{
		pMesh->vertexSet.push_back(Vertex3D((*this)[i]));
	
	}

	pMesh->faceSet.reserve(12);

	//left
	pMesh->faceSet.push_back(Face(7, 3, 0));
	pMesh->faceSet.push_back(Face(0, 4, 7));

	//back
	pMesh->faceSet.push_back(Face(0, 3, 1));
	pMesh->faceSet.push_back(Face(1, 3, 2));

	//right
	pMesh->faceSet.push_back(Face(1, 2, 5));
	pMesh->faceSet.push_back(Face(2, 6, 5));


	int nextBase = 0;

	//top
	pMesh->faceSet.push_back(Face(nextBase + 2, nextBase + 3, nextBase + 6));
	pMesh->faceSet.push_back(Face(nextBase + 3, nextBase + 7, nextBase + 6));

	//front
	pMesh->faceSet.push_back(Face(nextBase + 4, nextBase + 5, nextBase + 7));
	pMesh->faceSet.push_back(Face(nextBase + 5, nextBase + 6, nextBase + 7));

	//bottom
	pMesh->faceSet.push_back(Face(nextBase + 0, nextBase + 1, nextBase + 4));
	pMesh->faceSet.push_back(Face(nextBase + 1, nextBase + 5, nextBase + 4));

	return pMesh;
}

Mesh* Plane::toMesh()
{
	Mesh* pMesh = new Mesh();
	pMesh->vertexSet.reserve(4);
	for (int i = 0; i < 4; ++i)
	{
		pMesh->vertexSet.push_back(Vertex3D(corners[i]));
	}

	pMesh->faceSet.reserve(2);

	pMesh->faceSet.push_back(Face(0, 1, 3));
	pMesh->faceSet.push_back(Face(1, 2, 3));
	return pMesh;
}

Node::Node(GroupNode* parent)
{
	this->parent = parent;
	parent->addChild(this);
}

ModelNode::~ModelNode()
{
	for (int i = 0; i < renderableSet.size(); ++i) delete renderableSet[i];
}

void ModelNode::addRenderable(Material* pMaterial, Mesh* pMesh)
{
	renderableSet.push_back(new Renderable(pMaterial, pMesh));
}

Renderable::Renderable(Material* material, Mesh* mesh)
{
	this->material = material;
	this->mesh = mesh;
}

Renderable::~Renderable()
{
	delete mesh;
	delete material;
}

Material::Material()
{
	ambient.set(0.2, 0.2, 0.2, 1.0);
	diffuse.set(0.8, 0.8, 0.8, 1.0);
	specular.set(0.8, 0.8, 0.8, 1.0);
	shininess = 25.0;

	fillMode = FILL_SOLID;
	shadingMode = SHADING_GOURAUD;
}

