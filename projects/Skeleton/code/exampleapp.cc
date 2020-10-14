//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"

#include <cstring>

#define PI 3.14159265
#define WIDTH 400
#define HEIGHT 300

using namespace Display;
namespace Example
{

	TextureResource tex;

	Matrix4D perspectiveProjection;

	Vector4D cameraPos = Vector4D(0.0f, 1.0f, 5.0f, 1);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1);

	Vector4D position = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Matrix4D rotX = Matrix4D::rotX(0);
	Matrix4D rotY = Matrix4D::rotY(0);

	bool click = false;
	bool isHeldDown = false;

	int windowSizeX;
	int windowSizeY;
	float fov = 75.0f;

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

				if (key == GLFW_KEY_ESCAPE)
					this->window->Close();

				if (key == GLFW_KEY_T && action == GLFW_PRESS)
					soldier.ds();

				if (key == GLFW_KEY_Y && action == GLFW_PRESS)
					soldier.db();
			});

		if (this->window->Open())
		{
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			this->window->GetSize(windowSizeX, windowSizeY);

			perspectiveProjection = Matrix4D::perspective(nvgDegToRad(fov), (float)windowSizeX / (float)windowSizeY, 1000, 0.1);
			shared_ptr<MeshResource> mesh = make_shared<MeshResource>();
			std::shared_ptr<TextureResource> tex = std::make_shared<TextureResource>();
			std::shared_ptr<Shader> shader = std::make_shared<Shader>();

			mesh->setup("sphere.obj");

			soldier.load();
			soldier.setup(mesh, shader, tex);
			
			return true;
		}
		return false;
	}



	//------------------------------------------------------------------------------
	/**
	*/
	void
		ExampleApp::Run()
	{
		std::chrono::high_resolution_clock clock = std::chrono::high_resolution_clock();
		auto start = clock.now();

		while (this->window->IsOpen() && true)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			Matrix4D view = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			soldier.update(clock, start);

			soldier.draw(Matrix4D::transpose(view), perspectiveProjection);

			this->window->SwapBuffers();
		}
	}

} // namespace Example