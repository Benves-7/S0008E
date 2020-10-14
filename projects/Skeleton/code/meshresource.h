#pragma once
#include <core/app.h>
#include <vector>
#include <map>
#include "Vector4D.h"

#pragma region MeshResource
using namespace std;
struct Vertex
{
	Vertex() 
	{

	}
	GLfloat pos[3];
	GLfloat uv[2];
	GLfloat normal[3];
};

class MeshResource
{
public:

	MeshResource()
	{

	}
	~MeshResource()
	{
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	bool loadOBJ(char* filename)
	{
		map<string, int> indexMap;
		vector<int> indexBuffer_;
		vector<Vertex> vertexBuffer_;
		int index = 0;
		bool quad = false;

		vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		vector<Vector4D> temp_vertices;
		vector<Vector4D> temp_indicies;
		vector<Vector4D> temp_normals;

		FILE* file = fopen(filename, "r");
		// if read file failed. 
		if (file == NULL) 
		{
			printf("File %s failed to load.\n", filename);
			return false;
		}
		while (true)
		{
			char lineheader[128];
			int res = fscanf(file, "%s", lineheader);
			if (res == EOF) {
				break;
			}
			// Compare first char of each string.
			if (strcmp(lineheader, "v") == 0)
			{
				Vector4D position;
				fscanf(file, "%f %f %f\n", &position[0], &position[1], &position[2]);
				temp_vertices.push_back(position);
			}
			else if (strcmp(lineheader, "vt") == 0)
			{
				Vector4D uv;
				fscanf(file, "%f %f\n", &uv[0], &uv[1]);
				temp_indicies.push_back(uv);
			}
			if (strcmp(lineheader, "vn") == 0)
			{
				Vector4D normal;
				fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineheader, "f") == 0) {
				unsigned int vIndex[4], uIndex[4], nIndex[4];
				/// The changes the operation depending on the number of faces in the obj file
				if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &uIndex[0], &nIndex[0], &vIndex[1], &uIndex[1], &nIndex[1], &vIndex[2], &uIndex[2], &nIndex[2], &vIndex[3], &uIndex[3], &nIndex[3]) == 12)
				{
					quad = true;
					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[1]);
					vertexIndices.push_back(vIndex[2]);
					uvIndices.push_back(uIndex[0]);
					uvIndices.push_back(uIndex[1]);
					uvIndices.push_back(uIndex[2]);
					normalIndices.push_back(nIndex[0]);
					normalIndices.push_back(nIndex[1]);
					normalIndices.push_back(nIndex[2]);

					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[2]);
					vertexIndices.push_back(vIndex[3]);
					uvIndices.push_back(uIndex[0]);
					uvIndices.push_back(uIndex[2]);
					uvIndices.push_back(uIndex[3]);
					normalIndices.push_back(nIndex[0]);
					normalIndices.push_back(nIndex[2]);
					normalIndices.push_back(nIndex[3]);

				}
				else
				{
					vertexIndices.push_back(vIndex[0]);
					vertexIndices.push_back(vIndex[1]);
					vertexIndices.push_back(vIndex[2]);
					uvIndices.push_back(uIndex[0]);
					uvIndices.push_back(uIndex[1]);
					uvIndices.push_back(uIndex[2]);
					normalIndices.push_back(nIndex[0]);
					normalIndices.push_back(nIndex[1]);
					normalIndices.push_back(nIndex[2]);
				}
			}
		}
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			std::string faceVertex = std::to_string(vertexIndices[i]) + "/" + std::to_string(uvIndices[i]) + "/" + std::to_string(normalIndices[i]);

			if (indexMap.count(faceVertex))
			{
				indexBuffer_.push_back(indexMap[faceVertex]);
			}
			else
			{
				unsigned int vIndex = vertexIndices[i];
				Vertex v;
				v.pos[0] = temp_vertices[vIndex - 1][0];
				v.pos[1] = temp_vertices[vIndex - 1][1];
				v.pos[2] = temp_vertices[vIndex - 1][2];

				unsigned int uIndex = uvIndices[i];
				v.uv[0] = temp_indicies[uIndex - 1][0];
				v.uv[1] = temp_indicies[uIndex - 1][1];

				unsigned int nIndex = normalIndices[i];
				v.normal[0] = temp_normals[nIndex - 1][0];
				v.normal[1] = temp_normals[nIndex - 1][1];
				v.normal[2] = temp_normals[nIndex - 1][2];

				vertexBuffer_.push_back(v);
				indexBuffer_.push_back(index);
				indexMap[faceVertex] = index;
				index++;
			}
		}
		sizeOfIndexBuffer = indexBuffer_.size();
		sizeOfVertexBuffer = vertexBuffer_.size();
		vertexBuffer = vertexBuffer_;
		indexBuffer = indexBuffer_;
		bindVertexBuffer(vertexBuffer_);
		bindIndexBuffer(indexBuffer_);
	}
	int getVertexSize() 
	{ 
		return sizeOfVertexBuffer; 
	}
	int getIndexSize() 
	{
		return sizeOfIndexBuffer; 
	}
	void setupBuffers()
	{
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	}
	void bindVertexBuffer(const std::vector<Vertex>& buffer) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeOfVertexBuffer * sizeof(Vertex), &buffer[0], GL_STATIC_DRAW);
	}
	void bindIndexBuffer(const std::vector<int>& buffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexBuffer * sizeof(int), &buffer[0], GL_STATIC_DRAW);
	}
	void unBindBuffers()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void bindAttrPointer()
	{
		//verticies
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		//uv cordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		//normals
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
	}
	void bind() 
	{
		glBindVertexArray(VAO); 
	}
	void setupHandles()
	{
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);
	}
	void unbindVAO() 
	{
		glBindVertexArray(0); 
	}
	void destroy()
	{
		glBindVertexArray(0);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	void setup(char* filename)
	{
		setupHandles();
		setupBuffers();
		bind();
		loadOBJ(filename);
		bindAttrPointer();
		unBindBuffers();
	}
	std::vector<Vertex> getVertexBuffer() 
	{
		return vertexBuffer; 
	}
	std::vector<int> getIndexBuffer() 
	{
		return indexBuffer; 
	}

private:

	int sizeOfIndexBuffer;
	int sizeOfVertexBuffer;
	std::vector<Vertex> vertexBuffer;
	std::vector<int> indexBuffer;

	uint32 EBO;
	uint32 VBO;
	uint32 VAO;
};
#pragma endregion