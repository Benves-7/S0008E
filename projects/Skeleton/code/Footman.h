#pragma once
#include "Skeleton.h"
#include "Animation.h"
#include "Cubesphere.h"
#include <chrono>
#include <GL/GLU.h>

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
		Vector4D scaleBalls(0.001, 0.001, 0.001, 1);

		// draw skeleton lines using the old openGL pipeline
		glUseProgram(0);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)&viewMatrix);
		glMatrixMode(GL_PROJECTION);
		auto dood = (perspectiveProjection);
		glLoadMatrixf((GLfloat*)&dood);
		if (drawSkeleton)
		{
			glBegin(GL_LINES);
			glColor3f(255, 0, 0);
			for (int i = 0; i < skeleton.joints.size(); ++i) {
				Joint joint = skeleton.joints.at(i);
				if (joint.parent != -1)
				{
					Vector4D a = joint.worldspaceTransform.getPositionVector();
					Vector4D b = skeleton.joints.at(joint.parent).worldspaceTransform.getPositionVector();
					glVertex3f(a[0], a[1], a[2]);
					glVertex3f(b[0], b[1], b[2]);
				}
			}
			glEnd();
		}
		if (drawBalls)
		{
			for (int i = 0; i < skeleton.joints.size(); i++)
			{
				Joint joint = skeleton.joints.at(i);
				if (joint.parent != -1)
				{
					Vector4D a = joint.worldspaceTransform.getPositionVector();
					glPushMatrix();
					glTranslatef(a[0], a[1], a[2]);
					Cubesphere sphere(0.03f, 4, false);
					sphere.draw();
					glPopMatrix();
				}
			}
		}
	}
	void setup(shared_ptr<MeshResource> mesh, shared_ptr<Shader> shader, shared_ptr<TextureResource> texture)
	{
		for (int i = 0; i < skeleton.joints.size(); i++)
		{
			skeleton.joints[i].node.setShaderObject(shader);
			skeleton.joints[i].node.setMeshResource(mesh);
			skeleton.joints[i].node.setTextureResource(texture);
			skeleton.joints[i].node.load("Footman_Diffuse.tga", "vs.shader", "fs.shader", -1);
		}
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