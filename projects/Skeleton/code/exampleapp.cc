//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

#include <cstring>

#define PI 3.14159265359

using namespace Display;
namespace Example
{
	bool ExampleApp::Open()
	{
		App::Open();
		this->window = new Display::Window;

		window->SetMousePressFunction([this](int32 button, int32 action, int32 mods)
			{
				if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				{
					click = true;
				}
				else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
				{
					click = false;
					firstMouse = true;
				}
			});
		window->SetMouseMoveFunction([this](float64 xPos, float64 yPos)
			{
				if (click)
				{
					if (firstMouse)
					{
						lastX = xPos;
						lastY = yPos;
                        firstMouse = false;
					}

                    if (lastX > 10000)
                        lastX = 0;

					float xOffset = xPos - lastX;
					float yOffset = lastY - yPos;

					lastX = xPos;
					lastY = yPos;

					float sensitivity = 0.3;
					xOffset *= sensitivity;
					yOffset *= sensitivity;

					yaw += xOffset;
					pitch += yOffset;

					if (pitch > 89.0f)
					{
						pitch = 89.0f;
					}
					if (pitch < -89.0f)
					{
						pitch = -89.0f;
					}

					Vector4D front;
                    front[0] = cosf(yaw*(PI/180.0f)) * cosf(pitch*(PI/180.0f));
                    front[1] = sinf(pitch*(PI / 180.0f));
                    front[2] = sinf(yaw*(PI / 180.0f)) * cosf(pitch*(PI / 180.0f));
					cameraFront = front.normalize();
				}
			});
		window->SetKeyPressFunction([this](int32 key, int32 i, int32 action, int32 modifier)
			{
                if (key == GLFW_KEY_ESCAPE) {
                    this->window->Close();
                }
                if (key == GLFW_KEY_W){
                    cameraPos = cameraPos + (cameraFront * cameraSpeed);
                }
                if (key == GLFW_KEY_S) {
                    cameraPos = cameraPos - (cameraFront * cameraSpeed);
                }
                if (key == GLFW_KEY_D) {
                    cameraPos = cameraPos + (cameraFront.crossProduct(cameraUp)).normalize() * cameraSpeed;
                }
                if (key == GLFW_KEY_A) {
                    cameraPos = cameraPos - (cameraFront.crossProduct(cameraUp)).normalize() * cameraSpeed;
                }
                // Animations.
				if (key == GLFW_KEY_1)
					soldier.setAnimationClip(0);
				if (key == GLFW_KEY_2)
					soldier.setAnimationClip(1);
				if (key == GLFW_KEY_3)
					soldier.setAnimationClip(2);
				if (key == GLFW_KEY_4)
					soldier.setAnimationClip(3);
				if (key == GLFW_KEY_5)
					soldier.setAnimationClip(4);
				if (key == GLFW_KEY_6)
					soldier.setAnimationClip(5);
				if (key == GLFW_KEY_7)
					soldier.setAnimationClip(6);
				if (key == GLFW_KEY_8)
					soldier.setAnimationClip(7);

                // Drawstates.
				if (key == GLFW_KEY_T && action == GLFW_PRESS)
					soldier.ds();
				if (key == GLFW_KEY_Y && action == GLFW_PRESS)
					soldier.db();
				if (key == GLFW_KEY_M && action == GLFW_PRESS)
					soldier.dm();
				if (key == GLFW_KEY_P && action == GLFW_PRESS)
					soldier.pa();

			});

		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glEnable(GL_DEPTH_TEST);
            //Perspective projection
            const float n = 0.1f;
            const float f = 100000.0f;
            const float r = 0.1f;
            const float l = -0.1f;
            const float t = 0.1f;
            const float b = -0.1f;

            perspectiveProjection = Matrix4D::perspective(n, f, r, l, t, b);

			soldier.load();
			soldier.loadmesh();

			return true;
		}
		return false;
	}



	//------------------------------------------------------------------------------
	/**
	*/
	void ExampleApp::Run()
	{
		auto start = clock.now();
        Vector4D pos = Vector4D(0,0,-4,1);

		while (this->window->IsOpen())
		{
			this->window->Update();
            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);

            lookat = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			soldier.drawModel(perspectiveProjection*lookat, (Matrix4D::getPositionMatrix(pos)), cameraPos);

			this->window->SwapBuffers();
		}
	}

} // namespace Example