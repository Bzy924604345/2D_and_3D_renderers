#include"Graphic.h"
#include"GeoDefine3D.h"
#include"Matrix4.h"

//struct point3D
//{
//	int x;
//	int y;
//	double z;
//	double i;
//};

struct Geometry;
struct Dataset;
struct Layer;
class Renderer3D
{
public:
	Matrix4 g_modelMatrix;//3Dģ�ͱ任����
	Matrix4 g_viewMatrix;//3D��ͼ�任����
	Matrix4 g_projectMatrix;//3DͶӰ�任����
	Matrix4 g_viewportMatrix;
	Matrix4 g_transformMatrix3D;//�ܱ任����
	Matrix4 g_normalMatrix;//���������߱任�ܾ���


	void applyMaterial(Material* pMaterial);
	void applyViewMatrix(Matrix4& m);
	void setModelMatrix(Matrix4& m);

	void makeLookAt(Vec3 eye, Vec3 center, Vec3 up);
	void LookAt(Vec3 eye, Vec3 center, Vec3 up);

	void renderSceneNode(Node* pNode, Matrix4& parentTransform);
	void render(Renderable* pRenderable);
	void render(Mesh* pMesh);
};