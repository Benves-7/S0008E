//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

#include <cstring>

#define PI 3.14159265

using namespace Display;
namespace Example
{
	Vector4D cameraPos = Vector4D(-1.0f, 0.0f, 5.0f, 1);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1);

	Vector4D position = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);

	bool click = false;
	bool isHeldDown = false;

	int windowSizeX;
	int windowSizeY;
	float fov = 90.0f;

	float lastX, lastY, yaw = -90.0f, pitch = 0.0f;

	float radianConversion = PI / 180;

	ExampleApp::ExampleApp()
	{
	}
	ExampleApp::~ExampleApp()
	{
	}

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
					isHeldDown = false;
				}
			});
		window->SetMouseMoveFunction([this](float64 xPos, float64 yPos)
			{
				if (click)
				{
					if (!isHeldDown)
					{
						lastX = xPos;
						lastY = yPos;
						isHeldDown = true;
					}

					float xOffset = xPos - lastX;
					float yOffset = lastY - yPos;

					lastX = xPos;
					lastY = yPos;

					float sensitivity = 0.2;
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
					front[0] = cos(yaw * radianConversion) * cos(pitch * radianConversion);
					front[1] = sin(pitch * radianConversion);
					front[2] = sin(yaw * radianConversion) * cos(pitch * radianConversion);
					front[3] = 1;
					front.normalize();
					cameraFront = front;
				}
			});
		window->SetKeyPressFunction([this](int32 key, int32 i, int32 action, int32 modifier)
			{
				float speed = 0.05f;
				if (key == GLFW_KEY_W)
				{
					cameraPos = cameraPos + (cameraFront * speed);
				}
				if (key == GLFW_KEY_S)
				{
					cameraPos = cameraPos - (cameraFront * speed);
				}
				if (key == GLFW_KEY_D)
				{
					cameraPos = cameraPos + (Vector4D::normalize(Vector4D::cross(cameraFront, cameraUp)) * speed);
				}
				if (key == GLFW_KEY_A)
				{
					cameraPos = cameraPos - (Vector4D::normalize(Vector4D::cross(cameraFront, cameraUp)) * speed);
				}
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


				if (key == GLFW_KEY_ESCAPE)
					this->window->Close();

				if (key == GLFW_KEY_T && action == GLFW_PRESS)
					soldier.ds();
				if (key == GLFW_KEY_Y && action == GLFW_PRESS)
					soldier.db();
				if (key == GLFW_KEY_M && action == GLFW_PRESS)
					soldier.dm();
				if (key == GLFW_KEY_P && action == GLFW_PRESS)
					soldier.ra();

			});


		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			this->window->GetSize(windowSizeX, windowSizeY);

			perspectiveProjection = Matrix4D::perspective(nvgDegToRad(fov), (float)windowSizeX / (float)windowSizeY, 1000, 0.1);

			soldier.load(perspectiveProjection, view);
			
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

		while (this->window->IsOpen() && true)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			glUseProgram(0);
			glMatrixMode(GL_MODELVIEW);
			Matrix4D view = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			auto viewMat = Matrix4D::transpose(view);
			glLoadMatrixf((GLfloat*)&viewMat);
			glMatrixMode(GL_PROJECTION);
			auto dood = (perspectiveProjection);
			glLoadMatrixf((GLfloat*)&dood);

			float runtime = std::chrono::duration_cast<ms>(clock.now() - start).count();

			soldier.update(runtime);

			soldier.draw();

			this->window->SwapBuffers();
		}
	}

} // namespace Example