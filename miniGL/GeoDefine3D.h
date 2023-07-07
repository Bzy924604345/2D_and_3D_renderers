#pragma once
#include <vector>
#include "Vec3.h"
#include "Vec4.h"
using namespace std;

///����������������
enum VertexAttributeType { vtXYZ = 1, vtCOLOR = 2, vtNORMAL = 4, vtUV = 8 };

///��������
struct VertexAttribute
{
	Vec4 color;//��ɫ
	Vec3 normal;//����
	double uv[2];//��������
};

///3D����
struct Vertex3D {
	Vertex3D(){}
	Vertex3D(Vec3& v) :pos(v) { }

	Vec4 pos;//����λ��
	VertexAttribute attribute;//��������

};

//struct Vertex3D
//{
//	Vertex3D() {}
//	Vertex3D(double x, double y, double z)
//	{
//		wpos.set(x, y, z);
//	}
//
//	Vertex3D(Vec3& pt)
//	{
//		wpos = pt;
//	}
//
//	Vec3 wpos;
//	Vec4 pos;
//	Vec3 color;
//	Vec3 normal;
//};


//�����Σ�������ʵ�ʶ������ݣ�ֻ��¼�����ڶ����������е�������
struct Face
{
	Face()
	{
		set(0, 0, 0);
	}

	Face(int v0, int v1, int v2)
	{
		set(v0, v1, v2);
	}

	void set(int v0, int v1, int v2)
	{
		ids[0] = v0, ids[1] = v1, ids[2] = v2;
	}

	int operator[](int i) { return ids[i]; }//���±��ȡ�����ζ����ڶ��������е�����

	int next(int i) { return ids[(i + 1) % 3]; }//��һ������id
	int ids[3];//���涥������
};

///������
struct Mesh
{
	vector<Vertex3D> vertexSet;//��������
	vector<Face> faceSet;//����������
	unsigned vertexDeclaration;//�����ʽ
	vector<Vec3> faceNormals;//�����η���
	void calcNormals();//���㷨��

	bool attributeEnabled(VertexAttributeType vt) { return vertexDeclaration & vt; }
	bool colorAttributeEnabled() { return attributeEnabled(vtCOLOR); }
	bool normalAttributeEnabled() { return attributeEnabled(vtNORMAL); }
	bool uvAttributeEnabled() { return attributeEnabled(vtUV); }
};

//3D��Χ��
struct Box3D
{
	Box3D() { setBox( -1, -1, -1, 1, 1, 1 ); }
	Box3D(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
	{
		setBox(xmin, ymin, zmin, xmax, ymax, zmax);
	}

	void setBox(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
	{
		this->xmin = xmin, this->xmax = xmax, this->ymin = ymin, this->ymax = ymax, this->zmin = zmin, this->zmax = zmax;
	}

	Vec3 operator[](int index);

	Mesh* toMesh();

	double xmin, ymin, zmin;
	double xmax, ymax, zmax;
};

///����ƽ��
struct Plane
{
	Plane()
	{
		setByCenter( 0, 0, 0, 2, 2);
	}

	void setByCenter(double x, double y, double z, double width, double height)
	{
		corners[0].set(x - width / 2, y - height / 2, z);
		corners[1].set(x + width / 2, y - height / 2, z);
		corners[2].set(x + width / 2, y + height / 2, z);
		corners[3].set(x - width / 2, y + height / 2, z);
	}

	Mesh* toMesh();

	Vec3 corners[4];
};

enum NodeType { ntGroup, ntTransform, ntModel };
struct GroupNode;

///�����࣬���ڴ����ӹ�ϵ��������һ�ö����
struct Node
{
	Node() { this->parent = NULL; }
	Node(GroupNode* parent);

	virtual ~Node() {}
	virtual NodeType getNodeType() = 0;

	GroupNode* parent;//���ڵ�
};

///���㣬���ԹҶ���ӽڵ�
struct GroupNode : public Node
{
	GroupNode() :Node() {}
	GroupNode(GroupNode* parent) :Node(parent) {}

	virtual ~GroupNode() { for (int i = 0, size = children.size(); i < size; ++i) delete children[i]; }

	virtual NodeType getNodeType() { return ntGroup; }
	void addChild(Node* pChild) { children.push_back(pChild); }
	Node* operator[](int i) { return children[i]; }

	vector<Node*> children;//�ӽڵ㼯��
};

///�任��㣬���ӽڵ���б任
struct TransformNode : public GroupNode
{
	TransformNode() :GroupNode() {}
	TransformNode(GroupNode* parent) :GroupNode(parent) {}

	virtual NodeType getNodeType() { return ntTransform; }

	Matrix4 transformMatrix;//�任����
};

struct Material;
struct Renderable;
///ģ�ͽ�㣬������֯����Ⱦ����Renderable
struct ModelNode : public Node
{
	ModelNode() :Node() {}
	ModelNode(GroupNode* parent) :Node(parent) {}
	~ModelNode();

	virtual NodeType getNodeType() { return ntModel; }
	void addRenderable(Material* pMaterial, Mesh* pMesh);

	vector<Renderable*> renderableSet;
};

///����Ⱦ����,�ɼ�������mesh�Ͳ�������material���
struct Renderable
{
	Renderable(Material* material, Mesh* pMesh);
	~Renderable();

	Material* material;//����
	Mesh* mesh;//��������
};

enum FillMode{	FILL_SOLID,	FILL_WIREFRAME,	FILL_VERTEX };
enum ShadingMode{	SHADING_FLAT,	SHADING_GOURAUD,	SHADING_PHONG };
enum CullMode { CULL_NONE, CULL_CLOCKWISE, CULL_COUNTERCLOCKWISE };  //�ر��޳��������棬ǰ����

///����
struct Material
{
	Material();

	Vec4 ambient;//������
	Vec4 diffuse;//�������
	Vec4 specular;//���淴���
	double shininess;//���淴��ϵ��

	ShadingMode shadingMode;//����ģ��
	FillMode fillMode;//���ģʽ
};