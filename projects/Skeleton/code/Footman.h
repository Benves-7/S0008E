#pragma once
#include "Skeleton.h"

class Footman
{
public:
	Footman() {};
	~Footman() {};
	void load();
	void update();
	void draw(Matrix4D viewMatrix, Matrix4D perspectiveProjection);

private:
	Skeleton skeleton;
};


inline void Footman::load()
{
	skeleton.loadSkeleton("Unit_Footman.constants");
}
inline void Footman::update() 
{
	skeleton.update(0);
}
inline void Footman::draw(Matrix4D viewMatrix, Matrix4D perspectiveProjection) 
{
	// draw skeleton lines using the old openGL pipeline
	glUseProgram(0);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)&viewMatrix);
	glMatrixMode(GL_PROJECTION);
	auto dood = (perspectiveProjection);
	glLoadMatrixf((GLfloat*)&dood);
	glBegin(GL_LINES);
	glColor3f(255, 0, 0);

	for (int i = 0; i < skeleton.joints.size(); ++i) {
		Joint joint = skeleton.joints.at(i);
		if (joint.parent != -1)
		{
			Vector4D a = joint.worldspaceTransform.GetPositionVector();
			Vector4D b = skeleton.joints.at(joint.parent).worldspaceTransform.GetPositionVector();
			glVertex3f(a[0], a[1], a[2]);
			glVertex3f(b[0], b[1], b[2]);
		}
	}
	glEnd();
}