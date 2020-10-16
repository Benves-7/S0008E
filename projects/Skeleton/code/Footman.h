#pragma once
#include "Skeleton.h"
#include "Animation.h"
#include "Cubesphere.h"
#include <chrono>
#include <GL/GLU.h>



#define PI 3.14159265

class Footman
{
public:
	Footman() {};
	~Footman() {};
	void load()
	{
		skeleton.loadSkeleton("Unit_Footman.constants");
	}
	void update(std::chrono::high_resolution_clock clock, std::chrono::high_resolution_clock::time_point start)
	{
		//using ms = std::chrono::duration<float, std::milli>;
		//float animationSpeed = std::chrono::duration_cast<ms>(clock.now() - start).count() / animation.clips[clipToPlay].keyDuration;

		//for (int k = 0; k < skeleton.joints.size(); ++k)
		//{
		//	//Load animation data for one key in a clip
		//	Vector4D pos = animation.getKey(clipToPlay, animationSpeed, k * 4, 0);
		//	Matrix4D po = Matrix4D::getPositionMatrix(pos);
		//	Vector4D rot = animation.getKey(clipToPlay, animationSpeed, k * 4 + 1, 1);
		//	Matrix4D ro = Matrix4D::getRotationFromQuaternian(rot);
		//	Vector4D scale = animation.getKey(clipToPlay, animationSpeed, k * 4 + 2, 0);
		//	Matrix4D sc = Matrix4D::getScaleMatrix(scale);
		//	Vector4D vel = animation.getKey(clipToPlay, animationSpeed, k * 4 + 3, 0);
		//	Matrix4D res = po * ro * sc;
		//	skeleton.joints.at(k).localTransform = res;
		//}
		skeleton.update(0);
	}
	void draw(Matrix4D viewMatrix, Matrix4D perspectiveProjection)
	{
		// draw skeleton lines using the old openGL pipeline
		glUseProgram(0);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)&viewMatrix);
		glMatrixMode(GL_PROJECTION);
		auto dood = (perspectiveProjection);
		glLoadMatrixf((GLfloat*)&dood);

		for (int i = 0; i < skeleton.joints.size(); ++i)
		{
			Joint joint = skeleton.joints.at(i);
			Vector4D a = joint.worldspaceTransform.getPositionVector();
			glBegin(GL_LINES);
			if (drawSkeleton)
			{
				if (joint.parent != -1)
				{
					glColor3f(255, 0, 0);
					Vector4D a = joint.worldspaceTransform.getPositionVector();
					Vector4D b = skeleton.joints.at(joint.parent).worldspaceTransform.getPositionVector();
					glVertex3f(a[0], a[1], a[2]);
					glVertex3f(b[0], b[1], b[2]);
				}
			}
			if (drawBalls)
			{

				glColor3f(0, 255, 0);
				int Stacks = 20;
				int Slices = 20;
				float radius = 0.03f;
				// Calc The Vertices
				for (int i = 0; i <= Stacks; ++i) {

					float V = i / (float)Stacks;
					float phi = V * PI;

					// Loop Through Slices
					for (int j = 0; j <= Slices; ++j) {

						float U = j / (float)Slices;
						float theta = U * (PI * 2);

						// Calc The Vertex Positions
						float x = a[0] + (cosf(theta) * sinf(phi) * radius);
						float y = a[1] + (cosf(phi) * radius);
						float z = a[2] + (sinf(theta) * sinf(phi) * radius);
						glVertex3f(x, y, z);
					}
				}
			}
			glEnd();
		}
	}

	void setup(shared_ptr<MeshResource> mesh, shared_ptr<Shader> shader, shared_ptr<TextureResource> texture)
	{
		for (int i = 0; i < skeleton.joints.size(); i++)
		{
			skeleton.joints[i].node.setShaderObject(shader);
			skeleton.joints[i].line.setShaderObject(shader);

			skeleton.joints[i].node.setMeshResource(mesh);


			skeleton.joints[i].node.setTextureResource(texture);
			skeleton.joints[i].line.setTextureResource(texture);
		}
			skeleton.joints[0].node.load("texture.tga", "vs.shader", "fs.shader", -1);
	}
	void ds()
	{
		drawSkeleton = !drawSkeleton;
	}
	void db()
	{
		drawBalls = !drawBalls;
	}
private:
	Skeleton skeleton;
	Animation animation;

	unsigned int clipToPlay = 0;

	bool drawSkeleton = true;
	bool drawBalls = true;

};