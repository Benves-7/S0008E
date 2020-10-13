#pragma once
#include "light.h"
#include "meshresource.h"
#include "textureresource.h"
#include "Shader.h"
#include "Camera.h"
#include <memory>

class GraphicsNode
{
public:

	GraphicsNode() { transform = Matrix4D::RotY(1); }
	~GraphicsNode()
	{
		this->shader = 0;
		this->meshResource = 0;
		this->textureResource = 0;
	}

	void setShaderObject(std::shared_ptr<Shader> shader) { this->shader = shader; }
	void setTextureResource(std::shared_ptr<TextureResource> textureResource) { this->textureResource = textureResource; }
	void setMeshResource(std::shared_ptr<MeshResource> meshResource) { this->meshResource = meshResource; }
	Shader* getShaderObject() { return shader.get(); }
	TextureResource* getTextureResource() { return textureResource.get(); }
	MeshResource* getMeshResource() { return meshResource.get(); }
	Matrix4D getTransform() { return transform; }
	void setTransform(Matrix4D mat) { transform = mat; }
	void load(string filename, string vertexShaderName, string fragmentShaderName, int texture)
	{
		shader.get()->loadVertexShader(vertexShaderName.c_str());
		shader.get()->loadFragmentShader(fragmentShaderName.c_str());
		shader.get()->linkShaders();
		textureResource.get()->bind(texture);
		light = LightSource(Vector4D(5, 2, 0, 1), Vector4D(0.5f, 0.5f, 0.5f, 1), 1);
		shader.get()->useProgram();
	}
	void draw()
	{
		rend.clearZbuffer();
		shader.get()->modifyUniformMatrix("transform", transform.GetPointer());

		shader.get()->modifyUniformVector("lightPos", light.getPosition());
		shader.get()->modifyUniformVector("lightColor", light.getColor());
		shader.get()->modifyUniformFloat("intensity", light.getIntensity());

		meshResource.get()->bind();
		glDrawElements(GL_TRIANGLES, meshResource->getIndexSize(), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		meshResource.get()->unBindBuffers();
	}

private:

	Renderer rend;
	shared_ptr<Shader> shader;
	shared_ptr<TextureResource> textureResource;
	shared_ptr<MeshResource> meshResource;

	Matrix4D transform;
	float rotation = 0;
	vector<Vertex> vertices;
	vector<int> indices;
	vector<Vector4D> uv;
	vector<Vector4D> normals;
	vector<Vector4D> combinedBuffer;

	LightSource light;
};