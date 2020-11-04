#pragma once
#include "Skeleton.h"
#include "Animation.h"
#include "Graphics.h"
#include "Cubesphere.h"
#include <chrono>

#define PI 3.14159265

class Footman
{
public:
	Footman(Vector4D pos)
	{
	    positionMatrix = Matrix4D::getPositionMatrix(pos);
	}
	~Footman() {}

	void load()
	{
		// Skeleton
		skeleton.loadSkeleton("Unit_Footman.constants");
		// Animation
		animation.loadAnimations("Unit_Footman.nax3");
	}
	void loadmesh()
    {
        // Graphics
        graphics.setup();
    }


    void update(float runtime)
    {
	    animateSkeleton(runtime);
        graphics.jointsTransform.clear();
	    updateMesh(0);
    }
	void animateSkeleton(float runtime)
	{
	    if (playAnimation && clipToPlay != -1)
        {
	        graphics.jointsTransform.clear();
		    float animationSpeed = runtime / animation.clips[clipToPlay].keyDuration;
			for (int i = 0; i < skeleton.joints->size(); ++i)
			{
				//Load animation data for one key in a clip
				Vector4D pos = animation.getKey(clipToPlay, animationSpeed, i * 4, 0);
				Matrix4D po = Matrix4D::getPositionMatrix(pos);
				Vector4D rot = animation.getKey(clipToPlay, animationSpeed, i * 4 + 1, 1);
				Matrix4D ro = Matrix4D::getRotationFromQuaternian(rot);
				Vector4D scale = animation.getKey(clipToPlay, animationSpeed, i * 4 + 2, 0);
				Matrix4D sc = Matrix4D::getScaleMatrix(scale);
				Vector4D vel = animation.getKey(clipToPlay, animationSpeed, i * 4 + 3, 0);
				Matrix4D res = (po * ro) * sc;
				skeleton.joints->at(i).localTransform = res;
			}
		}
		skeleton.update(0);
	}
	void updateMesh(int index)
    {
	    Joint joint = skeleton.joints->at(index);

	    if (joint.parent == -1)
            graphics.jointsTransform.push_back(joint.localTransform);
	    else
	        graphics.jointsTransform.push_back(skeleton.joints->at(joint.parent).worldspaceTransform * joint.localTransform);
        for (int i = 0; i < joint.children.size(); ++i) {
            updateMesh(joint.children.at(i));
        }
    }

	void draw(Matrix4D viewMatrix)
    {
	    Matrix4D mat = Matrix4D::transpose(viewMatrix);
		for (int i = 0; i < skeleton.joints->size(); ++i)
		{
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf((GLfloat*)&mat);

            Joint joint = skeleton.joints->at(i);
			Vector4D a = joint.worldspaceTransform.getPositionVector();

			Vector4D pos = positionMatrix.getPositionVec();

			glBegin(GL_LINES);
			if (drawSkeleton)
			{
				if (joint.parent != -1)
				{
					glColor3f(255, 0, 0);
					Vector4D b = skeleton.joints->at(joint.parent).worldspaceTransform.getPositionVector();
					glVertex3f(pos[0]+a[0], pos[1]+a[1], pos[2]+a[2]);
					glVertex3f(pos[0]+b[0], pos[1]+b[1], pos[2]+b[2]);
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
						float x = pos[0]+a[0] + (cosf(theta) * sinf(phi) * radius);
						float y = pos[1]+a[1] + (cosf(phi) * radius);
						float z = pos[2]+a[2] + (sinf(theta) * sinf(phi) * radius);
						glVertex3f(x, y, z);
					}
				}
			}
			glEnd();
		}
	}
	void drawModel(Matrix4D viewMatrix, Vector4D camerapos)
    {
	    if (drawMesh)
	    {
            graphics.draw(viewMatrix, positionMatrix, camerapos, playAnimation);
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
	void dm()
	{
		drawMesh = !drawMesh;
	}
	void pa()
	{
		playAnimation = !playAnimation;
	}

	void setAnimationClip(int i)
	{
		clipToPlay = i;
	}

private:
	Skeleton skeleton;
	Animation animation;
	Graphics graphics;

	Matrix4D positionMatrix;

	unsigned int clipToPlay = -1;

	bool drawSkeleton = true;
	bool drawBalls = true;
	bool drawMesh = true;
	bool playAnimation = true;
};