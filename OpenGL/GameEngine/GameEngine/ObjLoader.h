#pragma once
class ObjLoader
{
public:
	ObjLoader();
	float *calculateNormal(float *ptr_coord1, float *ptr_coord2, float *ptr_coord3);
	int load(char *ptr_fileName);
	void Draw();
	void Release();

	float *ptr_normals;
	float *ptr_facesTriangles;
	float *ptr_vertexBuffer;
	long _totalConnectedPoints;
	long _totalConnectedTriangles;
};