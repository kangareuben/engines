#include "ObjLoader.h"
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include "Errors.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
using namespace std;
ObjLoader::ObjLoader()
{
	this->_totalConnectedPoints = 0;
	this->_totalConnectedTriangles = 0;
}




float *ObjLoader::calculateNormal(float *ptr_coord1, float *ptr_coord2, float *ptr_coord3)
{
	//Calculate Vector 1 & Vector 2
	float va[3], vb[3], vr[3], val;
	va[0] = ptr_coord1[0] - ptr_coord2[0];
	va[1] = ptr_coord1[1] - ptr_coord2[1];
	va[2] = ptr_coord1[2] - ptr_coord2[2];

	vb[0] = ptr_coord1[0] - ptr_coord3[0];
	vb[1] = ptr_coord1[1] - ptr_coord3[1];
	vb[2] = ptr_coord1[2] - ptr_coord3[2];

	//cross product

	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	//normalization factor

	val = sqrt(vr[0] * vr[0] + vr[1] * vr[1] + vr[2] * vr[2]);

	float norm[3];
	norm[0] = vr[0] / val;
	norm[1] = vr[1] / val;
	norm[2] = vr[2] / val;
	return norm;
}

int ObjLoader::load(char *fileName)
{
	string line;
	ifstream objFile(fileName);
	if (objFile.is_open())
	{
		objFile.seekg(0, ios::end);
		long fileSize = objFile.tellg();
		objFile.seekg(0, ios::beg);

		//Memory for verices
		ptr_vertexBuffer = (float *)malloc(fileSize);

		//Memory for triangles
		ptr_facesTriangles = (float *)malloc(fileSize *sizeof(float));

		//Memory for normals
		ptr_normals = (float *)malloc(fileSize*sizeof(float));

		int triangleIndex = 0;
		int normalIndex = 0;

		while (!objFile.eof())
		{
			getline(objFile, line);
			if (line.c_str()[0] == 'v')
			{
				line[0] = ' ';

				//Reads value from v x y z
				sscanf_s(line.c_str(), "%f%f%f ", &ptr_vertexBuffer[_totalConnectedPoints],
					&ptr_vertexBuffer[_totalConnectedPoints + 1],
					&ptr_vertexBuffer[_totalConnectedPoints + 2]);

				//Add 3 to the total connected points to get back to the first point
				_totalConnectedPoints += 3;
			}

			if (line.c_str()[0] == 'f')
			{
				line[0] = ' ';
				int vertexNumer[4] = { 0, 0, 0 };
				sscanf_s(line.c_str(), "%i%i%i", &vertexNumer[0], &vertexNumer[1], &vertexNumer[2]);

				vertexNumer[0] -= 1;
				vertexNumer[1] -= 1;
				vertexNumer[2] -= 1;

				int tCounter = 0;

				//Load the values of vertices into the triangle
				for (int i = 0; i < 3; i++)
				{
					ptr_facesTriangles[triangleIndex + tCounter] = ptr_vertexBuffer[3 * vertexNumer[i]];
					ptr_facesTriangles[triangleIndex + tCounter + 1] = ptr_vertexBuffer[3 * vertexNumer[i] + 1];
					ptr_facesTriangles[triangleIndex + tCounter + 2] = ptr_vertexBuffer[3 * vertexNumer[i] + 2];
					tCounter += 3;
				}

				//Calculate Normals used for lighting
				float coord1[3] = { ptr_facesTriangles[triangleIndex], ptr_facesTriangles[triangleIndex + 1], ptr_facesTriangles[triangleIndex + 2] };
				float coord2[3] = { ptr_facesTriangles[triangleIndex + 3], ptr_facesTriangles[triangleIndex + 4], ptr_facesTriangles[triangleIndex + 5] };
				float coord3[3] = { ptr_facesTriangles[triangleIndex + 6], ptr_facesTriangles[triangleIndex + 7], ptr_facesTriangles[triangleIndex + 8] };
				float *norm = this->calculateNormal(coord1, coord2, coord3);

				tCounter = 0;
				for (int i = 0; i < 3; i++)
				{
					ptr_normals[normalIndex + tCounter] = norm[0];
					ptr_normals[normalIndex + tCounter + 1] = norm[1];
					ptr_normals[normalIndex + tCounter + 2] = norm[2];
					tCounter += 3;
				}

				triangleIndex += 9;
				normalIndex += 9;
				_totalConnectedTriangles += 9;
			}
		}

		objFile.close();
	}

	else
	{
		fatalError("Unable to open obj file");
	}

	return 0;
}

void ObjLoader::Release()
{
	free(this->ptr_facesTriangles);
	free(this->ptr_normals);
	free(this->ptr_vertexBuffer);
}

void ObjLoader::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, ptr_facesTriangles);
	glNormalPointer(GL_FLOAT, 0, ptr_normals);
	glDrawArrays(GL_TRIANGLES, 0, _totalConnectedTriangles);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}