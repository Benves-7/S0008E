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

        Footman soldier = Footman(Vector4D(0.0f,0.0f,0.0f,1.0f));

        Vector4D cameraPos = Vector4D(0.0f, 1.0f, 3.0f, 1.0f);
        Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
        Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
        float cameraSpeed = 0.05f;

        bool click = false;
        bool firstMouse;

        float lastX, lastY, yaw = 0.0f, pitch = 0.0f;

        Matrix4D lookat;
        Matrix4D perspectiveProjection;

        bool shutdown = false;
        Display::Window* window;

        chrono::high_resolution_clock clock = chrono::high_resolution_clock();
        using ms = chrono::duration<float, milli>;
        chrono::time_point<chrono::high_resolution_clock> start;
	};
}