#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "Camera.h"
#include "Footman.h"
#include <chrono>

namespace Example
{
	class ExampleApp : public Core::App
	{
	public:

		/// constructor
		ExampleApp(){}
		/// destructor
		~ExampleApp(){}

		/// open app
		bool Open();
		/// run app
		void Run();

	private:

		std::chrono::high_resolution_clock clock = std::chrono::high_resolution_clock();
		using ms = std::chrono::duration<float, std::milli>;

        Footman soldier;

        Vector4D cameraPos = Vector4D(0.0f, 1.0f, 3.0f, 1.0f);
        Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
        Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
        float cameraSpeed = 0.05f;

        bool click = false;
        bool firstMouse;

        float lastX, lastY, yaw = 0.0f, pitch = 0.0f;

        Matrix4D lookat;
        Matrix4D perspectiveProjection;

        Display::Window* window;
	};
}