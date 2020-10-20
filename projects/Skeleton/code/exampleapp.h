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
		ExampleApp();
		/// destructor
		~ExampleApp();

		/// open app
		bool Open();
		/// run app
		void Run();

	private:

		std::chrono::high_resolution_clock clock = std::chrono::high_resolution_clock();
		using ms = std::chrono::duration<float, std::milli>;

		Display::Window* window;

		Camera camera;

		Footman soldier;

	};
}