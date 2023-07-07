#include"Renderer3D.h"
#include"GeoDefine3D.h"
#include"GeoDefine.h"
#include"Graphic.h"
#include"Vec4.h"
#include"Matrix3.h"
#include"Matrix4.h"
#include"Draw.h"
#include"Sta.h"
#include"Cut3D.h"

void Renderer3D::renderSceneNode(Node* pNode, Matrix4& parentTransform)
{
	NodeType nt = pNode->getNodeType();
	switch (nt)
	{
	case ntTransform:
	case ntGroup:
	{
		GroupNode* pParentNode = (GroupNode*)pNode;
		for (int i = 0, size = pParentNode->children.size(); i < size; ++i)
		{
			if (nt == ntGroup)
				renderSceneNode(pParentNode->children[i], parentTransform);
			else
				renderSceneNode(pParentNode->children[i], ((TransformNode*)pNode)->transformMatrix * parentTransform);
		}
	}
	break;
	case ntModel:
	{
		setModelMatrix(parentTransform);
		ModelNode* pModelNode = (ModelNode*)pNode;
		for (int i = 0, size = pModelNode->renderableSet.size(); i < size; ++i)
		{
			render(pModelNode->renderableSet[i]);
		}
	};
	}
}

void Renderer3D::render(Renderable* pRenderable)
{
	applyMaterial(pRenderable->material);
	render(pRenderable->mesh);
}

void Renderer3D::applyMaterial(Material* pMaterial)
{

}

void Renderer3D::applyViewMatrix(Matrix4& m)
{
	this->g_viewMatrix *= m;
}

void Renderer3D::setModelMatrix(Matrix4& m)
{
	this->g_modelMatrix = m;
}

void Renderer3D::render(Mesh* pMesh)
{
	int width = getWindowWidth(), height = getWindowHeight();

	if (width == 0 || height == 0)
		return;

	//模型矩阵*视图矩阵*投影矩阵*视口变换（平移、缩放）
	g_transformMatrix3D = g_modelMatrix * g_viewMatrix * g_projectMatrix *
		Matrix4::scale(0.5, 0.5, 1.0) * Matrix4::translate(0.5, 0.5, 0.0);
	// 法线矩阵变换
	g_normalMatrix = g_viewMatrix * g_projectMatrix;
	Matrix4 m_1;
	m_1.makeScale(width, height, 1);
	g_normalMatrix *= m_1;
	g_normalMatrix = Matrix4::Inverse(g_normalMatrix);
	g_normalMatrix = Matrix4::transpose(g_normalMatrix);

#pragma region vertex transform 顶点变换
	vector<Vertex3D>& vertexSet = pMesh->vertexSet; //顶点数组
	static vector<Vertex3D> transformedVertexSet;
	transformedVertexSet.resize(vertexSet.size());
	for (int i = 0, c = vertexSet.size(); i < c; ++i)
	{
		//模型坐标*整个变换矩阵
		transformedVertexSet[i].attribute = vertexSet[i].attribute; //属性
		transformedVertexSet[i].pos = vertexSet[i].pos * g_transformMatrix3D; //位置
	}
	static vector<Vertex3D> transformedVertexSet_re;
	transformedVertexSet_re.resize(6);

#pragma endregion
	int color[12][3] = { { 255, 0, 0 },	{ 255, 0, 0 },
							{255, 255, 0},{255, 255, 0},
							{0, 255, 0},{0, 255, 0},
							{0, 0, 255},{0, 0, 255},
							{0, 255, 255},{0, 255, 255},
							{255, 0, 255} ,{255, 0, 255} };

	int k = 0;

	vector<Face>& faceSet = pMesh->faceSet;//画整个三角网

	pMesh->calcNormals();  //计算法线
	vector<Vec3>& normals = pMesh->faceNormals; //三角形法线
	vector<Vec4> normalss;//变换法线
	normalss.resize(12);
	for (int i = 0; i < 12; i++) {

		normalss[i] = Vec4(normals[i]);
		//normalss[i].h() = 0;
	}
	
	vector<Vec3> faceN;
	vector<double> Idensity;


	
#pragma endregion

	for (int i = 0, c = faceSet.size(); i < c; ++i)
	{
		Face& face = faceSet[i]; //把每一个三角形拿出
#pragma region cut
		// 裁剪方法一
		if (g_Sta.g_3d_CutWay == false)
		{
			int clipFlag[3] = { 0 };
			for (int j = 0; j < 3; ++j)
			{
				Vec4& pos = transformedVertexSet[face[j]].pos;
				double h = pos.h();
				double x = pos.x();
				double y = pos.y();
				double z = pos.z();
				if (h <= 0 || x<0 || x>h || y<0 || y>h || z<0 || z>h)
					clipFlag[j] = 1;
			} 

			//TODO:剪裁不可见部分，生成新的三角形
			if (clipFlag[0] || clipFlag[1] || clipFlag[2]) //简化处理：有一个顶点不可见，则剔除整个三角形
				continue;
#pragma endregion

#pragma region viewportTransform
			Vertex3D pixelSet[3];

			Vec3 pos1[3];
			vector<point3D> ptss;

			for (int j = 0; j < 3; ++j)
			{
				Vertex3D& v = transformedVertexSet[face[j]];
				//TODO:使用剪裁后的顶点
				Vec4& pos = v.pos;
				double r_h = 1.0 / pos.h();
				//透视除法
				pixelSet[j].pos.x() = pos.x() * r_h * (width);
				//视口变换
				pixelSet[j].pos.y() = pos.y() * r_h * (height);
				pixelSet[j].pos.z() = pos.z() * r_h;
				pixelSet[j].pos.h() = r_h;
				pixelSet[j].attribute = v.attribute;


				point3D Pt;
				//double I = Idensity[face[j]];
				//pos1[j].set(v.pos[0], v.pos[1], v.pos[2]);
				////pos1[j] /= v.pos[3];
				//pos1[j][0] *= width;
				//pos1[j][1] *= height;


				Pt.x = pixelSet[j].pos.x();
				Pt.y = pixelSet[j].pos.y();
				Pt.z = pixelSet[j].pos.z();

				//Pt.x = pos1[j][0];
				//Pt.y = pos1[j][1];
				//Pt.z = pos1[j][2];
				//Pt.i = I;
				ptss.push_back(Pt);


			}
#pragma endregion

#pragma region cull
			if (g_Sta.CullMode != CULL_NONE
				&& Draw::cull(pixelSet[0].pos, pixelSet[1].pos, pixelSet[2].pos))
				continue;
#pragma endregion						
						
#pragma region rasterrize
			if (!g_Sta.draw3D)
			{
				for (int j = 0; j < 3; ++j)
				{
					Vec4& pt1 = pixelSet[j].pos;
					Vec4& pt2 = pixelSet[(j + 1) % 3].pos;
					if (pt1 == Vec4(0, 0, 0, 0) && pt2 == Vec4(0, 0, 0, 0))
					{
						continue;
					}
					else
						Draw::drawLine(pt1.x(), pt1.y(), pt2.x(), pt2.y(), getPenColor());//像素坐标下绘制
				}
			}
			else if (g_Sta.draw3D)
			{
				int r = color[k][0];
				int g = color[k][1];
				int b = color[k][2];
				
				normalss[k] = normalss[k] * g_normalMatrix; 

				Vec3 target_normalss = Vec3(normalss[k].x(), normalss[k].y(), normalss[k].z());
				Draw::Setgeneraltriangle3d(ptss[0], ptss[1], ptss[2], r, g, b, target_normalss);
				k++;
			}
#pragma endregion

		}
		else
		{
			// 裁剪方法二
			for (int j = 0; j < 3; ++j)
			{
				Vec4& pos1 = transformedVertexSet[face[j]].pos;
				Vec4& pos2 = transformedVertexSet[face[(j + 1) % 3]].pos;
				return_Vec4& return_V = Cut3D::CohenCut3D(pos1.x(), pos1.y(), pos1.z(), pos1.h(), pos2.x(), pos2.y(), pos2.z(), pos2.h());
				transformedVertexSet_re[2 * j].pos = return_V.p1;
				transformedVertexSet_re[2 * j + 1].pos = return_V.p2;
			}

#pragma region viewportTransform
			Vertex3D pixelSet[6];
			for (int j = 0; j < 6; ++j)
			{
				Vertex3D& v = transformedVertexSet_re[j];
				//TODO:使用剪裁后的顶点
				Vec4& pos = v.pos;
				double r_h = 1.0 / pos.h();
				//透视除法
				pixelSet[j].pos.x() = pos.x() * r_h * (width);
				//视口变换
				pixelSet[j].pos.y() = pos.y() * r_h * (height);
				pixelSet[j].pos.z() = pos.z() * r_h;
				pixelSet[j].pos.h() = r_h;
				pixelSet[j].attribute = v.attribute;
			}
#pragma endregion

#pragma region rasterrize
			for (int j = 0; j < 3; ++j)
			{
				Vec4& pt1 = pixelSet[2 * j].pos;
				Vec4& pt2 = pixelSet[2 * j + 1].pos;

				if (pt1 == Vec4(0, 0, 0, 0) && pt2 == Vec4(0, 0, 0, 0))
				{
					continue;
				}
				else
					Draw::drawLine(pt1.x(), pt1.y(), pt2.x(), pt2.y(), getPenColor());//像素坐标下绘制
			}

#pragma endregion
		}

#pragma endregion
	}


}

void Renderer3D::makeLookAt(Vec3 eye, Vec3 center, Vec3 up)
{
	g_viewMatrix.makeLookAt(eye, center, up);
}
void Renderer3D::LookAt(Vec3 eye, Vec3 center, Vec3 up)
{
	g_viewMatrix.makeLookAt(eye, center, up);
}

